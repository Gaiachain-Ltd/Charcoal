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


#include "address.h"

/*!
 * \defgroup Example Example code
 * @{
 */

/*!
 * \class Address
 * \brief User address data
 */

Address::Address(QObject *parent) :
    QObject(parent)
{
    //TODO
}

QString Address::firstName() const
{
    return mFirstName;
}

QString Address::lastName() const
{
    return mLastName;
}

QString Address::phone() const
{
    return mPhone;
}

QString Address::address() const
{
    return mAddress;
}

QString Address::city() const
{
    return mCity;
}

QString Address::country() const
{
    return mCountry;
}

QString Address::zip() const
{
    return mZip;
}

void Address::setFirstName(const QString& name)
{
    if (mFirstName != name) {
        mFirstName = name.toUtf8();
        emit firstNameChanged();
    }
}

void Address::setLastName(const QString& name)
{
    if (mLastName != name) {
        mLastName = name.toUtf8();
        emit lastNameChanged();
    }
}

void Address::setPhone(const QString& phone)
{
    if (mPhone != phone) {
        mPhone = phone.toUtf8();
        emit phoneChanged();
    }
}

void Address::setAddress(const QString& address)
{
    if (mAddress == address)
        return;

    mAddress = address.toUtf8();
    emit addressChanged();
}

void Address::setCity(const QString& city)
{
    if (mCity != city) {
        mCity = city.toUtf8();
        emit cityChanged();
    }
}

void Address::setCountry(const QString& country)
{
    if (mCountry != country) {
        mCountry = country.toUtf8();
        emit countryChanged();
    }

}

void Address::setZip(const QString& zip)
{
    if (mZip != zip) {
        mZip = zip.toUtf8();
        emit zipChanged();
    }
}

/*! @} */
