#pragma once

#include "rest/baserequest.h"

#include <QObject>
#include <QFileInfo>
#include <QMultiHash>

class MultiPartRequest : public BaseRequest
{
    Q_OBJECT

public:
    explicit MultiPartRequest(const QString &path,
                              const Type &type,
                              const QString &token = QString());

    void addPart(const QString &key, const QString &value);
    void addPart(const QString &key, const QFileInfo &file);

protected:
    bool isMultiPart() const override;
    QHttpMultiPart* requestMultiPart() const override;

private:
    QMultiHash<QString, QString> m_stringParts;
    QMultiHash<QString, QFileInfo> m_fileParts;
};

