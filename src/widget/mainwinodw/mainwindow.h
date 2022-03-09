#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <libraries/OCR_Image/OCR_Image.h>
#include "libraries/APP_Setting/APP_Setting.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // variable
    OCR_Image ocr_image;
    App_Data app_data;
    
private:
    Ui::MainWindow *ui;

    QStringList get_ocr_files_path(QString);

private slots:
    QString openfile();

    void start_ocr();

    void OCR_Test();

    

};
#endif // MAINWINDOW_H
