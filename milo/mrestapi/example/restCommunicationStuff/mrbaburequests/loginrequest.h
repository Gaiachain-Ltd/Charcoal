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


#ifndef LOGINREQUEST_H
#define LOGINREQUEST_H

#include "mrbaburequest.h"

#include <QString>
#include <QStringList>
#include <QObject>

class LoginRequest : public MrBabuRequest
{
    Q_OBJECT

public:
    LoginRequest(const QString &email = QString::null,
                 const QString &password = QString::null);

signals:
    void loginSuccessful(const QString &token) const;
    void invalidPassword() const;
    void invalidUsername() const;
    void userInfo(const QString& firstName,
                  const QString& lastName,
                  const QString& phoneNumber,
                  const QString& civilId,
                  const QString& email) const;
protected:
    virtual void parse() override final;
};

#include <QSharedPointer>
using LoginRequestPtr = QSharedPointer<LoginRequest>;

#endif // LOGINREQUEST_H
