#include <QtWidgets/QApplication>
#include <src/widget/main_window//main_window.h>
#include <lib/Config/Files_Path/Files_Path.h>
#include <src/widget/ocr_detect_widget/ocr_detect_widget.h>
#include <QDebug>

int main(int argc, char* argv[])
{
    qDebug() << Config::Files_Path::get_config_path().data();
    QApplication app(argc, argv);
    MainWindow main_window;
//    OCR_Detect_Widget widget;
//    widget.show();
    main_window.show();
    return QApplication::exec();
}