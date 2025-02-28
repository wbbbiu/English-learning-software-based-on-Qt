import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id:primary_item
    anchors.fill: parent
     property bool isReview:false
    signal navigateTo(string topage)

    //制作一个滑动界面，包含复习和学习两种情况
        SwipeView{
            id:swipe
            anchors.fill: parent
            interactive:true
            orientation:Qt.Horizontal
           Repeater{
               model:[primary_base,primary_base]
               delegate:Loader{
                   sourceComponent:modelData
               }
           }
           onCurrentIndexChanged: {
                  // 根据 currentIndex 直接切换 isReview
                  isReview = (currentIndex === 1);
              }
        }

    Component{

        id:primary_base
        Page{
            anchors.fill:parent
            background: Rectangle{
                color: "white"
            }

            Image {
                id: book_image
                source: "qrc:/image/image/welcome.jpg"
                anchors.bottom: progress.top
                anchors.bottomMargin: 10
                width: parent.width*0.5
                height: parent.height*0.5
                anchors.horizontalCenter: parent.horizontalCenter
            }
            //学习进度条展示
            ProgressBar{
                id:progress
                from: 0
                to:2790
                value: 1000
                width: parent.width*0.8
                height: 10
                anchors.horizontalCenter:parent.horizontalCenter
                anchors.bottom: progress_text.top
                anchors.bottomMargin: 10
                background: Rectangle {
                       implicitHeight: 4
                       color: "lightgray"
                   }
                // 进度条样式
                    contentItem: Item {
                            Rectangle {
                                width: progress.visualPosition * parent.width
                                height: parent.height
                                color: "#35b1ee"  // 进度条颜色
                                radius: 3
                            }
                        }
            }
            //位于进度条下方的进度条百分比展示
            Text{
                anchors.horizontalCenter: parent.horizontalCenter
                id:progress_text
                anchors.bottom: leaning_task.top
                text: "进度: " + progress.value + "/"+progress.to +" ("+Number(progress.value/progress.to*100).toFixed(2) +"%)"
               color: "gray"
               font.pixelSize: 10
            }
            //个人界面按钮
            Button{
                id:person_button
                anchors.right: parent.right
                anchors.top:parent.top
                spacing: 10
                width:parent.width*0.1
                 height: parent.width*0.1
                 flat: true
                Image {
                    source: "qrc:/image/image/person.png"
                     anchors.fill: parent
                }
            }
           //学习任务展示栏
            Rectangle
            {
                id:leaning_task
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.bottom: primary_layout.top
                anchors.bottomMargin: parent.height*0.02
                height: parent.height*0.1
                width:parent.width*0.7
                    RowLayout{
                        anchors.centerIn: parent
                        Text{
                            text: isReview?"今日还需复习":"今日还需要学习"
                            font.pixelSize: 30
                            anchors.verticalCenter: parent.verticalCenter

                        }
                         Text{
                             text: isReview?sql_op.review_count-sql_op.today_review_count:sql_op.flag_count-sql_op.today_learn_count>0?sql_op.flag_count-sql_op.today_learn_count:0
                             font.pixelSize: 50
                             font.bold: true
                             font.italic: true
                             anchors.verticalCenter: parent.verticalCenter
                         }
                    }
           }

            RowLayout{
                id:primary_layout
                width: parent.width
                anchors{
                    bottom: parent.bottom
                    right: parent.right
                    left: parent.left

                }
                height: parent.height*0.1

                Button{
                    id:collect_button
                     anchors.left: parent.left
                     Layout.preferredWidth: parent.width*0.2
                     Layout.preferredHeight: parent.height*0.7
                     anchors.leftMargin: parent.width*0.05
                     flat: true
                     Image {

                         source: "qrc:/image/image/collect.png"
                         anchors.fill: parent
                     }
                }
                //开始学习和开始复习按钮
                Button{
                    id:leaning_button
                    anchors.horizontalCenter: parent.horizontalCenter
                    Layout.preferredWidth: parent.width*0.4
                    Layout.preferredHeight: parent.height
                    padding: 10
                    background: Rectangle{
                        color: "#35b1ee"
                        radius: 15
                    }
                    Text{
                        text: isReview?"开始复习！":"开始学习！"
                        anchors.centerIn: parent
                        color: "white"
                        property real fontScale:leaning_button.width*0.125
                        font.pixelSize: fontScale
                    }
                    onClicked: {
                        console.log("开始学习")
                       if(!isReview){
                           console.log("学习模式")
                           if(sql_op.init_flag===0){
                               sql_op.learn_init();
                               sql_op.init_flag=1;
                           }
                           sql_op.type=1
                            primary_item.navigateTo("qrc:/learning1.qml")
                       }else{
                           if(sql_op.review_count-sql_op.today_review_count===0){
                                 tip_popup.open()
                           }
                           else{
                               sql_op.type=2
                               primary_item.navigateTo("qrc:/learning1.qml")
                           }
                       }
                    }
                }
                Button{
                    id:history_button
                    anchors.right: parent.right
                    Layout.preferredWidth: parent.width*0.2
                    Layout.preferredHeight: parent.height*0.7
                    anchors.rightMargin: parent.width*0.05
                    flat: true
                    Image {
                        anchors.fill: parent
                        source: "qrc:/image/image/history.png"
                    }
                }
            }
        }
    }
    Popup {
        id: tip_popup
        modal: true
        width: parent.width * 0.3
        height: contentHeight+10
        anchors.centerIn: parent
        background:Rectangle{
            radius:15
        }
        Column {
            anchors.fill: parent
            spacing: 10
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "今日复习已完成！"
                color: "#333333"
                font.bold: true
            }
            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "朕知道了，退下吧"
                onClicked: tip_popup.close()
            }
        }
    }

}
