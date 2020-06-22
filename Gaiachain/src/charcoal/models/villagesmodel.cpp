#include "villagesmodel.h"

#include "database/dbhelpers.h"

VillagesModel::VillagesModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT name FROM Villages");
}
