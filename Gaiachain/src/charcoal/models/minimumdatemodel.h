#pragma once

#include "querymodel.h"

#include <QDateTime>

class MinimumDateModel : public QueryModel
{
    Q_OBJECT

    Q_PROPERTY(int plotId READ plotId WRITE setPlotId NOTIFY plotIdChanged)
    Q_PROPERTY(QDateTime date READ date NOTIFY dateChanged)

public:
    explicit MinimumDateModel(QObject *parent = nullptr);

    void refresh() override;

    void setPlotId(const int id);
    int plotId() const;

    QDateTime date() const;

public slots:
    void setDate(const QDateTime &date);

signals:
    void plotIdChanged(const int plotId) const;
    void dateChanged(const QDateTime &date) const;

private:
    int m_plotId = -1;
    QDateTime m_date;
};

