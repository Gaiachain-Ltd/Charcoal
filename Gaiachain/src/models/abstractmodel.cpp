#include "abstractmodel.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>
#include <QSqlQuery>

#include "../common/types.h"
#include "../database/dbhelpers.h"

AbstractModel::AbstractModel(QObject *parent)
    : AbstractReadModel(parent)
{
    connect(this, &QAbstractItemModel::rowsInserted, this, &AbstractModel::handleRowsInserted);
    connect(this, &QAbstractItemModel::rowsAboutToBeRemoved, this, &AbstractModel::handleRowsToBeRemoved);
}

void AbstractModel::setSourceModel(QSqlTableModel *sourceModel)
{
    m_writableModel = QPointer<QSqlTableModel>(sourceModel);
    AbstractReadModel::setSourceModel(sourceModel);
}

void AbstractModel::setSourceModel(QSqlQueryModel *sourceModel)
{
    m_writableModel.clear();
    AbstractReadModel::setSourceModel(sourceModel);
}

QList<int> AbstractModel::editableRoles() const
{
    return {};
}

bool AbstractModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role >= Qt::UserRole) {
        if (!editableRoles().contains(role)) {
            return false;
        }
    } else {
        if (index.column() < firstColumn() || index.column() >= lastColumn()) {
            return QIdentityProxyModel::setData(index, value);
        }
    }

    auto column = role >= Qt::UserRole ? columnShift(role) : index.column();
    auto updatedIndex = this->index(index.row(), column, index.parent());

    auto updatedValue = value;
    types::convert(updatedValue, roleDatabaseTypes().value(roleShift(column)) );

    return QIdentityProxyModel::setData(updatedIndex, updatedValue);
}

bool AbstractModel::clearData()
{
    if (m_writableModel.isNull()) {
        qCWarning(dataModels) << "Trying to clear data with an empty model!";
        return false;
    }

    static const auto DeleteAllQuery = QString{"DELETE FROM `%1`"};

    auto query = QSqlQuery{m_writableModel->database()};
    query.prepare(DeleteAllQuery.arg(m_writableModel->tableName()));

    db::Helpers::execQuery(query);

    m_writableModel->select();
    return !db::Helpers::hasError(query);
}

void AbstractModel::appendData(const Gaia::ModelData &modelData)
{
    if (m_writableModel.isNull()) {
        qCWarning(dataModels) << "Trying to append data into an empty model!";
        return;
    }

    const int FirstColumn = firstColumn();
    const int LastColumn = lastColumn();
    const int ColumnCount = LastColumn - FirstColumn;

    auto record = QSqlRecord{};
    for (auto column = FirstColumn; column < LastColumn; ++column) {
        auto role = roleShift(column);
        record.append(QSqlField{ roleNames().value(role), static_cast<QVariant::Type>(roleDatabaseTypes().value(role)) });
    }

    for (const auto &entryData : modelData) {
        if (entryData.count() > ColumnCount) {
            qCWarning(dataModels) << "Wrong input fields count:" << entryData.count() << " > " << ColumnCount;
            break;
        }

        for (auto column = FirstColumn; (column < LastColumn && (column - FirstColumn) < entryData.count()); ++column) {
            auto name = roleNames().value(roleShift(column));
            auto value = entryData.value(column - FirstColumn);
            types::convert(value, roleDatabaseTypes().value(roleShift(column)) );

            record.setValue(QString::fromLatin1(name), value);
        }

        if (!m_writableModel->insertRecord(rowCount(), record)) {
            qCWarning(dataModels) << "Error while inserting data:" << m_writableModel->lastError().text();
        }
    }
}

void AbstractModel::updateData(const Gaia::ModelEntry &searchEntryData, const Gaia::ModelEntryInfo &updateEntryData)
{
    if (m_writableModel.isNull()) {
        qCWarning(dataModels) << "Trying to append data into an empty model!";
        return;
    }

    for (const auto &dataIndex : find(searchEntryData)) {
        for (const auto &role : updateEntryData.keys()) {
            setData(dataIndex, updateEntryData.value(role), role);
        }
        submit();
    }
}

void AbstractModel::removeData(const Gaia::ModelEntry &entryData)
{
    if (m_writableModel.isNull()) {
        qCWarning(dataModels) << "Trying to append data into an empty model!";
        return;
    }

    for (const auto &dataIndex : find(entryData)) {
        removeRow(dataIndex.row());
    }
}

void AbstractModel::handleRowsInserted(const QModelIndex &, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        emit entryInserted(getEntry(row));
    }
}

void AbstractModel::handleRowsToBeRemoved(const QModelIndex &, int first, int last)
{
    for (auto row = first; row <= last; ++row) {
        emit entryRemoved(getEntry(row));
    }
}
