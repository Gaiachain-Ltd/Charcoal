#include "baserequest.h"

#include "../common/globals.h"
#include "../common/tags.h"

BaseRequest::BaseRequest(const QString &method, const QString &token)
    : MRestRequest()
    , mToken(token)
{
    setMethod(method);
    setPriority(Priority::Normal);
}

void BaseRequest::setMethod(const QString &apiMethodPath)
{
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
