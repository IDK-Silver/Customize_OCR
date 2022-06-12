//
// Created by idk on 2022/5/24.
//

#include "main_window.h"
#include "./ui_main_window.h"
#include <src/widget/ocr_detect_widget/ocr_detect_widget.h>
#include <src/widget/ocr_format_create_widget/ocr_format_create_widget.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->ui->setupUi(this);

    this->tab_widget_init();


}

void MainWindow::tab_widget_init() {
    // Create Other Widget
    auto ocr_detect_widget = new OCR_Detect_Widget();
    auto ocr_format_create_widget = new OCR_Format_Create_Widget();

    // Add to MainWindow TabWidget
    this->ui->tabWidget->addTab(ocr_detect_widget->window(), ocr_detect_widget->widget_name);
    this->ui->tabWidget->addTab(ocr_format_create_widget->window(), ocr_format_create_widget->widget_name);
}

MainWindow::~MainWindow() = default;

