#pragma once

#include "querymodel.h"

class ReplantationsSender : public QueryModel
{
    Q_OBJECT

public:
    explicit ReplantationsSender(QObject *parent = nullptr);

public slots:
    void sendEvents();

protected slots:
    void webErrorHandler(const QString &errorString,
                         const QNetworkReply::NetworkError code) override;
    void webReplyHandler(const QJsonDocument &reply) override;
};

