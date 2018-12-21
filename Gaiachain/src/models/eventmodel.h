#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include <QAbstractListModel>
#include <QMultiHash>

class EventModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum ModelRole {
        EventId = Qt::UserRole +1,
        Company,
        LogId,
        ArrivalDateTime,
        DepartureDateTime,
        LastRole
    }; //!!! Add new roles at the end

    explicit EventModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

private:
    const QHash<int, QByteArray> m_roleNames = {
        { EventId, "id" },
        { Company, "company" },
        { LogId, "logId"},
        { ArrivalDateTime, "arrivalDateTime"},
        { DepartureDateTime, "departureDateTime"}
    };
    // TO_DO add gps and others

    QMultiHash<int, QVariant> m_data;
};

#endif // EVENTMODEL_H
