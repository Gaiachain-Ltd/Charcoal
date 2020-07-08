#pragma once

#include "querymodel.h"
#include "common/enums.h"

#include <QTimer>
#include <QColor>
#include <QPointer>
#include <QVariantList>
#include <QJsonObject>

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

    struct Event;
    struct Oven {
        int id = -1;
        int typeId = -1;
        int plotId = -1;
        int carbonizationBeginningId = -1;
        int carbonizationEndingId = -1;
        qint64 carbonizationBeginning = -1;
        qint64 carbonizationEnding = -1;
        QString name;
        float height = -1.0;
        float width = -1.0;
        float length = -1.0;

        QString carbonizerId;

        float volume() const { return height * width * length; }

        void updateDates(const Oven &other, const Event &otherEvent);
    };

    struct Event {
        int id = -1;
        int typeId = -1;
        QString userId;
        qint64 timestamp = -1;
        qint64 date = -1;
        QJsonObject properties;

        Oven loadOven(const QString &connectionName) const;
    };

    struct Entity {
        int id = -1;
        int parent = -1;
        int typeId = -1;
        QString name;

        bool loadFromDb(const QString &connectionName, const int id);
        QVector<Event> loadEvents(const QString &connectionName) const;
    };

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
    QString dateString(const qint64 timestamp) const;

    void startPackageDetailsUpdate();

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

