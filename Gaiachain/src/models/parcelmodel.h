#ifndef PARCELMODEL_H
#define PARCELMODEL_H

#include "abstractmodel.h"

#include <QHash>

class ParcelModel : public AbstractModel
{
    Q_OBJECT

public:
    enum Columns {
        Id = Qt::UserRole,
        Code,
        ProducerId,
        LastColumn
    }; //!!! Keep the lastcolumn, a last entry in the enum

    ParcelModel(QObject *parent = nullptr);

    static QString columnName(const Columns &column);

    int firstColumn() const override;
    int lastColumn() const override;

    QHash<int, QByteArray> roleNames() const override;
    QHash<int, QMetaType::Type> roleDatabaseTypes() const override;
    QHash<int, QMetaType::Type> roleAppTypes() const override;

private:
    static const QHash<int, QByteArray> sc_roleNames;
    static const QHash<int, QMetaType::Type> sc_roleDatabaseTypes;
    static const QHash<int, QMetaType::Type> sc_roleAppTypes;
};

#endif // PARCELMODEL_H
