#pragma once

#include <QSqlQueryModel>

class UnusedTransportIdsModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit UnusedTransportIdsModel(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
};

