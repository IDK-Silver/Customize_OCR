#include <QtWidgets/QApplication>
#include <src/widget/mainwinodw/mainwindow.h>
#include "libraries/APP_Setting/APP_Setting.h"

int main(int argc, char* argv[])
{
    APP_Setting();
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return QApplication::exec();
}