#ifndef ENTITYREQUEST_H
#define ENTITYREQUEST_H

#include "baserequest.h"

#include <QObject>

class EntityRequest : public BaseRequest
{
    Q_OBJECT
public:
    enum RequestType {
        RequestGet = 0,
        RequestBatch,
        RequestCalendar,
        RequestUninitializedGet,
        RequestUninitializedPost,
        RequestEntityGet,
        RequestEntityPut
    };
    Q_ENUM(RequestType)

    enum EntityAction {
        EntityInitialize = 0,
        EntityDepart,
        EntityArrive,
        EntityFinalize
    };
    Q_ENUM(EntityAction)

    EntityRequest(const QString &token, const int requestType = RequestType::RequestGet);
    EntityRequest(const QString &token, const int count, const QString &type);
    EntityRequest(const QString &token, const QString &id);
    EntityRequest(const QString &token, const QString &id, const EntityAction action);
    EntityRequest(const QString &token, const QString &dateFrom, const QString &dateTo);

private:
    RequestType m_requestType = RequestType::RequestGet;

protected:
    virtual bool isTokenRequired() const Q_DECL_OVERRIDE;
    virtual void parse() Q_DECL_OVERRIDE final;
};


#endif // ENTITYREQUEST_H
