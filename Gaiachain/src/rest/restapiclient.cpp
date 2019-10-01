#include "restapiclient.h"

#include <QLoggingCategory>
Q_LOGGING_CATEGORY(sessionClient, "session.client")

RestAPIClient::RestAPIClient(QObject *parent)
    : MRestRequestManager(parent)
{
}

