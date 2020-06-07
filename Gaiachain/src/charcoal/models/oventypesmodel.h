#pragma once

#include <QSqlQueryModel>

class OvenTypesModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit OvenTypesModel(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
};

