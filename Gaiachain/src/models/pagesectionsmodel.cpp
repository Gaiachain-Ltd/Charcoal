#include "pagesectionsmodel.h"

#include <QDebug>

#include "../helpers/utility.h"

PageSectionsModel::PageSectionsModel(QObject *parent)
    : QAbstractListModel (parent)
{
    Q_ASSERT_X(m_pageToSection.count() == static_cast<int>(Enums::Page::PageCount),
               "PageSectionsModel::PageSectionsModel",
               "You should provide section for all pages!");

}

int PageSectionsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_data.count();
}

QVariant PageSectionsModel::data(const QModelIndex &index, const int role) const
{
    if (role < Qt::UserRole || role >= ModelRole::LastRole)
        return {};

    if (index.row() < 0 || index.row() > m_data.count())
        return {};

    const int shiftedIndex = role - static_cast<int>(ModelRole::SectionId);
    auto values = m_data[index.row()];

    if (0 <= shiftedIndex && shiftedIndex < values.count())
        return values[shiftedIndex];

    Q_ASSERT(false);
    return {};
}

QHash<int, QByteArray> PageSectionsModel::roleNames() const
{
    return m_roleNames;
}

int PageSectionsModel::maxSectionsDepth() const
{
    return m_maxDepth;
}

void PageSectionsModel::pagePushed(const Enums::Page page)
{
    Enums::PageSections newSection = m_pageToSection[page];

    int shiftedIndex = getShiftedIndex(ModelRole::SectionId);
    Enums::PageSections currentSection = m_data.last()[shiftedIndex].value<Enums::PageSections>();

    if (newSection == Enums::PageSections::DefaultSection)
        return;

    // First section is special because it is removed after new one
    if (m_isInitialSection) {
        m_isInitialSection = false;
        m_data.pop_back();
    }

    if (currentSection != newSection) {
        if (m_data.count() >= m_maxDepth)
            qWarning() << "Page not pushed! Maximum depth of sections exceeded! Fix this!";

        pushBackData(newSection);
    }
}

void PageSectionsModel::pagePopped(const Enums::Page currentTopPage)
{
    Enums::PageSections topSection = m_pageToSection[currentTopPage];

    int shiftedIndex = getShiftedIndex(ModelRole::SectionId);
    Enums::PageSections currentSection = m_data.last()[shiftedIndex].value<Enums::PageSections>();

    if (currentSection != topSection) {
        m_data.pop_back();
        if (m_data.empty()) {
            m_isInitialSection = true;
            pushBackData(m_initialSection);
        }
    }
}

void PageSectionsModel::stackReset(Enums::Page initialPage)
{
    m_data.clear();
    m_isInitialSection = true;
    m_initialSection = m_pageToSection[initialPage];

    pushBackData(m_initialSection);
}

Enums::Page PageSectionsModel::getPageForSection(const Enums::PageSections section) const
{
    if (!m_sectionToFirstPage.contains(section)) {
        qWarning() << "Section" << section << "is invalid!";
        return Enums::Page::ViewType;
    }

    return m_sectionToFirstPage[section];
}

int PageSectionsModel::getShiftedIndex(const ModelRole role) const
{
    return static_cast<int>(role) - static_cast<int>(ModelRole::SectionId);
}

void PageSectionsModel::pushBackData(const Enums::PageSections section)
{
    QVariantList rowToInsert;
    rowToInsert.append({static_cast<int>(section),
                       Utility::enumToQString<Enums::PageSections>(section, "PageSections")});
    m_data.push_back(rowToInsert);
}
