#ifndef CRYPTOHELPER_H
#define CRYPTOHELPER_H

#include <QString>
#include <QByteArray>
#include <QSettings>

class QIODevice;

struct CryptoHelper
{
    static QByteArray randomSalt();
    static QByteArray passwordHash(const QByteArray &password, const QByteArray &salt);

    static const QLatin1String EncryptedSettingsExt;
    static QSettings::Format encryptedSettingsFormat();
    static bool loadEncryptedSettings(QIODevice &device, QSettings::SettingsMap &map);
    static bool storeEncryptedSettings(QIODevice &device, const QSettings::SettingsMap &map);

    static QByteArray readBytes(const QVariant &data);
    static QVariant writeBytes(const QByteArray &data);
};

#endif // CRYPTOHELPER_H
