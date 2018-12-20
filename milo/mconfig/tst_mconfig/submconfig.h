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

#ifndef SUBMCONFIG_H
#define SUBMCONFIG_H

#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QString>
#include <QDateTime>

#include "mconfig.h"

class SubMConfig : public MConfig
{
public:
    SubMConfig(QByteArray groupName);
    ~SubMConfig();
    void zeroData();

    bool testBool = true;
    int testInt = 1;
    float testFloat = 1.0f;
    QByteArray testByteArray = "Test ByteArray";
    QString testString = "Test String";
    QPoint testPoint = QPoint(1, 1);
    QPointF testPointF = QPointF(1.0f, 1.0f);
    QRect testRect = QRect(4, 1, 2, 3);
    QRectF testRectF = QRectF(4.0f, 1.0f, 2.0f, 3.0f);
    QDateTime testDateTime = QDateTime::fromMSecsSinceEpoch(666);

    bool operator==(const SubMConfig &rhs) const {
        return (this->testBool == rhs.testBool &&
                this->testInt == rhs.testInt &&
                this->testString == rhs.testString &&
                this->testByteArray == rhs.testByteArray &&
                this->testPoint == rhs.testPoint &&
                this->testPointF == rhs.testPointF &&
                this->testRect == rhs.testRect &&
                this->testRectF == rhs.testRectF &&
                this->testDateTime.toString() == rhs.testDateTime.toString() &&
                this->testFloat - rhs.testFloat == 0);
    }

};

#endif // SUBMCONFIG_H
