#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>

#include "../models/eventmodel.h"
#include "../models/shipmentmodel.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);

signals:

public slots:

private:
    EventModel m_eventModel;
    ShipmentModel m_shipmentModel;
};

#endif // DATAMANAGER_H
