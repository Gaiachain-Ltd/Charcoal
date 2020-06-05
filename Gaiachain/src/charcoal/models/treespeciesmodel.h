#pragma once

#include <QObject>
#include <QSqlQueryModel>

class TreeSpeciesModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit TreeSpeciesModel(QObject *parent = nullptr);

    void setDatabasePath(const QString &connectionName);
};
