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


#ifndef ADDRESS_H
#define ADDRESS_H

#include <QObject>
#include <QByteArray>
#include <QList>

class Address : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY phoneChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString country READ country WRITE setCountry NOTIFY countryChanged)
    Q_PROPERTY(QString zip READ zip WRITE setZip NOTIFY zipChanged)

public:
    Address(QObject *parent  = nullptr);
    QString firstName() const;
    QString lastName() const;
    QString phone() const;
    QString address() const;
    QString city() const;
    QString country() const;
    QString zip() const;
    void setFirstName(const QString& name);
    void setLastName(const QString& name);
    void setPhone(const QString& phone);
    void setAddress(const QString &address);
    void setCity(const QString& city);
    void setCountry(const QString& country);
    void setZip(const QString& zip);

signals:
    void firstNameChanged() const;
    void lastNameChanged() const;
    void phoneChanged() const;
    void cityChanged() const;
    void countryChanged() const;
    void zipChanged() const;
    void addressChanged() const;

private:
    //TODO replace QByteArray with QString for all members
    QByteArray mFirstName;
    QByteArray mLastName;
    QByteArray mPhone;
    QByteArray mAddress;
    QByteArray mCity;
    QByteArray mCountry;
    QByteArray mZip;
};

#endif // ADDRESS_H
