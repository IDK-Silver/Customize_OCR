#include "ocr_format_create_widget.h"
#include "ui_ocr_format_create_widget.h"
#include <QDebug>
#include "lib/Config/Global_Config/Global_Config.h"
#include "lib/Config/Files_Path/Files_Path.h"



OCR_Format_Create_Widget::OCR_Format_Create_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OCR_Format_Create_Widget)
{
    ui->setupUi(this);
    connect(this->ui->pb_add_setting_file, SIGNAL(clicked()), SLOT(add_image()));
    connect(this->ui->pb_choose_crop_range, SIGNAL(clicked()), SLOT(choose_image_crop_range()));
    connect(this->ui->pb_clear_crop_range, SIGNAL(clicked()), SLOT(clear_image_crop_range()));
    connect(this->ui->pb_add_tag, SIGNAL(clicked()), SLOT(add_tag()));
    connect(this->ui->cb_tage_choose, SIGNAL(currentIndexChanged(int)), SLOT(cb_tag_choose_index_change(int)));
    connect(this->ui->le_tag_name, SIGNAL(textChanged(const QString &)), SLOT(le_tag_name_change(const QString &)));
    connect(this->ui->pb_load_setting_file, SIGNAL(clicked()), SLOT(load_setting_file()));
    widget_init();
}

OCR_Format_Create_Widget::~OCR_Format_Create_Widget()
{
    delete ui;
}

void OCR_Format_Create_Widget::add_image() {

    if (not is_import_image) {
        auto choose_file = QFileDialog::getOpenFileName(this, QString("選圖片檔"),
                                                        QDir::homePath().append("/Pictures").toLocal8Bit(),
                                                        QString("圖片檔 (*.png *.jpg *.tif)"));
        if (choose_file == QString() or choose_file == nullptr)
        {
            return;
        }
        QImage image(choose_file);

        this->ui->image_view->read_image(image);

        this->is_import_image = true;
        this->ui->pb_choose_crop_range->setEnabled(true);
        this->ui->pb_add_setting_file->setText("處存設定檔");
        this->ui->pb_load_setting_file->setText("捨棄設定檔");
    }
    else
    {
        auto save_file_path = QFileDialog::getSaveFileName(this, QString("設定檔處存"),
                                                           QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
                                                           QString("設定檔 (*.json)"));

        OCR_Format_Setting setting(this->ui->image_view->get_raw_image(), this->format_list);
        setting.save_file(save_file_path);


        widget_init();
    }



};

void OCR_Format_Create_Widget::choose_image_crop_range() {

    if (is_crop_image_range) {
        is_crop_image_range = false;
        this->ui->image_view->set_mouse_press_mode(MousePressEvent::MoveImage);
        this->ui->pb_choose_crop_range->setText(QString("選取範圍"));
        change_image_crop_label(this->ui->image_view->get_select_rect());

        this->ui->pb_add_tag->setEnabled(true);
    }
    else
    {
        is_crop_image_range = true;
        this->ui->image_view->set_mouse_press_mode(MousePressEvent::PainterPen);
        this->ui->image_view->clear_select_rect();
        this->ui->pb_choose_crop_range->setText(QString("確認範圍"));

        this->ui->pb_add_tag->setEnabled(false);
    }
}

void OCR_Format_Create_Widget::clear_image_crop_range() {
    is_crop_image_range = false;
    this->ui->image_view->set_mouse_press_mode(MousePressEvent::MoveImage);
    this->ui->pb_choose_crop_range->setText(QString("選取範圍"));
    change_image_crop_label(QRect());
    this->ui->image_view->clear_select_rect();

    this->ui->pb_add_tag->setEnabled(true);
}

void OCR_Format_Create_Widget::add_tag() {

    if (this->ui->le_tag_name->text() == nullptr) {
        return;
    }

    auto format = std::make_shared<OCR_Format_Data>();
    format->crop_rect = this->get_crop_image_rect();
    format->tag = this->ui->le_tag_name->text();
    format->excel_col = this->ui->le_excel_col->text();

    // create first tag
    if (format_list.empty() and now_edit_format == nullptr) {
        now_edit_format = format;
        format_list.push_back(format);
    }
    // edit format
    else if (not format_list.empty() and now_edit_format->tag == format->tag) {
        if (now_edit_format != nullptr) {
            format_list.at(now_edit_format_index) = format;
            this->now_edit_format = format;
        }
    }
    // add other tag
    else
    {
        this->format_list.push_back(format);
        this->now_edit_format = format;
        this->tag_choose_refresh();
        this->now_edit_format_index = this->ui->cb_tage_choose->count() - 1;
    }

    this->tag_choose_refresh();
    le_tag_name_change(this->ui->le_tag_name->text());
}

QRect OCR_Format_Create_Widget::get_crop_image_rect() const {
    return {QPoint(this->ui->label_begin_x->text().toInt(), this->ui->label_begin_y->text().toInt()),
                 QPoint(this->ui->label_end_x->text().toInt(), this->ui->label_end_y->text().toInt())};
}

void OCR_Format_Create_Widget::tag_choose_refresh() {
    this->ui->cb_tage_choose->clear();
    for (const auto& format : this->format_list) {
        this->ui->cb_tage_choose->addItem(format->tag);
    }
}

void OCR_Format_Create_Widget::cb_tag_choose_index_change(int index) {

    if (index < 0)
        return;

    auto format = format_list.at(index);
    change_image_crop_label(format->crop_rect);
    this->ui->le_tag_name->setText(format->tag);
    this->ui->le_excel_col->setText(format->excel_col);
    le_tag_name_change(this->ui->le_tag_name->text());
}

void OCR_Format_Create_Widget::change_image_crop_label(QRect input_rect) const {
    this->ui->label_begin_x->setNum(input_rect.topLeft().x());
    this->ui->label_begin_y->setNum(input_rect.topLeft().y());
    this->ui->label_end_x->setNum(input_rect.bottomRight().x());
    this->ui->label_end_y->setNum(input_rect.bottomRight().y());
}

void OCR_Format_Create_Widget::widget_init() {
    this->ui->cb_tage_choose->clear();
    this->ui->image_view->clear();
    this->ui->pb_add_setting_file->setText("新增設定檔");
    this->ui->pb_load_setting_file->setText("載入設定檔");
    this->format_list.clear();
    this->now_edit_format = nullptr;
    this->now_edit_format_index = 0;
    this->is_import_image = false;
    this->ui->pb_add_tag->setEnabled(false);
    this->ui->pb_clear_crop_range->setEnabled(false);
    this->ui->pb_choose_crop_range->setEnabled(false);
}

void OCR_Format_Create_Widget::le_tag_name_change(const QString & text) const {

    if (not is_import_image)
        return;

    if (this->now_edit_format != nullptr and this->now_edit_format->tag == text) {
        this->ui->pb_add_tag->setText("更新標籤內容");
    }
    else
    {
        this->ui->pb_add_tag->setText("新增標籤");
    }

    if (text.size() > 0) {
        this->ui->pb_add_tag->setEnabled(true);
    }
    else {
        this->ui->pb_add_tag->setEnabled(false);
    }
}

void OCR_Format_Create_Widget::load_setting_file() {

    if (not is_import_image) {
        auto choose_file = QFileDialog::getOpenFileName(this, QString("選設定檔"),
                                                        QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
                                                        QString("設定檔 (*.json)"));

        if (choose_file == QString() or choose_file == nullptr)
        {
            return;
        }

        OCR_Format_Setting setting;
        setting.load_file(choose_file);
        this->format_list = setting.get_format_list();

        if (not format_list.empty())
            this->now_edit_format = this->format_list.at(0);

        now_edit_format_index = 0;

        QImage image = setting.get_image();
        this->ui->image_view->read_image(image);

        this->is_import_image = true;
        this->ui->pb_choose_crop_range->setEnabled(true);
        this->ui->pb_add_setting_file->setText("處存設定檔");
        this->ui->pb_load_setting_file->setText("捨棄設定檔");
        tag_choose_refresh();
    }
    else
    {
        widget_init();
        this->ui->pb_add_setting_file->setText("新增設定檔");
        this->ui->pb_load_setting_file->setText("載入設定檔");
    }

};