#pragma once

#include "querymodel.h"

class OvenTypesModel : public QueryModel
{
    Q_OBJECT

public:
    enum OvenTypesRole {
        Id = Qt::UserRole + 1,
        Name,
        TranslatedName,
        IsTraditionalOven
    };
    Q_ENUM(OvenTypesRole)

    explicit OvenTypesModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

private:
    QHash<int, QByteArray> m_roleNames = {
        { OvenTypesRole::Id, "id" },
        { OvenTypesRole::Name, "name" },
        { OvenTypesRole::TranslatedName, "translatedName" },
        { OvenTypesRole::IsTraditionalOven, "isTraditionalOven" }
    };

    bool isTraditional() const;
};

