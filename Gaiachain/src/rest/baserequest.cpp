#include "baserequest.h"

#include <QMetaObject>

#include "../common/globals.h"
#include "../common/tags.h"

BaseRequest::BaseRequest(const QString &method, const QString &token)
    : MRestRequest()
    , mToken(token)
{
    Q_ASSERT_X(isTokenRequired() ? token.isEmpty() == false : true,
               this->metaObject()->className(),
               "This request require token!");
    setMethod(method);
    setPriority(Priority::Normal);
}

void BaseRequest::setMethod(const QString &apiMethodPath)
{
    Q_ASSERT_X(apiMethodPath.isEmpty() == false,
               this->metaObject()->className(),
               "Method address not provided!");
    mApiMethod = apiMethodPath;
    setAddress(QUrl(SERVER_ADDRESS + mApiMethod));
}

void BaseRequest::customizeRequest(QNetworkRequest &request)
{
    if (!mToken.isEmpty())
        request.setRawHeader(QByteArray("Authorization"), QStringLiteral("%1 %2").arg(Tags::bearerHeader, mToken).toLatin1());
}

void BaseRequest::parse()
{
    emit requestFinished(mReplyDocument);
}

bool BaseRequest::isTokenRequired() const
{
    return false;
}
