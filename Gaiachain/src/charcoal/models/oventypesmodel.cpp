#include "oventypesmodel.h"

#include <QSqlQuery>
#include <QDebug>

OvenTypesModel::OvenTypesModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT id, name FROM OvenTypes");
}

QVariant OvenTypesModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() == false) {
        return {};
    }

    const int row = index.row();
    query().seek(row);
    if (role == Qt::ItemDataRole::DisplayRole || role == OvenTypesRole::TranslatedName) {
        if (isTraditional()) {
            return tr("Traditional oven");
        } else {
            return tr("Metallic oven");
        }
    } else if (role == OvenTypesRole::Id) {
        return query().value("id").toInt();
    } else if (role == OvenTypesRole::Name) {
        return query().value("name").toString();
    } else if (role == OvenTypesRole::IsTraditionalOven) {
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
