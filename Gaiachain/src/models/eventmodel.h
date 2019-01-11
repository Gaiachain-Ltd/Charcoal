#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QAbstractListModel>
#include <QHash>

#include "../common/globals.h"

class EventModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ModelRole {
        ShipmentId = Qt::UserRole, //Foreign key
        Timestamp,
        Location,
        Company,
        Place,
        PlaceAction,
        LastRole
    }; //!!! Add new roles at the end

    explicit EventModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void appendData(const Gaia::ModelData &inData);

private:
    const QHash<int, QByteArray> m_roleNames = {
        { ModelRole::ShipmentId, "shipmentId" },
        { ModelRole::Timestamp, "timestamp" },
        { ModelRole::Location, "location" },
        { ModelRole::Company, "company" },
        { ModelRole::Place, "place" },
        { ModelRole::PlaceAction, "action" }
    };

    QHash<int, QVariantList> m_data;

    int shiftedIndex(const int idx) const;
};

#endif // EVENTMODEL_H
