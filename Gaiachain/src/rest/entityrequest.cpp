#include "entityrequest.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "../common/tags.h"

#define ADDRESS QStringLiteral("/entities/")
#define ADDRESS_DATA QStringLiteral("%1%2/").arg(ADDRESS)

EntityRequest::EntityRequest(const QString &token)
    : BaseRequest(ADDRESS, token)
{
    if (!token.isEmpty()) {
        mType = Type::Get;
    } else {
        qCritical() << "Error: missing entities GET token";
    }
}

EntityRequest::EntityRequest(const QString &token, const int count, const QString &type)
    : BaseRequest(ADDRESS, token)
{
    if (!token.isEmpty() && count > 0 && !type.isEmpty()) {
        QJsonObject object;
        object.insert(Tags::count, QJsonValue(count));
        object.insert(Tags::commodityType, QJsonValue(type));
        mRequestDocument.setObject(object);

        mType = Type::Post;

    } else {
        qCritical() << "Error: missing entities POST info" << count << type.length();
    }
}

EntityRequest::EntityRequest(const QString &token, const QString &id)
    : BaseRequest(ADDRESS_DATA.arg(id), token)
{
    if (!token.isEmpty() && !id.isEmpty()) {
        mType = Type::Get;
    } else {
        qCritical() << "Error: missing entity GET info" << id;
    }
}

bool EntityRequest::isTokenRequired() const
{
    return true;
}


void EntityRequest::parse()
{
    qDebug() << "-------- ENTITITES" << mReplyDocument;

    if (mReplyDocument.isArray()) {
        QJsonArray array = mReplyDocument.array();
        emit checkEntity(array.first().toObject().value(Tags::id).toString());
    }
}
