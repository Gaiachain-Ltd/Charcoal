#include "querymodel.h"

#include "database/dbhelpers.h"
#include "helpers/requestshelper.h"
#include "common/logs.h"

#include "controllers/session/restsessionmanager.h"

#include <QDebug>

QueryModel::QueryModel(QObject *parent) : QSqlQueryModel(parent)
{
    connect(this, &QueryModel::webDataRefreshed,
            this, &QueryModel::onWebDataRefreshed);
}

void QueryModel::setSessionManager(const QSharedPointer<RestSessionManager> &manager)
{
    m_sessionManager = manager;
}

void QueryModel::setUserManager(const QSharedPointer<UserManager> &manager)
{
    m_userManager = manager;
}

void QueryModel::setDbQuery(const QString &query)
{
    m_query = query;
}

void QueryModel::setDbConnection(const QString &connectionName)
{
    m_connectionName = connectionName;
    refresh();
}

bool QueryModel::isValid() const
{
    return m_connectionName.isEmpty() == false;
}

bool QueryModel::isDirty() const
{
    return m_isDirty;
}

void QueryModel::markDirty()
{
    m_isDirty = true;
}

/*!
 * Returns true if model data can change on Web side during application run.
 *
 * If set to `true`, model will try to get new data when refresh() is called.
 */
bool QueryModel::webModelCanChange() const
{
    return m_webModelCanChange;
}

bool QueryModel::hasQueuedRequests() const
{
    return m_queuedRequests.isEmpty() == false;
}

/*!
 * Begins sending queued requests. In order not to jam the server, we send POST
 * requests one by one (next request is sent when previous is finished - that
 * is, when we receive a reply).
 *
 * All other request types are sent en masse.
 */
void QueryModel::sendQueuedRequests()
{
    if (hasQueuedRequests()) {
        if (RequestsHelper::isOnline(m_sessionManager.get(), m_userManager.get()) == false) {
            return;
        }

        bool postSent = m_sessionManager->hasPostRequests();
        auto newQueue = m_queuedRequests;
        const QString token(m_sessionManager->token());
        for (const auto &request : qAsConst(m_queuedRequests)) {
            const bool isPost = (request->type() == BaseRequest::Type::Post);

            if (isPost) {
                if (postSent) {
                    continue;
                } else {
                    postSent = true;
                }
            }

            qDebug() << "SENDING QUEUED" << request->document();

            request->setToken(token);
            m_sessionManager->sendRequest(request,
                                          this,
                                          &QueryModel::webErrorHandler,
                                          &QueryModel::webReplyHandler);
            newQueue.removeOne(request);
        }

        m_queuedRequests = newQueue;
    }
}

void QueryModel::continueSendingQueuedRequests()
{
    if (hasQueuedRequests()) {
        qDebug() << "Continuing with the queue!";
        sendQueuedRequests();
    }
}

/*!
 * Sets whether model data can be changed by Web update. If \a canChange is
 * `true`, model will try to get new data when refresh() is called.
 *
 * \warning QueryModel does not send any signal when this property is changed.
 * Best call it only once, in constructor!
 */
void QueryModel::setWebModelCanChange(const bool canChange)
{
    m_webModelCanChange = canChange;
}

bool QueryModel::shouldRefreshWebData() const
{
    return webModelCanChange() || isDirty();
}

void QueryModel::refresh()
{
    if (m_query.isEmpty()) {
        qWarning() << RED("Cannot establish DB connection when query is empty!")
                   << m_connectionName;
        return;
    }

    if (m_connectionName.isEmpty()) {
        qWarning() << RED("Cannot establish DB connection when connection name is empty!")
                   << m_query;
        return;
    }

    if (shouldRefreshWebData()) {
        refreshWebData();
    } else {
        onWebDataRefreshed();
    }

    // Displays the data already present in DB.
    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));
}

/*!
 * Fetches new data from Web server. Reimplement in subclass to get Web
 * functionality.
 */
void QueryModel::refreshWebData()
{
    emit webDataRefreshed();
}

void QueryModel::onWebDataRefreshed()
{
    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));
    m_isDirty = false;
    emit refreshed();
}

void QueryModel::webErrorHandler(const QString &errorString,
                                 const QNetworkReply::NetworkError code)
{
    qDebug() << "Request error!" << errorString << code;
}

void QueryModel::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;
    emit webDataRefreshed();
}
