/*
 * SPDX-FileCopyrightText: 2015 Dominik Haumann <dhaumann@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
import QtQuick 2.1
import QtQuick.Layouts 1.1

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.components 2.0 as PlasmaComponents // for Highlight
import org.kde.plasma.components 3.0 as PlasmaComponents3
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.kde.plasma.private.diskquota 1.0

Item {
    id: quotaApplet

    Layout.minimumWidth: units.gridUnit * 10
    Layout.minimumHeight: units.gridUnit * 2

    Plasmoid.status: {
        switch (diskQuota.status) {
            case DiskQuota.NeedsAttentionStatus:
                return PlasmaCore.Types.NeedsAttentionStatus
            case DiskQuota.ActiveStatus:
                return PlasmaCore.Types.ActiveStatus
        }
        // default case: DiskQuota.PassiveStatus
        return PlasmaCore.Types.PassiveStatus
    }

    Plasmoid.switchWidth: units.gridUnit * 10
    Plasmoid.switchHeight: units.gridUnit * 10

    Plasmoid.icon: diskQuota.iconName
    Plasmoid.toolTipMainText: diskQuota.toolTip
    Plasmoid.toolTipSubText: diskQuota.subToolTip

    Component.onCompleted: plasmoid.removeAction("configure")

    DiskQuota {
        id: diskQuota
    }

    Plasmoid.fullRepresentation: Item {
        id: root

        width: units.gridUnit * 20
        height: units.gridUnit * 14

        // HACK: connection to reset currentIndex to -1. Without this, when
        // uninstalling filelight, the selection highlight remains fixed (which is wrong)
        Connections {
            target: diskQuota
            function onCleanUpToolInstalledChanged() {
                if (!diskQuota.cleanUpToolInstalled) {
                    listView.currentIndex = -1
                }
            }
        }

        PlasmaComponents3.Label {
            visible: !diskQuota.quotaInstalled || listView.count == 0
            anchors.fill: parent
            text: diskQuota.quotaInstalled ? i18n("No quota restrictions found.") : i18n("Quota tool not found.\n\nPlease install 'quota'.")
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }

        PlasmaExtras.ScrollArea {
            anchors.fill: parent
            ListView {
                id: listView
                model: diskQuota.model
                boundsBehavior: Flickable.StopAtBounds
                highlight: PlasmaComponents.Highlight { }
                highlightMoveDuration: 0
                highlightResizeDuration: 0
                currentIndex: -1
                delegate: ListDelegateItem {
                    enabled: diskQuota.cleanUpToolInstalled
                    width: listView.width
                    mountPoint: model.mountPoint
                    details: model.details
                    iconName: model.icon
                    usedString: model.used
                    freeString: model.free
                    usage: model.usage
                }
            }
        }
    }
}
