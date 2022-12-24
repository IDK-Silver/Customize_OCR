//
// Created by idk on 2022/6/10.
//

#include <QFileDialog>
#include <Utility/ImageFormat/ImageFormat.h>
#include "ocr_detect_widget.h"
#include "ui_ocr_detect_widget.h"
#include <Config/Files_Path/Files_Path.h>


OCR_Detect_Widget::OCR_Detect_Widget(QWidget *parent): QWidget(parent), ui(new Ui::OCR_Detect_Widget)
{
    this->ui->setupUi(this);
    this->connect_setup();
    this->ocr_data_list = std::make_shared<OCR_Data_List>();
    this->ocr_processing = std::make_unique<ImageOCR>();
    this->x_c = std::make_shared<XlntControl>();
}

OCR_Detect_Widget::~OCR_Detect_Widget() = default;


//void OCR_Detect_Widget::add_list_widget_ocr_data(const OCR_Display_Data& data) {
//
//    // create widget and add to list_widget
////    auto ocr_data_widget = new OCR_Data_ListWidget(this);
//    auto ocr_data_widget = std::make_shared<OCR_Data_ListWidget>(this);
//
//    ocr_data_widget_list.push_back(ocr_data_widget);
//
//    auto item = new QListWidgetItem();
//    ui->listWidget->setViewMode(QListView::ListMode);
//
//    // set widget height size
//    item->setSizeHint(QSize(0, 100));
//    this->ui->listWidget->addItem(item);
//    this->ui->listWidget->setItemWidget(item, ocr_data_widget.get());
//
//    // change widget data
//    ocr_data_widget->change_tag(data.tag);
//    ocr_data_widget->change_crop_image(data.crop_image);
//    ocr_data_widget->change_ocr_text(data.ocr_text);
//    ocr_data_widget->change_xlsx_text(data.xlsx);
//}

void OCR_Detect_Widget::load_setting_file() {
    auto choose_file = QFileDialog::getOpenFileName(this, QString("選設定檔"),
                                                    QString::fromStdString(Config::Files_Path::get_format_setting_folder_path()),
                                                    QString("設定檔 (*.json)")).toLocal8Bit();
    /* load the setting file*/
    this->ocr_data_list->load_file(choose_file);
}

void OCR_Detect_Widget::ocr_image() {

    /* clear the list widget element */
    this->ui->listWidget->clear_widget();

    /* check is image need ocr */
    if (this->wait_ocr_path_list.empty()) {
        return;
    }

    /* load image by the first index of wait ocr path list */
    cv::Mat  image = cv::imread(this->wait_ocr_path_list.at(0).toLocal8Bit().toStdString());

    /* del the front path */
    this->wait_ocr_path_list.pop_front();


    /* add list widget element (the ocr data)  */
    /* set OCR display data from OCR Format Data
     * and add to the list widget to display in UI
     * */

    for (int index = 0; index < this->ocr_data_list->size(); index++)
    {
        /* to make ptr to ocr data list */
        auto ocr_data = this->ocr_data_list->at(index);

        /* crop image by format crop rect*/
        cv::Mat crop_image = Utility::MatCropByQRect(image, ocr_data->crop_rect);

        ocr_processing->set_image(crop_image);

        /* set OCR display data */
        ocr_data->crop_image = Utility::Mat2QPixmap(crop_image);
        ocr_data->ocr_text = ocr_processing->ocr();

        this->ui->listWidget->add_item_widget(ocr_data);
    }
}

void OCR_Detect_Widget::action_open_file() {
    load_setting_file();
}

void OCR_Detect_Widget::action_open_xlsx_file() {
    /* get the choose file path */
    auto choose_file = QFileDialog::getOpenFileName(this, QString("選設定檔"),
                                                    QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation
                                                    ).at(0).toLocal8Bit(),
                                                    QString("設定檔 (*.xlsx)")).toLocal8Bit();
    /* load the xlsx file*/
    x_c->load(choose_file.toStdString());
    x_c->set_sheet(0);
    QMessageBox::information(nullptr, "開啟XLSX檔案成功", "以載入XLSX");
}


void OCR_Detect_Widget::connect_setup() {
    connect(this->ui->pb_load_setting_file, SIGNAL(clicked()), SLOT(load_setting_file()));
    connect(this->ui->pb_star_ocr, SIGNAL(clicked()), SLOT(ocr_image()));
    connect(this->ui->pb_choose_image, SIGNAL(clicked()), SLOT(choose_images()));
    connect(this->ui->pb_write_xlsx, SIGNAL(clicked()), SLOT(write_xlsx_data()));
}

void OCR_Detect_Widget::choose_images() {
    QStringList choose_files = QFileDialog::getOpenFileNames(this, QString("選擇圖片"),
                                                    QString::fromStdString(Config::Files_Path::get_system_picture_path()),
                                                    QString("圖片 (*.jpg *.png *tif)"));

    this->wait_ocr_path_list = choose_files;
}

void OCR_Detect_Widget::write_xlsx_data() {



    /* get data form list widget */
//    for (const auto& data_widget : this->ocr_data_widget_list)
//    {
//        unsigned int max_row = this->x_c->sheet.highest_row();
//        QString ocr_text = data_widget->get_ocr_text();
//        QString tag_name = data_widget->get_tag();
//        QString xlsx_col = data_widget->get_xlsx_text();
//
//
//        /* conversion xlsx to program index  */
//        xlnt::cell_reference ce(fuzzy_reference(xlsx_col.toLocal8Bit().toStdString()));
//
////        qDebug() << ce.column().index;
//
//        /* find the xlsx col index
//         * to write a new data
//         * */
//
//
//        this->x_c->write_data(ce.column().index - 1, max_row - 1, ocr_text.toLocal8Bit().toStdString());
//
////
////        this->x_c->find_element_inCol(ce.column_index() - 1, "");
//
//    }

    x_c->save("after_write_data.xlsx");


}

