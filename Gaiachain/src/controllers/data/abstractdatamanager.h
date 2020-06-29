#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "controllers/abstractmanager.h"
#include "common/enums.h"

#include <QThread>
#include <QColor>
#include <QVariant>
#include <QVariantMap>

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

    Q_INVOKABLE QVariantMap createSummaryItem(const QString &headerText,
        const QVariant &value,
        const QString &inputIconSource = QString(),
        const QString &suffix = QString(),
        const QColor &highlightColor = QColor(),
        const QColor &decorationColor = QColor(),
        const QColor &secondaryTextColor = QColor(),
        const Enums::DelegateType delegateType = Enums::DelegateType::Standard
        ) const;

protected:
    QThread m_processingThread;
    UserData m_userData;
};

#endif // DATAMANAGER_H
