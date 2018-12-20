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

#include <QtTest>
#include <QCoreApplication>

#include "../mlog.h"

#include "loggingthread.h"

class TestMLog : public QObject
{
  Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testEnableLogToFile();
    void testDisableLogToFile();
    void testLogToConsole();
    void testInThread();
    void testInMultipleThreads();

private:
    void clean();
};

void TestMLog::initTestCase()
{
    Q_ASSERT(MLog::instance());
    QCoreApplication::setApplicationName("MLog Test");
    QCoreApplication::setOrganizationName("Milo");
}

void TestMLog::cleanupTestCase()
{
    clean();
}

void TestMLog::clean()
{
    logger()->enableLogToConsole();
    logger()->disableLogToFile();
    QFile::remove(logger()->currentLogPath());
    QFile::remove(logger()->previousLogPath());
}

void TestMLog::testEnableLogToFile()
{
    logger()->enableLogToFile(QCoreApplication::applicationName());
    QVERIFY(QFile::exists(logger()->currentLogPath()));
    QVERIFY(!QFile::exists(logger()->previousLogPath()));
    clean();
    logger()->enableLogToFile("Test log");
    QVERIFY(QFile::exists(logger()->currentLogPath()));
    QVERIFY(logger()->currentLogPath() != logger()->previousLogPath());
    QFile logFile(logger()->currentLogPath());
    quint64 fileSize1 = logFile.size();
    qDebug() << "Test log file size: " << fileSize1;
    quint64 fileSize2 = logFile.size();
    qDebug() << "Test log file size: " << fileSize2;
    QVERIFY(!(fileSize1 == fileSize2));
    quint64 fileSize3 = logFile.size();
    QVERIFY(!(fileSize3 == fileSize2));
    
}

void TestMLog::testDisableLogToFile()
{
    logger()->disableLogToFile();
    QVERIFY(QFile::exists(logger()->currentLogPath()));
    QFile logFile(logger()->currentLogPath());
    quint64 fileSize1 = logFile.size();
    qDebug() << "Test log file size: " << fileSize1;
    quint64 fileSize2 = logFile.size();
    clean();
    logger()->enableLogToFile(QCoreApplication::applicationName());
    qDebug() << "Test log file size: " << fileSize2;
    QCOMPARE(fileSize1, fileSize2);
    clean();
}

void TestMLog::testLogToConsole()
{
    logger()->enableLogToConsole();
    logger()->enableLogToFile(QCoreApplication::applicationName());
    QVERIFY(QFile::exists(logger()->currentLogPath()));
    qDebug() << "Test debug text";
    qDebug() << "Test debug text2";
    QFile logFile(logger()->currentLogPath());
    quint64 fileSize1 = logFile.size();

    logger()->enableLogToFile(QCoreApplication::applicationName());
    logger()->disableLogToConsole();
    QVERIFY(QFile::exists(logger()->currentLogPath()));
    qDebug() << "Test debug text";
    qDebug() << "Test debug text2";
    QFile logFile2(logger()->currentLogPath());
    quint64 fileSize2 = logFile2.size();

    QVERIFY(fileSize1 == fileSize2);

    clean();
}

void TestMLog::testInThread()
{
    logger()->enableLogToFile("Thread log");
    QVERIFY(QFile::exists(logger()->currentLogPath()));
    qDebug() << "Test debug text";
    QFile logFile(logger()->currentLogPath());
    quint64 fileSize1 = logFile.size();

    LoggingThread thread;
    thread.start();
    thread.wait();

    quint64 fileSize2 = logFile.size();

    QVERIFY(fileSize2 > fileSize1);
    clean();
}

void TestMLog::testInMultipleThreads()
{
    logger()->enableLogToFile("Threads log");
    QVERIFY(QFile::exists(logger()->currentLogPath()));
    QFile logFile(logger()->currentLogPath());

    LoggingThread thread;
    thread.start();
    thread.wait();

    quint64 fileSize1 = logFile.size();

    const int numberOfThreads = 20;
    QVector<LoggingThread*> threadVec;
    for (int i = 0 ; i < numberOfThreads; i++)
        threadVec.push_back(new LoggingThread());

    Q_FOREACH(LoggingThread *thread, threadVec)
        thread->start();

    Q_FOREACH(LoggingThread *thread, threadVec)
        thread->wait();

    Q_FOREACH(LoggingThread *thread, threadVec)
        thread->deleteLater();
    threadVec.clear();

    quint64 fileSize2 = logFile.size();
    int logNumber = std::ceil((float)fileSize2/(float)fileSize1);

    QVERIFY(logNumber == numberOfThreads+1);
}

QTEST_MAIN(TestMLog)

#include "tst_mlog.moc"
