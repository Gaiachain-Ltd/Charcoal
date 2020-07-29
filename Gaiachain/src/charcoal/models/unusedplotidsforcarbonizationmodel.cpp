#include "unusedplotidsforcarbonizationmodel.h"

UnusedPlotIdsForCarbonizationModel::UnusedPlotIdsForCarbonizationModel(QObject *parent)
    : SimpleListQueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM Entities WHERE isFinished=0 "
               "AND typeId IN (SELECT id FROM EntityTypes WHERE name=\"Plot\")");
}
