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

    static Q_INVOKABLE bool isNetworkError(const QNetworkReply::NetworkError &error);
    static Q_INVOKABLE bool isServerError(const QNetworkReply::NetworkError &error);
    static Q_INVOKABLE bool isAuthenticationError(const QNetworkReply::NetworkError &error);

    static Enums::UserType userTypeFromString(const QString &text);
    static QString userTypeToString(const Enums::UserType &userType);

    static Enums::PackageType packageTypeFromString(const QString &text);
    static QString packageTypeToString(const Enums::PackageType &packageType);

    static Enums::SupplyChainAction supplyChainActionFromString(const QString &text);
    static QString supplyChainActionToString(const Enums::SupplyChainAction &action);

private:
    RequestsHelper();

    static const QHash<Enums::UserType, QString> sc_userTypeStrings;
    static const QHash<Enums::PackageType, QString> sc_packageTypeStrings;
    static const QHash<Enums::SupplyChainAction, QString> sc_supplyChainActionStrings;
};

#endif // REQUESTSHELPER_H
