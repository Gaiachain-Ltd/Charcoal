#include "unusedplotidsmodel.h"

#include "common/tags.h"

#include <QSqlQuery>

UnusedPlotIdsModel::UnusedPlotIdsModel(QObject *parent) : SimpleListQueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM Entities WHERE isFinished=0 "
               "AND typeId IN (SELECT id FROM EntityTypes WHERE name=\"Plot\") "
               "AND (SELECT (COUNT(DISTINCT id) < 2) FROM Events "
               "WHERE entityId=Entities.id)");
}
