#include "cryptohelper.h"

#include <qpassworddigestor.h>
#include <QRandomGenerator>
#include <QIODevice>
#include <QJsonDocument>
#include <QJsonObject>

#include <array>

#include "mcrypto.h"

namespace {
const QByteArray p12a34s56w67r89d = QSettings::staticMetaObject.className()
    + QByteArray("16") + QIODevice::staticMetaObject.className();
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

    return QPasswordDigestor::deriveKeyPbkdf2(QCryptographicHash::Sha512,
                                              password + pepper, salt, iterations,
                                              keySize).toHex();
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
    const auto encryptedData = device.readAll();
    auto jsonData = MCrypto().decrypt(encryptedData, QByteArray(APP_STORAGE_PASSWORD));
    if (jsonData.isEmpty()) {
        qWarning() << "Decryption failed, trying with obsolete password";
        jsonData = MCrypto().decrypt(encryptedData, p12a34s56w67r89d);

        if (jsonData.isEmpty()) {
            qWarning() << "Could not decrypt, even with old password!";
            return false;
        }
    }

    auto error = QJsonParseError{};
    const auto jsonDoc = QJsonDocument::fromJson(jsonData, &error);
    if (error.error != QJsonParseError::NoError || !jsonDoc.isObject()) {
        qWarning() << "Settings decryption error - JSON data could not be read!";
        return false;
    }

    map = jsonDoc.object().toVariantMap();

    return true;
}

bool CryptoHelper::storeEncryptedSettings(QIODevice &device,
                                          const QSettings::SettingsMap &map)
{
    const auto jsonData = QJsonDocument(QJsonObject::fromVariantMap(map)).toJson();
    const auto encryptedData = MCrypto().encrypt(jsonData, QByteArray(APP_STORAGE_PASSWORD));

    return device.write(encryptedData);
}

QByteArray CryptoHelper::readBytes(const QVariant &data)
{
    QByteArray result(data.toByteArray());
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    return QByteArray::fromBase64Encoding(result).decoded;
#else
    return QByteArray::fromBase64(result);
#endif
}

QVariant CryptoHelper::writeBytes(const QByteArray &data)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    // Data will be auto-translated to base64
    return data;
#else
    // We manually encode to base64
    const QByteArray encoded(data.toBase64());
    return encoded;
#endif
}
