#ifndef ABSTRACTREADMODEL_H
#define ABSTRACTREADMODEL_H

#include <QIdentityProxyModel>

#include "modelchangedextension.h"
#include "../common/globals.h"

class QSqlQueryModel;

class AbstractReadModel : public QIdentityProxyModel, public ModelChangedExtension
{
    Q_OBJECT
    Q_INTERFACES(ModelChangedExtension)
    Q_PROPERTY(int size READ rowCount NOTIFY modelChanged)

public:
    explicit AbstractReadModel(QObject *parent = nullptr);

    virtual void setSourceModel(QSqlQueryModel *sourceModel);

    virtual int firstColumn() const;
    virtual int lastColumn() const = 0;

    virtual QHash<int, QMetaType::Type> roleDatabaseTypes() const = 0;
    virtual QHash<int, QMetaType::Type> roleAppTypes() const = 0;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QModelIndexList match(const QModelIndex &start, int role, const QVariant &value,
                          int hits = 1, Qt::MatchFlags flags = Qt::MatchFlags(Qt::MatchStartsWith|Qt::MatchWrap)) const override;

    QModelIndexList find(const Gaia::ModelEntry &entryData);

    Gaia::ModelEntry getEntry(int row) const;
    Gaia::ModelData getData() const;

signals:
    void modelChanged() const override;

protected:
    using QIdentityProxyModel::setSourceModel;

    int columnShift(const int &role) const;
    int roleShift(const int &column) const;
};

#endif // ABSTRACTREADMODEL_H
