#pragma once

#include <QQuickPaintedItem>

#include <QColor>

using PointList = QVector<QPointF>;

class TrapezoidOvenItem : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(Dimension selectedDimension READ selectedDimension WRITE setSelectedDimension NOTIFY selectedDimensionChanged)

    Q_PROPERTY(QString ovenWidth READ ovenWidth WRITE setOvenWidth NOTIFY ovenWidthChanged)
    Q_PROPERTY(QString ovenLength READ ovenLength WRITE setOvenLength NOTIFY ovenLengthChanged)
    Q_PROPERTY(QString ovenHeight1 READ ovenHeight1 WRITE setOvenHeight1 NOTIFY ovenHeight1Changed)
    Q_PROPERTY(QString ovenHeight2 READ ovenHeight2 WRITE setOvenHeight2 NOTIFY ovenHeight2Changed)

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

    QString ovenWidth() const;
    QString ovenLength() const;
    QString ovenHeight1() const;
    QString ovenHeight2() const;

public slots:
    void setSelectedDimension(const Dimension selectedDimension);

    void setOvenWidth(const QString ovenWidth);
    void setOvenLength(const QString ovenLength);
    void setOvenHeight1(const QString ovenHeight1);
    void setOvenHeight2(const QString ovenHeight2);

signals:
    void selectedDimensionChanged(const Dimension selectedDimension) const;

    void ovenWidthChanged(const QString &ovenWidth) const;
    void ovenLengthChanged(const QString &ovenLength) const;
    void ovenHeight1Changed(const QString &ovenHeight1) const;
    void ovenHeight2Changed(const QString &ovenHeight2) const;

private:
    PointList getPoints(const QRectF &boundingRectangle) const;
    PointList getTextAnchors(const PointList &points, const int fontSize) const;

    const quint8 pointsCount = 8;
    const quint8 linesCount = 12;
    const QColor foregroundColor = QColor(Qt::GlobalColor::black);
    const QColor backgroundColor = QColor(Qt::GlobalColor::gray);
    const QColor selectionColor = QColor("#65CB00");

    Dimension m_selectedDimension = Dimension::None;
    QString m_ovenWidth;
    QString m_ovenLength;
    QString m_ovenHeight1;
    QString m_ovenHeight2;
};

