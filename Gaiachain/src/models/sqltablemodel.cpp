#include "sqltablemodel.h"

SqlTableModel::SqlTableModel(const QLatin1String &tableName, QSqlDatabase db, QObject *parent)
    : QSqlTableModel(parent, db)
{
    setTable(tableName);
    setEditStrategy(QSqlTableModel::OnRowChange);

    select();
}
