#include "picturesmanager.h"
#include "common/logs.h"
#include "common/tags.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include <QMetaEnum>

#include <QDebug>

PicturesManager::PicturesManager(QObject *parent) : AbstractManager(parent)
{
    prepareDirectories();
}

PicturesManager::~PicturesManager()
{
}

void PicturesManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("picturesManager"), this);
}

QString PicturesManager::pictureStoragePath() const
{
    return m_path;
}

QString PicturesManager::cachePath() const
{
    return m_cachePath;
}

void PicturesManager::saveDocumentPhoto(const QString &path) const
{
    savePhoto(path, PicturesManager::PictureType::Document);
}

void PicturesManager::saveReceiptPhoto(const QString &path) const
{
    savePhoto(path, PicturesManager::PictureType::Receipt);
}

void PicturesManager::discardPhoto(const QString &path) const
{
    if (QFile::exists(path)) {
        qDebug() << "Removing photo:" << path;
        QFile::remove(path);
    } else {
        qWarning() << "Could not remove photo:" << path;
    }
}

QStringList PicturesManager::documents() const
{
    return photosOfType(PicturesManager::PictureType::Document);
}

QStringList PicturesManager::receipts() const
{
    return photosOfType(PicturesManager::PictureType::Receipt);
}

/*!
 * Resets current "selection" of pictures - used by TickMarkIconProvider.
 */
void PicturesManager::resetCurrentPictures()
{
    m_currentDocuments.clear();
    m_currentReceipts.clear();
}

QStringList PicturesManager::moveToCache(const QVariantList &photos) const
{
    QStringList list;
    for (const QVariant &photo : photos) {
        list.append(photo.toString());
    }

    return moveToCache(list);
}

QStringList PicturesManager::moveToCache(const QStringList &photos) const
{
    QStringList result;
    for (const QString &photo : photos) {
        const QFileInfo info(photo);
        const QString fileName(info.fileName());
        const QString destination(m_cachePath + "/" + fileName);

        if (QFile::rename(photo, destination)) {
            result.append(fileName);
        } else {
            qWarning() << "Moving to cache has failed!" << photo << destination;
        }
    }

    return result;
}

QString PicturesManager::pictureTypeString(const PicturesManager::PictureType type)
{
    const auto se = QMetaEnum::fromType<PicturesManager::PictureType>();
    return se.valueToKey(int(type));
}

void PicturesManager::prepareDirectories() const
{
    if (QDir(m_path).exists() == false) {
        if (QDir(m_base).mkdir(m_picturesDir) == false) {
            qWarning() << "Could not create the path for pictures!" << m_path;
            return;
        }
    }

    const QString dir(m_savePath);
    if (QDir(dir).exists() == false) {
        if (QDir(m_path).mkdir(m_saveDir) == false) {
            qWarning() << "Could not create the path for pictures!" << dir;
            return;
        }
    }

    const QString dirCache(m_cachePath);
    if (QDir(dirCache).exists() == false) {
        if (QDir(m_path).mkdir(m_cacheDir) == false) {
            qWarning() << "Could not create the cache path for pictures!" << dir;
            return;
        }
    }
}

void PicturesManager::savePhoto(const QString &path,
                                const PicturesManager::PictureType type) const
{
    qDebug() << "Checking photo:" << path << type;
    if (QFile::exists(path)) {
        QString typeString;
        if (type == PicturesManager::PictureType::Document) {
            typeString = Tags::document;
        } else if (type == PicturesManager::PictureType::Receipt) {
            typeString = Tags::receipt;
        }

        const QString dir(m_savePath);
        const QString fileName(typeString + "-"
                               + QDateTime::currentDateTime()
                                     .toString(Qt::DateFormat::ISODate));
        const QString destination(dir + "/" + fileName + "." + QFileInfo(path).suffix());
        qDebug() << "Saving photo:" << destination;

        if (QFile::rename(path, destination) == false) {
            qWarning() << "File could not be moved!" << path << destination;
        }
    } else {
        qWarning() << "Photo does not exist!" << path;
    }
}

void PicturesManager::cleanUpWaitingPictures() const
{
    const QDir picturesDir(m_path);
    const QStringList files(picturesDir.entryList(QDir::Files | QDir::NoDotAndDotDot));
    for (const QString &file : files) {
        QFile::remove(file);
    }
}

QStringList PicturesManager::photosOfType(const PicturesManager::PictureType type) const
{
    const QString dir(m_savePath);
    QString filter;
    if (type == PicturesManager::PictureType::Document) {
        filter = Tags::document;
    } else if (type == PicturesManager::PictureType::Receipt) {
        filter = Tags::receipt;
    }

    filter += "*";

    QStringList result;
    const auto list = QDir(dir).entryInfoList(QStringList { filter }, QDir::Files);
    for (const QFileInfo &info : list) {
        result += info.absoluteFilePath();
    }

    return result;
}

QStringList PicturesManager::currentReceipts() const
{
    return m_currentReceipts;
}

void PicturesManager::setCurrentReceipts(const QStringList &currentReceipts)
{
    m_currentReceipts = currentReceipts;
}

QStringList PicturesManager::currentDocuments() const
{
    return m_currentDocuments;
}

void PicturesManager::setCurrentDocuments(const QStringList &currentDocuments)
{
    m_currentDocuments = currentDocuments;
}
