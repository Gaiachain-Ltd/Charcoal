#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include <QString>
#include <QMetaEnum>

#include "../common/enums.h"

class Utility : public QObject
{
    Q_OBJECT
public:
    static Utility *instance();

    template<typename QEnum>
    static QString enumToQString(QEnum value, const char *enumName)
    {
        const QMetaObject smo = Enums::staticMetaObject;
        const int indexOfType = smo.indexOfEnumerator(enumName);
        const QMetaEnum enumerator = smo.enumerator(indexOfType);
        return QString(enumerator.valueToKey(static_cast<int>(value)));
    }

    Q_INVOKABLE qreal scaleByDpi(qreal num);
    Q_INVOKABLE qreal scaleRoundByDpi(qreal num);

private:
    Utility();

    qreal setupDpiScale();

    qreal m_dpiScale;
};

#endif // UTILITY_H
