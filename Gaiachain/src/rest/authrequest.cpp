#include "authrequest.h"

#include <QJsonObject>
#include <QLoggingCategory>

#include "../common/logs.h"
#include "../common/tags.h"

namespace {
const auto PING_PATH = QStringLiteral("/ping/");
const auto LOGIN_PATH = QStringLiteral("/auth/login/");
}

AuthRequest::AuthRequest()
    : BaseRequest(PING_PATH, Type::Get)
{}

AuthRequest::AuthRequest(const QString &email, const QString &password)
    : BaseRequest(LOGIN_PATH, Type::Post)
{
    if (!email.isEmpty() && !password.isEmpty()) {
        mRequestDocument.setObject({ { Tags::email, email },
                                     { Tags::password, password } });
    } else {
        qCCritical(sessionRequest) << "Error: missing login info"
                                   << email << password.length();
    }
}

bool AuthRequest::isTokenRequired() const
{
    return false;
}
