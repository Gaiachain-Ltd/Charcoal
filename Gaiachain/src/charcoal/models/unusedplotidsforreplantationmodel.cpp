#include "unusedplotidsforreplantationmodel.h"

UnusedPlotIdsForReplantationModel::UnusedPlotIdsForReplantationModel(QObject *parent)
    : SimpleListQueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM Entities WHERE isReplanted=0 AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Plot\")");
}
