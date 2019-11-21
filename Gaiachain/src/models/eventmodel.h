#ifndef EVENTMODEL_H
#define EVENTMODEL_H

#include "abstractmodel.h"

#include <QHash>

#include "../common/enums.h"

class EventModel : public AbstractModel
{
    Q_OBJECT

public:
    enum Columns {
        PackageId = Qt::UserRole + 1, // 1 as 0 column is id column not used in UI
        Action,
        Timestamp,
        UserRole,
        CooperativeId,
        Properties,
        LocationLat,
        LocationLon,
        IsLocal,
        LastUsed,
        LastColumn
    }; //!!! Keep the lastcolumn, a last entry in the enum

    EventModel(QSqlDatabase db, QObject *parent = nullptr);

    int firstColumn() const override;
    int lastColumn() const override;

    QList<int> editableRoles() const override;

    QHash<int, QByteArray> roleNames() const override;
    QHash<int, QVariant::Type> roleDatabaseTypes() const override;
    QHash<int, QVariant::Type> roleAppTypes() const override;

    void updateLocal(const QString &packageId, const Enums::SupplyChainAction &action, bool isLocal);

private:
    static const QHash<int, QByteArray> sc_roleNames;
    static const QHash<int, QVariant::Type> sc_roleDatabaseTypes;
    static const QHash<int, QVariant::Type> sc_roleAppTypes;
};

#endif // EVENTMODEL_H
