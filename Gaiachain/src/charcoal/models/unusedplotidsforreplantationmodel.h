#pragma once

#include <QSqlQueryModel>

class UnusedPlotIdsForReplantationModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit UnusedPlotIdsForReplantationModel(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
};

