#ifndef SEARCHEVENTSPROXYMODEL_H
#define SEARCHEVENTSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class SearchEventsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit SearchEventsProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void search(const QString &keyWord, bool exact = false);

private:
};

#endif // SEARCHEVENTSPROXYMODEL_H
