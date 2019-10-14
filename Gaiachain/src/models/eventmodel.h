#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QAbstractListModel>
#include <QHash>

#include "../common/globals.h"
#include "modelchangedextension.h"

class EventModel : public QAbstractListModel, public ModelChangedExtension
{
    Q_OBJECT
    Q_INTERFACES(ModelChangedExtension)

public:
    enum ModelRole {
        ShipmentId = Qt::UserRole, //Foreign key
        Timestamp,
        Location,
        Company,
        UserRole,
        Action,
        ActionProgress,
        LastRole
    }; //!!! Add new roles at the end

    explicit EventModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void clearModel();

    void appendData(const Gaia::ModelData &inData);

signals:
    void modelChanged() const override;

private:
    static const QHash<int, QByteArray> sc_roleNames;

    QHash<int, QVariantList> m_data;

    int shiftedIndex(const int idx) const;
};

#endif // EVENTMODEL_H
