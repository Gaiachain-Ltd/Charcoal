#ifndef RESTAPICLIENT_H
#define RESTAPICLIENT_H

#include "mrestrequestmanager.h"

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(sessionClient)

class RestAPIClient : public MRestRequestManager
{
    Q_OBJECT
public:
    explicit RestAPIClient(QObject *parent = nullptr);
};

#endif // RESTAPICLIENT_H
