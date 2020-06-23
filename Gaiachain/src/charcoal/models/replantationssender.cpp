#include "replantationssender.h"

#include "rest/baserequest.h"
#include "controllers/session/restsessionmanager.h"
#include "controllers/usermanager.h"

#include <QSqlQuery>
#include <QSqlRecord>

ReplantationsSender::ReplantationsSender(QObject *parent) : QueryModel(parent)
{
    setWebModelCanChange(false);
    setDbQuery("SELECT id, plotId, userId, numberOfTrees, locationLatitude, "
               "locationLongitude, beginningDate "
               "FROM Replantations WHERE isCommitted=0");

    connect(this, &ReplantationsSender::refreshed,
            this, &ReplantationsSender::sendEvents);
}

void ReplantationsSender::sendEvents()
{
    const int count = rowCount();
    if (count == 0) {
        return;
    }

    /*
{
  "trees_planted": 0,
  "beginning_date": 0,
  "ending_date": 0,
  "location": "string",
  "plot": 0,
  "tree_specie": 0
}
     */

    const bool isLoggedIn = m_userManager->isLoggedIn();
    for (int i = 0; i < count; ++i) {
        // use query().next()?
        if (query().seek(i) == false) {
            continue;
        }

        const auto record = query().record();
        qDebug() << "Offline replantation is:" << record;

        const auto request = QSharedPointer<BaseRequest>::create(
            "/entities/replantation/",
            BaseRequest::Type::Post
            );

        if (isLoggedIn) {
            request->setToken(m_sessionManager->token());
            m_sessionManager->sendRequest(request);
        } else {
            // TODO: Use list of requests everywhere (in QueryModel).
            m_queuedRequests.append(request);
        }
    }
}
