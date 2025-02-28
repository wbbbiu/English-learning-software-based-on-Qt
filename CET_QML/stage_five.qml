import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

Item {
    id: stage_five
    signal stage_move(string stage)
    property var word_info: ({})
    property var info: ({})
    property bool iserror: false
    anchors.fill: parent

    Component.onCompleted: {
        info = sql_op.stage_five_query(word_info["id"])
        sql_op.fetchPronunciation(info.word, 2)
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 20

        TextField {
            id: spelling
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: parent.height / 10
            font.pixelSize: parent.height / 20
            placeholderText: "请输入单词拼写"
            background: Rectangle {
                color: "#00000000"
                radius: 5
            }

            onEditingFinished: {
                console.log(info.word)
                if(spelling.enabled){
                    spelling.enabled=false
                    if (spelling.text === info.word) {
                        resultText.text = "拼写正确！"
                        resultText.color = "green"
                        sql_op.upgrade(word_info["id"])
                        console.log("拼写正确！")
                    } else {
                        resultText.text = "拼写错误! 答案是：" + info.word
                        resultText.color = "red"
                    }
                    resultText.visible = true
                }


            }

            Rectangle {
                id: underline
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: spelling.bottom
                width: stage_five.width / 3 > spelling.width ? stage_five.width / 3 : spelling.width
                height: 2
            }
        }

        Text {
            id: resultText
            Layout.alignment: Qt.AlignHCenter
            visible: false
            font.pixelSize: parent.height / 25
            color: "black"
        }

        Button {
            id: pronounce_button
            Layout.alignment: Qt.AlignHCenter
           contentItem:Text{
               anchors.fill:parent
                text: "/" + info.usphone + "/"
                font.pixelSize: 20
           }

            flat: true
             onClicked:{
                 sql_op.fetchPronunciation(info.word, 2)
             }
        }

        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true // 让这个 Item 填充剩余空间
        }

        Button {
            id: next_btn
            Layout.alignment: Qt.AlignHCenter
            contentItem: Text {
                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                text: "下一个"
                font.pixelSize: stage_five.height / 20
            }
            background: Rectangle {
                radius: 15
                color: "#00000000"
            }

            onClicked: {
                if(sql_op.type===2){
                    sql_op.review_word_finish()
                }
                AC.nextPage()
            }
        }
    }
}
