import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

ColumnLayout {
    signal loginRequested(string login, string password)
    signal logoutRequested
    signal cancelRequested
    property bool isLoggedIn: false

    RowLayout {
        Layout.fillWidth: true

        Label { text: "Login" }

        TextField {
            id: loginField
            Layout.fillWidth: true
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

        Button {
            text: "Cancel"
            onClicked: cancelRequested()
        }
    }
}
