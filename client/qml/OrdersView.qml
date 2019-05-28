import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    id: root
    property alias model: listView.model


    Rectangle {
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillWidth: true
        implicitHeight: 400

        border.width: 1
        border.color: "black"

        ListView {
            id: listView
            anchors.fill: parent

            highlight: Rectangle { color: "green"; opacity: 0.5 }

            delegate: GoodsDelegate {
                value: listView.model.record.good
                onClicked: listView.currentIndex = index;
            }

            onCountChanged: console.debug("Delegates count:", count);
        }
    }
}
