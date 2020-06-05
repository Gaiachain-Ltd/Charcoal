#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "controllers/abstractmanager.h"

#include <QThread>

#ifdef COCOA
#include "cocoa/cocoauserdata.h"
#elif CHARCOAL
#include "charcoal/charcoaluserdata.h"
#endif

class QGeoCoordinate;

class AbstractDataManager : public AbstractManager
{
    Q_OBJECT

public:
    AbstractDataManager(QObject *parent = nullptr);
    ~AbstractDataManager() override;

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    virtual void updateUserData(const UserData &userData);
    virtual void sendOfflineActions();
    virtual void setupDatabase(const QString &dbPath);

protected:
    QThread m_processingThread;
    UserData m_userData;
};

#endif // DATAMANAGER_H
