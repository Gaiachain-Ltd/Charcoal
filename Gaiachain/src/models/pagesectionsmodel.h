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
        LastRole
    }; //!!! Add new roles at the end

    explicit PageSectionsModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QHash<int, QByteArray> roleNames() const Q_DECL_OVERRIDE;

    void pagePushed(Enums::Page page);
    void pagePopped(Enums::Page currentTopPage);
    void stackReset(Enums::Page initialPage);

signals:

public slots:

private:
    const QHash<int, QByteArray> m_roleNames = {
        { SectionId, "id" },
        { SectionName, "sectionName" }
    };

    const QHash<Enums::Page, Enums::PageSections> m_pageToSection = {
        {Enums::Page::CalendarEvents, Enums::PageSections::CalendarSection},
        {Enums::Page::CalendarMonth, Enums::PageSections::CalendarSection},
        {Enums::Page::Calendar, Enums::PageSections::CalendarSection},
        {Enums::Page::EventDetails, Enums::PageSections::EventsDetailsSection},
        {Enums::Page::EventsList, Enums::PageSections::EventsListSection},
        {Enums::Page::ShipmentDetails, Enums::PageSections::ShipmentDetailsSection},
        {Enums::Page::ViewType, Enums::PageSections::ViewTypeSection},
        {Enums::Page::QRScanning, Enums::PageSections::QRSection},

        {Enums::Page::LanguageChosing, Enums::PageSections::DefaultSection},
        {Enums::Page::Login, Enums::PageSections::DefaultSection},
        {Enums::Page::ResourceChosing, Enums::PageSections::DefaultSection}
    };

    Gaia::ModelData m_data;

    int getShiftedIndex(ModelRole role) const;
};

#endif // PAGESECTIONSMODEL_H
