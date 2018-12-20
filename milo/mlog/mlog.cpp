/*******************************************************************************
Copyright (C) 2017 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/


#include "mlog.h"

#include <QString>
#include <QTextStream>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QtDebug>

#ifdef ANDROID
#include "android/log.h"
#endif

Q_LOGGING_CATEGORY(coreLogger, "core.logger")

/*!
 * \class MLog
 * \brief Simple logger with capability to log into file
 *
 * This class is (sadly!) a singleton. If you call enableLogToFile(), MLog
 * will start writing logs (all uses of qDebug, qCDebug, qInfo, qWarning, etc.)
 * into both the console and a file.
 *
 * Location of the log file can be read using currentLogPath().
 *
 * Whenever enableLogToFile() is called, the old log (created in the previous
 * run of the application) is moved to previousLogPath(), and all subsequent
 * messages are saved to currentLogPath().
 *
 * To include MLog in your project, write this in your main routine:
  \code
  MLog::instance(); // This will setup the logger
  Q*Application a(argc, argv);
  logger()->enableLogToFile(a.applicationName()); // To enable writing to file
  \endcode
 *
 * You also need to include milolog.pri in your .pro file.
 *
 * You do not need to include milolog.h in files (other than main.cpp) - just
 * using qDebug and friends is enough.
 */

MLog *MLog::_instance = nullptr;
bool MLog::_logToFile = false;
bool MLog::_logToConsole = true;

/*!
 * Installs Qt message handler. Sets up the default message pattern.
 *
 * If you are getting undefined method names in log output, add this to your
 * .pro file:
 \code
 DEFINES += QT_MESSAGELOGCONTEXT
 \endcode
 */
MLog::MLog()
{
    // use backslashes between '%' and '{' to avoid shadowing this placeholders with
    // similar placeholders from wizard.json file during the Qt Creator wizard creation
    qSetMessagePattern("%\{time}|%\{type}%\{if-category}|%\{category}%\{endif}|%\{function}: "
                       "%\{message}");
    qInstallMessageHandler(&messageHandler);
}

MLog::~MLog()
{
}

/*!
 * Returns the instance of MLog (which is a singleton). You can also use the
 * logger() function to achieve the same goal.
 *
 * \sa logger()
 */
MLog *MLog::instance()
{
    if (!_instance) {
        _instance = new MLog();
    }

    return _instance;
}

/*!
 * Creates a log file based on \a appName (application name) and opens it for
 * writing. All future uses of qDebug, qCDebug, qInfo, etc. will be printed to
 * the console and written into that file.
 *
 * When this method is called, MLog saves log file created earlier (during
 * previous run of the application, or when enableLogToFile() was last run)
 * to previousLogPath().
 *
 * Log file path can be read using currentLogPath(). Previous log file is available
 * at previousLogPath().
 *
 * If you want to disable writing log to file while the application is still
 * running, use disableLogToFile().
 *
 * If you do not have logs directory it will be created.
 */
void MLog::enableLogToFile(const QString &appName)
{
    // Prepare the log file
    const QString logFilePath(QStandardPaths::writableLocation(
                                  QStandardPaths::DocumentsLocation));

    // Check if logs directory exists and if does not exist try to create it
    QDir logsDir(logFilePath);
    if (!logsDir.exists()) {
        qCDebug(coreLogger) << "Creating logs directory";
        if (logsDir.mkpath(logFilePath))
            qCDebug(coreLogger) << "Directory was created successfully";
        else
            qCWarning(coreLogger) << "Could not create logs directory!";
    }

    _previousLogPath = logFilePath + '/' + appName + "-previous.log";
    _currentLogPath = logFilePath + '/' + appName + "-current.log";

    // Move old log (if present) to appName-previous.log
    if (QFileInfo(_currentLogPath).exists()) {
        QFile::remove(_previousLogPath);
        QFile::rename(_currentLogPath, _previousLogPath);
    }

    // Open appName-current.log and write init message
    _logFile.setFileName(_currentLogPath);
    if (!_logFile.open(QFile::WriteOnly | QFile::Text)) {
        qCWarning(coreLogger) << "Could not open log file for writing!";
    }
    else
      _logToFile = true;
}

/*!
 * Disables writing logs into a file. Log messages will continue to be printed
 * into the console (cerr).
 */
void MLog::disableLogToFile()
{
    _logFile.close();
    _logToFile = false;
}

/*!
 * Enables writing logs into a file. Log messages will continue to be printed
 * into the console (cerr).
 */
void MLog::enableLogToConsole()
{
    _logToConsole = true;
}

/*!
 * Disables writing logs into a console. Log messages will continue to be printed
 * into the background (cerr).
 */
void MLog::disableLogToConsole()
{
    _logToConsole = false;
}

/*!
 * Returns path where previous log file is saved. Previous log file was saved
 * after earlier run of enableLogToFile().
 *
 * Warning: this method will return an empty string if invoked *before*
 * enableLogToFile().
 */
QString MLog::previousLogPath() const
{
    return _previousLogPath;
}

/*!
 * Returns path where current log file is saved. MLog will write all calls to
 * qDebug(), qCDebug(), qInfo(), etc. into this file.
 *
 * Warning: this method will return an empty string if invoked *before*
 * enableLogToFile().
 */
QString MLog::currentLogPath() const
{
    return _currentLogPath;
}

/*!
 * Standard Qt messageHandler. MLog only adds the option to write to file
 * (use enableLogToFile() to, well, enable writing logs to a file).
 *
 * \a type, \a context, \a message have exactly the same meaning as in Qt's
 * built-in message handler and behave the same.
 *
 * MLog respects categorized logging settings (qCDebug() and friends).
 */
void MLog::messageHandler(QtMsgType type, const QMessageLogContext &context,
                          const QString &message)
{
    const QString formatted(qFormatLogMessage(type, context, message));
    if (_logToFile)
      logger()->write(formatted + "\n");
    if (!_logToConsole)
      return;
#ifdef ANDROID
    android_LogPriority priority = ANDROID_LOG_DEBUG;
    switch (type) {
        case QtWarningMsg: priority = ANDROID_LOG_WARN; break;
        case QtCriticalMsg: priority = ANDROID_LOG_ERROR; break;
        case QtFatalMsg: priority = ANDROID_LOG_FATAL; break;
        case QtInfoMsg: priority = ANDROID_LOG_INFO; break;
        default: priority = ANDROID_LOG_DEBUG; break;
    };
    __android_log_print(priority, "Qt", "%s", qPrintable(formatted));
#else
    fprintf(stderr, "%s\n", qPrintable(formatted));
    fflush(stderr);
#endif
}

/*!
 * Wrties \a message into current log file.
 *
 * This function first checks whether log file is open and writable.
 */
void MLog::write(const QString &message)
{
    QMutexLocker locker(&_mutex);
    if (_logFile.isOpen() && _logFile.isWritable()) {
        QTextStream logStream(&_logFile);
        logStream.setCodec("UTF-8");
        logStream << message;
    }
}

/*!
 * Returns the singleton instance of MLog. Synonym to MLog::instance(),
 * but easier to write.
 */
MLog *logger()
{
    return MLog::instance();
}
