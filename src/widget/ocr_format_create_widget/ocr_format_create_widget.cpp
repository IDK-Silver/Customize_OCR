#include "ocr_format_create_widget.h"
#include "ui_ocr_format_create_widget.h"
#include <QDebug>
#include <Config/Global_Config/Global_Config.h>
#include <Config/Files_Path/Files_Path.h>



OCR_Format_Create_Widget::OCR_Format_Create_Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OCR_Format_Create_Widget)
{
    ui->setupUi(this);
    connect(this->ui->pb_load_file, SIGNAL(clicked()), SLOT(load_file()));
    connect(this->ui->pb_new_file, SIGNAL(clicked()), SLOT(new_file()));
    connect(this->ui->pb_save_file, SIGNAL(clicked()), SLOT(save_file()));
    connect(this->ui->pb_crop_range, SIGNAL(clicked()), SLOT(crop_range()));
    connect(this->ui->pb_clear_crop_range, SIGNAL(clicked()), SLOT(clear_crop_range()));
    connect(this->ui->pb_add_tag, SIGNAL(clicked()), SLOT(add_tag()));
    connect(this->ui->cb_tag, SIGNAL(currentIndexChanged(int)), SLOT(cb_tag_select(int)));
    connect(this->ui->le_tag_name, SIGNAL(textChanged(const QString &)), SLOT(le_tag_name_change(const QString &)));
    connect(this->ui->cb_is_search, SIGNAL(clicked(bool)), SLOT(cb_is_search(bool)));

    widget_init();
}

OCR_Format_Create_Widget::~OCR_Format_Create_Widget()
{
    delete ui;
}

void OCR_Format_Create_Widget::crop_range()
{
    // if not load file or image then out function
    if (not status.load_image || not status.load_file) return;

    // if is not crop mode
    if (not status.is_crop)
    {
        this->status.is_crop = true;
        this->ui->image_view->set_mouse_press_mode(MousePressEvent::PainterPen);
        this->ui->pb_crop_range->setText(QString("確認範圍"));
    }

    // if is crop mode
    else
    {
        this->status.is_crop = false;
        this->ui->image_view->set_mouse_press_mode(MousePressEvent::MoveImage);
        this->ui->pb_crop_range->setText(QString("新增範圍"));
        this->crop_label_refresh(this->ui->image_view->get_select_rect());
    }

    this->widget_refresh();
}

void OCR_Format_Create_Widget::clear_crop_range()
{
    // if not load file or image then out function
    if (not status.load_image || not status.load_file) return;

    // set status
    this->status.is_crop = false;

    // set image view to move mode and clear select rect
    this->ui->image_view->set_mouse_press_mode(MousePressEvent::MoveImage);
    this->ui->image_view->clear_select_rect();

    // update ui
    this->ui->pb_crop_range->setText(QString("新增範圍"));
    this->crop_label_refresh(QRect());
//    this->ui->pb_add_tag->setEnabled(true);
}

void OCR_Format_Create_Widget::crop_label_refresh(QRect input_rect) const
{
    this->ui->label_begin_x->setNum(input_rect.topLeft().x());
    this->ui->label_begin_y->setNum(input_rect.topLeft().y());
    this->ui->label_end_x->setNum(input_rect.bottomRight().x());
    this->ui->label_end_y->setNum(input_rect.bottomRight().y());
}

void OCR_Format_Create_Widget::add_tag()
{
    // if tage name is empty then out function
    if (this->ui->le_tag_name->text().isEmpty()) return;

    // if not load any file then out function
    if (not this->status.load_file) return;


    // create blank ocr data aka (tag file)
    auto ocr_data = std::make_shared<OCR_Data>();

    // set crop rect
    ocr_data->crop_rect = {
            QPoint(this->ui->label_begin_x->text().toInt(),
                   this->ui->label_begin_y->text().toInt()),
                   QPoint(this->ui->label_end_x->text().toInt(),
                          this->ui->label_end_y->text().toInt())
    };


    // set tag name
    ocr_data->tag = this->ui->le_tag_name->text();

    // set xlsx
    ocr_data->xlsx_cell = this->ui->le_xlsx_col->text();

    // set search and weight
    ocr_data->is_search = this->ui->cb_is_search->isChecked();
    ocr_data->search_weight = this->ui->sb_search_weight->value();


    // if not have any format, then create first tag and set current format to new format
    if (this->format_datas.empty() and this->current_format == nullptr)
    {
        this->current_format = ocr_data;
        this->format_datas.push_back(ocr_data);
    }

    // edit same tage
    else if (not this->format_datas.empty() and this->current_format->tag == ocr_data->tag)
    {
        qDebug() << "Change Edit";
        this->format_datas.at(this->status.current_format_index) = ocr_data;
        qDebug() << this->status.current_format_index;
    }

    // add tag
    else
    {
        this->format_datas.push_back(ocr_data);
        this->current_format = ocr_data;

    }

    // update current format index
    this->status.current_format_index = this->format_datas.distance(ocr_data);
    this->current_format = this->format_datas.at(this->status.current_format_index);
//    qDebug() << this->status.current_format_index;
//    qDebug() << this->ui->sb_search_weight->value();
//    qDebug() << ocr_data->search_weight;
//    qDebug() << this->format_datas.at(this->status.current_format_index)->search_weight;
//    qDebug() << this->current_format->search_weight;
    this->widget_refresh();
//    le_tag_name_change(this->ui->le_tag_name->text())
}


void OCR_Format_Create_Widget::widget_refresh() {

    if (this->status.load_file)
    {
        this->ui->pb_new_file->setEnabled(false);

        this->ui->pb_save_file->setEnabled(not this->status.is_crop);

        // update pb_crop_range
        this->ui->pb_crop_range->setEnabled(true);

        // update ui pb_clear_crop_range
        if (this->status.is_crop)   this->ui->pb_clear_crop_range->setEnabled(true);
        else    this->ui->pb_clear_crop_range->setEnabled(false);;

        // update ui pb_add_tag
        if (not this->ui->le_tag_name->text().isEmpty() && not this->status.is_crop)
        {
            this->ui->pb_add_tag->setEnabled(true);
        }
        else(this->ui->pb_add_tag->setEnabled(false));


        // pb add tag
        if (this->current_format != nullptr and this->ui->le_tag_name->text() == this->current_format->tag)
        {
            this->ui->pb_add_tag->setText("編輯標籤");
        }
        else
        {
            this->ui->pb_add_tag->setText("新增標籤");
        }

        // update ui cb_tag
        {
            this->status.is_create_cb = true;
            int current_in_cb = 0;
            this->ui->cb_tag->clear();
            for (int index = 0; index < this->format_datas.size(); index++)
            {
                // if is current index in cb, recode it
                if (format_datas.at(index) == current_format)
                    current_in_cb = index;

                // add cb item
                QString tag_name = this->format_datas.at(index)->tag;
                this->ui->cb_tag->addItem(tag_name);

            }
            this->ui->cb_tag->setCurrentIndex(current_in_cb);
            this->status.is_create_cb = false;
        }

        this->ui->cb_is_search->setEnabled(true);

        if (this->status.load_file)
        {
            this->ui->pb_load_file->setText("捨棄設定檔");
        }
        else
        {
            this->ui->pb_load_file->setText("載入設定檔");
        }

    }
    else
    {
        // update ui status
        this->ui->pb_crop_range->setEnabled(false);
        this->ui->sb_search_weight->setEnabled(false);
        this->ui->cb_is_search->setEnabled(false);
        this->ui->pb_new_file->setEnabled(true);
        this->ui->pb_save_file->setEnabled(false);
    }
};


void OCR_Format_Create_Widget::cb_tag_select(int index)
{
    if (not this->status.load_file)
        return;

    if (this->status.is_create_cb)
        return;

    if (this->ui->cb_tag->count() == 0)
        return;

    int current_in_format = 0;
    QString cb_tag = this->ui->cb_tag->itemText(index);

    for (current_in_format = 0; current_in_format < this->format_datas.size(); current_in_format++)
    {
        if (this->format_datas.at(current_in_format)->tag == cb_tag)
            break;
    }

    this->current_format = format_datas.at(current_in_format);
    this->status.current_format_index = this->format_datas.distance(this->current_format);

    this->crop_label_refresh(this->current_format->crop_rect);
    this->ui->le_tag_name->setText(this->current_format->tag);
    this->ui->le_xlsx_col->setText(this->current_format->xlsx_cell);

    this->ui->cb_is_search->setChecked(this->current_format->is_search);
    this->ui->sb_search_weight->setEnabled(this->current_format->is_search);
    this->ui->sb_search_weight->setValue(this->current_format->search_weight);

    this->widget_refresh();
}

void OCR_Format_Create_Widget::save_file() {

    // if not any file exist then out function
    if (not status.load_file)
    {
        return;
    }

    // get save file name and ptah
    auto save_file_path = QFileDialog::getSaveFileName(this, QString("設定檔處存"),
                                                   QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
                                                   QString("設定檔 (*.json)"));

    if (save_file_path.isEmpty())
        return;

    // save file
    this->format_datas.save_file(save_file_path);

};

void OCR_Format_Create_Widget::widget_init() {
    this->ui->cb_tag->clear();
    this->ui->image_view->clear();
    this->ui->pb_add_tag->setEnabled(false);
    this->ui->pb_clear_crop_range->setEnabled(false);
    this->ui->pb_crop_range->setEnabled(false);
    this->ui->sb_search_weight->setEnabled(false);
    this->ui->cb_is_search->setEnabled(false);
    this->ui->label_begin_x->clear();
    this->ui->label_begin_y->clear();
    this->ui->label_end_x->clear();
    this->ui->label_end_y->clear();
    this->ui->sb_search_weight->setEnabled(false);
    this->ui->sb_search_weight->setValue(0);
    this->ui->cb_is_search->setEnabled(false);
    this->ui->cb_is_search->setChecked(false);
    this->ui->le_tag_name->clear();
    this->ui->le_xlsx_col->clear();
    this->ui->pb_new_file->setEnabled(true);
    this->ui->pb_save_file->setEnabled(false);

    this->ui->pb_load_file->setText("載入設定檔");
}


void OCR_Format_Create_Widget::load_file() {
    // widget init

    if (not status.load_file)
    {
        // get format path
        QString choose_file = QFileDialog::getOpenFileName(this, QString("選設定檔"),
                                                        QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
                                                        QString("設定檔 (*.json)"));
        // ensure path is exit
        if (choose_file == QString() or choose_file == nullptr or
             not Utility::Files_Manager::is_exists_file(choose_file.toLocal8Bit().toStdString()))
        {
            if (Config::Global::is_output_io_error_message)
            {
                std::cerr <<  this->widget_name.toLocal8Bit().toStdString()
                          << " : load format file path is error";
            }
            return;
        }

        this->widget_init();

        // loading file format
        this->format_datas.load_file(choose_file);
        this->status.load_file = true;

        // init the widget part data import from load file
        this->status.current_format_index = 0;
        this->current_format = this->format_datas.at(this->status.current_format_index);

        // image view import image from load file
        this->ui->image_view->read_image(this->format_datas.get_image());
        this->status.load_image = true;

        this->widget_refresh();

        if (not this->format_datas.empty())
            this->cb_tag_select(0);
    }
    else
    {
        this->status.load_file = false;
        this->status.load_image = false;
        this->widget_init();
    }
}

void OCR_Format_Create_Widget::new_file() {
    // init widget
    this->widget_init();

    // is import image or file
    if (this->status.load_file || this->status.load_image)
    {
        QMessageBox message_box;
        message_box.setText("有檔案");

    }

    // clear old format datas
    this->current_format = nullptr;
    this->format_datas.clear();

    // update status
    this->status.load_file = true;

    // select file
    QString select_file = QFileDialog::getOpenFileName(this, QString("選圖片檔"),
                                                        QDir::homePath().append("/Pictures").toLocal8Bit(),
                                                        QString("圖片檔 (*.png *.jpg *.tif)"));
    // path is existed
    if (select_file.isEmpty() || Utility::Files_Manager::is_exists_file(select_file.toLocal8Bit().toStdString()))
    {
        if (Config::Global::is_output_io_error_message)
        {
            std::cerr << this->widget_name.toLocal8Bit().toStdString()
                      << " : creat new file error : select file path is not exist\n";
        }
    }

    // image view import image from select path
    this->ui->image_view->read_image(QImage(select_file));
    this->status.load_image = true;
    this->widget_refresh();
}

void OCR_Format_Create_Widget::le_tag_name_change(const QString & str) {
    this->widget_refresh();
}

void OCR_Format_Create_Widget::cb_is_search(bool is_clicked) {
    this->ui->sb_search_weight->setEnabled(is_clicked);
}


//void OCR_Format_Create_Widget::add_image() {
//
//    if (not is_import_image) {
//        auto choose_file = QFileDialog::getOpenFileName(this, QString("選圖片檔"),
//                                                        QDir::homePath().append("/Pictures").toLocal8Bit(),
//                                                        QString("圖片檔 (*.png *.jpg *.tif)"));
//        if (choose_file == QString() or choose_file == nullptr)
//        {
//            return;
//        }
//        QImage image(choose_file);
//
//        this->ui->image_view->read_image(image);
//
//        this->is_import_image = true;
//        this->ui->pb_choose_crop_range->setEnabled(true);
//        this->ui->pb_add_setting_file->setText("處存設定檔");
//        this->ui->pb_load_setting_file->setText("捨棄設定檔");
//    }
//    else
//    {
//        auto save_file_path = QFileDialog::getSaveFileName(this, QString("設定檔處存"),
//                                                           QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
//                                                           QString("設定檔 (*.json)"));
//
//        ocr_data_list.save_file(save_file_path);
//
//        widget_init();
//    }
//
//
//
//};

//void OCR_Format_Create_Widget::add_tag() {
//
//    if (this->ui->le_tag_name->text() == nullptr) {
//        return;
//    }
//
//    auto ocr_data = std::make_shared<OCR_Data>();
//
//
//    ocr_data->crop_rect = this->get_crop_image_rect();
//    ocr_data->tag = this->ui->le_tag_name->text();
//    ocr_data->xlsx_cell = this->ui->le_excel_col->text();
//
//    // create first tag
//    if (ocr_data_list.empty() and now_edit_format == nullptr) {
//        now_edit_format = ocr_data;
//        ocr_data_list.push_back(ocr_data);
//    }
//
//    // edit format
//    else if (not ocr_data_list.empty() and now_edit_format->tag == ocr_data->tag) {
//        if (now_edit_format != nullptr) {
//            ocr_data_list.at(now_edit_format_index) = ocr_data;
//            this->now_edit_format = ocr_data;
//        }
//    }
//
//    // add other tag
//    else
//    {
//        this->ocr_data_list.push_back(ocr_data);
//        this->now_edit_format = ocr_data;
//        this->tag_choose_refresh();
//        this->now_edit_format_index = this->ui->cb_tage_choose->count() - 1;
//    }
//
//    this->tag_choose_refresh();
//    le_tag_name_change(this->ui->le_tag_name->text());
//}

