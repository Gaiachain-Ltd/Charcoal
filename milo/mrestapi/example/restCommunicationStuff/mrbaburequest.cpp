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


#include "mrbaburequest.h"
#include "mrbaburequestconfig.h"

#include <QDebug>

/*!
 * \ingroup Network
 * \ingroup Example
 * @{
 */

/*!
 * \class MrBabuRequest
 * \brief Convenience class - contains preset Web API URL.
 *
 * When subclassing, see the tips and warnings from Request class documentation.
 */

/*!
 * Adds \a method to the URL of Web API server.
 */
MrBabuRequest::MrBabuRequest(const QString &method)
    : MRestRequest()
{
    setMethod(method);
    setPriority(Priority::Normal);
}

/*!
 * Sets the method path to \a apiMethodPath. This means that \a apiMethodPath
 * string will be appended to MrBabu base API URL.
 *
 * Example:
 * \code
 * request.setMethod("basket");
 * // request URL is now: http://ec2-52-33-133-227.us-west-2.compute.amazonaws.com/api/basket
 * \endcode
 */
void MrBabuRequest::setMethod(const QString &apiMethodPath)
{
    mApiMethod = apiMethodPath;
    MrBabuRequestConfig base;
    setAddress(QUrl(base.baseUrl + mApiMethod + "/?format=json"));
}

/*! @} */
