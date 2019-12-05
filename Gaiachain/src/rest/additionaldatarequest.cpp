#include "additionaldatarequest.h"

#include <QDebug>
#include <QJsonObject>

#include "../common/logs.h"
#include "../common/tags.h"
#include "../common/dataglobals.h"
#include "../helpers/requestshelper.h"

const QString AdditionalDataRequest::sc_basePath = QStringLiteral("/additional_data/%1/");

const QMap<AdditionalDataRequest::DataType, QString> AdditionalDataRequest::sc_requestsPath = {
    { AdditionalDataRequest::DataType::All, sc_basePath.arg(QStringLiteral("all_data")) },
    { AdditionalDataRequest::DataType::Producers, sc_basePath.arg(QStringLiteral("producers")) },
    { AdditionalDataRequest::DataType::Companies, sc_basePath.arg(QStringLiteral("companies")) },
    { AdditionalDataRequest::DataType::Destinations, sc_basePath.arg(QStringLiteral("destinations")) }
};

AdditionalDataRequest::AdditionalDataRequest(const AdditionalDataRequest::DataType &dataType)
    : BaseRequest(sc_requestsPath.value(dataType), Type::Get)
{}
