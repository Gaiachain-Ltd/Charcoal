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


#include "weather.h"

/*!
 * \addtogroup Example
 * @{
 */

/*!
 * \class Weather
 * \brief Representation of weather
 */

Weather::Weather(QObject *parent):
    QObject(parent)
{
}

QString Weather::cityName() const
{
    return mCityName;
}

int Weather::humidity() const
{
    return mHumidity;
}

int Weather::pressure() const
{
    return mPressure;
}

float Weather::temp() const
{
    return mTemp;
}

QString Weather::getDataAsString()
{
    return QString("\nCity: %1 \nHumidity: %2 \nPressure: %3 \nTemperature: %4")
            .arg(cityName()).arg(humidity()).arg(pressure()).arg(temp());
}

void Weather::setCityName(const QString &cityName)
{
    if (mCityName == cityName)
        return;

    mCityName = cityName;
    emit cityNameChanged(cityName);
}

void Weather::setHumidity(int humidity)
{
    if (mHumidity == humidity)
        return;

    mHumidity = humidity;
    emit humidityChanged(humidity);
}

void Weather::setPressure(int pressure)
{
    if (mPressure == pressure)
        return;

    mPressure = pressure;
    emit pressureChanged(pressure);
}

void Weather::setTemp(float temp)
{
    if (mTemp == temp)
        return;

    mTemp = temp;
    emit tempChanged(temp);
}

/*! @} */
