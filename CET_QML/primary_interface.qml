import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

Window {
    visible: true
    width: 600
    height: 800
    title: "My QML Interface"
    StackView{
        id:stackview
        anchors.fill: parent
        initialItem: welcome
    }

    Component{
        id:welcome
        Page{
            Image {
                id: welcome_image
                anchors.fill: parent
                source: "qrc:/image/image/welcome.jpg"

            }
            Timer{
                    interval: 3000
                    running: true
                    repeat: false
                    onTriggered: {
                        stackview.push(swipe_container)
                    }
                }
        }
    }
    Component{
        id:swipe_container
        SwipeView{

            anchors.fill: parent
            currentIndex: 0


            Repeater {
                     model: [primary_leaning,primary_Review]
                     delegate: Loader {
                         id: pageLoader
                         sourceComponent: modelData // modelData 是当前模型项的引用
                     }
                 }

        }
    }



     Component{
         id:primary_Review
         Page{
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
             Text{
                 anchors.horizontalCenter: parent.horizontalCenter
                 id:progress_text
                 anchors.bottom: leaning_task.top
                 text: "进度: " + progress.value + "/"+progress.to +" ("+Number(progress.value/progress.to*100).toFixed(2) +"%)"
                color: "gray"
                font.pixelSize: 10
             }

             Button{
                 anchors.right: parent.right
                 anchors.top:parent.top
                 spacing: 10
                 width:parent.width*0.1
                  height: parent.width*0.1
                 Image {
                     source: "qrc:/image/image/person.png"
                      anchors.fill: parent
                 }
             }
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
                             text:"今日还需复习:"
                             font.pixelSize: 30
                             anchors.verticalCenter: parent.verticalCenter

                         }
                          Text{
                              text: "50词"
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
                      Image {

                          source: "qrc:/image/image/collect.png"
                          anchors.fill: parent
                      }
                 }
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
                         text: "开始复习！"
                         anchors.centerIn: parent
                         color: "white"
                         property real fontScale:leaning_button.width*0.125
                         font.pixelSize: fontScale
                     }

                 }
                 Button{
                     id:history_button
                     anchors.right: parent.right
                     Layout.preferredWidth: parent.width*0.2
                     Layout.preferredHeight: parent.height*0.7
                     anchors.rightMargin: parent.width*0.05
                     Image {
                         anchors.fill: parent
                         source: "qrc:/image/image/history.png"
                     }
                 }
             }
         }
         }


     Component{
         id:primary_leaning

         Page{
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
             Text{
                 anchors.horizontalCenter: parent.horizontalCenter
                 id:progress_text
                 anchors.bottom: leaning_task.top
                 text: "进度: " + progress.value + "/"+progress.to +" ("+Number(progress.value/progress.to*100).toFixed(2) +"%)"
                color: "gray"
                font.pixelSize: 10
             }

             Button{
                 anchors.right: parent.right
                 anchors.top:parent.top
                 spacing: 10
                 width:parent.width*0.1
                  height: parent.width*0.1
                 Image {
                     source: "qrc:/image/image/person.png"
                      anchors.fill: parent
                 }
             }
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
                             text:"今日还需学习:"
                             font.pixelSize: 30
                             anchors.verticalCenter: parent.verticalCenter

                         }
                          Text{
                              text: "50词"
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
                      Image {

                          source: "qrc:/image/image/collect.png"
                          anchors.fill: parent
                      }
                 }
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
                         text: "我要学习！"
                         anchors.centerIn: parent
                         color: "white"
                         property real fontScale:leaning_button.width*0.125
                         font.pixelSize: fontScale
                     }

                 }
                 Button{
                     id:history_button
                     anchors.right: parent.right
                     Layout.preferredWidth: parent.width*0.2
                     Layout.preferredHeight: parent.height*0.7
                     anchors.rightMargin: parent.width*0.05
                     Image {
                         anchors.fill: parent
                         source: "qrc:/image/image/history.png"
                     }
                 }
             }
         }
         }

}

