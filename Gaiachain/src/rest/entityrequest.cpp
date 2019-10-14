#include "entityrequest.h"

#include <QJsonObject>
#include <QJsonArray>

#include "../common/logs.h"
#include "../common/dataglobals.h"
#include "../common/tags.h"

#define ADDRESS_BASE QStringLiteral("/entities")
#define ADDRESS QStringLiteral("/entities/")
#define ADDRESS_DATA QStringLiteral("%1/%2/").arg(ADDRESS_BASE)
#define ADDRESS_UNINITIALIZED QStringLiteral("%1/uninitialized/").arg(ADDRESS_BASE)
#define ADDRESS_CALENDAR QStringLiteral("%1/calendar/").arg(ADDRESS_BASE)
#define ADDRESS_BATCH QStringLiteral("%1/batch/").arg(ADDRESS_BASE)

EntityRequest::EntityRequest(const QString &token, const RequestType requestType)
    : BaseRequest(requestType == RequestType::RequestUninitializedGet ? ADDRESS_UNINITIALIZED : ADDRESS, token)
    , m_requestType(static_cast<RequestType>(requestType))
{
    mType = Type::Get;
}

EntityRequest::EntityRequest(const QString &token, const int count, const QString &type)
    : BaseRequest(ADDRESS_UNINITIALIZED, token)
    , m_requestType(RequestUninitializedPost)
{
    if (!token.isEmpty() && count > 0 && !type.isEmpty()) {
        m_timer.start();
        QJsonObject object;
        object.insert(Tags::count, QJsonValue(count));
        mRequestDocument.setObject(object);

        mType = Type::Post;
    } else {
        qCCritical(sessionRequest) << "Error: missing entities POST info" << count << type.length();
    }
}

EntityRequest::EntityRequest(const QString &token, const QString &id)
    : BaseRequest(ADDRESS_DATA.arg(id), token)
    , m_requestType(RequestEntityGet)
{
    if (!id.isEmpty()) {
        m_timer.start();
        mType = Type::Get;
    } else {
        qCCritical(sessionRequest) << "Error: missing entity GET info" << id;
    }
}

EntityRequest::EntityRequest(const QString &token, const QJsonArray &ids)
    : BaseRequest(ADDRESS_BATCH, token)
    , m_requestType(RequestBatch)
{
    if (!ids.isEmpty()) {
        m_timer.start();
        QJsonObject object;
        object.insert(Tags::ids, QJsonValue(ids));
        mRequestDocument.setObject(object);
        mType = Type::Post;
    } else {
        qCCritical(sessionRequest) << "Error: missing entity GET info" << ids;
    }
}

EntityRequest::EntityRequest(const QString &token, const QString &dateFrom, const QString &dateTo)
    : BaseRequest(ADDRESS_CALENDAR, token)
    , m_requestType(RequestCalendar)
{
    m_timer.start();
    QJsonObject object;
    if (!dateFrom.isEmpty()) {
        const int timestampFrom = dateFrom.toInt();
        if (timestampFrom > 0) {
            object.insert(Tags::timestampFrom, QJsonValue(timestampFrom));
        } else {
            object.insert(Tags::dateFrom, QJsonValue(dateFrom));
        }
    }
    if (!dateTo.isEmpty()) {
        const int timestampTo = dateFrom.toInt();
        if (timestampTo > 0) {
            object.insert(Tags::timestampTo, QJsonValue(timestampTo));
        } else {
            object.insert(Tags::dateTo, QJsonValue(dateTo));
        }
        mRequestDocument.setObject(object);
    }

    mType = Type::Get;
}

EntityRequest::EntityRequest(const QString &token, const QString &id, const Enums::SupplyChainAction action, const Enums::ActionProgress actionProgress)
    : BaseRequest(ADDRESS_DATA.arg(id), token)
    , m_requestType(RequestEntityPut)
{
    if (!token.isEmpty() && !id.isEmpty()) {
        m_timer.start();
        QJsonObject object;
        QString actionString;
        object.insert(Tags::action, DataGlobals::supplyChainActionToString(action));
        object.insert(Tags::actionProgress, DataGlobals::actionProgressToString(actionProgress));
        mRequestDocument.setObject(object);

        mType = Type::Put;
    } else {
        qCCritical(sessionRequest) << "Error: missing entity PUT info" << id << action;
    }

}

void EntityRequest::parse()
{
    if (m_timer.isValid())
        qCDebug(sessionRequest) << "Request" << m_requestType << "elapsed time:" << m_timer.elapsed();
    BaseRequest::parse();
}
