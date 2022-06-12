#include "ocr_format_create_widget.h"
#include "ui_ocr_format_create_widget.h"

OCR_Format_Create_Widget::OCR_Format_Create_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OCR_Format_Create_Widget)
{
    ui->setupUi(this);
}

OCR_Format_Create_Widget::~OCR_Format_Create_Widget()
{
    delete ui;
}
