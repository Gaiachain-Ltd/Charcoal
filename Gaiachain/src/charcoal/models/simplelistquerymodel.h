#pragma once

#include "querymodel.h"

class SimpleListQueryModel : public QueryModel
{
    Q_OBJECT

public:
    enum ListRole {
        Id = Qt::UserRole + 1,
        Name
    };
    Q_ENUM(ListRole)

    explicit SimpleListQueryModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    const QHash<int, QByteArray> m_roleNames = {
        { ListRole::Id, "idNumber" },
        { ListRole::Name, "name" }
    };
};

