#pragma once

#include "rest/baserequest.h"

class ImageRequest : public BaseRequest
{
public:
    ImageRequest(const QString &filePath, const QString &cachePath);

protected:
    void setPath(const QString &path) override;
    bool isTokenRequired() const override;
    void parse() override;
    void readReplyData(const QString &requestName, const QString &status) override;

private:
    QString mCachePath;
};

