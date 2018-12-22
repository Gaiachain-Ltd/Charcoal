#ifndef SHIPMENTMODEL_H
#define SHIPMENTMODEL_H

#include <QAbstractListModel>
#include <QHash>

#include "../common/globals.h"

class ShipmentModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ModelRole {
        ShipmentId = Qt::UserRole +1,
        Commodity,
        LastRole
    }; //!!! Add new roles at the end

    explicit ShipmentModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void appendData(const Gaia::ModelData &inData);

private:
    const QHash<int, QByteArray> m_roleNames = {
        { ShipmentId, "id" },
        { Commodity, "commodity"}
    };

    QHash<int, QVariantList> m_data;
};

#endif // SHIPMENTMODEL_H
