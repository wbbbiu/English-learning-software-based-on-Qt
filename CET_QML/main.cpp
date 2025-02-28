#include"mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow main_widow;
    main_widow.show();
    return a.exec();
}
