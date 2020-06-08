#pragma once

#include <QSqlQueryModel>

class UnusedPlotIdsModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit UnusedPlotIdsModel(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
};

