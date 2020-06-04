#include "cocoadatamanager.h"

#include <QMetaObject>
#include <QJsonArray>

CocoaDataManager::CocoaDataManager(QObject *parent)
    : AbstractDataManager(parent)
{
}

void CocoaDataManager::onUnusedLotIdsLoaded(const QJsonArray &idsArray)
{
    QMetaObject::invokeMethod(
        &m_requestsHandler,
        std::bind(&DataRequestsManager::processUnusedLotIdsLoaded,
                  &m_requestsHandler,
                  idsArray));
}
