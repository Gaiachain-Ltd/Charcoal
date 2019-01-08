#ifndef GLOBALS_H
#define GLOBALS_H

#include <QVector>
#include <QObject>

#define SERVER_ADDRESS QStringLiteral("https://beta.gaiachain.io/api/v1")
#define QR_CODE_LENGTH 10

namespace Gaia {
    using ModelData = QVector<QVariantList>;
}

#endif // GLOBALS_H
