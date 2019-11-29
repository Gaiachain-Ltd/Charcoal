#ifndef AUTHREQUEST_H
#define AUTHREQUEST_H

#include "baserequest.h"

#include <QObject>

class AuthRequest : public BaseRequest
{
    Q_OBJECT
public:
    AuthRequest();
    AuthRequest(const QString &login, const QString &password);

    bool isTokenRequired() const override;
};

#endif // AUTHREQUEST_H
