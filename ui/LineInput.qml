import Qt 4.7

FocusScope {
    property alias text: input.text
    property alias maximumLength: input.maximumLength
    height: 28
    BorderImage {
        source: "images/lineedit.sci"
        anchors.fill: parent
    }
    TextInput {
        id: input
        color: "#151515"
        font.pixelSize: 16; font.bold: true
        width: parent.width-16
        anchors.centerIn: parent
        focus: true
    }
}
