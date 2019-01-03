#include "utility.h"

#include <QGuiApplication>
#include <QScreen>

Utility *Utility::instance()
{
    static Utility u;
    return &u;
}

qreal Utility::scaleByDpi(qreal num) const
{
    return num * m_dpiScale;
}

qreal Utility::scaleRoundByDpi(qreal num) const
{
    if (num < 0 && num >= -5) {
        const qreal res = qRound(scaleByDpi(num));
        return std::min(res, -1.0);
    } else if (num > 0 && num <= 5) {
        const qreal res = qRound(scaleByDpi(num));
        return std::max(res, 1.0);
    } else {
        return scaleByDpi(num);
    }
}

qreal Utility::proportionalWidth(qreal val) const
{
    qreal prop = clamp(val, 0.0, m_refWidth) / m_refWidth;
    return prop * m_refWidth;
}

qreal Utility::proportionalHeight(qreal val) const
{
    qreal prop = clamp(val, 0.0, m_refHeight) / m_refHeight;
    return prop * m_refHeight;
}

qreal Utility::clamp(qreal v, qreal min, qreal max) const
{
    return qMin(max, qMax(min, v));
}

Utility::Utility()
{
    m_dpiScale = setupDpiScale();
}

qreal Utility::setupDpiScale()
{
    // 332.5 is the reference dpi (BlackBerry Q5 / Q10).
    qreal dpiScale = QGuiApplication::primaryScreen()->physicalDotsPerInch() / 332.5;

#ifdef Q_OS_LINUX_DESKTOP
    dpiScale = QGuiApplication::primaryScreen()->physicalDotsPerInch() / (332.5 * 0.6);
#endif

#ifdef Q_OS_IOS
    dpiScale = QGuiApplication::primaryScreen()->logicalDotsPerInch() / QGuiApplication::primaryScreen()->physicalDotsPerInch();
#endif

    return dpiScale;
}

int Utility::parseInt(const QString &num) const
{
    return num.toInt();
}
