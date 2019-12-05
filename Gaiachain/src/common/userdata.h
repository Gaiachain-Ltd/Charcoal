#ifndef USERDATA_H
#define USERDATA_H

#include <QObject>

#include "enums.h"

struct UserData
{
    Q_GADGET
    Q_PROPERTY(QString email MEMBER email)
    Q_PROPERTY(Enums::UserType type MEMBER type)
    Q_PROPERTY(quint32 cooperativeId MEMBER cooperativeId)
    Q_PROPERTY(QString cooperativeCode MEMBER cooperativeCode)
    Q_PROPERTY(QString cooperativeName MEMBER cooperativeName)

public:
    QString email;
    Enums::UserType type = Enums::UserType::Annonymous;

    quint32 cooperativeId;
    QString cooperativeCode;
    QString cooperativeName;

    Q_INVOKABLE bool isAnonymous() const;
};
Q_DECLARE_METATYPE(UserData)

#endif // USERDATA_H
