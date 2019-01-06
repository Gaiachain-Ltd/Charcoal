#ifndef SESSION_H
#define SESSION_H

#include <QObject>
#include "RestAPIClient.h"

class Session : public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr);

    Q_INVOKABLE void login(const QString &email, const QString &password);

private:
    RestAPIClient mClient;
};

#endif // SESSION_H
