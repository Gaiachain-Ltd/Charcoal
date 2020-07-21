#pragma once

#include "querymodel.h"

class OvensModel : public QueryModel
{
    Q_OBJECT

    Q_PROPERTY(int plotId READ plotId WRITE setPlotId NOTIFY plotIdChanged)

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

    void setPlotId(const int id);
    int plotId() const;

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

signals:
    void plotIdChanged(const int plotId) const;

private:
    const QHash<int, QByteArray> m_roleNames = {
        { OvenRole::Id, "ovenId" },
        { OvenRole::LetterId, "letterId" },
        { OvenRole::FirstRow, "firstRow" },
        { OvenRole::SecondRow, "secondRow" }
    };

    int m_plotId = -1;
};

