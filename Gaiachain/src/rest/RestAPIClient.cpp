#include "RestAPIClient.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(mrbrequest, "request")

RestAPIClient::RestAPIClient(QObject *parent)
    : MRestRequestManager(parent)
{
}

