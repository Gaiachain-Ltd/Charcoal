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


#include "user.h"
#include "address.h"

/*!
 * \addtogroup Example
 * @{
 */

/*!
 * \class User
 * \brief Represetation of application user
 */

User::User(QObject *parent) :
    QObject(parent)
{
    AddressPtr billingAddress = AddressPtr::create();
    mAddresses.append(billingAddress);
}

QString User::email() const
{
    return mEmail;
}

QString User::title() const
{
    return mTitle;
}

QString User::firstName() const
{
    return mFirstName;
}

QString User::lastName() const
{
    return mLastName;
}

QString User::phone() const
{
    return mPhone;
}

QString User::civilId() const
{
    return mCivilId;
}

QByteArray User::password() const
{
    return mPassword;
}

void User::setEmail(const QString& email)
{
    if (mEmail != email) {
        mEmail = email;
        emit emailChanged();
    }
}

void User::setTitle(const QString& title)
{
    if (mTitle != title) {
        mTitle = title;
        emit titleChanged();
    }
}

void User::setFirstName(const QString& name)
{
    if (mFirstName != name) {
        mFirstName = name;
        emit firstNameChanged();
    }
}

void User::setLastName(const QString& name)
{
    if (mLastName != name) {
        mLastName = name;
        emit lastNameChanged();
    }
}

void User::setPhone(const QString& phone)
{
    if (mPhone != phone) {
        mPhone = phone;
        emit phoneChanged();
    }
}

void User::setCivilId(const QString& id)
{
    if (mCivilId != id) {
        mCivilId = id;
        emit civilIdChanged();
    }
}

void User::setPassword(const QByteArray& password)
{
    if (mPassword != password) {
        mPassword = password;
        emit passwordChanged();
    }
}

Address *User::billingAddress()
{
    if (mAddresses.isEmpty()) {
        AddressPtr billingAddress = AddressPtr::create();
        mAddresses.append(billingAddress);
    }

    return mAddresses.first().data();
}

void User::clear()
{
    mId.clear();
    mEmail.clear();
    mPassword.clear();
    mTitle.clear();
    mFirstName.clear();
    mLastName.clear();
    mPhone.clear();
    mCivilId.clear();
    mAddresses.clear();
}

QByteArray User::id() const
{
    return mId;
}

void User::setId(const QByteArray &id)
{
    if (mId != id) {
        mId = id;
        emit idChanged();
    }
}

/*! @} */
