#ifndef REQUESTSHELPER_H
#define REQUESTSHELPER_H

#include <QObject>
#include <QString>
#include <QMetaEnum>
#include <QDate>
#include <QNetworkReply>

#include "../common/enums.h"

class RequestsHelper : public QObject
{
    Q_OBJECT
public:
    static RequestsHelper &instance();

    static Q_INVOKABLE QNetworkReply::NetworkError authenticationError();
    static Q_INVOKABLE QNetworkReply::NetworkError actionDuplicatedError();

    static Q_INVOKABLE bool isOfflineError(const QNetworkReply::NetworkError &error);
    static Q_INVOKABLE bool isNetworkError(const QNetworkReply::NetworkError &error);
    static Q_INVOKABLE bool isServerError(const QNetworkReply::NetworkError &error);
    static Q_INVOKABLE bool isAuthenticationError(const QNetworkReply::NetworkError &error);
    static Q_INVOKABLE bool isActionMissingError(const QNetworkReply::NetworkError &error);
    static Q_INVOKABLE bool isActionDuplicatedError(const QNetworkReply::NetworkError &error);

    static QJsonValue checkAndValue(const QJsonObject &object, const QLatin1String tag);

    static Enums::UserType userTypeFromString(const QString &text);
    static QString userTypeToString(const Enums::UserType &userType);

    static Enums::CompanyType companyTypeFromString(const QString &text);
    static QString companyTypeToString(const Enums::CompanyType &companyType);

    static Enums::PackageType packageTypeFromString(const QString &text);
    static QString packageTypeToString(const Enums::PackageType &packageType);

    static Enums::SupplyChainAction supplyChainActionFromString(const QString &text);
    static QString supplyChainActionToString(const Enums::SupplyChainAction &action);

    static QVariantMap convertProperties(const QVariantMap &properties);

private:
    RequestsHelper();
};

#endif // REQUESTSHELPER_H
