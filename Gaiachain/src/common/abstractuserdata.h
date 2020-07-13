#pragma once

#include <QObject>

#include "enums.h"

struct AbstractUserData
{
    Q_GADGET

    Q_PROPERTY(QString email MEMBER email)
    Q_PROPERTY(Enums::UserType type MEMBER type)

public:
    QString email;
    Enums::UserType type = Enums::UserType::Anonymous;

    Q_INVOKABLE bool isAnonymous() const;
};
