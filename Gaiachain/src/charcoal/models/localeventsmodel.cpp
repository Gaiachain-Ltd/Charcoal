#include "localeventsmodel.h"

LocalEventsModel::LocalEventsModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT id FROM Events WHERE isCommitted=0"
               "UNION ALL"
               "SELECT id FROM Replantations WHERE isCommitted=0");
}
