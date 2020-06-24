/*******************************************************************************
Copyright (C) 2017 Milo Solutions
Contact: https://www.milosolutions.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/


#pragma once

#include <QObject>
#include <QUrl>
#include <QJsonDocument>
#include <QNetworkReply>

class QTimer;
class QNetworkReply;
class QNetworkAccessManager;

class MRestRequest : public QObject
{
    Q_OBJECT

public:
    enum class Type {None, Put, Post, Get, Delete};
    Q_ENUM(Type)

    explicit MRestRequest(const QUrl& url = QUrl());
    virtual ~MRestRequest();
    void setAddress(const QUrl& url);
    void setRequestTimeout(const quint32 msec = 5000);
    bool isHigherPriority(const MRestRequest& request);
    void sendWith(QNetworkAccessManager* manager);
    QUrl address() const;
    QString lastError() const;
    QJsonDocument document() const;
    void setDocument(const QJsonDocument &document);
    QByteArray rawData() const;

    void setRetryLimit(const uint retryLimit);
    uint retryCount() const;


signals:
    void finished() const;
    void replyError(const QString &msgs,
                    const QNetworkReply::NetworkError errorCode = QNetworkReply::UnknownServerError) const;

protected:
    enum class Priority {Bottom, Low, Normal, High, Top};
    void setPriority(const Priority priority);

    void send();
    virtual void retry();
    virtual void customizeRequest(QNetworkRequest &request);
    virtual void parse() = 0;

    Priority mPriority = Priority::Normal;
    Type mType = Type::Get;
    QUrl mUrl;
    QNetworkReply *mActiveReply = nullptr;
    uint mRequestRetryCounter = 0;
    quint32 mRequestTimeout = 0;
    uint mMaxRequestRetryCount = 3;
    QByteArray mReplyData;
    QJsonDocument mReplyDocument;
    QJsonDocument mRequestDocument;

private slots:
    void onReplyError(QNetworkReply::NetworkError code);
    void onReadyRead();
    void onReplyFinished();

private:
    QString mLastError;
    QTimer *mRequestTimer = nullptr;
    QNetworkAccessManager *mNetworkManager = nullptr;
};
