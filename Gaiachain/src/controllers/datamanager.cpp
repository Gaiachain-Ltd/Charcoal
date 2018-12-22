#include "datamanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

DataManager::DataManager(QObject *parent)
    : QObject(parent)
{
    setupModels();
}

void DataManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("commodityRangeProxy"), &m_commodityDateRangeProxyModel);
}

void DataManager::setupModels()
{
    m_commodityProxyModel.setSourceModel(&m_shipmentModel);
    m_dateRangeProxyModel.setSourceModel(&m_eventModel);

    m_commodityDateRangeProxyModel.setSourceModel(&m_eventModel);
    m_commodityDateRangeProxyModel.setCommodityProxyModel(&m_commodityProxyModel);
}

void DataManager::populateModels()
{

}
