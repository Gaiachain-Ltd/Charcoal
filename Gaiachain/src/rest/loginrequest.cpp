#include "loginrequest.h"

#include <QJsonObject>
#include <QLoggingCategory>

#include "../common/logs.h"
#include "../common/tags.h"

LoginRequest::LoginRequest(const QString &email, const QString &password)
    : BaseRequest(QStringLiteral("/auth/login/"))
{
    if (!email.isEmpty() && !password.isEmpty()) {
        QJsonObject object;
        object.insert(Tags::email, QJsonValue(email));
        object.insert(Tags::password, QJsonValue(password));
        mRequestDocument.setObject(object);

        mType = Type::Post;

    } else {
        qCCritical(sessionRequest) << "Error: missing login info"
                                 << email << password.length();
    }
}
