#include "abstractmanager.h"

AbstractManager::AbstractManager(QObject *parent)
    : QObject(parent)
{}

void AbstractManager::setupQmlContext(QQmlApplicationEngine &)
{}

bool AbstractManager::processing() const
{
    return m_activeProcessesCount > 0;
}

void AbstractManager::processStarted() const
{
    if (++m_activeProcessesCount == 1) {   // first action
        emit processingChanged(true);
    }
}

void AbstractManager::processFinished() const
{
    if (m_activeProcessesCount == 0) {
        return;
    }

    if (--m_activeProcessesCount == 0) {   // last action
        emit processingChanged(false);
    }
}

AbstractManager::ProcessCounter::ProcessCounter(const AbstractManager *manager)
    : m_manager(manager)
{
    if (m_manager) {
        m_manager->processStarted();
    }
}

AbstractManager::ProcessCounter::~ProcessCounter() {
    if (m_manager) {
        m_manager->processFinished();
    }
}
