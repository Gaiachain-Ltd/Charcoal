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

    QPen fgPen = painter->pen();
    fgPen.setWidth(2);
    fgPen.setCapStyle(Qt::PenCapStyle::RoundCap);
    fgPen.setColor(foregroundColor);
    painter->setPen(fgPen);

    QPen bgPen(fgPen);
    bgPen.setColor(backgroundColor);
    QPen selectionPen(fgPen);
    selectionPen.setColor(selectionColor);

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

    const auto textAnchors = getTextAnchors(points);
    Dimension dimension = Dimension::Width;
    char text('A');
    for (const QPointF &point : textAnchors) {
        if (dimension == m_selectedDimension) {
            painter->setPen(selectionPen);
        } else {
            painter->setPen(bgPen);
        }

        painter->drawText(point, QChar(text));

        text++;
        dimension = Dimension(int(dimension) + 1);
    }

    painter->restore();
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

PointList TrapezoidOvenItem::getPoints(const QRectF &boundingRectangle) const
{
    PointList result(pointsCount);

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

    return result;
}

/*!
 * Based on list of \a points, prepares coordinates where dimension numbers
 * should be drawn.
 */
PointList TrapezoidOvenItem::getTextAnchors(const PointList &points) const
{
    PointList result(4);

    QPointF a = QLineF(points.at(0), points.at(4)).center();
    QPointF b = QLineF(points.at(2), points.at(3)).center();
    QPointF c = QLineF(points.at(0), points.at(3)).center();
    QPointF d = QLineF(points.at(1), points.at(2)).center();

    const int padding = 10;
    a.setX(a.x() - padding);
    b.setY(b.y() + padding);
    c.setX(c.x() - padding);
    d.setX(d.x() + padding);

    result.append(a);
    result.append(b);
    result.append(c);
    result.append(d);

    return result;
}
