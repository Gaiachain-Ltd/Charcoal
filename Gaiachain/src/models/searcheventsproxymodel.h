#ifndef SEARCHEVENTSPROXYMODEL_H
#define SEARCHEVENTSPROXYMODEL_H

#include "abstractsortfilterproxymodel.h"

class SearchEventsProxyModel : public AbstractSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString keyword READ keyword NOTIFY keywordChanged)

public:
    explicit SearchEventsProxyModel(QObject *parent = nullptr);

    Q_INVOKABLE void search(const QString &keyword);
    Q_INVOKABLE void clearSearch();

    QString keyword() const;

signals:
    void keywordChanged(QString keyword);

private:
    QString m_keyword;
};

#endif // SEARCHEVENTSPROXYMODEL_H
