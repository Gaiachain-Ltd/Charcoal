#ifndef ADDITIONALDATAREQUEST_H
#define ADDITIONALDATAREQUEST_H

#include "baserequest.h"

#include "../common/enums.h"

class AdditionalDataRequest : public BaseRequest
{
    Q_OBJECT
public:
    enum class DataType {
        All = 0,
        Producers,
        Buyers,
        Transporters,
        Destinations
    };
    Q_ENUM(DataType)

    AdditionalDataRequest(const DataType &dataType);

private:
    static const QString sc_basePath;
    static const QMap<DataType, QString> sc_requestsPath;
};


#endif // ADDITIONALDATAREQUEST_H
