#include "unusedtransportidsmodel.h"

UnusedTransportIdsModel::UnusedTransportIdsModel(QObject *parent)
    : SimpleListQueryModel(parent)
{
    setWebModelCanChange(true);
    setDbQuery("SELECT id, name FROM Entities WHERE isFinished=0 AND typeId IN "
               "(SELECT id FROM EntityTypes WHERE name=\"Transport\")");
}
