import QtQuick 2.16
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15
Rectangle{
    id:this_window
    anchors.fill: parent
    visible: true
    property var query: ({})

    Image {
        id: background
        source: "qrc:/image/image/background.jpg"
        anchors.fill:parent
    }
    Connections{
        target:model_learn.item
       function onNavigateTo(topage){
            model_learn.setSource(topage,{"word_info":query})
        }

    }
    Connections{
        target:AC
        function onWord_show_sig(id,type,query){
            query["id"]=id
            model_learn.setSource("qrc:/word_show_interface.qml",{"word_info":query,"type":type,"query":query})
        }
        function onBack_sig(query){
             model_learn.setSource("qrc:/stage_six.qml",{"query":query})
        }
    }

    Component.onCompleted:{
        if(sql_op.sentence_flag===1){
            query=sql_op.sentence_state(sql_op.type);
            model_learn.setSource("qrc:/stage_six.qml",{"query":query})
        }
        else{
            query=sql_op.stage_state(sql_op.type)
            if(query["stage"]===0||query["stage"]===6){
                model_learn.setSource("qrc:/stage_one.qml",{"word_info":query})
            }
            else if(query["stage"]===1||query["stage"]===7||query["stage"]===11){
                model_learn.setSource("qrc:/stage_two.qml",{"word_info":query})
            }else if(query["stage"]===2||query["stage"]===12){
               model_learn.setSource("qrc:/stage_three.qml",{"word_info":query})
            }
            else if(query["stage"]===3||query["stage"]===13||query["stage"]===8){
                      model_learn.setSource("qrc:/stage_four.qml",{"word_info":query})
                    }
            else if(query["stage"]===4||query["stage"]===9||query["stage"]===14){
                        model_learn.setSource("qrc:/stage_five.qml",{"word_info":query})
                    }
            else{
                console.log("该阶段不存在")
            }
        }
    }
    ColumnLayout{
        anchors.fill: parent
        //背景栏，包含删除，收藏，返回按钮，删除提示窗口
      RowLayout{
          id:meun_layout
          Layout.preferredHeight: parent.height*0.05
          Layout.preferredWidth: parent.width
          Layout.alignment:Qt.AlignTop
          spacing:20
          Button{
              id:back_button
              Layout.preferredWidth: meun_layout.height
              Layout.preferredHeight: meun_layout.height
              flat: true
            anchors.left:parent.left
              Image{
                  id:back_icon
                  source: "qrc:/image/image/back.png"
                  anchors.fill: parent
                  fillMode: Image.PreserveAspectFit
                  anchors.centerIn: parent
              }

              onClicked: {
                  AC.back_home();
              }
          }
          Button{
              id:star_button
              Layout.preferredWidth: meun_layout.height
              Layout.preferredHeight: meun_layout.height
              anchors.right: delete_button.left
              flat: true
              anchors.rightMargin: 20
                   Image {
                       anchors.centerIn: parent
                       anchors.fill: parent
                       id: star_icon
                       source: "qrc:/image/image/disstar.png"
                       fillMode: Image.PreserveAspectFit
                   }
                   property int flag: 1
              onClicked: {
                  if(flag==1){
                      if(sql_op.collect(query["id"])){
                          star_icon.source="qrc:/image/image/star.png"
                          flag=0
                              }else{
                          console.log("收藏失败")
                      }
                  }
                else{
                      if(sql_op.cancel_collect(query["id"])){
                          flag=1
                          star_icon.source="qrc:/image/image/disstar.png"
                      }else{
                          console.log("取消收藏失败");
                      }
                  }

              }
          }
          Button{
              id:delete_button
              Layout.preferredWidth: meun_layout.height
              Layout.preferredHeight: meun_layout.height
              flat: true
              anchors.right: parent.right
              Image{
                  source: "qrc:/image/image/delete.png"
                  anchors.fill: parent
                  anchors.centerIn: parent
                  fillMode: Image.PreserveAspectFit
              }

              onClicked: {
                      delete_tips_dialog.open()
              }
          }
          //删除提示窗口
          Dialog{
              id:delete_tips_dialog
              modal: true
              width:this_window*0.6
              height: this_window*0.3
              x:(this_window.width-delete_tips_dialog.width)/2
              y:(this_window.height-delete_tips_dialog.height)/3
              ColumnLayout{
                  spacing: 20
                  Text {
                      id: tips_text
                      text: "确定要删除这个单词吗？"+"\n删除以后你将再也不会背诵这个单词"

                  }
                  RowLayout{
                      spacing: 20
                      Button{
                          text:"我确认删除该单词"
                          background: Rectangle{
                              color: "red"
                          }
                        onClicked: {
                            delete_tips_dialog.close()
                        }
                      }
                      Button{
                          text:"我点错了"
                          background: Rectangle{
                              color: "gray"
                          }
                          onClicked: {
                              delete_tips_dialog.close()
                          }
                      }
                  }
              }
          }

      }
      Loader{
          id:model_learn
          Layout.fillHeight: true
          Layout.fillWidth: true
      }
    }

}
