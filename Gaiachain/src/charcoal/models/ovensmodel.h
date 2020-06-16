#pragma once

#include "querymodel.h"

class OvensModel : public QueryModel
{
    Q_OBJECT

    Q_PROPERTY(QString plotId READ plotId WRITE setPlotId NOTIFY plotIdChanged)

public:
    enum OvenRole {
        Id = Qt::UserRole + 1,
        LetterId,
        FirstRow,
        SecondRow
    };
    Q_ENUM(OvenRole)

    explicit OvensModel(QObject *parent = nullptr);

    void refresh() override;

    void setPlotId(const QString &id);
    QString plotId() const;

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void plotIdChanged(const QString &plotId) const;

private:
    QHash<int, QByteArray> m_roleNames = {
        { OvenRole::Id, "id" },
        { OvenRole::LetterId, "letterId" },
        { OvenRole::FirstRow, "firstRow" },
        { OvenRole::SecondRow, "secondRow" }
    };

    QString m_plotId;
};

