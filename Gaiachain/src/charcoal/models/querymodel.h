#pragma once

#include <QSqlQueryModel>

class QueryModel : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit QueryModel(QObject *parent = nullptr);

    void setDbQuery(const QString &query);
    void setDbConnection(const QString &connectionName);
    bool isValid() const;

signals:
    void refreshed() const;

public slots:
    virtual void refresh();

protected:
    QString m_query;
    QString m_connectionName;
};

