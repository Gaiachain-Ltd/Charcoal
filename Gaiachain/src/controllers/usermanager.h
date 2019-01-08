#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

class UserManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);

    virtual void setupQmlContext(QQmlApplicationEngine &engine) Q_DECL_OVERRIDE;

public slots:
    void parseLoginData(const QJsonDocument &doc);
};

#endif // USERMANAGER_H
