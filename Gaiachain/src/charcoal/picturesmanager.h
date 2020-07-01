#ifndef PICTURESMANAGER_H
#define PICTURESMANAGER_H

#include "controllers/abstractmanager.h"

#include <QObject>
#include <QVariantList>

#include <QStandardPaths>

class PicturesManager : public AbstractManager
{
    Q_OBJECT

    Q_PROPERTY(QString pictureStoragePath READ pictureStoragePath CONSTANT)

public:
    enum class PictureType {
        Document,
        Receipt
    };
    Q_ENUM(PictureType)

    explicit PicturesManager(QObject *parent = nullptr);
    virtual ~PicturesManager();

    void setupQmlContext(QQmlApplicationEngine &engine) override;

    QString pictureStoragePath() const;
    QString cachePath() const;

    Q_INVOKABLE void saveDocumentPhoto(const QString &path) const;
    Q_INVOKABLE void saveReceiptPhoto(const QString &path) const;
    Q_INVOKABLE void discardPhoto(const QString &path) const;

    Q_INVOKABLE QStringList documents() const;
    Q_INVOKABLE QStringList receipts() const;

    Q_INVOKABLE void resetCurrentPictures();
    Q_INVOKABLE void cleanUpWaitingPictures() const;

    QStringList moveToCache(const QVariantList &photos) const;
    QStringList moveToCache(const QStringList &photos) const;

    void checkFileIsCached(const QString &fileName);

    static QString pictureTypeString(const PictureType type);

    QStringList currentDocuments() const;
    void setCurrentDocuments(const QStringList &currentDocuments);

    QStringList currentReceipts() const;
    void setCurrentReceipts(const QStringList &currentReceipts);

signals:
    void fetchPhoto(const QString &fileName) const;

private:
    void prepareDirectories() const;
    void savePhoto(const QString &path, const PicturesManager::PictureType type) const;
    QStringList photosOfType(const PicturesManager::PictureType type) const;

    const QString m_base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString m_saveDir = "saved";
    const QString m_cacheDir = "cache";
    const QString m_picturesDir = "pictures";
    const QString m_path = m_base + "/" + m_picturesDir;
    const QString m_savePath = m_path + "/" + m_saveDir;
    const QString m_cachePath = m_path + "/" + m_cacheDir;

    QStringList m_currentDocuments;
    QStringList m_currentReceipts;
};

#endif // PICTURESMANAGER_H
