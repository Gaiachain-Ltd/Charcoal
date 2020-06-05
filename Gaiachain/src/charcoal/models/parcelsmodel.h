#pragma once

#include <QSqlQueryModel>

class ParcelsModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ParcelsModel(QObject *parent = nullptr);

    void setDatabasePath(const QString &connectionName);

};

