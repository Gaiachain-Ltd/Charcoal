#ifndef TICKMARKICONPROVIDER_H
#define TICKMARKICONPROVIDER_H

#include <QPointer>
#include <QQuickImageProvider>

class PicturesManager;

class TickMarkIconProvider : public QQuickImageProvider
{
public:
    explicit TickMarkIconProvider();

    QPixmap requestPixmap(const QString &id, QSize *size,
                          const QSize &requestedSize) override;

    static QString name();;

    void setPicturesManager(PicturesManager *manager);

private:
    QPointer<PicturesManager> m_picturesManager;
};

#endif // TICKMARKICONPROVIDER_H
