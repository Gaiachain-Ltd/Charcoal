#include "imagerequest.h"

#include "common/globals.h"
#include "common/logs.h"

#include <QFile>
#include <QFileInfo>

ImageRequest::ImageRequest(const QString &filePath, const QString &cachePath)
    : BaseRequest(filePath, BaseRequest::Type::Get, QString()),
      mCachePath(cachePath)
{
    setPath(filePath);
}

void ImageRequest::setPath(const QString &path)
{
    Q_ASSERT_X(!path.isEmpty(), __PRETTY_FUNCTION__, "Path address not provided!");

    mPath = path;
    mUrl.setUrl(SERVER_ADDRESS + mPath);
}

bool ImageRequest::isTokenRequired() const
{
    return false;
}

void ImageRequest::parse()
{
    if (mElapsedTimer.isValid()) {
        qCDebug(sessionRequest) << "Request (" << objectName()
                                << ") finished. Elapsed time:"
                                << mElapsedTimer.elapsed();
    }

    emit requestFinished(mReplyDocument);
}

void ImageRequest::readReplyData(const QString &requestName, const QString &status)
{
    QFile file(mCachePath + "/" + QFileInfo(mPath).fileName());
    if (file.open(QFile::WriteOnly) == false) {
        qDebug() << RED("Could not open file for writing") << file.fileName()
                 << requestName << status;
        return;
    }

    qDebug() << "Writing file" << file.fileName()
             << mPath;
    file.write(mReplyData);
}
