import QtQuick 2.15
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.15

Item{
    id:stage_one
    anchors.fill: parent
    signal navigateTo(string topage)

    property var query: ({})
    property var word_info:({})
    Component.onCompleted: {
      query=sql_op.stage_one_query(word_info["id"]);
        console.log(query)
       updateUI();
        sql_op.fetchPronunciation(query["word"],2)
    }
   function updateUI(){
       word_text.text=query["word"]
       m_pronounce_button.text="/"+query["ukphone"]+"/"
       e_pronounce_button.text="/"+query["usphone"]+"/"
   }
    Column{
        anchors.fill: parent
        id:content_layout
        spacing:20
        //单词展示
        Text{
            id:word_text
            text: "单词"
            font.pixelSize:  stage_one.height/14
        }
        //发音，分为美音和英式
        Row{
            id:pronunciation
            spacing:30
            width: stage_one.width/9
            height: stage_one.height/9
            Button{
                 id: m_pronounce_button
                  height: stage_one.height/25
                 text: "发音"
                 flat: true
                 font.pixelSize: m_pronounce_button.height
                 onClicked:{
                     sql_op.fetchPronunciation(query["word"],2)
                 }
             }
            Button{
                 id: e_pronounce_button
                  height: stage_one.height/25
                 text: "发音"
                 flat: true
                 font.pixelSize: m_pronounce_button.height
                 onClicked:{
                     sql_op.fetchPronunciation(query["word"],1)
                 }
             }
        }
       //学习情况反馈
        Item {
               height: parent.height - (word_text.height + pronunciation.height + learn_state.height +60) // 计算剩余高度
               width:parent.width
           }
        RowLayout{
           width: stage_one.width
           height: stage_one.height/8
           id:learn_state
           spacing:20
           Item{
               height:parent.height
               width:20
           }

           Button{
               id:grasp_button
               text: "已掌握"
             Layout.preferredHeight: parent.height*0.8
              Layout.preferredWidth: sql_op.type===1?parent.width*0.4:parent.width*0.25
               onClicked: {
                    if(sql_op.type===1){
                        if(sql_op.learn_end(query["id"])){
                           AC.word_show(query["id"],1,{})
                        }
                    }else{
                        if(sql_op.review_end(query["id"])){
                           AC.word_show(query["id"],1,{})
                        }
                    }
               }
           }
           Button{
               id:blur_button
              Layout.preferredHeight: parent.height*0.8
               Layout.preferredWidth: parent.width*0.25
               text:"模糊"
               visible:sql_op.type===1?false : true
               onClicked: {
                   if(sql_op.review_1(query["id"])){
                        AC.word_show(query["id"],1,{})
                   }
               }
           }

           Button{
               id:disgrasp_button
              Layout.preferredHeight: parent.height*0.8
               Layout.preferredWidth: sql_op.type===1?parent.width*0.4:parent.width*0.25
               text:sql_op.type===1?"学习":"忘记"
               anchors.right: parent.right
               anchors.rightMargin: 20
               onClicked: {
                   if(sql_op.type===2){
                       if(sql_op.review_2(query["id"])){
                            AC.word_show(query["id"],1,{})
                       }
                   }else{
                       if(sql_op.upgrade(query["id"])){
                            AC.word_show(query["id"],1,{})
                       }
                   }
               }
           }
       }
    }

}
