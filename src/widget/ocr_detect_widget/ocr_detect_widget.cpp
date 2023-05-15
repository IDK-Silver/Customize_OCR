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

    this->widget_init();
}

OCR_Detect_Widget::~OCR_Detect_Widget() = default;

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
    x_c->file_path = choose_file.toStdString();
    QMessageBox::information(nullptr, "開啟XLSX檔案成功", "以載入XLSX");
}


void OCR_Detect_Widget::connect_setup() {
    connect(this->ui->pb_load_setting_file, SIGNAL(clicked()), SLOT(load_setting_file()));
    connect(this->ui->pb_star_ocr, SIGNAL(clicked()), SLOT(ocr_image()));
    connect(this->ui->pb_choose_image, SIGNAL(clicked()), SLOT(choose_images()));
    connect(this->ui->pb_write_xlsx, SIGNAL(clicked()), SLOT(write_xlsx_data()));
    connect(this->ui->cm_write_mode, SIGNAL(currentIndexChanged(int)), SLOT(xlsx_write_mode_change(int)));
}

void OCR_Detect_Widget::choose_images() {
    QStringList choose_files = QFileDialog::getOpenFileNames(this, QString("選擇圖片"),
                                                    QString::fromStdString(Config::Files_Path::get_system_picture_path()),
                                                    QString("圖片 (*.jpg *.png *tif)"));

    this->wait_ocr_path_list = choose_files;
}

void OCR_Detect_Widget::write_xlsx_data() {

    /* if noting to write then pass */
    if (this->ui->listWidget->size() == 0)
        return;

    // load the widget config to read the xlsx write mode
    Config::AppConfig::OCRDetectWidgetConfig widget_config;

    /* get data form list widget */
    if (widget_config.get_xlsx_write_mode() == widget_config.XlsxOption.WriteMode.add)
    {
        for (int data_widget_index = 0; data_widget_index < this->ui->listWidget->size(); data_widget_index++)
        {
            /* load the data widget from list widget item */
            auto data_widget = this->ui->listWidget->at(data_widget_index);

            /* the get each data from data widget */
            OCR_Data ocr_data;
            ocr_data.ocr_text = data_widget->ui->ocr_text_lineEdit->text();
            ocr_data.tag = data_widget->ui->tag_lab->text();
            ocr_data.xlsx_cell = data_widget->ui->xlsx_cell_lab->text();

            /* conversion xlsx to program index  */
            xlnt::cell_reference ce(fuzzy_reference(ocr_data.xlsx_cell.toLocal8Bit().toStdString()));

            /* to print the write data info */
            if (Config::Global::is_output_log_message)
            {
                qDebug() << "Write Xlsx Data at : ";
                qDebug() << "Col   : " << ce.column().index - 1;
                qDebug() << "ROW   : " << this->x_c->max_row_cursor;
                qDebug() << "Value : " << ocr_data.ocr_text;
            }

            /* to write the data to xlsx data */
            this->x_c->write_data(ce.column_index() - 1, this->x_c->max_row_cursor, ocr_data.ocr_text.toStdString());
        }
        this->x_c->max_row_cursor++;
    }
    else if (widget_config.get_xlsx_write_mode() == widget_config.XlsxOption.WriteMode.replace)
    {
        for (int data_widget_index = 0; data_widget_index < this->ui->listWidget->size(); data_widget_index++)
        {
            /* load the data widget from list widget item */
            auto data_widget = this->ui->listWidget->at(data_widget_index);

            /* the get each data from data widget */
            OCR_Data ocr_data;
            ocr_data.ocr_text = data_widget->ui->ocr_text_lineEdit->text();
            ocr_data.tag = data_widget->ui->tag_lab->text();
            ocr_data.xlsx_cell = data_widget->ui->xlsx_cell_lab->text();


        }
    }

    if (Config::Global::is_output_log_message)
        qDebug() << "write end.";

    x_c->save(x_c->file_path);
}

void OCR_Detect_Widget::widget_init() {
    // read config to change widget object option
    Config::AppConfig::OCRDetectWidgetConfig config;
    if (config.get_xlsx_write_mode() == config.XlsxOption.WriteMode.add)
    {
        this->ui->cm_write_mode->setCurrentIndex(0);
    }
//    else if (config.get_xlsx_write_mode() == config.XlsxOption.WriteMode.replace)
//    {
//        this->ui->cm_write_mode->setCurrentIndex(1);
//    }
    else
    {
        if (Config::Global::is_output_io_error_message)
        {
            std::cerr << "Can't read config setting - Xlsx Option - Unknown" << std::endl;
        }
        this->ui->cm_write_mode->setCurrentIndex(0);
        config.set_xlsx_write_mode(config.XlsxOption.WriteMode.add);
    }
}

void OCR_Detect_Widget::xlsx_write_mode_change(int index) {

    // print the change index
    if (Config::Global::is_output_log_message)
    {
        qDebug() << "Change Xlsx write mode to index : "<< index;
    }

    Config::AppConfig::OCRDetectWidgetConfig config;
    QString select_mode;
    switch (index) {
        case 0:
            select_mode = config.XlsxOption.WriteMode.add;
            break;

        case 1:
            select_mode = config.XlsxOption.WriteMode.replace;
            break;

        default:
            break;
    }

    config.set_xlsx_write_mode(select_mode);
}

