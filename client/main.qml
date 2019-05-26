import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12

import Accounting.Clients 1.0

Window {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Accounting")

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar
            currentIndex: 1

            TabButton { implicitWidth: 100; text: "Goods"; enabled: Controller.connected }
            TabButton { implicitWidth: 100; text: "Login/logout" }
            TabButton { implicitWidth: 100; text: "Orders"; enabled: Controller.loggedIn }
        }

        StackLayout {
            id: layout
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 20
            currentIndex: tabBar.currentIndex

            Goodsview {
                enabled: Controller.connected
                model: Controller.goodsModel

                onOrderRequested: Controller.orderGood(index, count);
            }

            Loginform {
                isLoggedIn: Controller.loggedIn

                onLoginRequested: Controller.loginAsEmployee(login, password)
                onLogoutRequested: Controller.logOff();
            }

            Ordersview {
                enabled: Controller.loggedIn

                Label { anchors.centerIn: parent; text: "Orders" }
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
}
