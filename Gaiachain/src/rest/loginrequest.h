#ifndef LOGINREQUEST_H
#define LOGINREQUEST_H

#include "baserequest.h"

#include <QObject>

class LoginRequest : public BaseRequest
{
    Q_OBJECT
public:
    LoginRequest(const QString &email, const QString &password);

    bool isTokenRequired() const override;
};

#endif // LOGINREQUEST_H
