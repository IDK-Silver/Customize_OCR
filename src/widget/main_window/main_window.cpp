//
// Created by idk on 2022/5/24.
//

#include <Config/Files_Path/Files_Path.h>
#include "main_window.h"
#include "./ui_main_window.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    // init widget object
    this->ui->setupUi(this);

    // to init all tab widget - create each widget object and add to tab widget
    this->tab_widget_init();

    // set this widget Qt connect
    this->setup_connect();

    // to check program path is existed
    Config::Files_Path::get_config_path();
}

void MainWindow::tab_widget_init() {
    // Create Other Widget
    ocr_detect_widget = new OCR_Detect_Widget();
    ocr_format_create_widget = new OCR_Format_Create_Widget();

    // Add to MainWindow TabWidget
    this->ui->tabWidget->addTab(ocr_detect_widget->window(), ocr_detect_widget->widget_name);
    this->ui->tabWidget->addTab(ocr_format_create_widget->window(), ocr_format_create_widget->widget_name);
}

/* the function is the all action center
 * to cll different widget function
 * */
void MainWindow::action_forward(ACTION_MAINWINDOWS action) {

    /* get the current windows name -> check the function in each windows */
    const QString current_object = this->ui->tabWidget->currentWidget()->objectName();

    /* check the menu action */
    switch (action) {

        /* open setting file */
        case ACTION_MAINWINDOWS::OPEN_FILE:
            if (current_object == ocr_detect_widget->objectName())
                ocr_detect_widget->action_open_file();
            break;

        /* open xlsx file */
        case ACTION_MAINWINDOWS::OPEN_XLSX_FILE:
            if (current_object == ocr_detect_widget->objectName())
                ocr_detect_widget->action_open_xlsx_file();
        default:
            break;
    }


}

void MainWindow::setup_connect() {

    /* Qt action will forward to other widget by function action_forward */

    /* open setting file */
    connect(this->ui->actionOpenFIle, &QAction::triggered,  [&] () {
        action_forward(ACTION_MAINWINDOWS::OPEN_FILE);
    });

    /* open xlsx file to write data */
    connect(this->ui->actionLoadXlxs, &QAction::triggered, [&] () {
        action_forward(ACTION_MAINWINDOWS::OPEN_XLSX_FILE);
    });


}

MainWindow::~MainWindow() = default;

