#include "commondummydata.h"

#include <QHash>

namespace {
const QHash<QString, Enums::UserType> UsersData = {
#ifdef COCOA
    { QStringLiteral("god@gaiachain.io"), Enums::UserType::SuperUser },
    { QStringLiteral("inspector@gaiachain.io"), Enums::UserType::Inspector },
    { QStringLiteral("pca@gaiachain.io"), Enums::UserType::PCA },
    { QStringLiteral("warehouseman@gaiachain.io"), Enums::UserType::Warehouseman },
    { QStringLiteral("cooperative@gaiachain.io"), Enums::UserType::CooperativeRepresentative }
#elif CHARCOAL
    { QStringLiteral("god@milosolutions.com"), Enums::UserType::SuperUser },
#endif
};
const QString CommonPassword = QStringLiteral("milo1024");
}

QHash<QString, Enums::UserType> CommonDummyData::usersData()
{
    return UsersData;
}

QStringList CommonDummyData::availableLogins()
{
    const static auto logins = sortedLogins();
    return logins;
}

QString CommonDummyData::commonPassword()
{
    return CommonPassword;
}

QList<QString> CommonDummyData::sortedLogins()
{
    auto logins = UsersData.keys();
    std::sort(logins.begin(), logins.end(),
              [](const QString &l, const QString &r) {
                  return UsersData.value(l) < UsersData.value(r);
              });

    return logins;
}
