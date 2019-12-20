#include "mainwindow.h"

#include <QApplication>
#include <iostream>
#include <ctime>

int main(int argc, char *argv[])
{
    srand(static_cast<unsigned>(time(nullptr)));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    try {
        a.exec();
    } catch (const std::bad_alloc &) {
        qDeleteAll(a.findChildren<QWidget *>(QString(), Qt::FindDirectChildrenOnly));
    }
    return 0;
}
