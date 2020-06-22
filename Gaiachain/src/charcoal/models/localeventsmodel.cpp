#include "localeventsmodel.h"
#include <QDebug>
LocalEventsModel::LocalEventsModel(QObject *parent) : QueryModel(parent)
{
    setDbQuery("SELECT id FROM Events WHERE isCommitted=0 "
               "UNION ALL "
               "SELECT id FROM Replantations WHERE isCommitted=0");

    // TODO: emit sizeChanged when size changes ;-)
}

int LocalEventsModel::size()
{
    if (m_connectionName.isEmpty()) {
        return 0;
    }

    const int previous = m_oldRowCount;
    refresh();
    m_oldRowCount = rowCount();
    qDebug() << "RC" << m_oldRowCount;

    if (previous != m_oldRowCount) {
        emit sizeChanged(m_oldRowCount);
    }
    return m_oldRowCount;
}
