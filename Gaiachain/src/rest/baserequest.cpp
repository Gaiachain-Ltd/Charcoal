#include "baserequest.h"

#include <QMetaEnum>

#include "../common/globals.h"
#include "../common/tags.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(sessionRequest, "session.request")

BaseRequest::BaseRequest(const QString &path, const Type &type, const QString &token)
    : MRestRequest()
    , mToken(token)
{
    setObjectName(QStringLiteral("%1 [%2]").arg(path)
                  .arg(QMetaEnum::fromType<Type>().valueToKey(static_cast<int>(type))));

    mType = type;
    setPath(path);
    setPriority(Priority::Normal);
}

void BaseRequest::setToken(const QString &token)
{
    mToken = token;
}

QString BaseRequest::path() const
{
    return mPath;
}

void BaseRequest::setPath(const QString &path)
{
    Q_ASSERT_X(!path.isEmpty(), __PRETTY_FUNCTION__, "Path address not provided!");
    mPath = path;
    mUrl.setUrl(SERVER_ADDRESS + API_PATH + path);
}

void BaseRequest::setQuery(const QUrlQuery &query)
{
    mUrl.setQuery(query);
}

void BaseRequest::customizeRequest(QNetworkRequest &request)
{
    Q_ASSERT_X(!isTokenRequired() || !mToken.isEmpty(), objectName().toLatin1(),
               "This request require token and it's not provided!");

    if (!mToken.isEmpty()) {
        request.setRawHeader(QByteArray("Authorization"), QStringLiteral("%1 %2").arg(Tags::bearerHeader, mToken).toLatin1());
    }
}

void BaseRequest::parse()
{
    if (mElapsedTimer.isValid()) {
        qCDebug(sessionRequest) << "Request (" << objectName() << ") finished. Elapsed time:" << mElapsedTimer.elapsed();
    }

    emit requestFinished(mReplyDocument);
}

bool BaseRequest::isTokenRequired() const
{
    return (mType != Type::None &&
            mType != Type::Get);
}
