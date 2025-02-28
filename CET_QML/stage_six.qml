import QtQuick 2.15
import QtQuick.Controls 2.15
Item {
    id:stage_six
  anchors.fill:parent
  property var query:{}
  Column{
      anchors.fill:parent
      Text{
          textFormat:Text.RichText
          width:parent.width
          font.pixelSize:parent.height/20
          wrapMode:Text.WordWrap
          text:sentence_deal(query.words,query.cet4s)
          function sentence_deal(word_list,cet4s){
              let sentence=""
              for(let i=0;i<word_list.length;i++){
                  let flag=true;
                  for(let j=0;j<cet4s.length;j++){
                      if(i===cet4s[j].index){
                          sentence=sentence+`<a href="word://${encodeURIComponent(cet4s[j].id)}" style="color: blue; text-decoration: underline;">`+word_list[i]+`</a>`+" "
                         flag=false
                          break
                      }
                  }
                  if(flag===true){
                       sentence +=word_list[i]+" ";
                  }
              }
              return sentence
          }
          onLinkActivated: function(link) {
                    if (link.startsWith("word://")) {
                        let id = decodeURIComponent(link.split("word://")[1]);
                        AC.word_show(id,2,query)
                    }
                }
      }
      Item{
          width:parent.width
          height:parent.height/2
      }

      Button {
          id: next_btn
          anchors.horizontalCenter:parent.horizontalCenter
          contentItem: Text {
              anchors.fill: parent
              horizontalAlignment: Text.AlignHCenter
              text: "下一个"
              font.pixelSize: stage_six.height /18
          }
          background: Rectangle {
              radius: 15
              color: "#00000000"
          }
          onClicked: {

                 if(sql_op.sentence_next()){
                    AC.nextPage()
                    }else{
                     next_btn.enabled=false
                      if(sql_op.type==1){
                          sql_op.learn_finish()
                          console.log("learn finish")
                         tip_popup.open()
                      }else{
                           next_btn.enabled=false
                          sql_op.review_sentence_finish()
                          console.log("review finish")
                          tip.text="该组复习已经完成"
                          tip_popup.open()
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
      onOpened: {
             console.log("Popup opened")  // 调试用
         }

         onClosed: {
             console.log("Popup closed")  // 调试用
             AC.back_home() // 返回主界面
         }
      Column {
          anchors.fill: parent
          spacing: 10
          Text {
              id:tip
              anchors.horizontalCenter: parent.horizontalCenter
              text: "恭喜您，学习已完成！"
              color: "#333333"
              font.bold: true
          }
          Button {
              anchors.horizontalCenter: parent.horizontalCenter
              text: "返回主界面"
              onClicked: tip_popup.close()
              background:Rectangle{
                  color:"#e9f3e8"
                  border.color:"blue"
                  border.width:2
              }
          }
      }
  }
}
