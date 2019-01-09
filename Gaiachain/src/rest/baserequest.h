#ifndef BASEREQUEST_H
#define BASEREQUEST_H

#include "mrestrequest.h"

class BaseRequest : public MRestRequest
{
    Q_OBJECT
public:
    explicit BaseRequest(const QString &method, const QString &token = QString());
    void setMethod(const QString &apiMethodPath);

signals:
    void requestFinished(const QJsonDocument &doc) const;

protected:
    const QString mToken;
    QString mApiMethod;

    virtual void customizeRequest(QNetworkRequest &request) Q_DECL_OVERRIDE;
    virtual void parse() Q_DECL_OVERRIDE;
};

#endif // BASEREQUEST_H
