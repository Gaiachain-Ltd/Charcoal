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
        connect(this, &LoginRequest::replyError, this, &LoginRequest::onError);

    } else {
        qCDebug(requestLogin) << "Error: missing login info"
                                 << email << password.length();
    }
}

void LoginRequest::parse()
{
//    const QJsonObject object(mReplyDocument.object());
}

void LoginRequest::onError(const QString &error, const int errorCode)
{
    Q_UNUSED(error)
    Q_UNUSED(errorCode)
}
