import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15
Item {
    id:stage_three
    anchors.fill: parent
        signal stage_move(string stage)
    property var word_info:({})
    property var info: {}
    property bool show: false
    Component.onCompleted:{
        info=sql_op.stage_three_query(word_info["id"])

    }
        Column {
            anchors.fill: parent
            spacing:15
            Item{
                width:parent.widthChanged
                height:20
            }

            Text {
                id: word_text
                text: info.question
                 width:parent.width
                anchors.left:parent.left
                font.pixelSize: parent.height/20
                wrapMode:Text.WordWrap
                anchors.leftMargin:20
            }
            Text{
                text:"解释：" +info.explain
                 width:parent.width
                anchors.left:parent.left
                font.pixelSize: parent.height/30
                wrapMode:Text.WordWrap
                visible:show
                anchors.leftMargin:20
            }
            ListView{
                id:select_list
                width:parent.width
                height:contentHeight
                model:info&&info.anwers!==undefined?info.anwers:[]
                property bool buttonenable:true
                property int selectindex: -1
                delegate:Button{
                    id:select_btn
                    width:parent.width
                    height:stage_three.height/7
                    enabled:select_list.buttonenable
                    contentItem:Text{
                        anchors.fill:parent
                            text:modelData.anwer
                            font.pixelSize:stage_three.height/23
                            horizontalAlignment:Text.AlignHCenter
                            verticalAlignment:Text.AlignVCenter
                            anchors.centerIn: parent
                            color: "#333333" //
                        }
                    background:Rectangle{
                        id:select
                        radius:5
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
                        show=true
                        //让所有按钮不在能按
                    }
                }
            }
             Button{
                 anchors.horizontalCenter:parent.horizontalCenter
                 contentItem:Text{
                     anchors.fill:parent
                     horizontalAlignment:Text.AlignHCenter
                     text:"下一个"
                     font.pixelSize:stage_three.height/13
                 }
                 onClicked:{

                          AC.word_show(query["id"],1,{})
                 }
             }



        }

}
