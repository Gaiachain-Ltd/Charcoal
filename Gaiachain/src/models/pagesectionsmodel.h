#ifndef PAGESECTIONSMODEL_H
#define PAGESECTIONSMODEL_H

#include <QAbstractListModel>

class PageSectionsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit PageSectionsModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // PAGESECTIONSMODEL_H
