/*
 * Copyright 2013 Heena Mahour <heena393@gmail.com>
 * Copyright 2013 Sebastian Kügler <sebas@kde.org>
 * Copyright 2013 Martin Klapetek <mklapetek@kde.org>
 * Copyright 2014 David Edmundson <davidedmundson@kde.org>
 * Copyright 2014 Kai Uwe Broulik <kde@privat.broulik.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.0
import QtQuick.Layouts 1.1
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as Components

Item {
    id: main

    Layout.minimumWidth: vertical ? 0 : sizehelper.paintedWidth + (units.smallSpacing * 2)
    Layout.maximumWidth: vertical ? Infinity : Layout.minimumWidth
    Layout.preferredWidth: vertical ? undefined : Layout.minimumWidth

    Layout.minimumHeight: vertical ? sizehelper.paintedHeight + (units.smallSpacing * 2) : 0
    Layout.maximumHeight: vertical ? Layout.minimumHeight : Infinity
    Layout.preferredHeight: vertical ? Layout.minimumHeight : theme.mSize(theme.defaultFont).height * 2

    property bool vertical: plasmoid.formFactor == PlasmaCore.Types.Vertical

    property int fuzzyness: plasmoid.configuration.fuzzyness

    readonly property var hourNames: [
        [ i18n("One o’clock"),
          i18n("Five past one"),
          i18n("Ten past one"),
          i18n("Quarter past one"),
          i18n("Twenty past one"),
          i18n("Twenty five past one"),
          i18n("Half past one"),
          i18n("Twenty five to one"),
          i18n("Twenty to one"),
          i18n("Quarter to one"),
          i18n("Ten to one"),
          i18n("Five to one") ],
        [ i18n("Two o’clock"),
          i18n("Five past two"),
          i18n("Ten past two"),
          i18n("Quarter past two"),
          i18n("Twenty past two"),
          i18n("Twenty five past two"),
          i18n("Half past two"),
          i18n("Twenty five to two"),
          i18n("Twenty to two"),
          i18n("Quarter to two"),
          i18n("Ten to two"),
          i18n("Five to two") ],
        [ i18n("Three o’clock"),
          i18n("Five past three"),
          i18n("Ten past three"),
          i18n("Quarter past three"),
          i18n("Twenty past three"),
          i18n("Twenty five past three"),
          i18n("Half past three"),
          i18n("Twenty five to three"),
          i18n("Twenty to three"),
          i18n("Quarter to three"),
          i18n("Ten to three"),
          i18n("Five to three") ],
        [ i18n("Four o’clock"),
          i18n("Five past four"),
          i18n("Ten past four"),
          i18n("Quarter past four"),
          i18n("Twenty past four"),
          i18n("Twenty five past four"),
          i18n("Half past four"),
          i18n("Twenty five to four"),
          i18n("Twenty to four"),
          i18n("Quarter to four"),
          i18n("Ten to four"),
          i18n("Five to four") ],
        [ i18n("Five o’clock"),
          i18n("Five past five"),
          i18n("Ten past five"),
          i18n("Quarter past five"),
          i18n("Twenty past five"),
          i18n("Twenty five past five"),
          i18n("Half past five"),
          i18n("Twenty five to five"),
          i18n("Twenty to five"),
          i18n("Quarter to five"),
          i18n("Ten to five"),
          i18n("Five to five") ],
        [ i18n("Six o’clock"),
          i18n("Five past six"),
          i18n("Ten past six"),
          i18n("Quarter past six"),
          i18n("Twenty past six"),
          i18n("Twenty five past six"),
          i18n("Half past six"),
          i18n("Twenty five to six"),
          i18n("Twenty to six"),
          i18n("Quarter to six"),
          i18n("Ten to six"),
          i18n("Five to six") ],
        [ i18n("Seven o’clock"),
          i18n("Five past seven"),
          i18n("Ten past seven"),
          i18n("Quarter past seven"),
          i18n("Twenty past seven"),
          i18n("Twenty five past seven"),
          i18n("Half past seven"),
          i18n("Twenty five to seven"),
          i18n("Twenty to seven"),
          i18n("Quarter to seven"),
          i18n("Ten to seven"),
          i18n("Five to seven") ],
        [ i18n("Eight o’clock"),
          i18n("Five past eight"),
          i18n("Ten past eight"),
          i18n("Quarter past eight"),
          i18n("Twenty past eight"),
          i18n("Twenty five past eight"),
          i18n("Half past eight"),
          i18n("Twenty five to eight"),
          i18n("Twenty to eight"),
          i18n("Quarter to eight"),
          i18n("Ten to eight"),
          i18n("Five to eight") ],
        [ i18n("Nine o’clock"),
          i18n("Five past nine"),
          i18n("Ten past nine"),
          i18n("Quarter past nine"),
          i18n("Twenty past nine"),
          i18n("Twenty five past nine"),
          i18n("Half past nine"),
          i18n("Twenty five to nine"),
          i18n("Twenty to nine"),
          i18n("Quarter to nine"),
          i18n("Ten to nine"),
          i18n("Five to nine") ],
        [ i18n("Ten o’clock"),
          i18n("Five past ten"),
          i18n("Ten past ten"),
          i18n("Quarter past ten"),
          i18n("Twenty past ten"),
          i18n("Twenty five past ten"),
          i18n("Half past ten"),
          i18n("Twenty five to ten"),
          i18n("Twenty to ten"),
          i18n("Quarter to ten"),
          i18n("Ten to ten"),
          i18n("Five to ten") ],
        [ i18n("Eleven o’clock"),
          i18n("Five past eleven"),
          i18n("Ten past eleven"),
          i18n("Quarter past eleven"),
          i18n("Twenty past eleven"),
          i18n("Twenty five past eleven"),
          i18n("Half past eleven"),
          i18n("Twenty five to eleven"),
          i18n("Twenty to eleven"),
          i18n("Quarter to eleven"),
          i18n("Ten to eleven"),
          i18n("Five to eleven") ],
        [ i18n("Twelve o’clock"),
          i18n("Five past twelve"),
          i18n("Ten past twelve"),
          i18n("Quarter past twelve"),
          i18n("Twenty past twelve"),
          i18n("Twenty five past twelve"),
          i18n("Half past twelve"),
          i18n("Twenty five to twelve"),
          i18n("Twenty to twelve"),
          i18n("Quarter to twelve"),
          i18n("Ten to twelve"),
          i18n("Five to twelve") ]
    ]

    readonly property var dayTime: [
        i18n("Night"), i18n("Early morning"), i18n("Morning"), i18n("Almost noon"),
        i18n("Noon"), i18n("Afternoon"), i18n("Evening"), i18n("Late evening")
    ]

    readonly property var weekTime: [
        i18n("Start of week"), i18n("Middle of week"), i18n("End of week"), i18n("Weekend!")
    ]

    function timeString() {
        var d = new Date(dataSource.data["Local"]["DateTime"])
        var hours = d.getHours()
        var minutes = d.getMinutes()

        if (main.fuzzyness == 1 || main.fuzzyness == 2) {
            var sector = 0
            var realHour = 0

            if (main.fuzzyness == 1) {
                if (minutes > 2) {
                    sector = (minutes - 3) / 5 + 1
                }
            } else {
                if (minutes > 6) {
                    sector = ((minutes - 7) / 15 + 1) * 3
                }
            }

            var deltaHour = (sector < 7 ? 0 : 1)
            if ((hours + deltaHour) % 12 > 0) {
                realHour = (hours + deltaHour) % 12 - 1
            } else {
                realHour = 12 - ((hours + deltaHour) % 12 + 1)
            }

            sector = Math.floor(sector);
            if (sector == 12) {
                sector = 0
            }

            return hourNames[Math.floor(realHour)][sector]
        } else if (main.fuzzyness == 3) {
            return dayTime[Math.floor(hours / 3)]
        } else {
            var dow = d.getDay()

            var weekTimeId
            if (dow == 1) {
                weekTimeId = 0
            } else if (dow >= 2 && dow <= 4) {
                weekTimeId = 1
            } else if (dow == 5) {
                weekTimeId = 2
            } else {
                weekTimeId = 3
            }

            return weekTime[weekTimeId]
        }
    }

    Components.Label  {
        id: timeLabel
        font {
            weight: plasmoid.configuration.boldText ? Font.Bold : Font.Normal
            italic: plasmoid.configuration.italicText
            pixelSize: 1024
            pointSize: 0 // we need to unset pointSize otherwise it breaks the Text.Fit size mode
        }
        minimumPixelSize: theme.mSize(theme.smallestFont).height
        fontSizeMode: Text.Fit
        text: timeString()

        wrapMode: Text.NoWrap
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        height: 0
        width: 0
        anchors {
            fill: parent
            leftMargin: units.smallSpacing
            rightMargin: units.smallSpacing
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: plasmoid.expanded = !plasmoid.expanded
    }

    Text {
        id: sizehelper
        font.weight: timeLabel.font.weight
        font.italic: timeLabel.font.italic
        font.pixelSize: vertical ? theme.mSize(theme.defaultFont).height * 2 : 1024 // random "big enough" size - this is used as a max pixelSize by the fontSizeMode
        minimumPixelSize: theme.mSize(theme.smallestFont).height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: timeLabel.text
        fontSizeMode: vertical ? Text.HorizontalFit : Text.VerticalFit

        wrapMode: Text.NoWrap
        visible: false
        anchors {
            fill: parent
            leftMargin: units.smallSpacing
            rightMargin: units.smallSpacing
        }
    }
}
