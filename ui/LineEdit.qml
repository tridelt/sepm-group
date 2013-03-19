import QtQuick 1.0

 FocusScope {
     id: focusScope
     width: 250; height: 28
     property alias text: textInput.text
     property alias hint: typeSomething.text

     BorderImage {
         source: "images/lineedit-bg.png"
         width: parent.width; height: parent.height
         border { left: 4; top: 4; right: 4; bottom: 4 }
     }

     BorderImage {
         source: "images/lineedit-bg-focus.png"
         width: parent.width; height: parent.height
         border { left: 4; top: 4; right: 4; bottom: 4 }
         visible: parent.activeFocus ? true : false
     }

     Text {
         id: typeSomething
         anchors.fill: parent; anchors.leftMargin: 8
         verticalAlignment: Text.AlignVCenter
         text: "Type something..."
         color: "gray"
         font.italic: true
     }

     TextInput {
         id: textInput
         anchors { left: parent.left; leftMargin: 8; right: parent.right; rightMargin: 8; verticalCenter: parent.verticalCenter }
         focus: true
         selectByMouse: true
     }

     states: State {
         name: "hasText"; when: textInput.text != ''
         PropertyChanges { target: typeSomething; opacity: 0 }
     }

     transitions: [
         Transition {
             from: ""; to: "hasText"
             NumberAnimation { exclude: typeSomething; properties: "opacity" }
         },
         Transition {
             from: "hasText"; to: ""
             NumberAnimation { properties: "opacity" }
         }
     ]
 }
