import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2

Item {

    anchors.fill: parent

    property alias img: img
    property alias lbl_status: lbl_status
    property alias bt_stop: bt_stop
    property alias bt_connect: bt_connect

    Rectangle {
        id: rect
        color: "#69d56f"
        anchors.fill: parent
        ColumnLayout {
            id: columnLayout
            anchors.fill: parent
            anchors.margins: 3

            Button {
                id: button
                Layout.fillWidth: true
                text: qsTr("SMART HOME MONITOR")
                activeFocusOnPress: false
            }

            RowLayout {
                anchors.margins: 5
                spacing: 10
                Layout.fillWidth: true

                Label {
                    id: label
                    text: qsTr("STATUS: ")
                    font.pointSize: 12
                    fontSizeMode: Text.Fit
                    font.family: "Courier"
                }

                Label {
                    id: lbl_status
                    Layout.fillWidth: true
                    text: qsTr("Ready!")
                    fontSizeMode: Text.Fit
                    font.pointSize: 12
                    font.family: "Courier"
                }
            }

            Rectangle {
                id: img_container
                border.color: "black"
                Layout.fillHeight: true
                Layout.fillWidth: true

                Image {
                    id: img
                    anchors.fill: parent
                    source: "image://stream/q_img"
                    cache: false
                }
            }

            Button {
                id: bt_connect
                Layout.fillWidth: true
                text: qsTr("CONNECT")
                enabled: true
            }

            Button {
                id: bt_stop
                Layout.fillWidth: true
                text: qsTr("STOP")
                enabled: false
            }
        }
    }
}
