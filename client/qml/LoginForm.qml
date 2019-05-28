import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    signal loginRequested(string login, string password)
    signal logoutRequested
    property bool isLoggedIn: false

    RowLayout {
        Layout.fillWidth: true

        Label { text: "Login" }

        TextField {
            id: loginField
            Layout.fillWidth: true
            text: "admin"
        }
    }

    RowLayout {
        Layout.fillWidth: true

        Label { text: "Password" }

        TextField {
            id: passwordField
            Layout.fillWidth: true
            passwordCharacter: "*"
            echoMode: TextInput.Password
            text: "1"
        }
    }

    RowLayout {
        Button {
            text: "Login"
            onClicked: loginRequested(loginField.text, passwordField.text)
            enabled: !isLoggedIn
        }

        Button {
            text: "Logout"
            onClicked: logoutRequested()
            enabled: isLoggedIn
        }
    }
}
