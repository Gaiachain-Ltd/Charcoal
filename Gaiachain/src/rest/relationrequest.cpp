#include "relationrequest.h"

#include <QDebug>
#include <QJsonObject>

#include "../common/logs.h"
#include "../common/tags.h"
#include "../common/dataglobals.h"
#include "../helpers/requestshelper.h"

const QString RelationRequest::sc_basePath = QStringLiteral("/relations/");
const QString RelationRequest::sc_multiplePath = sc_basePath + QStringLiteral("batch/");
const QString RelationRequest::sc_singlePath = sc_basePath + QStringLiteral("%1/");

RelationRequest::RelationRequest(const QStringList &ids)
    : BaseRequest(sc_multiplePath, Type::Get)
{
    auto docObj = QJsonObject{};
    if (ids.isEmpty()) {
        docObj.insert(Tags::packageIds, StaticValues::all);
    } else {
        docObj.insert(Tags::packageIds, QJsonValue::fromVariant(ids));
    }

    mRequestDocument.setObject(docObj);
}

RelationRequest::RelationRequest(const QString &id)
    : BaseRequest(sc_singlePath.arg(id), Type::Get)
{
}

RelationRequest::RelationRequest(const QString &token, const QString &id, const QStringList &relatedIds)
    : BaseRequest(sc_singlePath.arg(id), Type::Post, token)
{
    auto docObj = QJsonObject{};
    if (relatedIds.isEmpty()) {
        qCWarning(sessionRequest) << "Missing related ids to add!";
    } else if (relatedIds.count() == 1) {
        docObj.insert(Tags::packageId, relatedIds.first());
    } else {
        docObj.insert(Tags::packageIds, QJsonValue::fromVariant(relatedIds));
    }

    mRequestDocument.setObject(docObj);
}
