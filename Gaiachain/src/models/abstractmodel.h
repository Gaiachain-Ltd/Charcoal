#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QSqlTableModel>
#include <QHash>

#include "modelchangedextension.h"
#include "../common/globals.h"

class AbstractModel : public QSqlTableModel, public ModelChangedExtension
{
    Q_OBJECT
    Q_INTERFACES(ModelChangedExtension)

public:
    explicit AbstractModel(const QLatin1String &tableName, QSqlDatabase db, QObject *parent = nullptr);

    virtual int firstColumn() const;
    virtual int lastColumn() const = 0;

    virtual QList<int> editableRoles() const;

    virtual QHash<int, QVariant::Type> roleDatabaseTypes() const = 0;
    virtual QHash<int, QVariant::Type> roleAppTypes() const = 0;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void appendData(const Gaia::ModelData &inData);

signals:
    void modelChanged() const override;

protected:
    virtual void initialize();

    int columnShift(const int &role) const;
    int roleShift(const int &column) const;
};

#endif // ABSTRACTMODEL_H
