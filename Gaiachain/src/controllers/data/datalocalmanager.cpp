#include "datalocalmanager.h"

#include <QStandardPaths>
#include <QSettings>
#include <QDateTime>
#include <QUrl>

#include "../../helpers/cryptohelper.h"
#include "../../common/tags.h"
#include "../../common/types.h"

namespace {
const QLatin1String LocalDataDirName = QLatin1String("localActions");
const QString LocalDataFileName = QString("%1_a%2.") + CryptoHelper::EncryptedSettingsExt;
}

DataLocalManager::DataLocalManager(QObject *parent)
    : AbstractManager(parent), c_dataPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/" + LocalDataDirName),
      c_dataDir(c_dataPath)
{}

void DataLocalManager::addLocalAction(const QString &packageId, const Enums::SupplyChainAction &action,
                                      const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties) const
{
    const auto actionFile = filePath(packageId, action);

    if (!QFile::exists(actionFile)) {
        QSettings localActionData(actionFile, CryptoHelper::encryptedSettingsFormat());
        localActionData.setValue(Tags::pid, packageId);
        localActionData.setValue(Tags::action, QVariant::fromValue(action));
        localActionData.setValue(Tags::latitude, coordinate.latitude());
        localActionData.setValue(Tags::longitude, coordinate.longitude());
        localActionData.setValue(Tags::timestamp, timestamp);
        localActionData.setValue(Tags::properties, properties);
    }
}

void DataLocalManager::removeLocalAction(const QString &packageId, const Enums::SupplyChainAction &action) const
{
    QFile::remove(filePath(packageId, action));
}

void DataLocalManager::sendLocalAction(const QString &packageId, const Enums::SupplyChainAction &action,
                                       const QGeoCoordinate &coordinate, const QDateTime &timestamp, const QVariantMap &properties) const
{
    QSettings localActionData(filePath(packageId, action), CryptoHelper::encryptedSettingsFormat());
    const auto localPackageId = localActionData.value(Tags::pid).toString();
    const auto localAction = localActionData.value(Tags::action).value<Enums::SupplyChainAction>();
    const auto localLatitude = localActionData.value(Tags::latitude).toDouble();
    const auto localLongitude = localActionData.value(Tags::longitude).toDouble();
    const auto localTimestamp = localActionData.value(Tags::timestamp).toDateTime();
    const auto localProperties = localActionData.value(Tags::properties).toMap();

    auto localTimestampSecs = types::convert(types::convert(timestamp, QMetaType::LongLong), QMetaType::QDateTime);
    auto timestampSecs = types::convert(types::convert(timestamp, QMetaType::LongLong), QMetaType::QDateTime);
    if (localPackageId != packageId ||
            localAction != action ||
            !qFuzzyCompare(localLatitude, coordinate.latitude()) ||
            !qFuzzyCompare(localLongitude, coordinate.longitude()) ||
            localTimestampSecs != timestampSecs ||
            localProperties != properties) {
        qWarning(dataModels) << "Mismatch in local data! Possible hacking attempt!\n"
                             << localPackageId << "==" << packageId << "\n"
                             << localAction << "==" << action << "\n"
                             << localLatitude << "==" << coordinate.latitude() << "\n"
                             << localLongitude << "==" << coordinate.longitude() << "\n"
                             << localTimestamp << "==" << timestamp << "\n"
                             << localProperties << "==" << properties;

        emit localActionDataError(packageId, action);
    } else {
        emit localActionAddRequest(packageId, action, coordinate, timestamp, properties);
    }
}

QString DataLocalManager::filePath(const QString &packageId, const Enums::SupplyChainAction &action) const
{
    return c_dataDir.filePath(QUrl::toPercentEncoding(
                                  LocalDataFileName.arg(packageId).arg(static_cast<int>(action))));
}
