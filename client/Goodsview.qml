import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    id: root
    property alias model: listView.model

    signal orderRequested(int index, int count)


    Rectangle {
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillWidth: true
        implicitHeight: 400

        border.width: 1
        border.color: "black"

        ListView {
            id: listView
            anchors.fill: parent

            highlight: Rectangle { color: "green" }

            delegate: Label {
                text: model.record.name

                MouseArea {
                    anchors.fill: parent
                    onClicked: listView.currentIndex = index;
                }
            }

            onCountChanged: console.debug("Delegates count:", count);
        }
    }

    ColumnLayout {
        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillWidth: true

        RowLayout {
            Label { text: "Count" }
            TextField { id: countField }
        }

        Button {
            text: "Order"
            enabled: listView.currentIndex != -1

            onClicked: orderRequested(listView.currentIndex,
                                     countField.text);
        }
    }
}
