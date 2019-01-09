#include "entitiesrequest.h"

#include <QJsonObject>
#include <QLoggingCategory>
#include <QDebug>

#include "../common/tags.h"

#define ADDRESS QStringLiteral("/entities/")

EntitiesRequest::EntitiesRequest(const QString &token)
    : BaseRequest(ADDRESS, token)
{
    if (!token.isEmpty()) {
        mType = Type::Get;
    } else {
        qCritical() << "Error: missing entities GET token";
    }
}

EntitiesRequest::EntitiesRequest(const QString &token, const int count, const QString &type)
    : BaseRequest(ADDRESS, token)
{
    if (!token.isEmpty() && count > 0 && !type.isEmpty()) {
        QJsonObject object;
        object.insert(Tags::count, QJsonValue(count));
        object.insert(Tags::commodityType, QJsonValue(type));
        mRequestDocument.setObject(object);

        mType = Type::Post;

    } else {
        qCritical() << "Error: missing entities POST info"
                                 << count << type.length();
    }
}

void EntitiesRequest::parse()
{
    qDebug() << "-------- ENTITITES" << mReplyDocument;
}
