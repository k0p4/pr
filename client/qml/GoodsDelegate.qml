import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

Item {
    id: root

    property var value
    signal clicked()

    implicitWidth: layout.implicitWidth
    implicitHeight: layout.implicitHeight

    GridLayout {
        id: layout

        anchors.fill: parent
        columnSpacing: 10

        Label { text: "Name: " + value.name }
        Label { text: "Vendor code: " + value.vendorCode }
        Label { text: "Vendor name: " + value.vendorName }
        Label { text: "Product type: " + value.productType }
        Label { text: "Capacity: " + value.capacity }
        Label { text: "Cores: " + value.cores }
        Label { text: "Count: " + value.count; font.bold: true }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked();
    }
}
