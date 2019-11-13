#include "abstractmodel.h"

#include <QSqlError>
#include <QSqlRecord>
#include <QSqlField>

#include "../common/types.h"

AbstractModel::AbstractModel(const QLatin1String &tableName, QSqlDatabase db, QObject *parent)
    : QSqlTableModel(parent, db)
{
    ModelChangedExtension::setupConnections(this);
    setTable(tableName);
    setEditStrategy(QSqlTableModel::OnRowChange);

    initialize();
}

int AbstractModel::firstColumn() const
{
    return 0;
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
            return QSqlTableModel::setData(index, value);
        }
    }

    auto column = role >= Qt::UserRole ? columnShift(role) : index.column();
    auto updatedIndex = this->index(index.row(), column, index.parent());

    auto updatedValue = value;
    auto targetType = static_cast<int>(roleDatabaseTypes().value(roleShift(column)) );
    if (!types::canCustomConvert(value, targetType)) {
        if (!value.canConvert(targetType)) {
            qCWarning(dataModels) << "Error while setting data. Cannot convert column data:" << QString(roleNames().value(roleShift(column)) );
        } else {
            updatedValue.convert(targetType);
        }
    } else {
        types::customConvert(updatedValue, targetType);
    }

    return QSqlTableModel::setData(updatedIndex, updatedValue);
}

QVariant AbstractModel::data(const QModelIndex &index, int role) const
{
    if (role >= Qt::UserRole) {
        if (columnShift(role) < firstColumn() || columnShift(role) >= lastColumn()) {
            return {};
        }
    } else {
        if (index.column() < firstColumn() || index.column() >= lastColumn()) {
            return QSqlTableModel::data(index);
        }
    }


    auto column = role >= Qt::UserRole ? columnShift(role) : index.column();
    auto updatedIndex = this->index(index.row(), column, index.parent());

    auto value = QSqlTableModel::data(updatedIndex);
    if (!value.isNull()) {
        auto targetType = static_cast<int>(roleAppTypes().value(roleShift(column)));
        if (!types::canCustomConvert(value, targetType)) {
            if (!value.canConvert(targetType)) {
                qCWarning(dataModels) << "Error while getting data. Cannot convert column data:" << QString(roleNames().value(roleShift(column)));
            } else {
                value.convert(targetType);
            }
        } else {
            types::customConvert(value, targetType);
        }
    }

    return value;
}

void AbstractModel::initialize()
{
    select();
}

void AbstractModel::appendData(const Gaia::ModelData &inData)
{
    const int FirstColumn = firstColumn();
    const int LastColumn = lastColumn();
    const int ColumnCount = LastColumn - FirstColumn;

    auto record = QSqlRecord{};
    for (auto column = FirstColumn; column < LastColumn; ++column) {
        auto role = roleShift(column);
        record.append(QSqlField{ roleNames().value(role), roleDatabaseTypes().value(role) });
    }

    for (const auto &dataRow : inData) {
        if (dataRow.count() > ColumnCount) {
            qCWarning(dataModels) << "Wrong input fields count:" << dataRow.count() << " > " << ColumnCount;
            break;
        }

        for (auto column = FirstColumn; (column < LastColumn && (column - FirstColumn) < dataRow.count()); ++column) {
            auto name = roleNames().value(roleShift(column));
            auto value = dataRow.value(column - FirstColumn);
            record.setValue(QString::fromLatin1(name), value);
        }

        if (!insertRecord(rowCount(), record)) {
            qCWarning(dataModels) << "Error while inserting data:" << lastError().text();
        }
    }
}

int AbstractModel::columnShift(const int &role) const
{
    return role - Qt::UserRole;
}

int AbstractModel::roleShift(const int &column) const
{
    return column + Qt::UserRole;
}
