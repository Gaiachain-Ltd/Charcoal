#ifndef SHIPMENTEVENTSPROXY_H
#define SHIPMENTEVENTSPROXY_H

#include <QSortFilterProxyModel>

class ShipmentEventsProxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ShipmentEventsProxy(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SHIPMENTEVENTSPROXY_H
