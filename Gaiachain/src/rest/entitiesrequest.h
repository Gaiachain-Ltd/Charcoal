#ifndef ENTITIESREQUEST_H
#define ENTITIESREQUEST_H

#include "baserequest.h"

#include <QObject>

class EntitiesRequest : public BaseRequest
{
    Q_OBJECT
public:
    EntitiesRequest(const QString &token);
    EntitiesRequest(const QString &token, const int count, const QString &type);

protected:
    virtual void parse() Q_DECL_OVERRIDE final;
};


#endif // ENTITIESREQUEST_H
