#ifndef UNUSEDIDSMODEL_H
#define UNUSEDIDSMODEL_H

#include "abstractmodel.h"

#include <QHash>

class UnusedIdsModel : public AbstractModel
{
    Q_OBJECT

public:
    enum Columns {
        PackageId = Qt::UserRole + 1, // 1 as 0 column is id column not used in UI
        PackageType,
        LastColumn
    }; //!!! Keep the lastcolumn, a last entry in the enum

    UnusedIdsModel(QObject *parent = nullptr);

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

#endif // UNUSEDIDSMODEL_H
