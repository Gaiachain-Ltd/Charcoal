#pragma once

#include <QQuickPaintedItem>

#include <QColor>

using PointList = QVector<QPointF>;

class TrapezoidOvenItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(Dimension selectedDimension READ selectedDimension WRITE setSelectedDimension NOTIFY selectedDimensionChanged)

public:
    enum class Dimension {
        None,
        Width,
        Length,
        Height1,
        Height2
    };
    Q_ENUM(Dimension)

    TrapezoidOvenItem(QQuickItem *parent = nullptr);

    Dimension selectedDimension() const;

    void paint(QPainter *painter) override;

public slots:
    void setSelectedDimension(const Dimension selectedDimension);

signals:
    void selectedDimensionChanged(const Dimension selectedDimension) const;

private:
    PointList getPoints(const QRectF &boundingRectangle) const;
    PointList getTextAnchors(const PointList &points) const;

    const quint8 pointsCount = 8;
    const quint8 linesCount = 12;
    const QColor foregroundColor = QColor(Qt::GlobalColor::black);
    const QColor backgroundColor = QColor(Qt::GlobalColor::gray);
    const QColor selectionColor = QColor(Qt::GlobalColor::green);

    Dimension m_selectedDimension = Dimension::None;
};

