#include "querymodel.h"

#include "database/dbhelpers.h"
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

bool QueryModel::hasQueuedRequest() const
{
    return m_queuedRequest.isNull() == false;
}

void QueryModel::sendQueuedRequest()
{
    m_sessionManager->sendRequest(m_queuedRequest,
                                  this,
                                  &QueryModel::webErrorHandler,
                                  &QueryModel::webReplyHandler);
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
}

/*!
 * Fetches new data from Web server. Reimplement in subclass to get Web
 * functionality.
 */
void QueryModel::refreshWebData()
{
    m_isDirty = false;
    emit webDataRefreshed();
}

void QueryModel::onWebDataRefreshed()
{
    setQuery(m_query, db::Helpers::databaseConnection(m_connectionName));
    emit refreshed();
}

void QueryModel::webErrorHandler(const QString &error,
                                 const QNetworkReply::NetworkError code)
{
    qDebug() << "Request error!" << error << code;
}

void QueryModel::webReplyHandler(const QJsonDocument &reply)
{
    qDebug() << "Request success!" << reply;
    emit webDataRefreshed();
}
