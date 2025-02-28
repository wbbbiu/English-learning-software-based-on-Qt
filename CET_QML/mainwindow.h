#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include<QQuickWidget>
#include <QMainWindow>
#include<QSqlDatabase>
#include<QFile>
#include"datastorage.h"
#include"ac_signal.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

    void setupUi();
    QQuickWidget *m_quickwidget;
    DataStorage *data_storage;
    void saveData();
};

#endif // MAINWINDOW_H
