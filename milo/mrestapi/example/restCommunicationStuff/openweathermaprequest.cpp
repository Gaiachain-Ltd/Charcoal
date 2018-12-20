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


#include "openweathermaprequest.h"
#include "openweathermaprequestconfig.h"

#include <QDebug>

/*!
 * \ingroup Network
 * \ingroup Example
 * @{
 */

/*!
 * \class OpenWeatherMapRequest
 * \brief Convenience class - contains preset Web API URL.
 *
 * When subclassing, see the tips and warnings from Request class documentation.
 */

/*!
 * Adds \a method to the URL of Web API server.
 */

OpenWeatherMapRequest::OpenWeatherMapRequest(const QString &method)
    : MRestRequest(method)
{
    setMethod(method);
    setPriority(Priority::Normal);
}

/*!
 * Sets the method path to \a apiMethodPath. This means that \a apiMethodPath
 * string will be appended to OpenWeatherMap base API URL.
 *
 * Example:
 * \code
 * request.setMethod("weather?q=Lublin&units=metric");
 * // request URL is now: http://api.openweathermap.org/data/2.5/weather?q=Lublin
 * &units=metric&APPID=dd67c51ac1e36ec8fa90ecab89f3e850
 * \endcode
 */
void OpenWeatherMapRequest::setMethod(const QString &apiMethodPath)
{
    mApiMethod = apiMethodPath;
    OpenWeatherMapRequestConfig base;
    setAddress(QUrl(base.baseUrl + mApiMethod + "&APPID=" + base.mAppID));
}

/*! @} */
