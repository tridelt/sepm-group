import Qt 4.7

Rectangle {
    id: rect

    function sendMessage() {
        if(!ServerSelector.setServer(server.text, port.text, cert.text)) {
            reply.text = "Failed to set server"
        } else {
            reply.text = "Reply: " + ServerSelector.echo(msg.text)
        }
    }

    function browse() {
        var fileName = ServerSelector.browse()
        if("" != fileName) {
            cert.text = fileName
        }
    }

    anchors.fill: parent

    Image {
        anchors.fill: parent
        fillMode: Image.Tile
        source: "images/background.png"
    }

    Rectangle {
        id: server_config
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 35
        width: 500

        Rectangle {
            id: desc
            Text {
                text: "Connect to:                 "
                font.pixelSize: 16; font.bold: true
                anchors.centerIn: parent
            }
            height: 28
        }

        LineEdit {
            id: server
            width: 300
            text: "selinux.inso.tuwien.ac.at"
            hint: "server"
            focus: true
            anchors.left: desc.right
            anchors.leftMargin: 7
        }

        Rectangle {
            id: colon
            anchors.left: server.right
            anchors.leftMargin: 7
            Text {
                text: ":"
                font.pixelSize: 16; font.bold: true
                anchors.centerIn: parent
            }
            height: 28
        }

        LineEdit {
            id: port
            anchors.left: colon.right
            anchors.leftMargin: 5
            width: 80
            text: "1337"
            hint: "port"
            focus: true
        }

        LineEdit {
            id: cert
            anchors.left: server.left
            anchors.leftMargin: -110
            anchors.top: server.bottom
            anchors.topMargin: 10
            width: 475
            text: "ca.crt"
            hint: "certificate"
            focus: true
        }


        Rectangle {
            id: btn_browse

            property bool pressed: false

            width: 20; height: 28
            anchors.left: cert.right
            anchors.leftMargin: 5
            anchors.top: server.bottom
            anchors.topMargin: 10
            radius: 6
            color: pressed ? "gray" : "white"

            Text {
                anchors.centerIn: parent
                text: "..."
            }

            MouseArea {
                anchors.fill: parent
                onClicked: { browse() }
            }
        }
    }


    Rectangle {
        id: reply_rect
        anchors.top: server_config.bottom
        anchors.left: server_config.left
        anchors.topMargin: 100

        Rectangle {
            id: msg_desc
            Text {
                text: "Message:"
                font.pixelSize: 16; font.bold: true
                anchors.centerIn: parent
            }
            height: 28
        }

        LineEdit {
            id: msg
            width: 300
            text: "Hello world"
            focus: true
            anchors.left: msg_desc.right
            anchors.leftMargin: 50
        }
    }


    Rectangle {
        anchors.top: reply_rect.bottom
        anchors.horizontalCenter: server_config.horizontalCenter
        anchors.topMargin: 35

        id: reply
        property alias text: tmp.text
        Text {
            id: tmp
            text: "n/a"
            font.pixelSize: 16; font.bold: true
            anchors.centerIn: parent
        }
        height: 28
    }


    Rectangle {
        id: button

        property bool pressed: false

        width: 100; height: 40
        anchors.right: parent.right; anchors.rightMargin: 20
        anchors.bottom: parent.bottom; anchors.bottomMargin: 20
        radius: 6
        color: pressed ? "gray" : "white"

        Text {
            anchors.centerIn: parent
            text: "Send"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: { sendMessage() }
        }
    }
}
