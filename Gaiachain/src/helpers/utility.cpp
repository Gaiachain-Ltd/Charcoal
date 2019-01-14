#include "utility.h"

#include <QGuiApplication>
#include <QScreen>
#include <QDebug>

#include "../common/globals.h"

Utility *Utility::instance()
{
    static Utility u;
    return &u;
}

QString Utility::commodityToString(Enums::CommodityType ct)
{
    return enumToQString<Enums::CommodityType>(ct, "CommodityType").toLower();
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

qreal Utility::clamp(qreal v, qreal min, qreal max) const
{
    return qMin(max, qMax(min, v));
}

Utility::Utility()
{
    m_emailRegex = QRegExp(QStringLiteral("\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b"));
    m_emailRegex.setCaseSensitivity(Qt::CaseInsensitive);
    m_emailRegex.setPatternSyntax(QRegExp::RegExp);

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

QString Utility::formatRawId(QString id) const
{
    if (!id.contains('-')) {
        for (int i = QR_CODE_LENGTH - 4 ; i > 0 ; i-=4)
            id.insert(i, '-');
    }
    return id;
}

bool Utility::isWeekend(const QDate &date) const
{
    const int day = date.dayOfWeek();
    return day == 6 || day == 7;
}

int Utility::parseInt(const QString &num) const
{
    return num.toInt();
}

bool Utility::validateId(const QString &id) const
{
    QString::const_iterator it = id.constBegin();
    while(it != id.constEnd()) {
        const QChar &c = (*it);
        if (c.isLetterOrNumber() == false)
            return false;
        ++it;
    }
    return id.length() == QR_CODE_LENGTH;
}

int Utility::getScannedIdLength() const
{
    return QR_CODE_LENGTH;
}

bool Utility::validateEmail(const QString &email) const
{
    return m_emailRegex.exactMatch(email);
}

QDate Utility::convertDateString(const QString &dateStr, const QString &dateFormat) const
{
    QString format;
    if (dateFormat.isEmpty() || dateFormat.isNull()) {
        format = defaultDateFormat();
    } else {
        format = dateFormat;
    }

    return QDate::fromString(dateStr, format);
}

QString Utility::defaultDateFormat() const
{
    return m_dateFormat;
}

Enums::UserType Utility::userTypeFromString(const QString &text) const
{
    if (text == QStringLiteral("PRODUCER")) {
        return Enums::UserType::Producer;
    } else if (text == QStringLiteral("LOG_PARK")) {
        return Enums::UserType::LogParkWorker;
    } else if (text == QStringLiteral("SAWMILL")) {
        return Enums::UserType::SawmillWorker;
    } else if (text == QStringLiteral("EXPORTER")) {
        return Enums::UserType::Exporter;
    } else {
        return Enums::UserType::NotLoggedUser;
    }
}
