#pragma once

#include <QSqlQueryModel>

class EntitiesModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit EntitiesModel(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
};

