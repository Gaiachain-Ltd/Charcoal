#include "unusedplotidsforreplantationmodel.h"

UnusedPlotIdsForReplantationModel::UnusedPlotIdsForReplantationModel(QObject *parent)
    : QueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT name FROM Entities WHERE isReplanted=0 AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Plot\")");
}
