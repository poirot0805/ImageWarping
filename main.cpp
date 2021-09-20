#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
