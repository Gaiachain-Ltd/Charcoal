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
        CooperativeId,
        Properties,
        LocationLat,
        LocationLon,
        IsLocal,
        LastUsed,
        LastColumn
    }; //!!! Keep the lastcolumn, a last entry in the enum

    EventModel(QObject *parent = nullptr);

    static QString columnName(const Columns &column);

    int firstColumn() const override;
    int lastColumn() const override;

    QList<int> editableRoles() const override;

    QHash<int, QByteArray> roleNames() const override;
    QHash<int, QMetaType::Type> roleDatabaseTypes() const override;
    QHash<int, QMetaType::Type> roleAppTypes() const override;

private:
    static const QHash<int, QByteArray> sc_roleNames;
    static const QHash<int, QMetaType::Type> sc_roleDatabaseTypes;
    static const QHash<int, QMetaType::Type> sc_roleAppTypes;
};

#endif // EVENTMODEL_H
