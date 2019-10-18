#ifndef ABSTRACTMODEL_H
#define ABSTRACTMODEL_H

#include <QAbstractListModel>
#include <QHash>

#include "../common/globals.h"
#include "modelchangedextension.h"

class AbstractModel : public QAbstractListModel, public ModelChangedExtension
{
    Q_OBJECT
    Q_INTERFACES(ModelChangedExtension)

public:
    explicit AbstractModel(QObject *parent = nullptr);

    virtual int lastRole() const = 0;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void clearModel();
    void appendData(const Gaia::ModelData &inData);

signals:
    void modelChanged() const override;

private:
    QHash<int, QVariantList> m_data;

    int roleShift(const int role) const;
};

#endif // ABSTRACTMODEL_H
