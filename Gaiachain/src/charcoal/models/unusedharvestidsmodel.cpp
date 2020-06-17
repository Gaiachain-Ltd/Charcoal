#include "unusedharvestidsmodel.h"

UnusedHarvestIdsModel::UnusedHarvestIdsModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT name FROM Entities WHERE isFinished=0 AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Harvest\")");
}
