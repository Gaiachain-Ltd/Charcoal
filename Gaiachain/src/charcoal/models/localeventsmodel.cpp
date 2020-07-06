#include "localeventsmodel.h"
#include <QDebug>

LocalEventsModel::LocalEventsModel(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id FROM Events WHERE isCommitted=0 "
               "UNION ALL "
               "SELECT id FROM Replantations WHERE isCommitted=0");

    connect(this, &LocalEventsModel::refreshed,
            this, &LocalEventsModel::updateSize);
}

int LocalEventsModel::size()
{
    if (m_connectionName.isEmpty()) {
        return 0;
    }

    return m_rowCount;
}

void LocalEventsModel::updateSize()
{
    const int previous = m_rowCount;
    m_rowCount = rowCount();

    if (previous != m_rowCount) {
        emit sizeChanged(m_rowCount);
    }
}
