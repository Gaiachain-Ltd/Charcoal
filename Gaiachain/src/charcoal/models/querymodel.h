#pragma once

#include <QSqlQueryModel>
#include <QSharedPointer>
#include <QNetworkReply>

class RestSessionManager;
class UserManager;
class BaseRequest;

class QueryModel : public QSqlQueryModel
{
    Q_OBJECT

    Q_PROPERTY(bool webModelCanChange READ webModelCanChange CONSTANT)

public:
    explicit QueryModel(QObject *parent = nullptr);

    void setSessionManager(const QSharedPointer<RestSessionManager> &manager);
    void setUserManager(const QSharedPointer<UserManager> &manager);

    void setDbQuery(const QString &query);
    void setDbConnection(const QString &connectionName);
    bool isValid() const;

    bool isDirty() const;
    void markDirty();

    bool isRequestPending() const;

    bool webModelCanChange() const;

    virtual bool hasQueuedRequests() const;
    virtual void sendQueuedRequests();
    void continueSendingQueuedRequests();

signals:
    void refreshed() const;
    void webDataRefreshed() const;
    void error(const QString &text) const;

public slots:
    virtual void refresh();
    virtual void refreshWebData();

protected slots:
    void onWebDataRefreshed();
    virtual void webErrorHandler(const QString &errorString,
                                 const QNetworkReply::NetworkError code);
    virtual void webReplyHandler(const QJsonDocument &reply);

protected:
    void setWebModelCanChange(const bool canChange);
    bool shouldRefreshWebData() const;

    QSharedPointer<RestSessionManager> m_sessionManager;
    QSharedPointer<UserManager> m_userManager;
    QVector<QSharedPointer<BaseRequest>> m_queuedRequests;

    bool m_webModelCanChange = false;
    bool m_isDirty = true;
    bool m_isPending = false;

    QString m_query;
    QString m_connectionName;
};

