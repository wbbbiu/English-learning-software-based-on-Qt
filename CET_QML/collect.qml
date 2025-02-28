import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
Item {
    anchors.fill:parent
    // 模拟数据 - 收藏的单词
    property var favoriteWords: [
        { word: "audience", addDate: "2025-02-20" },
        { word: "artistic", addDate: "2025-02-21" },
        { word: "carriage", addDate: "2025-02-22" },
        { word: "consultant", addDate: "2025-02-23" },
        { word: "esoteric", addDate: "2025-02-24" },
        { word: "harmony",addDate: "2025-02-25"}
    ]

    // 模拟数据 - 收藏的句子
    property var favoriteSentences: [
        { sentence: "The ephemeral nature of cherry blossoms makes them all the more beautiful.", translation: "樱花的短暂性使它们更加美丽。", addDate: "2025-02-20" },
        { sentence: "Technology has become ubiquitous in modern society.", translation: "技术在现代社会已经无处不在。", addDate: "2025-02-21" },
        { sentence: "Finding the solution was a moment of pure serendipity.", translation: "找到解决方案纯属意外之喜。", addDate: "2025-02-22" },
        { sentence: "Time and tide wait for no man.", translation: "时间和潮水不等人。", addDate: "2025-02-25" }
    ]

    // 模拟数据 - 收藏的问题
    property var favoriteQuestions: [
        { question: "Our flight was ________.?", answer: "我们的航班取消了。", addDate: "2025-02-23", difficulty: "中等" },
        { question: "Because the gas is highly ________, it needs to be kept in high-pressure containers.", answer: "由于这种气体极易爆炸，因此需要保存在高压容器内。", addDate: "2025-02-24", difficulty: "中等" },
        { question: "the leaders who ________ the country", answer: "治理这个国家的领导人。", addDate: "2025-02-25", difficulty: "中等" }

    ]

    property string currentTab: "words" // 可选值: "words", "sentences", "questions"

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        // 标题栏
        Rectangle {
            Layout.fillWidth: true
            height: 60
            color: "#f0f0f0"
            radius: 8
            RowLayout {
                anchors.fill: parent
                anchors.margins: 10

                Label {
                    text: "我的收藏夹"
                    font.pixelSize: 20
                    font.bold: true
                    anchors.horizontalCenter:parent.horizontalCenter
                }

                Item { Layout.fillWidth: true }

            }
        }

        // 分类标签页
        TabBar {
            id: tabBar
            Layout.fillWidth: true
            background: Rectangle {
                color: "#e0e0e0"
                radius: 4
            }

            TabButton {
                text: "单词"
                width: Math.max(100, tabBar.width / 3)
                background: Rectangle {
                    color: currentTab === "words" ? "#ffffff" : "#e0e0e0"
                    radius: 4
                }
                onClicked: currentTab = "words"
            }

            TabButton {
                text: "句子"
                width: Math.max(100, tabBar.width / 3)
                background: Rectangle {
                    color: currentTab === "sentences" ? "#ffffff" : "#e0e0e0"
                    radius: 4
                }
                onClicked: currentTab = "sentences"
            }

            TabButton {
                text: "问题"
                width: Math.max(100, tabBar.width / 3)
                background: Rectangle {
                    color: currentTab === "questions" ? "#ffffff" : "#e0e0e0"
                    radius: 4
                }
                onClicked: currentTab = "questions"
            }
        }

        // 内容区域
        Rectangle {
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: "#ffffff"
            radius: 8
            border.color: "#e0e0e0"
            border.width: 1
            clip: true

            // 单词收藏视图
            ListView {
                id: wordsListView
                anchors.fill: parent
                anchors.margins: 10
                visible: currentTab === "words"
                model: favoriteWords
                spacing: 10
                clip: true

                delegate: Rectangle {
                    width: wordsListView.width
                    height: 80
                    radius: 6
                    color: "#f9f9f9"
                    border.color: "#e0e0e0"
                    border.width: 1

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 10

                        Rectangle {
                            width: 60
                            height: 60
                            radius: 30
                            color: "#4285f4"

                            Text {
                                anchors.centerIn: parent
                                text: modelData.word.charAt(0).toUpperCase()
                                color: "white"
                                font.pixelSize: 24
                                font.bold: true
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 5

                            Text {
                                text: modelData.word
                                font.pixelSize: 18
                                font.bold: true
                            }


                           /* Flow {
                                Layout.fillWidth: true
                                spacing: 5

                                Repeater {
                                    model: modelData.tags

                                    Rectangle {
                                        width: tagLabel.width + 16
                                        height: tagLabel.height + 8
                                        radius: height / 2
                                        color: "#e0e0ff"

                                        Text {
                                            id: tagLabel
                                            anchors.centerIn: parent
                                            text: modelData
                                            font.pixelSize: 12
                                            color: "#4040a0"
                                        }
                                    }
                                }
                            }*/
                        }

                        Text {
                            text: modelData.addDate
                            font.pixelSize: 12
                            color: "#999999"
                        }

                        Button {
                            text: "练习"
                            Layout.preferredWidth: 40
                        }

                        Button {
                            text: "删除"
                            Layout.preferredWidth:40
                        }
                    }
                }
            }

            // 句子收藏视图
            ListView {
                id: sentencesListView
                anchors.fill: parent
                anchors.margins: 10
                visible: currentTab === "sentences"
                model: favoriteSentences
                spacing: 10
                clip: true

                delegate: Rectangle {
                    width: sentencesListView.width
                    height: 120
                    radius: 6
                    color: "#f9f9f9"
                    border.color: "#e0e0e0"
                    border.width: 1

                    ColumnLayout {
                        id: contentColumn
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Text {
                            Layout.fillWidth: true
                            text: modelData.sentence
                            font.pixelSize: 16
                            font.italic: true
                            wrapMode: Text.WordWrap
                        }

                        Text {
                            Layout.fillWidth: true
                            text: modelData.translation
                            font.pixelSize: 14
                            color: "#666666"
                            wrapMode: Text.WordWrap
                        }

                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 10


                            Item { Layout.fillWidth: true }

                            Text {
                                text: "添加日期: " + modelData.addDate
                                font.pixelSize: 12
                                color: "#999999"
                            }

                            Button {
                                text: "练习"
                                Layout.preferredWidth:40
                            }

                            Button {
                                text: "删除"
                                Layout.preferredWidth:40
                            }
                        }
                    }
                }
            }

            // 问题收藏视图
            ListView {
                id: questionsListView
                anchors.fill: parent
                anchors.margins: 10
                visible: currentTab === "questions"
                model: favoriteQuestions
                spacing: 10
                clip: true

                delegate: Rectangle {
                    width: questionsListView.width
                    height: 100
                    radius: 6
                    color: "#f9f9f9"
                    border.color: "#e0e0e0"
                    border.width: 1

                    ColumnLayout {
                        id: questionColumn
                        anchors.fill: parent
                        anchors.margins: 10
                        spacing: 8

                        Row {
                            Layout.fillWidth: true
                            spacing: 10

                            Rectangle {
                                width: 24
                                height: 24
                                radius: 12
                                color: "#34a853"

                                Text {
                                    anchors.centerIn: parent
                                    text: "Q"
                                    color: "white"
                                    font.bold: true
                                }
                            }

                            Text {
                                width: parent.width - 34
                                text: modelData.question
                                font.pixelSize: 16
                                font.bold: true
                                wrapMode: Text.WordWrap
                            }
                        }




                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 10


                            Item { Layout.fillWidth: true }

                            Text {
                                text: "添加日期: " + modelData.addDate
                                font.pixelSize: 12
                                color: "#999999"
                            }

                            Button {
                                text: "练习"
                                Layout.preferredWidth:40
                            }

                            Button {
                                text: "删除"
                                Layout.preferredWidth: 40
                            }
                        }
                    }
                }
            }
        }

        // 底部统计信息
        Rectangle {
            Layout.fillWidth: true
            height: 50
            color: "#f0f0f0"
            radius: 8

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 20

                Text {
                    text: "单词收藏: " + favoriteWords.length
                    font.pixelSize: 14
                }

                Text {
                    text: "句子收藏: " + favoriteSentences.length
                    font.pixelSize: 14
                }

                Text {
                    text: "问题收藏: " + favoriteQuestions.length
                    font.pixelSize: 14
                }

                Item { Layout.fillWidth: true }


            }
        }
    }
}
