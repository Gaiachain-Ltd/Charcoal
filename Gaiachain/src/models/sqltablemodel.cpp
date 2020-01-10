#include "sqltablemodel.h"

#include "../database/dbhelpers.h"

SqlTableModel::SqlTableModel(const QLatin1String &tableName, const QString &dbConnectionName, QObject *parent)
    : QSqlTableModel(parent, db::Helpers::databaseConnection(dbConnectionName))
{
    setTable(tableName);
    setEditStrategy(QSqlTableModel::OnManualSubmit);

    select();
}

bool SqlTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    /* not sure why - but a view is not updated if I don't call
     * beginRemoveRows / endRemoveRows manually
     * I don't see them called in QSqlTableModel::removeRows
     */
    beginRemoveRows(parent, row, row + count - 1);
    auto result = QSqlTableModel::removeRows(row, count);
    endRemoveRows();

    return result;
}
