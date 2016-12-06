import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("SmarthHome Monitor")

    function reload_image(msg) {
        main_rect.lbl_status.text = msg
        main_rect.img.source = "image://stream/q_img"
        main_rect.img.source = "image://stream/qt_img"
    }

    MainForm {
        id: main_rect
        anchors.fill: parent
        bt_connect.onClicked: {
            main_qml.bt_connect_clicked();
            bt_stop.enabled = true;
            bt_connect.enabled = false;
        }

        bt_stop.onClicked: {
            main_qml.bt_stop_clicked();
            bt_connect.enabled = true;
            bt_stop.enabled = false;
        }
    }
}
