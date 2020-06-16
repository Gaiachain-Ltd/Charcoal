#include "villagesmodel.h"

#include "database/dbhelpers.h"

VillagesModel::VillagesModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT name FROM Villages");
}
