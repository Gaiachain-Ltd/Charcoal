#include "userdata.h"

bool UserData::isAnonymous() const
{
    return (type == Enums::UserType::Annonymous);
}
