#ifndef RELATIONREQUEST_H
#define RELATIONREQUEST_H

#include "baserequest.h"

#include "../common/enums.h"

class RelationRequest : public BaseRequest
{
    Q_OBJECT
public:
    // reading data
    RelationRequest(const QStringList &ids = {});
    RelationRequest(const QString &id);

    // writting data
    RelationRequest(const QString &token, const QString &id, const QStringList &relatedIds);

private:
    static const QString sc_basePath;
    static const QString sc_multiplePath;
    static const QString sc_singlePath;
};


#endif // RELATIONREQUEST_H
