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


#include "registerrequest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QLoggingCategory>
#include <QDebug>

Q_LOGGING_CATEGORY(requestRegister, "request.register")

/*!
 * \ingroup Network
 * \ingroup Example
 * @{
 */

/*!
 * \class RegisterRequest
 * \brief Registers a new user
 */

/*!
 * Creates a new registration request using all registration details.
 *
 * Send it using Register::send()
 */
RegisterRequest::RegisterRequest(const QString& email, const QString& password,
                                 const QString& firstName, const QString& lastName,
                                 const QString& phone, const QString& civilId,
                                 const QString& address, const QString& city,
                                 const QString& zip, const QString& country)
    : MrBabuRequest("register")
{
    if (!email.isEmpty() && !password.isEmpty() && !firstName.isEmpty()
            && !lastName.isEmpty() && !address.isEmpty() && !city.isEmpty()
            && !zip.isEmpty() && !country.isEmpty()) {
        QJsonObject object {
            {"email", QJsonValue(email)},
            {"password", QJsonValue(password)},
            {"first_name", QJsonValue(firstName)},
            {"last_name", QJsonValue(lastName)},
            {"phone", QJsonValue(phone)},
            {"civilid", QJsonValue(civilId)},
            {"address", QJsonValue(address)},
            {"city", QJsonValue(city)},
            {"zipcode", QJsonValue(zip)},
            {"country", QJsonValue(country)},
            {"username", QJsonValue(email)},
            {"token", QJsonValue("")}
        };
        mRequestDocument.setObject(object);
        mPriority = Priority::High;
        mType = Type::Post;
    } else {
        qCDebug(requestRegister) << "Error: missing registration info"
                                 << email << password.length()
                                 << firstName << lastName << phone << civilId
                                 << address << city << zip << country;
    }
}

/*!
 * Emits registrationSuccessful() with a Web API Token when all went OK.
 *
 * Otherwise, emits replyError().
 */
void RegisterRequest::parse()
{
    const QJsonObject object(mReplyDocument.object());
    const QString token(object.value("Token").toString());

    if (token.isEmpty()) {
        qCDebug(requestRegister) << "Error in parsing server reply"
                                 << mReplyDocument.toJson()
                                 << token;
        emit replyError("Error in parsing server reply");
        return;
    }

    qCDebug(requestRegister) << "Registration successful" << token;
    emit registrationSuccessful(token);
}

/*! @} */
