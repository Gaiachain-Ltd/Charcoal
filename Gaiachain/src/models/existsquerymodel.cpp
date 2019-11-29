#include "existsquerymodel.h"

#include <QSqlError>

#include "../common/types.h"
#include "../database/dbhelpers.h"

ExistsQueryModel::ExistsQueryModel(QObject *parent)
    : QObject(parent)
{}

ExistsQueryModel::ExistsQueryModel(QSqlDatabase db, QObject *parent)
    : QObject(parent), m_db(db)
{}

void ExistsQueryModel::setDatabase(QSqlDatabase db)
{
    m_db = db;
}

bool ExistsQueryModel::prepareQuery(const QString &tableName, const QStringList &conditionFields)
{
    static const auto ExistsQueryBase = QString{"SELECT `id` FROM `%1` WHERE %2"};
    static const auto ConditionQueryBase = QString{"`%1` IS ? AND"};

    m_conditionFields = conditionFields;
    if (conditionFields.isEmpty()) {
        qCWarning(databaseQuery) << "No conditions for exist query";
        m_query.clear();
        return false;
    }

    auto conditionString = QString{};
    std::for_each(conditionFields.constBegin(), conditionFields.constEnd(),
                  [&conditionString](const auto &conditionField) {
        conditionString += ConditionQueryBase.arg(conditionField);
    });
    conditionString = conditionString.left(conditionString.count() - 4);  // " AND"

    m_query = QSqlQuery(m_db);
    m_query.prepare(ExistsQueryBase.arg(tableName).arg(conditionString));
    return true;
}

bool ExistsQueryModel::exists(const QString &tableName, const QVariantMap &conditions)
{
    if (!prepareQuery(tableName, conditions.keys()) ) {
        return false;
    }

    return exists(conditions.values());
}

bool ExistsQueryModel::exists(const QVariantList &conditionValues)
{
    if (m_query.lastQuery().isEmpty() ||
            m_conditionFields.isEmpty()) {
        qCWarning(databaseQuery) << "Exist query wasn't prepared properly";
        return false;
    }

    if (m_conditionFields.size() != conditionValues.size()) {
        qCWarning(databaseQuery) << "Exist query prepared with different field count:"
                                 << m_conditionFields.size() << conditionValues.size();
        return false;
    }

    std::for_each(conditionValues.constBegin(), conditionValues.constEnd(),
                  [this](const auto &conditionValue) { m_query.addBindValue(conditionValue); });
    db::Helpers::execQuery(m_query);
    if (db::Helpers::hasError(m_query)) {
        return false;
    }

    auto result = m_query.first();
    m_query.finish();
    return result;
}
