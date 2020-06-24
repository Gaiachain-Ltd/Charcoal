#pragma once

#include "querymodel.h"

class EventsSender : public QueryModel
{
    Q_OBJECT
public:
    explicit EventsSender(QObject *parent = nullptr);

public slots:
    void sendEvents();

protected slots:
    void webErrorHandler(const QString &error,
                         const QNetworkReply::NetworkError code) override;
    void webReplyHandler(const QJsonDocument &reply) override;

private:
    QString getEventType(const QString &id) const;
    QString getEntityName(const QString &id) const;
    QJsonObject dbMapToWebObject(const QString &properties) const;
};

