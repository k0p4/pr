import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

GridView {
    id: root

    property var value

    Label { text: "Name: " + value.name }
    Label { text: "Vendor code: " + value.vendorCode }
    Label { text: "Count: " + value.count }
    Label { text: "Vendor name: " + value.vendorName }
    Label { text: "Product type: " + value.productType }
    Label { text: "Capacity: " + value.capacity }
    Label { text: "Cores: " + value.cores }
}
