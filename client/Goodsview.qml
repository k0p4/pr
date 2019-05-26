import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    id: root
    property var modelData

    onModelDataChanged: {
        console.debug(JSON.stringify(modelData));
    }

    signal orderRequested(string vendorCode, int count)


    ListView {
        id: listView

        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        Layout.fillWidth: true
        implicitHeight: 400

        model: modelData

        highlight: Rectangle { color: "green" }

        delegate: Label {
            text: modelData.name

            MouseArea {
                anchors.fill: parent
                onClicked: listView.currentIndex = index;
            }
        }

        onCountChanged: console.debug("Delegates count:", count);
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

            onClicked: orderRequested(modelData[listView.currentIndex].vendorCode,
                                     countField.text);
        }
    }
}
