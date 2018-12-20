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


#include "weatherbycitynamerequest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QLoggingCategory>
#include <QDebug>

Q_LOGGING_CATEGORY(requestWeahterCityByName, "request.weahterCityByName")

WeatherByCityNameRequest::WeatherByCityNameRequest(const QString &cityName)
    : MRestRequest()
{
    setAddress(QUrl("http://api.openweathermap.org/data/2.5/weather?q=" + cityName + "&units=metric&APPID=dd67c51ac1e36ec8fa90ecab89f3e850"));
    mPriority = Priority::High;
    mType = Type::Get;
}

void WeatherByCityNameRequest::parse()
{
    const QJsonObject object(mReplyDocument.object());

    /*
     * {"cod":"404","message":"Error: Not found city"}
     */

    const QString cod{"cod"};
    const QString  errorCode{"404"};

    if (object.contains(cod) && object.value(cod).toString() == errorCode) {
        const QString errorMsgKey("message");
        const QString errorMessage(object.value(errorMsgKey).toString());

        qCDebug(requestWeahterCityByName) << "Server returned errors:"
                                << errorMessage;
        emit replyError(errorMessage);
        return;
    }

    // let's take the data that we are interested
    const QJsonObject main = object.value("main").toObject();

    const QString cityName = object.value("name").toString();
    int humidity = main.value("humidity").toInt();
    int pressure = main.value("pressure").toInt();
    float temp = main.value("temp").toDouble();

    emit weatherInfo(cityName, humidity, pressure, temp);
}
