#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "abstractmanager.h"

#include "../models/eventmodel.h"
#include "../models/daterangeproxymodel.h"
#include "../models/latesteventsproxymodel.h"
#include "../models/packagetypeeventsproxymodel.h"
#include "../models/searcheventsproxymodel.h"
#include "../models/companyeventsproxymodel.h"
#include "../models/packagetypeproxymodel.h"
#include "../models/packagedataproxymodel.h"
#include "../common/packagedata.h"

class DataManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit DataManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) override;

    void updateCompanyId(const QString &companyId);

    Q_INVOKABLE PackageData getPackageData(const QString &packageId) const;

public slots:
    void clearModels();
    void onEntitiesLoaded(const QJsonArray &entities);
    void onRelationsLoaded(const QJsonObject &relations);

private:
    EventModel m_eventModel;
    PackageDataProxyModel m_packageDataModel;

    // TODO: if still needed in final implementation compose as extensions instead of single models
    DateRangeProxyModel m_calendarModel;
    CompanyEventsProxyModel m_companyCalendarModel;
    PackageTypeEventsProxyModel m_packagesCalendarModel;

    DateRangeProxyModel m_dateEventsModel;
    LatestEventsProxyModel m_latestDateEventsModel;
    CompanyEventsProxyModel m_companyLatestDateEventsModel;
    PackageTypeProxyModel m_packagesTypeCompanySearchLatestEventsModel;

    LatestEventsProxyModel m_latestEventsModel;
    SearchEventsProxyModel m_searchLatestEventsModel;
    CompanyEventsProxyModel m_companySearchLatestEventsModel;

    // INFO in final implementation will be a real model
    QMultiHash<QString, QString> m_relationsModel;

    void setupModels();

    QJsonValue checkAndValue(const QJsonObject &object, const QLatin1String tag);
    void loadEntity(const QJsonObject &entityObj);
};

#endif // DATAMANAGER_H
