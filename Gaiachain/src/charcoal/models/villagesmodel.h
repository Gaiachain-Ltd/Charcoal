#pragma once

#include <QSqlQueryModel>

class VillagesModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit VillagesModel(QObject *parent = nullptr);

    void setDbConnection(const QString &connectionName);
};
