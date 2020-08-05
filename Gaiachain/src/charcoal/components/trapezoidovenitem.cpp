#include "trapezoidovenitem.h"

#include <QPainter>
#include <QPen>

TrapezoidOvenItem::TrapezoidOvenItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setFlag(QQuickItem::Flag::ItemHasContents, true);
}

TrapezoidOvenItem::Dimension TrapezoidOvenItem::selectedDimension() const
{
    return m_selectedDimension;
}

void TrapezoidOvenItem::paint(QPainter *painter)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen fgPen = painter->pen();
    fgPen.setWidth(2);
    fgPen.setCapStyle(Qt::PenCapStyle::RoundCap);
    fgPen.setColor(foregroundColor);
    painter->setPen(fgPen);

    QPen bgPen(fgPen);
    bgPen.setColor(backgroundColor);
    bgPen.setWidth(1);

    QPen selectionPen(fgPen);
    selectionPen.setColor(selectionColor);
    selectionPen.setWidth(3);

    const auto boundingRectangle(boundingRect());
    const auto points = getPoints(boundingRectangle);

    for (int i = pointsCount - 1; i >= 0; --i) {
        switch (i) {
        case 0:
            painter->drawLine(points.at(i), points.at(1));

            if (m_selectedDimension == Dimension::Height1) {
                painter->setPen(selectionPen);
            }
            painter->drawLine(points.at(i), points.at(3));
            painter->setPen(fgPen);

            if (m_selectedDimension == Dimension::Width) {
                painter->setPen(selectionPen);
            }
            painter->drawLine(points.at(i), points.at(4));
            painter->setPen(fgPen);
            break;
        case 1:
            if (m_selectedDimension == Dimension::Height2) {
                painter->setPen(selectionPen);
            }
            painter->drawLine(points.at(i), points.at(2));
            painter->setPen(fgPen);

            painter->drawLine(points.at(i), points.at(5));
            break;
        case 2:
            if (m_selectedDimension == Dimension::Length) {
                painter->setPen(selectionPen);
            }
            painter->drawLine(points.at(i), points.at(3));
            painter->setPen(fgPen);

            painter->drawLine(points.at(i), points.at(6));
            break;
        case 3:
            painter->setPen(bgPen);
            painter->drawLine(points.at(i), points.at(7));
            painter->setPen(fgPen);
            break;
        case 4:
            painter->drawLine(points.at(i), points.at(5));

            painter->setPen(bgPen);
            painter->drawLine(points.at(i), points.at(7));
            painter->setPen(fgPen);
            break;
        case 5:
            painter->drawLine(points.at(i), points.at(6));
            break;
        case 6:
            painter->setPen(bgPen);
            painter->drawLine(points.at(i), points.at(7));
            painter->setPen(fgPen);
            break;
        case 7:
            // Nothing
            break;
        }
    }

    QFont font = painter->font();
    font.setFamily("PublicSans");
    font.setPixelSize(18);
    font.setBold(true);
    painter->setFont(font);

    const auto textAnchors = getTextAnchors(points, font.pixelSize());
    const QString measurement("%1 m");
    Dimension dimension = Dimension::Width;
    char currentLetter('A');
    for (const QPointF &point : textAnchors) {
        if (dimension == m_selectedDimension) {
            painter->setPen(selectionPen);
        } else {
            painter->setPen(bgPen);
        }

        QString text(currentLetter);
        switch (currentLetter) {
        case 'A':
            if (ovenWidth().isEmpty() == false) {
                text = measurement.arg(ovenWidth());
            }
            break;
        case 'B':
            if (ovenLength().isEmpty() == false) {
                text = measurement.arg(ovenLength());
            }
            break;
        case 'C':
            if (ovenHeight1().isEmpty() == false) {
                text = measurement.arg(ovenHeight1());
            }
            break;
        case 'D':
            if (ovenHeight2().isEmpty() == false) {
                text = measurement.arg(ovenHeight2());
            }
            break;
        }

        painter->drawText(point, text);

        currentLetter++;
        dimension = Dimension(int(dimension) + 1);
    }

    painter->restore();
}

QString TrapezoidOvenItem::ovenWidth() const
{
    return m_ovenWidth;
}

QString TrapezoidOvenItem::ovenLength() const
{
    return m_ovenLength;
}

QString TrapezoidOvenItem::ovenHeight1() const
{
    return m_ovenHeight1;
}

QString TrapezoidOvenItem::ovenHeight2() const
{
    return m_ovenHeight2;
}

void TrapezoidOvenItem::setSelectedDimension(
        const TrapezoidOvenItem::Dimension selectedDimension)
{
    if (m_selectedDimension == selectedDimension)
        return;

    m_selectedDimension = selectedDimension;
    emit selectedDimensionChanged(m_selectedDimension);
    update();
}

void TrapezoidOvenItem::setOvenWidth(const QString ovenWidth)
{
    if (m_ovenWidth == ovenWidth)
        return;

    m_ovenWidth = ovenWidth;
    emit ovenWidthChanged(m_ovenWidth);
    update();
}

void TrapezoidOvenItem::setOvenLength(const QString ovenLength)
{
    if (m_ovenLength == ovenLength)
        return;

    m_ovenLength = ovenLength;
    emit ovenLengthChanged(m_ovenLength);
    update();
}

void TrapezoidOvenItem::setOvenHeight1(const QString ovenHeight1)
{
    if (m_ovenHeight1 == ovenHeight1)
        return;

    m_ovenHeight1 = ovenHeight1;
    emit ovenHeight1Changed(m_ovenHeight1);
    update();
}

void TrapezoidOvenItem::setOvenHeight2(const QString ovenHeight2)
{
    if (m_ovenHeight2 == ovenHeight2)
        return;

    m_ovenHeight2 = ovenHeight2;
    emit ovenHeight2Changed(m_ovenHeight2);
    update();
}

PointList TrapezoidOvenItem::getPoints(const QRectF &boundingRectangle) const
{
    PointList result;

    const auto width = boundingRectangle.width();
    const auto height = boundingRectangle.height();

    /*
     * Points are added in this order:
     * - foreground: top left, top right, bottom right, bottom left
     * - background: top left, top right, bottom right, bottom left
     */

    for (int i = 0; i < pointsCount; ++i) {
        switch (i) {
        case 0:
            result.append(QPointF(.1 * width, .5 * height));
            break;
        case 1:
            result.append(QPointF(.7 * width, .5 * height));
            break;
        case 2:
            result.append(QPointF(.7 * width, .85 * height));
            break;
        case 3:
            result.append(QPointF(.1 * width, .6 * height));
            break;
        case 4:
            result.append(QPointF(.3 * width, .1 * height));
            break;
        case 5:
            result.append(QPointF(.9 * width, .1 * height));
            break;
        case 6:
            result.append(QPointF(.9 * width, .4 * height));
            break;
        case 7:
            result.append(QPointF(.3 * width, .2 * height));
            break;
        }
    }

    Q_ASSERT(result.length() == pointsCount);

    return result;
}

/*!
 * Based on list of \a points, prepares coordinates where dimension numbers
 * should be drawn.
 */
PointList TrapezoidOvenItem::getTextAnchors(const PointList &points,
                                            const int fontSize) const
{
    PointList result;

    QPointF a = QLineF(points.at(0), points.at(4)).center();
    QPointF b = QLineF(points.at(2), points.at(3)).center();
    QPointF c = QLineF(points.at(0), points.at(3)).center();
    QPointF d = QLineF(points.at(1), points.at(2)).center();

    const int padding = 14;
    const int halfPadding = padding / 2;
    a.setX(a.x() + halfPadding);
    b.setY(b.y() - halfPadding);
    c.setX(c.x() + halfPadding);
    d.setX(d.x() + halfPadding);

    const int vCenter = fontSize / 2;
    a.setY(a.y() + vCenter);
    //b.setY(b.y() + vCenter);
    c.setY(c.y() + vCenter);
    d.setY(d.y() + vCenter);

    result.append(a);
    result.append(b);
    result.append(c);
    result.append(d);

    Q_ASSERT(result.length() == 4);

    return result;
}
