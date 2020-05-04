import QtQuick 2.11

QtObject {
    id: top

    property var gridItem

    property date modelDate
    property int modelMonth
    property int modelDay
    property int modelWeekNumber

    property bool additionalRow: {
        if (modelMonth <= gridItem.month) {
            if (modelDay === gridItem.monthLastDay) {
                gridItem.lastDayWeekNumber = modelWeekNumber
            }
            return false
        }

        // a day from next week
        return (modelWeekNumber > gridItem.lastDayWeekNumber) ||
                (modelDate.getDate() === 0 && gridItem.locale.firstDayOfWeek === Locale.Sunday)
    }
}
