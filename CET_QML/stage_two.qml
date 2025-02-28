import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

Item {
    id:stage_two
    anchors.fill: parent
    signal stage_move(string stage)
    property var word_info:({})
    property var info: {}
    Component.onCompleted:{
        info=sql_op.stage_two_query(word_info["id"])
        console.log("info is ",JSON.stringify(info))
        sql_op.fetchPronunciation(info.word,2)
    }

        Column {
            anchors.fill: parent
            spacing:10
            Text {
                id: word_text
                text: info.word
               width: parent.width
                height: parent.height/10
                anchors.left:parent.left
                anchors.leftMargin:20
                font.pixelSize: parent.height/10
            }
            Button {
                id: pronounce_button

                flat: true
                // 使用 implicitHeight 让 Button 使用其默认内容高度
                anchors.left:parent.left
                anchors.leftMargin:15
                contentItem:Text{
                    anchors.fill:parent
                    horizontalAlignment: Text.AlignLeft
                    text: "/"+info.ukphone+"/"
                    font.pixelSize:stage_two.height/25
                    wrapMode:Text.WordWrap
                }
                onClicked:{
                   sql_op.fetchPronunciation(info.word,2)
                }
                }

            ListView{
                spacing:5
                id:select_list
                width:parent.width
                height:contentHeight
                model:info&&info.anwers!==undefined?info.anwers:[]
                property bool buttonenable:true
                property int selectindex: -1
                delegate:Button{
                    id:select_btn
                    width:parent.width
                    height:stage_two.height/7
                    enabled:select_list.buttonenable
                    contentItem: Text{
                            text:modelData.anwer
                            font.pixelSize:stage_two.height/25
                            horizontalAlignment:Text.AlignHCenter
                            verticalAlignment:Text.AlignVCenter
                            anchors.centerIn: parent
                            wrapMode:Text.WordWrap
                            color: "#333333" //
                        }
                    background:Rectangle{
                        id:select
                        radius:5
                        border.color:"#333333"
                        color:{
                            if(select_list.selectindex===-1){
                                return "#e9f3e8"
                            }else{
                                if(model.index===info.anwer){
                                     return "green" // 浅绿色
                                }else if(model.index===select_list.selectindex){
                                     return "red" // 浅红色
                                }else{
                                    return "#e9f3e8"
                                }
                            }
                        }

                    }
                    onClicked:{
                        select_list.selectindex=model.index
                        select_list.buttonenable=false
                        if(model.index===info.anwer){
                            sql_op.upgrade(word_info["id"])
                        }
                        //让所有按钮不在能按
                    }
                }
            }
             Button{
                 anchors.horizontalCenter:parent.horizontalCenter
                 contentItem:Text{
                     anchors.fill:parent
                     horizontalAlignment:Text.AlignHCenter
                     verticalAlignment:Text.AlignVCenter
                     text:"继续"
                     font.pixelSize:stage_two.height/13
                 }
                 onClicked:{
                        AC.word_show(query["id"],1,{})

                 }
             }
        }

}
