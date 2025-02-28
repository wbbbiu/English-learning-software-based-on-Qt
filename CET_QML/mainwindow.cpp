#include "mainwindow.h"
#include<QQuickWidget>
#include<QUrl>
#include<QQmlContext>
#include<QQmlEngine>
#include<QCoreApplication>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
   setupUi();
    connect(QCoreApplication::instance(),&QCoreApplication::aboutToQuit,this,&MainWindow::saveData);
}
MainWindow::~MainWindow()
{
}
void MainWindow::setupUi()
{
    m_quickwidget=new QQuickWidget(this);
    //设置窗口大小跟随qml试图
    m_quickwidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    //获取这个qml的上下文内容
    QQmlContext *content=m_quickwidget->engine()->rootContext();
    //讲这个数据库操作类注入到qml的上下文中，以便于使用
    AC_signal *sig=AC_signal::instance();
    if(sig){
         content->setContextProperty("AC",AC_signal::instance());
        qDebug() << "signal instance set to context.";
    }else{
        qWarning() << "Failed to get signal instance.";
    }
    DataStorage* dataStorageInstance = DataStorage::intance();
    if(dataStorageInstance) {
        content->setContextProperty("sql_op", dataStorageInstance);
        qDebug() << "DataStorage instance set to context.";
    } else {
        qWarning() << "Failed to get DataStorage instance.";
    }
    m_quickwidget->setSource(QUrl("qrc:/main.qml"));
    //将这个m_quickwidget
    setCentralWidget(m_quickwidget);
}

void MainWindow::saveData()
{
    DataStorage::intance()->SaveData();
}
