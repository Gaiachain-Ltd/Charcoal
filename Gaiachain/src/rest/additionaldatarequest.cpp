#include "additionaldatarequest.h"

#include <QDebug>
#include <QJsonObject>

#include "common/logs.h"
#include "common/tags.h"
#include "common/dataglobals.h"
#include "helpers/requestshelper.h"

const QString AdditionalDataRequest::sc_basePath = QStringLiteral("/additional_data/%1/");

const QMap<AdditionalDataRequest::DataType, QString> AdditionalDataRequest::sc_requestsPath = {
    { AdditionalDataRequest::DataType::All, sc_basePath.arg(QStringLiteral("all_data")) },
#ifdef COCOA
    { AdditionalDataRequest::DataType::Producers, sc_basePath.arg(QStringLiteral("producers")) },
    { AdditionalDataRequest::DataType::Companies, sc_basePath.arg(QStringLiteral("companies")) },
#endif
    { AdditionalDataRequest::DataType::Destinations, sc_basePath.arg(QStringLiteral("destinations")) },
#ifdef CHARCOAL
    { AdditionalDataRequest::DataType::OvenTypes, sc_basePath.arg(Tags::ovenTypes) },
    { AdditionalDataRequest::DataType::Parcels, sc_basePath.arg(Tags::parcels) },
    { AdditionalDataRequest::DataType::TreeSpecies, sc_basePath.arg(Tags::treeSpecies) },
    { AdditionalDataRequest::DataType::Villages, sc_basePath.arg(Tags::villages) },
#endif
};

AdditionalDataRequest::AdditionalDataRequest(const AdditionalDataRequest::DataType &dataType)
    : BaseRequest(sc_requestsPath.value(dataType), Type::Get)
{}
