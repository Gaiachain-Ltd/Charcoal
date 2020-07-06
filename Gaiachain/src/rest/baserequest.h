#ifndef BASEREQUEST_H
#define BASEREQUEST_H

#include "mrestrequest.h"

#include <QLoggingCategory>
#include <QElapsedTimer>

Q_DECLARE_LOGGING_CATEGORY(sessionRequest)

class BaseRequest : public MRestRequest
{
    Q_OBJECT
public:
    BaseRequest(const QString &path, const Type &type, const QString &token = QString());

    void setToken(const QString &token);
    QString path() const;

signals:
    void requestFinished(const QJsonDocument &doc) const;

protected:
    QString mToken;
    QString mPath;

    QElapsedTimer mElapsedTimer;

    virtual void setPath(const QString &path);
    void setQuery(const QUrlQuery &query);

    virtual bool isTokenRequired() const;
    virtual void customizeRequest(QNetworkRequest &request) override;
    virtual void parse() override;
};

#endif // BASEREQUEST_H
