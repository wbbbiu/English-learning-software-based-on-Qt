import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    anchors.fill:parent

    Rectangle {
        anchors.fill: parent
        color: "#f5f5f5"
        radius: 10

        // 使用 ScrollView 使内容可滑动
        ScrollView {
            id: scrollView
            anchors.fill: parent
            clip: true
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            contentWidth: parent.width

            Column {
                id: mainColumn
                width: scrollView.width
                spacing: 20
                padding: 20

                // 顶部标题
                Rectangle {
                    width: parent.width - 40
                    height: 60
                    color: "#4a86e8"
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: "个人设置"
                        font.pixelSize: 22
                        font.bold: true
                        color: "white"
                    }
                }

                // 每日学习目标
                Rectangle {
                    width: parent.width - 40
                    height: 120
                    color: "white"
                    radius: 8
                    border.color: "#e0e0e0"
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 15
                        spacing: 15

                        Text {
                            text: "每日学习目标"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#333333"
                        }

                        Row {
                            width: parent.width
                            spacing: 15

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: "每日新词数量:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            SpinBox {
                                id: dailyWordsSpinBox
                                from: 5
                                to: 100
                                stepSize: 5
                                value: 20
                                width: 120
                                height: 40

                                background: Rectangle {
                                    implicitWidth: 120
                                    implicitHeight: 40
                                    border.color: "#c0c0c0"
                                    radius: 4
                                }
                            }

                            Text {
                                anchors.verticalCenter: parent.verticalCenter
                                text: "个词"
                                font.pixelSize: 16
                                color: "#555555"
                            }
                        }
                    }
                }

                // 复习周期设置
                Rectangle {
                    width: parent.width - 40
                    height: 360
                    color: "white"
                    radius: 8
                    border.color: "#e0e0e0"
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 15
                        spacing: 10

                        Text {
                            text: "复习周期设置"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#333333"
                        }

                        Text {
                            text: "设置不同阶段的复习时间间隔（单位：天）"
                            font.pixelSize: 14
                            color: "#777777"
                        }

                        GridLayout {
                            width: parent.width
                            columns: 2
                            rowSpacing: 15
                            columnSpacing: 10

                            // 第一轮复习
                            Text {
                                text: "第一轮复习:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            SpinBox {
                                id: review1SpinBox
                                from: 1
                                to: 7
                                value: 1
                                Layout.preferredWidth: parent.width / 2
                                height: 40

                                background: Rectangle {
                                    implicitWidth: 120
                                    implicitHeight: 40
                                    border.color: "#c0c0c0"
                                    radius: 4
                                }
                            }

                            // 第二轮复习
                            Text {
                                text: "第二轮复习:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            SpinBox {
                                id: review2SpinBox
                                from: 3
                                to: 14
                                value: 7
                                Layout.preferredWidth: parent.width / 2
                                height: 40

                                background: Rectangle {
                                    implicitWidth: 120
                                    implicitHeight: 40
                                    border.color: "#c0c0c0"
                                    radius: 4
                                }
                            }

                            // 第三轮复习
                            Text {
                                text: "第三轮复习:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            SpinBox {
                                id: review3SpinBox
                                from: 7
                                to: 30
                                value: 15
                                Layout.preferredWidth: parent.width / 2
                                height: 40

                                background: Rectangle {
                                    implicitWidth: 120
                                    implicitHeight: 40
                                    border.color: "#c0c0c0"
                                    radius: 4
                                }
                            }

                            // 第四轮复习
                            Text {
                                text: "第四轮复习:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            SpinBox {
                                id: review4SpinBox
                                from: 15
                                to: 60
                                value: 30
                                Layout.preferredWidth: parent.width / 2
                                height: 40

                                background: Rectangle {
                                    implicitWidth: 120
                                    implicitHeight: 40
                                    border.color: "#c0c0c0"
                                    radius: 4
                                }
                            }

                            // 第五轮复习
                            Text {
                                text: "第五轮复习:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            SpinBox {
                                id: review5SpinBox
                                from: 30
                                to: 180
                                value: 90
                                Layout.preferredWidth: parent.width / 2
                                height: 40

                                background: Rectangle {
                                    implicitWidth: 120
                                    implicitHeight: 40
                                    border.color: "#c0c0c0"
                                    radius: 4
                                }
                            }
                        }
                    }
                }

                // 学习统计数据（新增部分）
                Rectangle {
                    width: parent.width - 40
                    height: 200
                    color: "white"
                    radius: 8
                    border.color: "#e0e0e0"
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 15
                        spacing: 15

                        Text {
                            text: "学习统计"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#333333"
                        }

                        GridLayout {
                            width: parent.width
                            columns: 2
                            rowSpacing: 15
                            columnSpacing: 10

                            Text {
                                text: "已学单词:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            Text {
                                text: "134个"
                                font.pixelSize: 16
                                color: "#4a86e8"
                                font.bold: true
                            }

                            Text {
                                text: "当前正在复习:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            Text {
                                text: "40个"
                                font.pixelSize: 16
                                color: "#4a86e8"
                                font.bold: true
                            }

                            Text {
                                text: "连续学习天数:"
                                font.pixelSize: 16
                                color: "#555555"
                            }

                            Text {
                                text: "28天"
                                font.pixelSize: 16
                                color: "#4a86e8"
                                font.bold: true
                            }
                        }
                    }
                }

                // 提醒设置（新增部分）
                Rectangle {
                    width: parent.width - 40
                    height: 140
                    color: "white"
                    radius: 8
                    border.color: "#e0e0e0"
                    border.width: 1

                    Column {
                        anchors.fill: parent
                        anchors.margins: 15
                        spacing: 15

                        Text {
                            text: "提醒设置"
                            font.pixelSize: 18
                            font.bold: true
                            color: "#333333"
                        }

                        Row {
                            width: parent.width
                            spacing: 15

                            Text {
                                text: "每日提醒:"
                                font.pixelSize: 16
                                color: "#555555"
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Switch {
                                id: reminderSwitch
                                checked: true
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Text {
                                text: "提醒时间:"
                                font.pixelSize: 16
                                color: "#555555"
                                anchors.verticalCenter: parent.verticalCenter
                                visible: reminderSwitch.checked
                            }

                            ComboBox {
                                id: reminderTimeCombo
                                model: ["08:00", "12:00", "18:00", "20:00", "21:30"]
                                currentIndex: 3
                                anchors.verticalCenter: parent.verticalCenter
                                visible: reminderSwitch.checked
                                width: 100
                            }
                        }
                    }
                }

                // 保存按钮
                Rectangle {
                    width: parent.width - 40
                    height: 50
                    color: "#4a86e8"
                    radius: 8

                    Text {
                        anchors.centerIn: parent
                        text: "保存设置"
                        font.pixelSize: 18
                        font.bold: true
                        color: "white"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: {
                            // 这里可以添加保存设置的信号
                            console.log("保存设置")
                            console.log("每日新词: " + dailyWordsSpinBox.value)
                            console.log("复习周期: " + review1SpinBox.value + ", " +
                                      review2SpinBox.value + ", " +
                                      review3SpinBox.value + ", " +
                                      review4SpinBox.value + ", " +
                                      review5SpinBox.value)
                            console.log("提醒设置: " + (reminderSwitch.checked ? "开启" : "关闭"))
                            if (reminderSwitch.checked) {
                                console.log("提醒时间: " + reminderTimeCombo.currentText)
                            }
                        }

                        onPressed: parent.color = "#3a76d8"
                        onReleased: parent.color = "#4a86e8"
                    }
                }

                // 底部间距
                Item {
                    width: parent.width
                    height: 20
                }
            }
        }
    }
}
