#include "unusedharvestidsmodel.h"

UnusedHarvestIdsModel::UnusedHarvestIdsModel(QObject *parent) : QueryModel(parent)
{
    // TODO: there is a difference between unused HarvestId that can be
    // used in Carbonisation Ending and Loading&Transport!
    setDbQuery("SELECT name FROM Entities WHERE isFinished=0 AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Harvest\")");
}
