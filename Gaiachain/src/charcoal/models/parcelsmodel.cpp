#include "parcelsmodel.h"

ParcelsModel::ParcelsModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT name FROM Parcels");
}
