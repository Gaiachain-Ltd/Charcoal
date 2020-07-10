#pragma once

#include "querymodel.h"

#include <QPointer>

class PicturesManager;

class EventsSender : public QueryModel
{
    Q_OBJECT
public:
    explicit EventsSender(QObject *parent = nullptr);

    void setPicturesManager(PicturesManager *manager);

public slots:
    void sendEvents();

    void onFinalizePackage(const int webId);
    void onFinalizePackages(const QVector<int> &webIds);

protected slots:
    void webErrorHandler(const QString &errorStringString,
                         const QNetworkReply::NetworkError code) override;
    void webReplyHandler(const QJsonDocument &reply) override;
    void finalizationReplyHandler(const QJsonDocument &reply);

    void onFetchPhoto(const QString &path);

private:
    bool updateEntityWebId(const qint64 webId, const int eventId) const;
    QJsonObject dbMapToWebObject(QJsonObject object, const int entityId) const;

    QPointer<PicturesManager> m_picturesManager;
};

