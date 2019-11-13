#ifndef LOGS_H
#define LOGS_H

#include <QDebug>

#define CYAN(x) "\033[1;36m" << x << "\033[0m"
#define BLUE(x) "\033[1;34m" << x << "\033[0m"
#define RED(x) "\033[1;31m" << x << "\033[0m"
#define GREEN(x) "\033[1;32m" << x << "\033[0m"

QDebug qLog(const QtMsgType &type);
QDebug qCLog(const QtMsgType &type, const QLoggingCategory &category);

#endif // LOGS_H
