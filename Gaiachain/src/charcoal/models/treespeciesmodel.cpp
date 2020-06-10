#include "treespeciesmodel.h"

TreeSpeciesModel::TreeSpeciesModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT name FROM TreeSpecies");
}
