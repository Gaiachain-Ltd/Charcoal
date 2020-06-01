#include "picturesmanager.h"

#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>

#include <QDebug>

PicturesManager::PicturesManager(QObject *parent) : AbstractManager(parent)
{
    prepareDirectories();
}

PicturesManager::~PicturesManager()
{
    qWarning() << "TODO: clean up photos! By this point, they should be in the DB already";
    //cleanUp();
}

void PicturesManager::setupQmlContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty(QStringLiteral("picturesManager"), this);
}

QString PicturesManager::pictureStoragePath() const
{
    return m_path;
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

void PicturesManager::prepareDirectories() const
{
    if (QDir(m_path).exists() == false) {
        if (QDir(m_base).mkdir(m_picturesDir) == false) {
            qWarning() << "Could not create the path for pictures!" << m_path;
            return;
        }
    }

    const QString dir(m_path + "/" + m_savedDir);
    if (QDir(dir).exists() == false) {
        if (QDir(m_path).mkdir(m_savedDir) == false) {
            qWarning() << "Could not create the path for pictures!" << dir;
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
            typeString = m_document_keyword;
        } else if (type == PicturesManager::PictureType::Receipt) {
            typeString = m_receipt_keyword;
        }

        const QString dir(m_path + "/" + m_savedDir);
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

void PicturesManager::cleanUp() const
{
    QDir(m_path).removeRecursively();
}

QStringList PicturesManager::photosOfType(const PicturesManager::PictureType type) const
{
    const QString dir(m_path + "/" + m_savedDir);
    QString filter;
    if (type == PicturesManager::PictureType::Document) {
        filter = m_document_keyword;
    } else if (type == PicturesManager::PictureType::Receipt) {
        filter = m_receipt_keyword;
    }

    filter += "*";

    QStringList result;
    const auto list = QDir(dir).entryInfoList(QStringList { filter }, QDir::Files);
    for (const QFileInfo &info : list) {
        result += info.absoluteFilePath();
    }

    return result;
}
