#ifndef LATESTEVENTSPROXYMODEL_H
#define LATESTEVENTSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class LatestEventsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LatestEventsProxyModel(QObject *parent = nullptr);
};

#endif // LATESTEVENTSPROXYMODEL_H
