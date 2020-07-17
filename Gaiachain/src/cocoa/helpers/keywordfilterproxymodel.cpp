#include "keywordfilterproxymodel.h"

#include <QDebug>

KeywordFilterProxyModel::KeywordFilterProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

QString KeywordFilterProxyModel::keyword() const
{
    return m_keyword;
}

void KeywordFilterProxyModel::setKeyword(const QString &keyword)
{
    if (m_keyword == keyword) {
        return;
    }

    m_keyword = keyword;
    setFilterWildcard("*" + m_keyword + "*");
    emit keywordChanged(m_keyword);
}

void KeywordFilterProxyModel::classBegin() {}

void KeywordFilterProxyModel::componentComplete() {}
