import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15
Item {
    anchors.fill: parent
    id:word_show
    property var wordData
    property var word_info:{}
    property bool isLoading:fasle
    property int type
    property var query:{}
    signal navigateTo(string topage)
    Component.onCompleted:{
        isLoading = true
           // 添加错误处理和检查
           if(word_info && word_info["id"]) {
               wordData = sql_op.get_word_info(word_info["id"])
               console.log("Loaded word data:", JSON.stringify(wordData))
               if(wordData && wordData.word && wordData.word.word) {
                   sql_op.fetchPronunciation(wordData.word.word, 2)
               }
           }
           isLoading = false
    }

      // 主要内容
      ScrollView {
          anchors.fill: parent
          clip: true
          visible:true
          Column {
             anchors.fill:parent
              spacing: 5
              // 单词展示区域
              Rectangle {
                  width: parent.width
                 height: 120
                  Layout.alignment:Qt.AlignHCenter
                  color:"#00000000"
                  radius: 8
                  ColumnLayout {
                      id: word_layout
                    anchors.centerIn: parent
                     width: parent.width
                     Layout.preferredHeight:childrenRect.height
                      Text {
                          id: word_text
                          text: wordData && wordData.word ? wordData.word.word : ""
                          font.pixelSize: 36
                          font.bold: true
                          Layout.alignment: Qt.AlignHCenter
                      }

                      RowLayout {
                          spacing: 20
                          id:pronuncition_layout
                          Layout.alignment: Qt.AlignHCenter
                          Button {
                              id: us_pronounce_button
                              text: "美 /" + (wordData && wordData.word ? wordData.word.usphone : "") + "/"
                              flat: true
                              font.pixelSize: 16
                              onClicked: {
                                  if (wordData && wordData.word) {
                                      sql_op.fetchPronunciation(wordData.word.word, 2)
                                  }
                              }
                          }

                          Button {
                              id: uk_pronounce_button
                              text: "英 /" + (wordData && wordData.word ? wordData.word.ukphone : "") + "/"
                              flat: true
                              font.pixelSize: 16
                              onClicked: {
                                  if (wordData && wordData.word) {
                                      sql_op.fetchPronunciation(wordData.word.word, 1)
                                  }
                              }
                          }
                      }
                  }
              }

              // 释义区域
              Rectangle {
                  width:word_show.width
                  height: meanColumn.height + 20
                  color:"#00000000"
                  radius: 8
                  border.color: "#e0e0e0"
                  border.width: 1
                  Layout.margins: 10
                  ColumnLayout {
                      id: meanColumn
                       Layout.preferredHeight:childrenRect.height
                       Layout.preferredWidth:parent.width
                      spacing: 5
                      Text {
                          text: "单词释义"
                          font.bold: true
                          font.pixelSize: 16
                          color: "#333333"
                      }
                      ListView {
                          id: mean_list
                          Layout.preferredWidth:word_show.width
                          Layout.preferredHeight:contentHeight
                          interactive: false
                          model: wordData && wordData.means !== undefined ? wordData.means : []
                          delegate: ItemDelegate {
                              width: parent.width
                              height: 40
                              RowLayout {
                                  anchors.fill: parent
                                  anchors.margins: 5
                                  spacing: 5

                                  Text {
                                      text: modelData.category
                                      font.bold: true
                                      color: "#666666"
                                  }
                                  Text {
                                      text: modelData.mean

                                      wrapMode: Text.WordWrap
                                  }
                              }
                          }


                      }

                  }
              }

              // 短语区域
              Rectangle {
                  width:word_show.width
                  Layout.margins: 10
                  height: phraseColumn.height+ 20
                  radius: 8
                  color:"#00000000"
                  border.color: "#e0e0e0"
                  border.width: 1
                  visible: wordData && wordData.phrases && wordData.phrases.length > 0
                  ColumnLayout {
                      id: phraseColumn
                     Layout.preferredHeight:childrenRect.height
                      Layout.preferredWidth:parent.width
                      spacing: 5
                      Text {
                          text: "常用短语"
                          font.bold: true
                          font.pixelSize: 16
                          color: "#333333"
                      }

                      ListView {
                          id: phrase_list
                          Layout.preferredWidth:word_show.width
                          Layout.preferredHeight:contentHeight
                          interactive: true
                          model: wordData && wordData.phrases !== undefined ? wordData.phrases : []
                          delegate: ItemDelegate {
                              width: parent.width
                              height: 50
                              ColumnLayout {
                                  anchors.fill: parent
                                  anchors.margins: 5
                                  spacing: 5

                                  Text {
                                      text: modelData.phrase
                                      font.pixelSize: 14
                                      color: "#333333"
                                  }

                                  Text {
                                      text: modelData.phrase_trans
                                      font.pixelSize: 13
                                      color: "#666666"
                                  }
                              }
                          }

                      }
                  }
              }

              // 例句区域
              Rectangle {
                  width:word_show.width
                  Layout.margins: 10
                height:sentenceColumn.height+20
                  radius: 8
                 color:"#00000000"
                  border.color: "#e0e0e0"
                  border.width: 1
                  visible: wordData && wordData.sentences && wordData.sentences.length > 0

                  ColumnLayout {
                      id: sentenceColumn
                      Layout.preferredHeight:childrenRect.height
                    Layout.preferredWidth:word_show.width
                      spacing: 5

                      Text {
                          text: "例句"
                          font.bold: true
                          font.pixelSize: 16
                          color: "#333333"
                      }

                      ListView {
                          id: sentence_list
                          Layout.preferredWidth:word_show.width
                          Layout.preferredHeight: contentHeight
                          interactive: false
                          model: wordData && wordData.sentences !== undefined ? wordData.sentences : []
                          delegate: ItemDelegate {
                              width: parent.width
                              height: 70

                              ColumnLayout {
                                  anchors.fill: parent
                                  anchors.margins: 5
                                  spacing: 5

                                  Text {
                                      text: modelData.sentence
                                      font.pixelSize: 14
                                      color: "#333333"
                                      wrapMode: Text.WordWrap
                                  }

                                  Text {
                                      text: modelData.sentence_trans
                                      font.pixelSize: 13
                                      color: "#666666"
                                      wrapMode: Text.WordWrap
                                  }
                              }
                          }

                      }

                  }
              }
              Button{
                  anchors.horizontalCenter:parent.horizontalCenter
                  contentItem:Text{
                      anchors.fill:parent
                      horizontalAlignment:Text.AlignHCenter
                      verticalAlignment:Text.AlignVCenter
                      text:type===1?"继续":"返回"
                      font.pixelSize:word_show.height/13
                  }
                  onClicked:{
                        if(type===1){
                            AC.nextPage()
                        }else{
                            AC.back(query)
                        }
                  }
              }
              // 底部留白
              Item {
                  Layout.fillWidth: true
                  Layout.preferredHeight: 20
              }
          }
      }

}
