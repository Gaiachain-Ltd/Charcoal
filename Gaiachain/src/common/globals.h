#ifndef GLOBALS_H
#define GLOBALS_H

#include <QVector>
#include <QObject>
#include <QLoggingCategory>

#define SERVER_ADDRESS QStringLiteral("https://api-beta.gaiachain.io")
#define QR_CODE_LENGTH 12

#if defined(Q_OS_LINUX) and !defined(Q_OS_ANDROID)
    #define Q_OS_LINUX_DESKTOP 1
#endif

Q_DECLARE_LOGGING_CATEGORY(dataTypes)
Q_DECLARE_LOGGING_CATEGORY(dataModels)
Q_DECLARE_LOGGING_CATEGORY(dataRequests)

namespace Gaia {
    using ModelEntryInfo = QMap<int, QVariant>;
    using ModelEntry = QVariantList;
    using ModelData = QVector<ModelEntry>;
}

#endif // GLOBALS_H
