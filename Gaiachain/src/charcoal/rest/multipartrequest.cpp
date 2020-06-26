#include "multipartrequest.h"

#include <QDebug>

#include <QHttpMultiPart>
#include <QHttpPart>

MultiPartRequest::MultiPartRequest(const QString &path, const Type &type, const QString &token)
    : BaseRequest(path, type, token)
{
}

void MultiPartRequest::addPart(const QString &key, const QString &value)
{
    qDebug() << "Adding part" << key << value;
    m_stringParts.insert(key, value);
}

void MultiPartRequest::addPart(const QString &key, const QFileInfo &file)
{
    qDebug() << "Adding part" << key << file.fileName();
    m_fileParts.insert(key, file);
}

bool MultiPartRequest::isMultiPart() const
{
    return true;
}

QHttpMultiPart* MultiPartRequest::requestMultiPart() const
{
    // Adapted from:
    // https://stackoverflow.com/questions/38179706/uploading-a-file-in-multipart-form-data-in-qt5

    // TODO: we can probably parent those things to the request itself.

    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    for (const QString &key : m_stringParts.uniqueKeys()) {
        for (const QString &value : m_stringParts.values(key)) {
            QHttpPart part;
            part.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"" + key + "\""));
            // TODO: this might cause us to lose UTF-8!
            part.setBody(value.toLocal8Bit());
            multiPart->append(part);
        }
    }

    for (const QString &key : m_fileParts.uniqueKeys()) {
        for (const QFileInfo &value : m_stringParts.values(key)) {
            QHttpPart part;
            part.setHeader(QNetworkRequest::ContentDispositionHeader,
                           QVariant("form-data; name=\"file\"; filename=\""
                                    + value.fileName() + "\""));
            auto file = new QFile(value.absoluteFilePath());
            file->setParent(multiPart);
            part.setBodyDevice(file);
            multiPart->append(part);
        }
    }

    return multiPart;
}
