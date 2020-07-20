#pragma once

#include "querymodel.h"
#include "dbobjects.h"

#include <QGeoCoordinate>

class UnusedHarvestIdsModel : public QueryModel
{
    Q_OBJECT

    Q_PROPERTY(bool hasPausedEvent READ hasPausedEvent NOTIFY hasPausedEventChanged)

public:
    explicit UnusedHarvestIdsModel(QObject *parent = nullptr);

    bool hasPausedEvent() const;

    Q_INVOKABLE void checkForPausedEvent();

    Q_INVOKABLE QString harvestId() const;
    Q_INVOKABLE QGeoCoordinate location() const;
    Q_INVOKABLE QString plateNumber() const;
    Q_INVOKABLE QString destination() const;
    Q_INVOKABLE QVariantList scannedQrs() const;
    Q_INVOKABLE QDateTime loadingDate() const;

signals:
    void hasPausedEventChanged(const bool hasPausedEvent) const;

public slots:
    void refreshWebData() override;

private:
    bool m_hasPausedEvent = false;
    Event m_pausedEvent;
    Entity m_pausedEntity;
};

