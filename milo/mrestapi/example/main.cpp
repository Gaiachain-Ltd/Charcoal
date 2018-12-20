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


#include <QCoreApplication>
#include <QLoggingCategory>

#include "mrBabu/session.h"
#include "restapiclient.h"

#include "openweathermaprequests/weatherbycitynamerequest.h"
#include "openWeatherMap/weather.h"

Q_LOGGING_CATEGORY(coreMain, "core.main")

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    app.setApplicationVersion(APP_VERSION);
    app.setOrganizationName("Milo Solutions");
    app.setOrganizationDomain("milosolutions.com");
    app.setApplicationName("REST API communication - demo.");

    qCInfo(coreMain) << "Application data set."
                 << "\n\tName:" << app.applicationName()
                 << "\n\tOrganisation:" << app.organizationName()
                 << "\n\tDomain:" << app.organizationDomain()
                 << "\n\tVersion:" << app.applicationVersion();

    // Crate rest api client.
    RestAPIClient client;


    // [1] ########  Simple example of usage, based on MrBabuQt project. ########.

    Session session;
    session.setClient(&client);

    // This is only example - we do not send any request here but we can see how use this classes.
    // session.registerUser(...);
    // session.login(...);
    // session.getUserInfo();

    // [1] ######################################################################



    // [2] ########  Simple example of usage, based on OpenWeatherMap API. ########.

    Weather weatherInLublin;

    auto errorLambda = [](const QString &error) {
        // do something with data
        qCInfo(coreMain) << "Print error msg: " << error;
    };

    auto onWeatherInfoLambda = [&weatherInLublin](const QString &cityName,
            int humidity, int pressure, int temp) {

        // do something with data
        weatherInLublin.setCityName(cityName);
        weatherInLublin.setHumidity(humidity);
        weatherInLublin.setPressure(pressure);
        weatherInLublin.setTemp(temp);

        qCInfo(coreMain) << "Weather: " + weatherInLublin.getDataAsString();
    };

    // prepare some request
    auto badCityNameWeatheRequest = QSharedPointer<WeatherByCityNameRequest>::create("");
    QObject::connect(badCityNameWeatheRequest.data(), &WeatherByCityNameRequest::replyError, errorLambda);
    QObject::connect(badCityNameWeatheRequest.data(), &WeatherByCityNameRequest::weatherInfo, onWeatherInfoLambda);

    auto lublinWeatherRequest = QSharedPointer<WeatherByCityNameRequest>::create("Lublin");
    QObject::connect(lublinWeatherRequest.data(), &WeatherByCityNameRequest::replyError, errorLambda);
    QObject::connect(lublinWeatherRequest.data(), &WeatherByCityNameRequest::weatherInfo, onWeatherInfoLambda);

    // send request
    client.send(badCityNameWeatheRequest);
    client.send(lublinWeatherRequest);

    // [2] ######################################################################

    return app.exec();
}
