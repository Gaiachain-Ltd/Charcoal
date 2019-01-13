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
        RequestGetFiltered,
        RequestBatch,
        RequestCalendar,
        RequestUninitializedGet,
        RequestUninitializedPost, // not used (web part)
        RequestEntityGet,
        RequestEntityPut
    };
    Q_ENUM(RequestType)

    enum EntityAction {
        EntityArrived = 0,
        EntityDeparted
    };
    Q_ENUM(EntityAction)

    EntityRequest(const QString &token, const RequestType requestType = RequestType::RequestGet);
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
