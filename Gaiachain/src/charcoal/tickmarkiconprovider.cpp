#include "tickmarkiconprovider.h"
#include "picturesmanager.h"

#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QBitmap>
#include <QPen>
#include <QRectF>
#include <QColor>

#include <QDebug>

TickMarkIconProvider::TickMarkIconProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}

QPixmap TickMarkIconProvider::requestPixmap(const QString &id, QSize *size,
                                            const QSize &requestedSize)
{
    qDebug() << "Pixmap request!" << id << *size << requestedSize;

    const int fullWidth = 44;
    const int fullHeight = 44;
    const int border = 1;
    const int width = fullWidth - (2*border);
    const int height = fullHeight - (2*border);

    if (size) {
        *size = QSize(width, height);
    }

    const QSize targetSize(requestedSize.width() > 0 ? requestedSize.width() : width,
                           requestedSize.height() > 0 ? requestedSize.height() : height);

    QPixmap pixmap(targetSize);

    if (m_picturesManager.isNull()) {
        qWarning() << "Pictures manager not set! Can't provide proper picture!"
                   << id << size << requestedSize;
        pixmap.fill(QColor(Qt::red));
        return pixmap;
    }

    const QStringList data = id.split('-');
    const bool hasPhotos = (data.at(1) == "true");

    if (hasPhotos == false) {
        pixmap = QPixmap::fromImage(QImage(":/ui/noPhoto"));
        //qDebug() << "No photos! Returning:" << id << pixmap.size() << *size;
        return pixmap;
    }

    QString firstImagePath;
    if (data.at(0) == PicturesManager::pictureTypeString(PicturesManager::PictureType::Document).toLower()) {
        firstImagePath = m_picturesManager->documents().at(0);
    } else if (data.at(0) == PicturesManager::pictureTypeString(PicturesManager::PictureType::Receipt).toLower()) {
        firstImagePath = m_picturesManager->receipts().at(0);
    }

    const QImage firstImage(firstImagePath);
    pixmap = QPixmap::fromImage(firstImage, Qt::ImageConversionFlag::MonoOnly)
                 .scaled(fullWidth, fullHeight,
                         Qt::AspectRatioMode::IgnoreAspectRatio,
                         Qt::TransformationMode::FastTransformation);

    // ROUNDED RECTANGLE WITH BORDER
    // Adapted from: https://stackoverflow.com/questions/50687993/how-do-i-add-a-border-to-a-rounded-qpixmap
    QBitmap map(pixmap.size());
    map.fill(Qt::color0);

    const int radius = 5;
    const QRectF borderRect(0.5, 0.5, width, height);

    // Transparent corners
    QPainter painter(&map);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(borderRect, radius, radius);
    pixmap.setMask(map);

    QPainter painter2(&pixmap);
    painter2.setRenderHint(QPainter::Antialiasing);
    painter2.setPen(QPen(QColor(QStringLiteral("#FF70F1")), border));
    painter2.drawRoundedRect(borderRect, radius, radius);

    //qDebug() << "Returning pixmap:" << id << pixmap.size() << *size;
    return pixmap;
}

QString TickMarkIconProvider::name()
{
    return QStringLiteral("tickmark");
}

void TickMarkIconProvider::setPicturesManager(PicturesManager *manager)
{
    m_picturesManager = manager;
}
