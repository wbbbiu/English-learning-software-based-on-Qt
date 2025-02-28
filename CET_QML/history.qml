import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtCharts 2.15

Item {
    anchors.fill:parent

    // 模拟数据
    property var dailyData: [
        { date: "02-16", count: 25 },
        { date: "02-17", count: 30 },
        { date: "02-18", count: 28 },
        { date: "02-19", count: 35 },
        { date: "02-20", count: 20 },
        { date: "02-21", count: 40 },
        { date: "02-22", count: 38 },
        { date: "02-23", count: 42 },
        { date: "02-24", count: 30 },
        { date: "02-25", count: 45 }
    ]

    property var weeklyData: [
        { date: "第1周", count: 180 },
        { date: "第2周", count: 200 },
        { date: "第3周", count: 220 },
        { date: "第4周", count: 190 },
        { date: "第5周", count: 210 },
        { date: "第6周", count: 250 },
        { date: "第7周", count: 240 },
        { date: "第8周", count: 260 },
        { date: "第9周", count: 230 },
        { date: "第10周", count: 280 }
    ]

    property var monthlyData: [
        { date: "1月", count: 750 },
        { date: "2月", count: 820 },
        { date: "3月", count: 880 },
        { date: "4月", count: 900 },
        { date: "5月", count: 950 },
        { date: "6月", count: 1000 },
        { date: "7月", count: 920 },
        { date: "8月", count: 970 },
        { date: "9月", count: 1050 },
        { date: "10月", count: 1100 }
    ]

    property var currentData: dailyData
    property string currentMode: "daily"
    property string currentDate: "2025-02-25"

    // 更新图表数据和标题
    function updateChartData() {
        // 清除旧数据
        lineSeries.clear()

        // 设置Y轴范围
        let maxCount = 0
        for (let i = 0; i < currentData.length; i++) {
            if (currentData[i].count > maxCount) {
                maxCount = currentData[i].count
            }
        }
        valueAxisY.max = Math.ceil(maxCount * 1.2)

        // 添加数据点
        for (let i = 0; i < currentData.length; i++) {
            lineSeries.append(i, currentData[i].count)
        }

        // 更新标题
        if (currentMode === "daily") {
            chartTitle.text = "每日单词学习记录"
        } else if (currentMode === "weekly") {
            chartTitle.text = "每周单词学习记录"
        } else {
            chartTitle.text = "每月单词学习记录"
        }

        // 更新横坐标标签
        xLabelsModel.clear()
        for (let i = 0; i < currentData.length; i++) {
            xLabelsModel.append({ labelText: currentData[i].date })
        }
    }

    // 切换到日视图
    function switchToDaily() {
        currentData = dailyData
        currentMode = "daily"
        updateChartData()
    }

    // 切换到周视图
    function switchToWeekly() {
        currentData = weeklyData
        currentMode = "weekly"
        updateChartData()
    }

    // 切换到月视图
    function switchToMonthly() {
        currentData = monthlyData
        currentMode = "monthly"
        updateChartData()
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 10

        // 顶部日期显示和切换按钮
        RowLayout {
            Layout.fillWidth: true

            Label {
                text: "当前日期: " + currentDate
                font.pixelSize: 16
                font.bold: true
            }

            Item { Layout.fillWidth: true }

            Row {
                spacing: 10

                Button {
                    id: dailyButton
                    text: "日"
                    checkable: true
                    checked: true
                    onClicked: {
                        switchToDaily()
                        weeklyButton.checked = false
                        monthlyButton.checked = false
                    }
                }

                Button {
                    id: weeklyButton
                    text: "周"
                    checkable: true
                    onClicked: {
                        switchToWeekly()
                        dailyButton.checked = false
                        monthlyButton.checked = false
                    }
                }

                Button {
                    id: monthlyButton
                    text: "月"
                    checkable: true
                    onClicked: {
                        switchToMonthly()
                        dailyButton.checked = false
                        weeklyButton.checked = false
                    }
                }
            }
        }

        // 图表区域
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true

            // 图表标题
            Text {
                id: chartTitle
                text: "每日单词学习记录"
                anchors.top: parent.top
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: 18
                font.bold: true
            }

            // 图表
            ChartView {
                id: chartView
                anchors.fill: parent
                anchors.topMargin: 30
                anchors.bottomMargin: 30
                legend.visible: false
                antialiasing: true

                // Y轴
                ValueAxis {
                    id: valueAxisY
                    min: 0
                    max: 50
                    titleText: "单词数量"
                    labelsFont.pixelSize: 10
                }

                // X轴 (只用于数值定位，不显示标签)
                ValueAxis {
                    id: valueAxisX
                    min: -0.5
                    max: 9.5
                    visible: true
                    labelsVisible: false
                    gridVisible: false
                }

                // 折线
                LineSeries {
                    id: lineSeries
                    axisX: valueAxisX
                    axisY: valueAxisY
                    width: 2
                    color: "#1f77b4"
                    pointsVisible: true
                    pointLabelsVisible: true
                    pointLabelsFormat: "@yPoint"
                }
            }

            // X轴标签 (自定义实现)
            ListView {
                id: xLabelsView
                anchors.left: chartView.left
                anchors.right: chartView.right
                anchors.bottom: parent.bottom
                height: 30
                orientation: ListView.Horizontal
                interactive: false

                model: ListModel { id: xLabelsModel }

                delegate: Item {
                    width: xLabelsView.width / xLabelsModel.count
                    height: xLabelsView.height

                    Text {
                        anchors.centerIn: parent
                        text: labelText
                        font.pixelSize: 10
                    }
                }
            }
        }

        // 详细数据列表
        GroupBox {
            title: "详细数据"
            Layout.fillWidth: true
            Layout.preferredHeight: 200

            ListView {
                anchors.fill: parent
                clip: true
                model: currentData

                delegate: ItemDelegate {
                    width: parent.width
                    height: 40

                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 5

                        Label {
                            text: modelData.date
                            font.pixelSize: 14
                            Layout.preferredWidth: 100
                        }

                        Item { Layout.fillWidth: true }

                        Label {
                            text: modelData.count + " 个单词"
                            font.pixelSize: 14
                            font.bold: true
                        }
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        // 初始化图表数据
        updateChartData()
    }
}
