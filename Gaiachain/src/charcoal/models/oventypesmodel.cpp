#include "oventypesmodel.h"

#include <QSqlQuery>
#include <QDebug>

OvenTypesModel::OvenTypesModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id, name FROM OvenTypes");
}

QVariant OvenTypesModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) {
        return {};
    }

    query().seek(index.row());

    switch (role) {
    case Qt::ItemDataRole::DisplayRole:
    case OvenTypesRole::TranslatedName:
        if (isTraditional()) {
            return tr("Traditional oven");
        } else {
            return tr("Metallic oven");
        }
        break;
    case OvenTypesRole::Id:
        return query().value("id").toInt();
    case OvenTypesRole::Name:
        return query().value("name").toString();
    case OvenTypesRole::IsTraditionalOven:
        return isTraditional();
    }

    return {};
}

QHash<int, QByteArray> OvenTypesModel::roleNames() const
{
    return m_roleNames;
}

bool OvenTypesModel::isTraditional() const
{
    const QString name(query().value("name").toString().toLower());
    if (name == QStringLiteral("metallic")) {
        return false;
    } else if (name == QStringLiteral("traditional")) {
        return true;
    }

    return false;
}
