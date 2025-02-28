import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

Item{
   width:450
   height: 800
   //用于加载切换qml
   Component.onCompleted:{
       if (sql_op) {
              console.log("sql_op is available, calling data_init().");
              sql_op.data_init();
               sql_op.review_check();
          } else {
              console.error("sql_op is not available.");
          }
   }
    Loader{
            id:contentLoder
            anchors.fill: parent
            source: "qrc:/Welcome.qml"
            // 添加调试信息
                   onLoaded: {
                       console.log("Successfully loaded:", source)
                   }
                   onStatusChanged: {
                       if (status == Loader.Error) {
                           console.log("Error loading source:", source)
                       }
                   }
        }
    Connections{
        target:AC
       function onNextPage_sig(){
           console.log("下一个单词哦")
           contentLoder.source=""
           contentLoder.source="qrc:/learning1.qml"
        }

       function onBack_home_sig(){
           contentLoder.source="qrc:/primary.qml"
       }
    }

    Connections{
        target: contentLoder.item
        function onNavigateTo(topage){
            console.log("Received navigateTo signal with page:", topage)
            console.log("接受到信号")
            contentLoder.source=topage
        }
    }
    //欢迎页面展示时间的计时器
    Timer{
            interval: 1000
            running: true
            repeat: false
            onTriggered: {
                contentLoder.source="qrc:/collect.qml"
            }
        }
}
