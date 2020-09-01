#pragma once

#include "querymodel.h"
#include "dbobjects.h"

#include <QTimer>
#include <QColor>
#include <QPointer>
#include <QVariantList>

class PicturesManager;
class TrackingModel;

class TrackingModel : public QueryModel
{
    Q_OBJECT

public:
    enum TrackingRole {
        Id = Qt::ItemDataRole::UserRole + 1,
        Name,
        Type,
        Events,
        EventSummary
    };
    Q_ENUM(TrackingRole)

    explicit TrackingModel(QObject *parent = nullptr);

    void setPicturesManager(PicturesManager *manager);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    QVariantList summaryForPlot(const Entity entity, const QVector<Event> &events) const;
    QVariantList summaryForHarvest(const Entity entity, const QVector<Event> &events) const;
    QVariantList summaryForTransport(const Entity entity, const QVector<Event> &events) const;

signals:
    void finalizePackages(const QVector<int> &webIds) const;

public slots:
    void refreshWebData() override;

protected slots:
    void webReplyHandler(const QJsonDocument &reply) override;
    void detailsReplyHandler(const QJsonDocument &reply);

private:
    QString dateString(const qint64 timestamp, const bool withTime = false) const;

    void startPackageDetailsUpdate();
    void getMissingPictures();

    const QHash<int, QByteArray> m_roleNames = {
        { TrackingRole::Id, "eventId" },
        { TrackingRole::Name, "name" },
        { TrackingRole::Type, "type" },
        { TrackingRole::Events, "events" },
        { TrackingRole::EventSummary, "eventSummary" }
    };

    const QColor m_plotTextColor = QColor("#2581EB");
    const QColor m_harvestTextColor = QColor("#65CB00");
    const QColor m_transportTextColor = QColor("#FF70F1");

    const QColor m_plotHighlightColor = QColor("#EEF9FE");
    const QColor m_harvestHighlightColor = QColor("#EBF1E6");
    const QColor m_transportHighlightColor = QColor("#FFE8FD");

    QPointer<PicturesManager> m_picturesManager;

    QTimer m_updateTimer;
};

