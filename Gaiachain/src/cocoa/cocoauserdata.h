#pragma once

#include <QObject>

#include "common/abstractuserdata.h"

struct UserData : public AbstractUserData
{
    Q_GADGET

    Q_PROPERTY(quint32 cooperativeId MEMBER cooperativeId)
    Q_PROPERTY(QString cooperativeCode MEMBER cooperativeCode)
    Q_PROPERTY(QString cooperativeName MEMBER cooperativeName)

public:
    quint32 cooperativeId;
    QString cooperativeCode;
    QString cooperativeName;
};

Q_DECLARE_METATYPE(UserData)
