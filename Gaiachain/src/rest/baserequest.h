#ifndef BASEREQUEST_H
#define BASEREQUEST_H

#include "mrestrequest.h"
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(sessionRequest)

class BaseRequest : public MRestRequest
{
    Q_OBJECT
public:
    explicit BaseRequest(const QString &method, const QString &token = QString());

signals:
    void requestFinished(const QJsonDocument &doc) const;

private:
    void setMethod(const QString &apiMethodPath);

protected:
    const QString mToken;
    QString mApiMethod;

    virtual bool isTokenRequired() const;
    virtual void customizeRequest(QNetworkRequest &request) Q_DECL_OVERRIDE;
    virtual void parse() Q_DECL_OVERRIDE;
};

#endif // BASEREQUEST_H
