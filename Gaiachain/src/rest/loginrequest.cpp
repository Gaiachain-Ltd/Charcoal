#include "loginrequest.h"

#include <QJsonObject>
#include <QLoggingCategory>

#include "../common/logs.h"
#include "../common/tags.h"

LoginRequest::LoginRequest(const QString &email, const QString &password)
    : BaseRequest(QStringLiteral("/auth/login/"), Type::Post)
{
    if (!email.isEmpty() && !password.isEmpty()) {
        mRequestDocument.setObject({ { Tags::email, email },
                                     { Tags::password, password } });
    } else {
        qCCritical(sessionRequest) << "Error: missing login info"
                                   << email << password.length();
    }
}

bool LoginRequest::isTokenRequired() const
{
    return false;
}
