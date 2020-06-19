#pragma once

#include "querymodel.h"

#include <QDateTime>

class MinimumDateModel : public QueryModel
{
    Q_OBJECT

    Q_PROPERTY(QString plotId READ plotId WRITE setPlotId NOTIFY plotIdChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY dateChanged)

public:
    explicit MinimumDateModel(QObject *parent = nullptr);

    void refresh() override;

    void setPlotId(const QString &id);
    QString plotId() const;

    QDateTime date() const;

public slots:
    void setDate(const QDateTime &date);

signals:
    void plotIdChanged(const QString &plotId) const;
    void dateChanged(const QDateTime &date) const;

private:
    QString m_plotId;
    QDateTime m_date;
};

