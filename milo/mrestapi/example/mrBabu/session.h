/*******************************************************************************
Copyright (C) 2017 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/


#ifndef SESSION_H
#define SESSION_H

#include <QByteArray>
#include <QObject>
#include <QPointer>
#include "userptr.h"
#include "config.h"

class RestAPIClient;

class Session : public QObject, public Config
{
    Q_OBJECT
    Q_PROPERTY(User* user READ user NOTIFY userChanged)

public:
    explicit Session(QObject *parent = nullptr);
    ~Session();
    void setClient(RestAPIClient *client);
    Q_INVOKABLE bool hasToken() const;
    Q_INVOKABLE void login(const QString& email, const QByteArray& password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void registerUser(const QString& email, const QString& password,
                                  const QString& firstName, const QString& lastName,
                                  const QString& phone, const QString& civilId,
                                  const QString& address, const QString& city,
                                  const QString& zip, const QString& country);
    Q_INVOKABLE void getUserInfo();

    User* user() const;

signals:
    void loginSuccessful(const QString& token) const;
    void loginError(const QString& msgs) const;
    void invalidPassword() const;
    void invalidUsername() const;
    void registrationSuccessful(const QString& apiToken) const;
    void registrationError(const QString& errors) const;
    void userChanged(User* user) const;
    void userInfoError(const QString& error) const;

private:
    void onLoginSuccessful(const QString& token);
    void onUserInfo(const QString& firstName,
                    const QString& lastName,
                    const QString& phoneNumber,
                    const QString& civilId,
                    const QString& email);

    QByteArray mToken;
    UserPtr mCurrentUser;
    QPointer<RestAPIClient> mClient;
};

#endif // SESSION_H
