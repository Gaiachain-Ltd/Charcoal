#pragma once

#include <QSqlQueryModel>

class UnusedHarvestIdsModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit UnusedHarvestIdsModel(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
};

