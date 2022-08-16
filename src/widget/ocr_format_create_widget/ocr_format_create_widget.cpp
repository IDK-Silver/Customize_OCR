#include "ocr_format_create_widget.h"
#include "ui_ocr_format_create_widget.h"



OCR_Format_Create_Widget::OCR_Format_Create_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OCR_Format_Create_Widget)
{
    ui->setupUi(this);
    connect(this->ui->pb_add_setting_file, SIGNAL(clicked()), SLOT(add_image()));
    connect(this->ui->pb_choose_crop_range, SIGNAL(clicked()), SLOT(choose_image_crop_range()));
    connect(this->ui->pb_clear_crop_range, SIGNAL(clicked()), SLOT(clear_image_crop_range()));
    connect(this->ui->image_view, &ImageViewLabel::pos_change, [&]()
    {
        if (this->ui->image_view->get_mouse_press_event() == MousePressEvent::MoveImage)
            return;
        auto select_rect = this->ui->image_view->get_select_rect();
        this->ui->label_begin_x->setNum(select_rect.topLeft().x());
        this->ui->label_begin_y->setNum(select_rect.topLeft().y());
        this->ui->label_end_x->setNum(select_rect.topRight().x());
        this->ui->label_end_y->setNum(select_rect.topRight().y());
    });
}

OCR_Format_Create_Widget::~OCR_Format_Create_Widget()
{
    delete ui;
}

void OCR_Format_Create_Widget::add_image() {
    auto choose_file = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("選圖片檔"),
                                                    QDir::homePath().append("/Pictures").toLocal8Bit(),
                                                    QString::fromLocal8Bit("圖片檔 (*.png *.jpg *.tif)"));
    if (choose_file == QString() or choose_file == nullptr)
    {
        return;
    }
    QImage image(choose_file);
    this->ui->image_view->read_image(image);
};


void OCR_Format_Create_Widget::choose_image_crop_range() {
    this->ui->image_view->set_mouse_press_mode(MousePressEvent::PainterPen);

}

void OCR_Format_Create_Widget::clear_image_crop_range() {
    this->ui->image_view->set_mouse_press_mode(MousePressEvent::MoveImage);
    this->ui->label_begin_x->setNum(0);
    this->ui->label_begin_y->setNum(0);
    this->ui->label_end_x->setNum(0);
    this->ui->label_end_y->setNum(0);
    this->ui->image_view->clear_get_select_rect();
}



