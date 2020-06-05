#include "cocoadatamanager.h"

#include <QMetaObject>
#include <QJsonArray>

CocoaDataManager::CocoaDataManager(QObject *parent)
    : AbstractDataManager(parent)
{
}

QString CocoaDataManager::generateHarvestId(const QDate &date, const QString &parcelCode)
{
    return date.toString(QStringLiteral("%1/d-M-yyyy")).arg(parcelCode);
}

void CocoaDataManager::onUnusedLotIdsLoaded(const QJsonArray &idsArray)
{
    QMetaObject::invokeMethod(
        &m_requestsHandler,
        std::bind(&DataRequestsManager::processUnusedLotIdsLoaded,
                  &m_requestsHandler,
                  idsArray));
}
