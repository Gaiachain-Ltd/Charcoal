#ifndef USERMANAGER_H
#define USERMANAGER_H

#include "abstractmanager.h"

class UserManager : public AbstractManager
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = nullptr);

    void setupQmlContext(QQmlApplicationEngine &engine);
};

#endif // USERMANAGER_H
