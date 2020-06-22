#include "destinationsmodel.h"

DestinationsModel::DestinationsModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT name FROM Destinations");
}
