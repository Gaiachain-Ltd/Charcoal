#ifndef PAGESECTIONSMODEL_H
#define PAGESECTIONSMODEL_H

#include <QAbstractListModel>
#include <QVector>
#include <QPair>

#include "../common/enums.h"
#include "../common/globals.h"

class PageSectionsModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ModelRole {
        SectionId = Qt::UserRole + 1,
        SectionName,
        EditSection,
        LastRole
    }; //!!! Add new roles at the end

    explicit PageSectionsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, const int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    Q_INVOKABLE int maxSectionsDepth() const;

    void pagePushed(const Enums::Page page, const bool isEdit = false);
    void pagePopped(const Enums::Page currentTopPage);
    void stackReset(const Enums::Page initialPage);

    Enums::Page getPageForSection(const Enums::PageSections section) const;

signals:

public slots:

private:
    const QHash<int, QByteArray> m_roleNames = {
        { SectionId, "id" },
        { SectionName, "sectionName" },
        { EditSection, "editSection" }
    };

    const QHash<Enums::Page, Enums::PageSections> m_pageToSection = {
        {Enums::Page::CalendarEvents, Enums::PageSections::CalendarSection},
        {Enums::Page::CalendarMonth, Enums::PageSections::CalendarSection},
        {Enums::Page::Calendar, Enums::PageSections::CalendarSection},
        {Enums::Page::EventDetails, Enums::PageSections::EventsDetailsSection},
        {Enums::Page::EventsList, Enums::PageSections::EventsListSection},
        {Enums::Page::ShipmentDetails, Enums::PageSections::ShipmentDetailsSection},
        {Enums::Page::ViewType, Enums::PageSections::ViewTypeSection},
        {Enums::Page::QRScanner, Enums::PageSections::QRSection},

        {Enums::Page::LanguageChosing, Enums::PageSections::DefaultSection}, // TO_DO_later
        {Enums::Page::Login, Enums::PageSections::DefaultSection}, // TO_DO_later
        {Enums::Page::ResourceChosing, Enums::PageSections::DefaultSection} // TO_DO_later
    };

    const QHash<Enums::PageSections, Enums::Page> m_sectionToFirstPage = {
        {Enums::PageSections::DefaultSection, Enums::Page::ViewType}, // TO_DO_later for default section
        {Enums::PageSections::ViewTypeSection, Enums::Page::ViewType},
        {Enums::PageSections::CalendarSection, Enums::Page::Calendar},
        {Enums::PageSections::EventsListSection, Enums::Page::EventsList},
        {Enums::PageSections::EventsDetailsSection, Enums::Page::EventDetails},
        {Enums::PageSections::ShipmentDetailsSection, Enums::Page::ShipmentDetails},
        {Enums::PageSections::QRSection, Enums::Page::QRScanner}
    };

    Gaia::ModelData m_data;
    const int m_maxDepth = 3;
    Enums::PageSections m_initialSection;
    bool m_isInitialSection = false;

    int getShiftedIndex(const ModelRole role) const;
    void pushBackData(const Enums::PageSections section, const bool isEdit = false);
};

#endif // PAGESECTIONSMODEL_H
