import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3

import Accounting.Clients 1.0

Window {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Accounting")

    MessageDialog {
        id: messageDialog
        title: "Low count warning"
        icon: StandardIcon.Warning
        standardButtons: StandardButton.Ok
    }

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar
            currentIndex: 1

            TabButton { implicitWidth: 100; text: "Goods"; enabled: Controller.connected }
            TabButton { implicitWidth: 100; text: "Login/logout" }
            TabButton { implicitWidth: 100; text: "Orders"; enabled: Controller.loggedIn }
            TabButton { implicitWidth: 100; text: "Goods add"; enabled: Controller.loggedIn }
        }

        StackLayout {
            id: layout
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20
            currentIndex: tabBar.currentIndex

            GoodsView {
                enabled: Controller.connected
                authorized: Controller.loggedIn
                model: Controller.goodsModel

                onOrderRequested: Controller.orderGood(index, count);
                onRemoveRequested: Controller.removeGood2(index);
            }

            LoginForm {
                isLoggedIn: Controller.loggedIn

                onLoginRequested: Controller.loginAsEmployee(login, password)
                onLogoutRequested: Controller.logOff();
            }

            OrdersView {
                enabled: Controller.loggedIn
                model: Controller.ordersModel
            }

            GoodsAddView {
                enabled: Controller.loggedIn

                onAddNewRequested: Controller.addGood2(name,
                                                       vendorCode,
                                                       count,
                                                       vendorName,
                                                       productType,
                                                       capacity,
                                                       cores);

                onUpdateExistingRequested: Controller.updateExistingGood(vendorCode, count);
            }
        }
    }


    Connections {
        target: Controller

        onLoggedInChanged: {
            if (Controller.loggedIn)
                tabBar.currentIndex = 0;
            else
                tabBar.currentIndex = 1;
        }
    }

    Connections {
        target: Controller.goodsModel

        onLowCountWarning: {
            messageDialog.text = "Warning! Too low goods count. Name: " + name + ", vendor code: " + vendorCode + ", count: " + count;
            messageDialog.open();
        }
    }
}
