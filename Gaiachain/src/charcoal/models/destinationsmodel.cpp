#include "destinationsmodel.h"

DestinationsModel::DestinationsModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT name FROM Destinations");
}
