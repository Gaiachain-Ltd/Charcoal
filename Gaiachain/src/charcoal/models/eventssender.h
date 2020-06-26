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
    void webErrorHandler(const QString &errorStringString,
                         const QNetworkReply::NetworkError code) override;
    void webReplyHandler(const QJsonDocument &reply) override;

private:
    QString getEventType(const QString &id) const;
    QString findEventByTimestamp(const qint64 timestamp) const;
    bool updateEntityWebId(const qint64 webId, const QString &eventId) const;
    QJsonObject dbStringToPropertiesObject(const QString &properties) const;
    QJsonObject dbMapToWebObject(QJsonObject object, const int entityId) const;
};

