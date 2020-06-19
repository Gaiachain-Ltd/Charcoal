#pragma once

#include <QObject>

#include "common/abstractuserdata.h"

struct UserData : public AbstractUserData
{
    Q_GADGET

    Q_PROPERTY(QString code MEMBER code)
    Q_PROPERTY(QString contact MEMBER contact)
    Q_PROPERTY(QString job MEMBER job)
    Q_PROPERTY(QString name MEMBER name)

public:
    QString code;
    QString contact;
    QString job;
    QString name;
};

Q_DECLARE_METATYPE(UserData)
