#pragma once

#include "mrestrequest.h"

#include <QObject>
#include <QUrl>

class TestRequest : public MRestRequest
{
    Q_OBJECT

public:
    TestRequest(const QUrl& url);

protected:
    void parse() override;
};
