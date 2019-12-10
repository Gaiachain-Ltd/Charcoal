#include "requestshelper.h"

#include <QJsonObject>
#include <QJsonValue>

#include "../common/globals.h"
#include "../common/tags.h"
#include "../common/types.h"

namespace {
const QHash<Enums::UserType, QString> UserTypeStrings = {
    { Enums::UserType::SuperUser, StaticValues::userSuperUser },
    { Enums::UserType::Inspector, StaticValues::userInspector },
    { Enums::UserType::PCA, StaticValues::userPca },
    { Enums::UserType::Warehouseman, StaticValues::userWarehouseman },
    { Enums::UserType::CooperativeRepresentative, StaticValues::userCooperativeRepresentative }
};
const QHash<Enums::CompanyType, QString> CompanyTypeStrings = {
    { Enums::CompanyType::Cooperative, StaticValues::companyCooperative },
    { Enums::CompanyType::Buyer, StaticValues::companyBuyer },
    { Enums::CompanyType::Transporter, StaticValues::companyTransporter }
};
const QHash<Enums::PackageType, QString> PackageTypeStrings = {
    { Enums::PackageType::Harvest, StaticValues::packageHarvest },
    { Enums::PackageType::Sac, StaticValues::packageSac },
    { Enums::PackageType::Lot, StaticValues::packageLot }
};
const QHash<Enums::SupplyChainAction, QString> SupplyChainActionStrings = {
    { Enums::SupplyChainAction::Harvest, StaticValues::actionHarvest },
    { Enums::SupplyChainAction::GrainProcessing, StaticValues::actionGrainProcessing },
    { Enums::SupplyChainAction::SectionReception, StaticValues::actionSectionReception },
    { Enums::SupplyChainAction::Bagging, StaticValues::actionBagging },
    { Enums::SupplyChainAction::LotCreation, StaticValues::actionLotCreation },
    { Enums::SupplyChainAction::WarehouseTransport, StaticValues::actionWarehouseTransport },
    { Enums::SupplyChainAction::ExportReception, StaticValues::actionExportReception }
};
}

RequestsHelper &RequestsHelper::instance()
{
    static RequestsHelper rh;
    return rh;
}

QNetworkReply::NetworkError RequestsHelper::authenticationError()
{
    return QNetworkReply::NetworkError::AuthenticationRequiredError;
}

QNetworkReply::NetworkError RequestsHelper::actionDuplicatedError()
{
    return QNetworkReply::NetworkError::ContentConflictError;
}

bool RequestsHelper::isNetworkError(const QNetworkReply::NetworkError &error)
{
    switch (error) {
    case QNetworkReply::RemoteHostClosedError:
    case QNetworkReply::HostNotFoundError:
    case QNetworkReply::TemporaryNetworkFailureError:
    case QNetworkReply::NetworkSessionFailedError:
        return true;
    default:
        ;
    }
    return false;
}

bool RequestsHelper::isServerError(const QNetworkReply::NetworkError &error)
{
    switch (error) {
    case QNetworkReply::ConnectionRefusedError:
    case QNetworkReply::RemoteHostClosedError:
    case QNetworkReply::InternalServerError:
    case QNetworkReply::ServiceUnavailableError:
    case QNetworkReply::UnknownNetworkError:
    case QNetworkReply::UnknownServerError:
        return true;
    default:
        ;
    }
    return false;
}

bool RequestsHelper::isAuthenticationError(const QNetworkReply::NetworkError &error)
{
    return (error == QNetworkReply::NetworkError::AuthenticationRequiredError);
}

bool RequestsHelper::isActionMissingError(const QNetworkReply::NetworkError &error)
{
    return (error == QNetworkReply::NetworkError::ContentNotFoundError);
}

bool RequestsHelper::isActionDuplicatedError(const QNetworkReply::NetworkError &error)
{
    return (error == QNetworkReply::NetworkError::ContentConflictError);
}

QJsonValue RequestsHelper::checkAndValue(const QJsonObject &object, const QLatin1String tag)
{
    if (!object.contains(tag)) {
        qCWarning(dataModels) << "Tag" << tag << "is missing in an object data!";
        return {};
    }
    return object.value(tag);
}

Enums::CompanyType RequestsHelper::companyTypeFromString(const QString &text)
{
    return CompanyTypeStrings.key(text);
}

QString RequestsHelper::companyTypeToString(const Enums::CompanyType &companyType)
{
    return CompanyTypeStrings.value(companyType);
}

Enums::UserType RequestsHelper::userTypeFromString(const QString &text)
{
    return UserTypeStrings.key(text, Enums::UserType::Annonymous);
}

QString RequestsHelper::userTypeToString(const Enums::UserType &userType)
{
    return UserTypeStrings.value(userType);
}

Enums::PackageType RequestsHelper::packageTypeFromString(const QString &text)
{
    return PackageTypeStrings.key(text, Enums::PackageType::Unknown);
}

QString RequestsHelper::packageTypeToString(const Enums::PackageType &packageType)
{
    return PackageTypeStrings.value(packageType);
}

Enums::SupplyChainAction RequestsHelper::supplyChainActionFromString(const QString &text)
{
    return SupplyChainActionStrings.key(text, Enums::SupplyChainAction::Unknown);
}

QString RequestsHelper::supplyChainActionToString(const Enums::SupplyChainAction &action)
{
    return SupplyChainActionStrings.value(action);
}

QVariantMap RequestsHelper::convertProperties(const QVariantMap &properties)
{
    static const auto ConversionTypes = QMap<QMetaType::Type, QMetaType::Type>{
        { QMetaType::QDateTime, QMetaType::LongLong },
        { QMetaType::QDate, QMetaType::LongLong }
    };

    auto updatedProperties = properties;
    for (const auto &key : updatedProperties.keys()) {
        auto &value = updatedProperties[key];

        auto metaType = static_cast<QMetaType::Type>(value.type());
        if (ConversionTypes.contains(metaType)) {
            types::convert(value, ConversionTypes.value(metaType));
        }
    }

    return updatedProperties;
}

RequestsHelper::RequestsHelper()
    : QObject()
{}
