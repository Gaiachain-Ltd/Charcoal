#ifndef ENTITYREQUEST_H
#define ENTITYREQUEST_H

#include "baserequest.h"

#include <QObject>

class EntityRequest : public BaseRequest
{
    Q_OBJECT
public:
    EntityRequest(const QString &token);
    EntityRequest(const QString &token, const int count, const QString &type);
    EntityRequest(const QString &token, const QString &id);

signals:
    void checkEntity(const QString &id) const;

protected:
    virtual bool isTokenRequired() const Q_DECL_OVERRIDE;
    virtual void parse() Q_DECL_OVERRIDE final;
};


#endif // ENTITYREQUEST_H
