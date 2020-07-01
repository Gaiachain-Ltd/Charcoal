#include "imagerequest.h"

#include "common/globals.h"
#include "common/logs.h"

#include <QDateTime>
#include <QFile>

ImageRequest::ImageRequest(const QString &fileName, const QString &cachePath)
    : BaseRequest("/media/photos", BaseRequest::Type::Get, QString()),
      mFileName(fileName),
      mCachePath(cachePath)
{
    setPath(mPath);
}

void ImageRequest::setPath(const QString &path)
{
    Q_ASSERT_X(!path.isEmpty(), __PRETTY_FUNCTION__, "Path address not provided!");

    if (mFileName.isEmpty()) {
        mPath = path;
    } else {
        QString justDateTime(mFileName);
        justDateTime = justDateTime.mid(mFileName.indexOf('-', 0) + 1);
        justDateTime.truncate(justDateTime.lastIndexOf('.'));

        const QDateTime timestamp(QDateTime::fromString(justDateTime, "yyyy-MM-ddTHHmmss"));

        mPath = QString("%1/%2/%3").arg(path, timestamp.toString("yyyy/MM/dd"),
                                        mFileName);
        mUrl.setUrl(SERVER_ADDRESS + mPath);
    }
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
}

void ImageRequest::readReplyData(const QString &requestName, const QString &status)
{
    QFile file(mCachePath + "/" + mFileName);
    if (file.open(QFile::WriteOnly) == false) {
        qDebug() << RED("Could not open file for writing") << file.fileName()
                 << requestName << status;
        return;
    }

    qDebug() << "Writing file" << file.fileName()
             << mPath << mFileName;
    file.write(mReplyData);
}
