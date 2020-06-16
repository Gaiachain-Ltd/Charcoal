#pragma once

#include "querymodel.h"

class OvensModel : public QueryModel
{
    Q_OBJECT

    Q_PROPERTY(QString plotId READ plotId WRITE setPlotId NOTIFY plotIdChanged)

public:
    explicit OvensModel(QObject *parent = nullptr);

    void setPlotId(const QString &id);
    QString plotId() const;

signals:
    void plotIdChanged(const QString &plotId) const;

private:
    QString m_plotId;
};

