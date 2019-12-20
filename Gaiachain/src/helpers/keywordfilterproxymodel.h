#ifndef KEYWORDSORTFILTERPROXYMODEL_H
#define KEYWORDSORTFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QQmlParserStatus>

class KeywordFilterProxyModel : public QSortFilterProxyModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QString keyword READ keyword WRITE setKeyword NOTIFY keywordChanged)

public:
    KeywordFilterProxyModel(QObject *parent = nullptr);

    QString keyword() const;

    // QQmlParserStatus
    void classBegin() override;
    void componentComplete() override;

public slots:
    void setKeyword(const QString &keyword);

signals:
    void keywordChanged(const QString &keyword) const;

private:
    QString m_keyword;
};

#endif // KEYWORDSORTFILTERPROXYMODEL_H
