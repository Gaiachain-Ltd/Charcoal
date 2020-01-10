#ifndef SQLTABLEMODEL_H
#define SQLTABLEMODEL_H

#include <QSqlTableModel>

class SqlTableModel : public QSqlTableModel
{
    Q_OBJECT
public:
    explicit SqlTableModel(const QLatin1String &tableName, const QString &dbConnectionName, QObject *parent = nullptr);

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
};

#endif // SQLTABLEMODEL_H
