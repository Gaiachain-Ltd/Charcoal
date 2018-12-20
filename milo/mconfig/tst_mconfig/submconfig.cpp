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

#include "submconfig.h"

SubMConfig::SubMConfig(QByteArray groupName) : MConfig(groupName)
{
    CONFIG_VALUE(testBool, QMetaType::Bool);
    CONFIG_VALUE(testByteArray, QMetaType::QByteArray);
    CONFIG_VALUE(testDateTime, QMetaType::QDateTime);
    CONFIG_VALUE(testFloat, QMetaType::Float);
    CONFIG_VALUE(testInt, QMetaType::Int);
    CONFIG_VALUE(testPoint, QMetaType::QPoint);
    CONFIG_VALUE(testPointF, QMetaType::QPointF);
    CONFIG_VALUE(testRect, QMetaType::QRect);
    CONFIG_VALUE(testRectF, QMetaType::QRectF);
    CONFIG_VALUE(testString, QMetaType::QString);
}

SubMConfig::~SubMConfig()
{

}

void SubMConfig::zeroData()
{
    testBool = false;
    testByteArray = NULL;
    testDateTime = QDateTime();
    testFloat = 0.0f;
    testInt = 0;
    testPoint = QPoint();
    testPointF = QPointF();
    testRect = QRect();
    testRectF = QRectF();
    testString = "";
}
