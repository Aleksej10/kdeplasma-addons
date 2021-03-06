/*
 *   SPDX-FileCopyrightText: 2008, 2014 Davide Bettio <davide.bettio@kdemail.net>
 *   SPDX-FileCopyrightText: 2015 Bernhard Friedreich <friesoft@gmail.com>
 *
 *   SPDX-License-Identifier: GPL-2.0-or-later
 */

import QtQuick 2.2
import org.kde.plasma.plasmoid 2.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.kquickcontrolsaddons 2.0 as QtExtra

Item {
    id: main

    readonly property date savedAt: plasmoid.configuration.savedAt;

    // display seconds in addition to hours and minutes (default: enabled)
    readonly property bool showSeconds: plasmoid.configuration.showSeconds;

    readonly property real digits: (showSeconds) ? 7 : 4.5;
    readonly property int digitH: ((height / 2) * digits < width ? height : ((width - (digits - 1)) / digits) * 2);
    readonly property int digitW: digitH / 2;

    PlasmaCore.Svg {
        id: timerSvg
        imagePath: "widgets/timer"
    }

    Column {
        anchors.centerIn: parent;
        Text {
            id: titleLabel;
            text: root.title;
            visible: root.showTitle;
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: parent.parent.height - digitH;
        }
        Row {
            id: timerDigits;
            SequentialAnimation on opacity {
                running: root.suspended;
                loops: Animation.Infinite;
                NumberAnimation {
                    duration: 800;
                    from: 1.0;
                    to: 0.2;
                    easing.type: Easing.InOutQuad;
                }
                PauseAnimation {
                    duration: 400;
                }
                NumberAnimation {
                    duration: 800;
                    from: 0.2;
                    to: 1.0;
                    easing.type: Easing.InOutQuad;
                }
                PauseAnimation {
                    duration: 400;
                }
            }

            TimerDigit {
                meaning: 60*60*10;
                num: ~~((root.seconds / (60*60)) / 10);
                suffix: (root.running && root.seconds < 60) ? "_1" : "";
            }
            TimerDigit {
                meaning: 60*60;
                num: ~~(~~(root.seconds / (60*60))) % 10;
                suffix: (root.running && root.seconds < 60) ? "_1" : "";
            }

            PlasmaCore.SvgItem {
                svg: timerSvg;
                width: digitW / 2;
                height: digitH;
                elementId: "separator" + ((root.running && root.seconds < 60) ? "_1" : "");
            }
            TimerDigit {
                meaning: 600;
                num: ~~(~~((root.seconds % (60*60)) / 60) / 10);
                suffix: (root.running && root.seconds < 60) ? "_1" : "";
            }
            TimerDigit {
                meaning: 60;
                num: ~~((root.seconds % (60*60)) / 60) % 10;
                suffix: (root.running && root.seconds < 60) ? "_1" : "";
            }

            PlasmaCore.SvgItem {
                svg: timerSvg;
                width: digitW / 2;
                height: digitH;
                elementId: "separator" + ((root.running && root.seconds < 60) ? "_1" : "");
                visible: showSeconds;
            }
            TimerDigit {
                meaning: 10;
                num: ~~((root.seconds % 60) / 10);
                suffix: (root.running && root.seconds < 60) ? "_1" : "";
                visible: showSeconds;
            }
            TimerDigit {
                meaning: 1;
                num: (root.seconds % 60) % 10;
                suffix: (root.running && root.seconds < 60) ? "_1" : "";
                visible: showSeconds;
            }
        }
    }

    MouseArea {
        anchors.fill: parent;
        onClicked: {
            if (root.running) {
                 root.stopTimer();
            } else {
                 root.startTimer();
            }
        }
        PlasmaCore.ToolTipArea {
            anchors.fill: parent
            subText: root.running ? i18n("Timer is running") : i18n("Use mouse wheel to change digits or choose from predefined timers in the context menu");
        }
    }

    function resetOpacity() {
        timerDigits.opacity = 1.0;
    }

    Component.onCompleted: {
        root.opacityNeedsReset.connect(resetOpacity);
    }

}
