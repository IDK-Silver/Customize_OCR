//
// Created by idk on 2022/5/24.
//

#ifndef CUSTOMIZE_OCR_MAIN_WINDOW_H
#define CUSTOMIZE_OCR_MAIN_WINDOW_H

#include <QMainWindow>
#include <src/widget/ocr_detect_widget/ocr_detect_widget.h>
#include <src/widget/ocr_format_create_widget/ocr_format_create_widget.h>
#include <src/widget/main_window/strcut/action_enum.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;


private:
    Ui::MainWindow *ui{};

    /* tab widget child widget */
    OCR_Detect_Widget* ocr_detect_widget{};
    OCR_Format_Create_Widget* ocr_format_create_widget{};

    /* setup widget object connect */
    void setup_connect();

    /* to create all tab widget and add to tab widget */
    void tab_widget_init();

    /* to forward action to other widget */
    void action_forward(ACTION_MAINWINDOWS action);

private
    slots:


};

#endif //CUSTOMIZE_OCR_MAIN_WINDOW_H
