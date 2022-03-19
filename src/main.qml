import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12

ApplicationWindow {
    visible: true

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Column {
        anchors.centerIn: parent

        PushButton { text: qsTr("Small") }
    }
}