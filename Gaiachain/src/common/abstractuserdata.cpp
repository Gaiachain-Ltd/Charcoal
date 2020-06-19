#include "abstractuserdata.h"

bool AbstractUserData::isAnonymous() const
{
    return (type == Enums::UserType::Annonymous);
}
