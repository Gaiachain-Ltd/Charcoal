#include "utility.h"

#include <QGuiApplication>
#include <QScreen>
#include <QColor>

#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include "../common/logs.h"
#include "../common/globals.h"

Utility &Utility::instance()
{
    static Utility u;
    return u;
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
    dpiScale = QGuiApplication::primaryScreen()->physicalDotsPerInch() / (332.5 * 0.55);
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

QString Utility::colorString(const QColor &color) const
{
    if (color.isValid() == false) {
        return QString();
    }

    return color.name(QColor::HexArgb);
}

QString Utility::pictureStoragePath() const
{
    const QString base(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    const QString pics("pictures");
    const QString path(base + "/" + pics);

    if (QDir(path).exists() == false) {
        if (QDir(base).mkdir(pics) == false) {
            qWarning() << "Could not create the path for pictures!" << path;
        }
    }

    return path;
}

void Utility::saveDocumentPhoto(const QString &path) const
{
    savePhoto(path, true);
}

void Utility::saveReceiptPhoto(const QString &path) const
{
    savePhoto(path, false);
}

void Utility::savePhoto(const QString &path, const bool isDocument) const
{
    qDebug() << "Checking photo:" << path << isDocument;
    if (QFile::exists(path)) {
        QString type;
        if (isDocument) {
            type = "document";
        } else {
            type = "receipt";
        }

        const QString base(Utility::pictureStoragePath());
        const QString saved("saved");
        const QString dir(base + "/" + saved);

        if (QDir(dir).exists() == false) {
            if (QDir(base).mkdir(saved) == false) {
                qWarning() << "Could not create the path for pictures!" << dir;
                QFile::remove(path);
                return;
            }
        }

        const QString fileName(type + "-" + QDateTime::currentDateTime()
                                                .toString(Qt::DateFormat::ISODate));
        const QString destination(dir + "/" + fileName + "." + QFileInfo(path).suffix());
        qDebug() << "Saving photo:" << destination;

        QFile::rename(path, destination);
    }
}

void Utility::discardPhoto(const QString &path) const
{
    if (QFile::exists(path)) {
        qDebug() << "Removing photo:" << path;
        QFile::remove(path);
    }
}

bool Utility::isWeekend(const QDate &date) const
{
    const int day = date.dayOfWeek();
    // TODO: Sat and Sun are not weekend days in all countries...
    return day == 6 || day == 7;
}

QDate Utility::previousMonth(const QDate &date) const
{
    return date.addMonths(-1);
}

QDate Utility::nextMonth(const QDate &date) const
{
    return date.addMonths(1);
}

int Utility::parseInt(const QString &num) const
{
    return num.toInt();
}

/*!
 * Returns a \a number with constant number of \a digits.
 */
QString Utility::constDigitsNumber(const int number, const int digits) const
{
    QString result(QString::number(number));

    if (result.size() > digits) {
        qWarning() << Q_FUNC_INFO << "Number is too big!" << number << digits;
    }

    while (result.size() < digits) {
        result.prepend("0");
    }

    return result;
}

bool Utility::validateId(const QString &id) const
{
    QString rawId = id;
    rawId.remove('-');
    QString::const_iterator it = rawId.constBegin();
    while(it != rawId.constEnd()) {
        const QChar &c = (*it);
        if (c.isLetterOrNumber() == false)
            return false;
        ++it;
    }
    return rawId.length() == QR_CODE_LENGTH;
}

bool Utility::validateEmail(const QString &email) const
{
    return m_emailRegex.exactMatch(email);
}

int Utility::getScannedIdLength() const
{
    return QR_CODE_LENGTH;
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
