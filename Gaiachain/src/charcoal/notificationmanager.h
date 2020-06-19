#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include "controllers/abstractmanager.h"
#include "common/enums.h"

#include <QObject>

class NotificationManager : public AbstractManager
{
    Q_OBJECT

public:
    explicit NotificationManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine) override;

public slots:
    void stepComplete(const Enums::SupplyChainAction step, const QString &id) const;

signals:
    void notify(const Enums::Page page,
                const QString &header,
                const QString &text,
                const QString &redirectText) const;
};

#endif // NOTIFICATIONMANAGER_H
