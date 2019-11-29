#include "cryptohelper.h"

#include <qpassworddigestor.h>
#include <QRandomGenerator>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>

#include <array>

#include "mcrypto.h"

namespace {
const QByteArray p12a34s56w67r89d = QSettings::staticMetaObject.className() + QByteArray("16") + QIODevice::staticMetaObject.className();
}

const QLatin1String CryptoHelper::EncryptedSettingsExt = QLatin1String("esdat");

QByteArray CryptoHelper::randomSalt()
{
    const int size = 16;
    auto salt = std::array<uint, (size * sizeof (char) / sizeof (uint))>{};

    QRandomGenerator::global()->fillRange(salt.data(), salt.size());
    return QByteArray(reinterpret_cast<char *>(salt.data()), size).toHex();
}

QByteArray CryptoHelper::passwordHash(const QByteArray &password, const QByteArray &salt)
{
    const auto pepper = QCryptographicHash::staticMetaObject.className();

    const int keySize = 64;
    const int iterations = 4096;

    return QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha512, password + pepper, salt, iterations, keySize).toHex();
}

QSettings::Format CryptoHelper::encryptedSettingsFormat()
{
    static QSettings::Format EncryptedSettinsFormat = QSettings::InvalidFormat;
    if (EncryptedSettinsFormat == QSettings::InvalidFormat) {
        EncryptedSettinsFormat = QSettings::registerFormat(EncryptedSettingsExt, &CryptoHelper::loadEncryptedSettings, &CryptoHelper::storeEncryptedSettings);
    }
    return EncryptedSettinsFormat;
}

bool CryptoHelper::loadEncryptedSettings(QIODevice &device, QSettings::SettingsMap &map)
{
    auto encryptedData = device.readAll();
    auto jsonData = MCrypto().decrypt(encryptedData, p12a34s56w67r89d);

    auto error = QJsonParseError{};
    auto jsonDoc = QJsonDocument::fromJson(jsonData, &error);
    if (error.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
        return false;
    }

    map = jsonDoc.object().toVariantMap();
    return true;
}

bool CryptoHelper::storeEncryptedSettings(QIODevice &device, const QSettings::SettingsMap &map)
{
    auto jsonData = QJsonDocument(QJsonObject::fromVariantMap(map)).toJson();
    auto encryptedData = MCrypto().encrypt(jsonData, p12a34s56w67r89d);

    return device.write(encryptedData);
}
