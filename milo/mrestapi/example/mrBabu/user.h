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


#ifndef USER_H
#define USER_H

#include <QObject>
#include <QByteArray>
#include <QString>
#include <QVector>
#include "addressptr.h"

class User : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY phoneChanged)
    Q_PROPERTY(QString civilId READ civilId WRITE setCivilId NOTIFY civilIdChanged)
    Q_PROPERTY(QByteArray password READ password WRITE setPassword NOTIFY passwordChanged)

    Q_PROPERTY(Address* billingAddress READ billingAddress NOTIFY billingAddressChanged)

public:
    User(QObject *parent = nullptr);
    QByteArray id() const;
    QString email() const;
    QString title() const;
    QString firstName() const;
    QString lastName() const;
    QString phone() const;
    QString civilId() const;
    QByteArray password() const;
    void setId(const QByteArray &id);
    void setEmail(const QString& email);
    void setTitle(const QString& title);
    void setFirstName(const QString& name);
    void setLastName(const QString& name);
    void setPhone(const QString& phone);
    void setCivilId(const QString& id);
    void setPassword(const QByteArray& password);
    Address* billingAddress();
    void clear();

signals:
    void emailChanged() const;
    void titleChanged() const;
    void firstNameChanged() const;
    void lastNameChanged() const;
    void phoneChanged() const;
    void civilIdChanged() const;
    void idChanged() const;
    void passwordChanged() const;
    void billingAddressChanged() const;

private:
    QByteArray mId;
    QString mEmail;
    QByteArray mPassword;
    QString mTitle;
    QString mFirstName;
    QString mLastName;
    QString mPhone;
    QString mCivilId;
    //TODO user can have multiple profiles
    enum AddressType { BillingAddress, ShippingAddress};
    QVector<AddressPtr> mAddresses;
};

#endif // USER_H
