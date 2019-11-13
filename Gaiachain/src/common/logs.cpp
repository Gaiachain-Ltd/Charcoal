#include "logs.h"

QDebug qLog(const QtMsgType &type)
{
    switch (type) {
    case QtWarningMsg:
        return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning();
    case QtCriticalMsg:
        return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).critical();
    default:
    case QtDebugMsg:
        return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug();
    }
}

QDebug qCLog(const QtMsgType &type, const QLoggingCategory &category)
{
    switch (type) {
    case QtWarningMsg:
        return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).warning(category);
    case QtCriticalMsg:
        return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).critical(category);
    default:
    case QtDebugMsg:
        return QMessageLogger(QT_MESSAGELOG_FILE, QT_MESSAGELOG_LINE, QT_MESSAGELOG_FUNC).debug(category);
    }
}
