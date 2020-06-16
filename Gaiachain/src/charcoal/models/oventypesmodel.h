#pragma once

#include "querymodel.h"

class OvenTypesModel : public QueryModel
{
    Q_OBJECT

public:
    enum OvenRole {
        Id = Qt::UserRole + 1,
        Name,
        TranslatedName,
        IsTraditionalOven
    };
    Q_ENUM(OvenRole)

    explicit OvenTypesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QHash<int, QByteArray> m_roleNames = {
        { OvenRole::Id, "id" },
        { OvenRole::Name, "name" },
        { OvenRole::TranslatedName, "translatedName" },
        { OvenRole::IsTraditionalOven, "isTraditionalOven" }
    };

    bool isTraditional() const;
};

