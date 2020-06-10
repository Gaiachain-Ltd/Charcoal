#include "oventypesmodel.h"

OvenTypesModel::OvenTypesModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT name FROM OvenTypes");
}
