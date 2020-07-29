#include "unusedharvestidsmodelforcarbonizationending.h"

UnusedHarvestIdsModelForCarbonizationEnding::UnusedHarvestIdsModelForCarbonizationEnding(
    QObject *parent) : SimpleListQueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM Entities WHERE isFinished=0 "
               "AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Harvest\") "
               "AND (SELECT COUNT(id) > 0 FROM Ovens "
               "WHERE carbonizationEnding IS NULL AND plot=Entities.parent)");
    // We only select harvests which have unfinished ovens (carbonizations)
}
