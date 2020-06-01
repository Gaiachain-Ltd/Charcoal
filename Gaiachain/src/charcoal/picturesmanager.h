#ifndef PICTURESMANAGER_H
#define PICTURESMANAGER_H

#include "controllers/abstractmanager.h"

#include <QObject>

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

    Q_INVOKABLE void saveDocumentPhoto(const QString &path) const;
    Q_INVOKABLE void saveReceiptPhoto(const QString &path) const;
    Q_INVOKABLE void discardPhoto(const QString &path) const;

    Q_INVOKABLE QStringList documents() const;
    Q_INVOKABLE QStringList receipts() const;

private:
    void prepareDirectories() const;
    void savePhoto(const QString &path, const PicturesManager::PictureType type) const;
    void cleanUp() const;
    QStringList photosOfType(const PicturesManager::PictureType type) const;

    const QString m_base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    const QString m_document_keyword = "document";
    const QString m_receipt_keyword = "receipt";
    const QString m_savedDir = "saved";
    const QString m_picturesDir = "pictures";
    const QString m_path = m_base + "/" + m_picturesDir;
};

#endif // PICTURESMANAGER_H
