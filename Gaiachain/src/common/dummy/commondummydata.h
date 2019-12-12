#ifndef COMMONDUMMYDATA_H
#define COMMONDUMMYDATA_H

#include <QStringList>

#include "../../common/enums.h"

class CommonDummyData final
{
public:
    static QHash<QString, Enums::UserType> usersData();

    static QStringList availableLogins();
    static QString commonPassword();

private:
    CommonDummyData();

    static QList<QString> sortedLogins();
};

#endif // COMMONDUMMYDATA_H
