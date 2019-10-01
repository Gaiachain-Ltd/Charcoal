#ifndef GLOBALS_H
#define GLOBALS_H

#include <QVector>
#include <QObject>
#include <QLoggingCategory>

#define SERVER_ADDRESS QStringLiteral("https://poc.gaiachain.io/api/v1")
#define QR_CODE_LENGTH 12

#define MAX_BATCH_SIZE 20

#if defined(Q_OS_LINUX) and !defined(Q_OS_ANDROID)
    #define Q_OS_LINUX_DESKTOP 1
#endif

Q_DECLARE_LOGGING_CATEGORY(dataModels)

namespace Gaia {
    using ModelData = QVector<QVariantList>;
}

#endif // GLOBALS_H
