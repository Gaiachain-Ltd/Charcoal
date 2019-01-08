#include "loginrequest.h"

#include <QJsonObject>
#include <QLoggingCategory>
#include <QDebug>

Q_LOGGING_CATEGORY(requestLogin, "request.login")

LoginRequest::LoginRequest(const QString &email, const QString &password)
    : BaseRequest(QStringLiteral("/auth/login/"))
{
    if (!email.isEmpty() && !password.isEmpty()) {
        QJsonObject object;
        object.insert(QStringLiteral("email"), QJsonValue(email));
        object.insert(QStringLiteral("password"), QJsonValue(password));
        mRequestDocument.setObject(object);

        mType = Type::Post;

    } else {
        qCDebug(requestLogin) << "Error: missing login info"
                                 << email << password.length();
    }
}

void LoginRequest::parse()
{
    emit requestFinished(mReplyDocument);
}
