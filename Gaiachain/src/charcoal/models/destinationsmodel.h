#pragma once

#include <QSqlQueryModel>

class DestinationsModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit DestinationsModel(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
};

