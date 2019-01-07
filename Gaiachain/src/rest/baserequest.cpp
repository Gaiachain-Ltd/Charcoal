#include "baserequest.h"

#include "../common/globals.h"

BaseRequest::BaseRequest(const QString &method)
    : MRestRequest()
{
    setMethod(method);
    setPriority(Priority::Normal);
}

void BaseRequest::setMethod(const QString &apiMethodPath)
{
    mApiMethod = apiMethodPath;
    setAddress(QUrl(SERVER_ADDRESS + mApiMethod));
}
