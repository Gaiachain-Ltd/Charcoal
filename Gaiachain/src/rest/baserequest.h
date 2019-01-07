#ifndef BASEREQUEST_H
#define BASEREQUEST_H

#include "mrestrequest.h"

class BaseRequest : public MRestRequest
{
    Q_OBJECT
public:
    explicit BaseRequest(const QString &method);
    void setMethod(const QString &apiMethodPath);

protected:
    QString mApiMethod;
};

#endif // BASEREQUEST_H
