#ifndef LOGINREQUEST_H
#define LOGINREQUEST_H

#include "BaseRequest.h"

#include <QObject>

class LoginRequest : public BaseRequest
{
    Q_OBJECT
public:
    LoginRequest(const QString &email, const QString &password);

private slots:
    void onError(const QString &error, const int errorCode);

protected:
    virtual void parse() override final;
};

#endif // LOGINREQUEST_H
