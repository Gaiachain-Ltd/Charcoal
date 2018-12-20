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


#include "userinforequest.h"

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QLoggingCategory>
#include <QDebug>

Q_LOGGING_CATEGORY(requestUserInfo, "request.userInfo")

/*!
 * \ingroup Network
 * \ingroup Example
 * @{
 */

/*!
 * \class UserInfoRequest
 * \brief Gets current user data
 *
 * Name, email, phone, etc.
 */

/*!
 * Contructs a new request. \a token is required and needs to be valid (you can
 * get it by logging in or registering).
 */
UserInfoRequest::UserInfoRequest(const QByteArray& token)
    : MrBabuRequest("change-user-data")
{
    if (!token.isEmpty()) {
        QJsonObject object {
            {"token", QJsonValue(QString(token))},
            {"first_name", QJsonValue("")},
            {"last_name", QJsonValue("")},
            {"username", QJsonValue("")},
            {"phone", QJsonValue("")},
            {"civilid", QJsonValue("")},
            {"email", QJsonValue("")}
        };

        mRequestDocument.setObject(object);
        mPriority = Priority::Normal;
        mType = Type::Post;
    } else {
        qCDebug(requestUserInfo) << "Error: missing user token" << token;
    }
}

/*!
 * All data is returned on success, by emitting userInfo() signal.
 */
void UserInfoRequest::parse()
{
    const QJsonObject object(mReplyDocument.object());

    const QString firstName(object.value("first_name").toString());
    const QString lastName(object.value("last_name").toString());
    const QString phoneNumber(object.value("phone").toString());
    const QString civilId(object.value("civilid").toString());
    const QString email(object.value("email").toString());

    if (firstName.isEmpty() && lastName.isEmpty()) {
        qCDebug(requestUserInfo) << mReplyDocument.toJson();
    }

    qCDebug(requestUserInfo).noquote() << "Received user info:"
                                       << firstName.left(1) << lastName.left(1)
                                       << phoneNumber.length()
                                       << civilId.length() << email.length();
    emit userInfo(firstName, lastName, phoneNumber, civilId, email);
}

/*! @} */
