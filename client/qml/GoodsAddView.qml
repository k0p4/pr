import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    id: root

    signal addNewRequested(string name,
                           string vendorCode,
                           int count,
                           string vendorName,
                           string productType,
                           string capacity,
                           int cores)

    signal updateExistingRequested(string vendorCode, int count)


    RowLayout {
        Text { text: "Name:" }
        TextField { id: nameField }
    }

    RowLayout {
        Text { text: "Vendor code:" }
        TextField { id: vendorCodeField }
    }

    RowLayout {
        Text { text: "Count:" }
        TextField { id: countField }
    }

    RowLayout {
        Text { text: "Vendor name:" }
        TextField { id: vendorNameField }
    }

    RowLayout {
        Text { text: "Product type:" }
        TextField { id: productTypeField }
    }

    RowLayout {
        Text { text: "Capacity:" }
        TextField { id: capacityField }
    }

    RowLayout {
        Text { text: "Cores:" }
        TextField { id: coresField }
    }


    RowLayout {
        Button {
            text: "Add new"
            enabled: nameField.text &&
                     vendorCodeField.text &&
                     countField.text &&
                     vendorNameField.text &&
                     productTypeField.text &&
                     capacityField.text &&
                     coresField.text

            onClicked: {
                addNewRequested(nameField.text,
                                vendorCodeField.text,
                                countField.text,
                                vendorNameField.text,
                                productTypeField.text,
                                capacityField.text,
                                coresField.text);
            }
        }

        Button {
            text: "Add existing"
            enabled: vendorCodeField.text &&
                     countField.text

            onClicked: updateExistingRequested(vendorCodeField.text,
                                            countField.text);
        }
    }
}
