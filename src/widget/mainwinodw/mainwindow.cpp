#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <qfiledialog.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qmap.h>
#include <qvector.h>
#include <qpixmap.h>
#include <qxmlstream.h>

#include "libraries/QXlsx/header/xlsxdocument.h"
#include "libraries/OCR_Image/OCR_Image.h"
#include "libraries/ImageUtility/ImageUtility.h"
#include "libraries/ExcelWrite/ExcelWrite.h"

#include <iostream>
#include <qdebug.h>

#pragma execution_character_set("utf-8")


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    system("CHCP 65001");

    // connect
    
    // ocr imgage
    connect(this->ui->pushButton_start, SIGNAL(pressed()), SLOT(start_ocr()));

    // open file 
    connect(this->ui->actionOpenFiles, &QAction::triggered, this, &MainWindow::openfile);;

    // data is right
    connect(this->ui->pushButton_yes, &QPushButton::pressed, this, &MainWindow::data_right);

    // data is error
    connect(this->ui->pushButton_no, &QPushButton::pressed, this, &MainWindow::data_error);

    // change mode
    connect(this->ui->checkBox_auto_write2excel, &QCheckBox::stateChanged, [&] () {
        this->app_data.auto_write_mode = this->ui->checkBox_auto_write2excel->isChecked();
            qDebug() << QString("Auto Write State Changed, Now is %1").arg((this->app_data.auto_write_mode == true) ? "Enable": "Disable");
     });





}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList MainWindow::get_ocr_files_path(QString ocr_folder_path)
{
    QDir dir;
    dir.setPath(ocr_folder_path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    QStringList files_path;
    QFileInfoList list = dir.entryInfoList();
   
    for (int i = 0; i < list.size(); i++) {
        QFileInfo fileInfo = list.at(i).absoluteFilePath();
        qDebug() << dir.absoluteFilePath(fileInfo.fileName()).toLocal8Bit();
        files_path.push_back(dir.absoluteFilePath(fileInfo.fileName()).toLocal8Bit());
    }
    return files_path;
}

bool MainWindow::move_to_folder(QString file_path, QString folder)
{
    QFile file(file_path);
    QDir move_path(folder);
    
    QString file_name = file.fileName().split("/").at(file.fileName().split("/").size() - 1);
    
    if (SHOW_DEBUG_IMAGE)
        qDebug() << move_path.absoluteFilePath(file_name);
  
    return file.rename(move_path.absoluteFilePath(file_name));
}

void MainWindow::clear_ui()
{
    this->ui->label_text_filename->clear();
    this->ui->lineEdit_text_home->clear();
    this->ui->lineEdit_text_name->clear();
    this->ui->lineEdit_text_phone->clear();
    this->ui->label_image_home->clear();
    this->ui->label_image_name->clear();
    this->ui->label_image_phone->clear();
}

void MainWindow::start_ocr()
{

    // get ocr foldr file list
    QStringList file_paths = get_ocr_files_path(OCR_FOLDER_PATH);

    // check folder have file
    QString file_path;

    if (file_paths.isEmpty())
    {
        qDebug() << "No file, foldr is empty";
        this->app_data.process_file_path = nullptr;
        return;
    }
    else
    {
        // set file path is first file path
        file_path = file_paths.at(0);
        this->app_data.process_file_path = file_path;
        this->ui->label_text_filename->setText(file_path.split("/").at(file_path.split("/").size() - 1).split(".").at(0));
    }

    // load src image
    auto src_image = cv::imread(file_path.toLocal8Bit().toStdString());

    // load setting file
    APP_Setting_Key key_word;
    APP_Setting setting;
    setting.load_file(SETTING_FILENAME);
    
    // get root json object
    QJsonObject root_json =  setting.get_obj().toObject();
    
    // get type json object
    QJsonObject type_json = root_json.value(key_word.types.key).toObject();


    // to set image type 
    QString image_type;

    // read all type
    for (auto type_name : type_json.keys())
    {
        // read type
        QJsonObject type = type_json.value(type_name).toObject();

        // read vector_list
        auto vector_list = type.value(key_word.types.type.vector.key).toArray();

        // read vector
        for (auto vector : vector_list)
        {
            OCR_Config ocr_config;
            // rotary src image
            // image orientation
            QString orientation_mode = vector.toObject().value(key_word.types.type.vector.data.image_orientation).toString();

            // read vector crop data
            // covnersion vector crop data from JsonArray to std::vector<int>
            std::vector<int> vector_crop;
            auto vector_crop_VariantList = vector.toObject().value(key_word.types.type.vector.data.vector_crop_addres).toArray().toVariantList();

            // read vector check string
            // covnersion vector crop data from JsonArray to std::vector<int>
            QString vector_check_string = vector.toObject().value(key_word.types.type.vector.data.vector_string).toString();

            for (auto variant : vector_crop_VariantList)
            {
                vector_crop.push_back(variant.toInt());
            }

            // first step
            // rotate image to true orientation
            if (orientation_mode == orientation_horizontal && !ImageUtility::is_horizontal(src_image))
                src_image = ImageUtility::rotate(src_image, -90);
            if (orientation_mode == orientation_vertical && !ImageUtility::is_vertical(src_image))
                src_image = ImageUtility::rotate(src_image, -90);


            // crop vector image
            cv::Mat vector_crop_image = ImageUtility::crop(src_image, vector_crop);

            // ocr crop vector image
            QString ocr_vector_string = QString::fromStdString(ocr_image.ocr_part_image(vector_crop_image, false, ocr_config.rec_model_dir));


            auto is_image_type_true = [&] () {
                auto check_list = type.value(key_word.types.type.check.key).toArray();
                for (auto check : check_list)
                {
                    // read check crop data
                    // covnersion check crop data from JsonArray to std::vector<int>
                    std::vector<int> check_crop;
                    auto check_crop_VariantList = check.toObject().value(key_word.types.type.check.data.check_crop_addres).toArray().toVariantList();

                    // read check check string
                    // covnersion check crop data from JsonArray to std::vector<int>
                    QString check_check_string = check.toObject().value(key_word.types.type.check.data.check_string).toString();

                    for (auto variant : check_crop_VariantList)
                    {
                        check_crop.push_back(variant.toInt());
                    }

                    // crop vector image
                    cv::Mat check_crop_image = ImageUtility::crop(src_image, check_crop);

                    // ocr crop vector image
                    QString ocr_check_string = QString::fromStdString(ocr_image.ocr_part_image(check_crop_image, true, ocr_config.rec_model_dir));
                }

                return false;
            };

            auto ocr_part = [] (QJsonObject& type, QString& key, OCR_Image& ocr_image,  cv::Mat& src_image, bool is_num_image) {

                OCR_Data data;
                OCR_Config orc_config;
                std::vector<int> crop;
                auto obj = type.value(key).toArray().toVariantList();
                for (auto pointer : obj) { crop.push_back(pointer.toInt()); }

                cv::Mat part_image = ImageUtility::crop(src_image, crop);
                cv::Mat det_image;
                cv::copyMakeBorder(part_image, det_image, 15, 15, 15, 15, cv::BORDER_CONSTANT, cv::Scalar(255, 255, 255));
                if (SHOW_DEBUG_IMAGE)
                {
                    cv::imshow("", det_image);
                    cv::waitKey();
                }
                QString string;
                if (is_num_image)
                    string = QString::fromStdString(ocr_image.ocr_part_image(det_image, true, orc_config.en_rec_model_dir));
                else 
                    string = QString::fromStdString(ocr_image.ocr_part_image(det_image, true, orc_config.rec_model_dir));


                data.image = part_image;
                data.string = string.toStdString();
                return data;
            };

            auto updata_ui = [&]() {
                auto name_data = ocr_part(type, key_word.types.type.name_crop_addres, ocr_image, src_image, false);
                auto home_data = ocr_part(type, key_word.types.type.home_crop_addres, ocr_image, src_image, false);
                auto photo_data = ocr_part(type, key_word.types.type.phone_crop_addres, ocr_image, src_image, false);

                ui->lineEdit_text_home->setText(QString::fromStdString(home_data.string));
                ui->lineEdit_text_phone->setText(QString::fromStdString(photo_data.string));
                ui->lineEdit_text_name->setText(QString::fromStdString(name_data.string));

                ui->label_image_home->setScaledContents(true);
                ui->label_image_home->setPixmap(ocr_image.Mat2QPixmap(home_data.image));

                ui->label_image_phone->setPixmap(ocr_image.Mat2QPixmap(photo_data.image));
                ui->label_image_name->setPixmap(ocr_image.Mat2QPixmap(name_data.image));
            };

            // OCR_Part name phone home
            // sec step
            // rotate image to true orientation
            if (vector_check_string == ocr_vector_string)
            {
                updata_ui();
            }
            else
            {
                src_image = ImageUtility::rotate(src_image, 180);
                vector_crop_image = ImageUtility::crop(src_image, vector_crop);;
                ocr_vector_string = QString::fromStdString(ocr_image.ocr_part_image(vector_crop_image, false, ocr_config.rec_model_dir));
                if (vector_check_string == ocr_vector_string)
                {
                    updata_ui();
                }
                else
                    // to next type
                    continue;
            }
        }
    }
  
}

void MainWindow::OCR_Test()
{
    QString image_path = QFileDialog::getOpenFileName(this, "Test Image", QDir::homePath(), "Image File (*.png *.TIF)");

    system("CHCP 65001");
    ocr_image.load_image(image_path);

    auto stack_str = ocr_image.ocr_src_image();

    std::cout << ocr_image.merge_src_string(stack_str);
}

void MainWindow::data_right()
{
    if (this->app_data.auto_write_mode)
    {
        excel_write::ExcelDoc excel_doc(this->app_data.excel_file_path);

        // load setting file
        APP_Setting_Key key_word;
        APP_Setting setting;
        setting.load_file(SETTING_FILENAME);

        // get root json object
        QJsonObject root_json = setting.get_obj().toObject();

        // get type json object
        QJsonObject xlsx_obj = root_json.value(key_word.xlsx.key).toObject();

        //auto name_col = xlsx_obj.value(key_word.xlsx.data.name_col).toString();
        // excel_doc.search(excel_write::col_en2num(name_col), this->ui->lineEdit_text_name->text());



    }

    bool is_move_success = MainWindow::move_to_folder(this->app_data.process_file_path, QString(SUCCESS_IMAGE_PATH));

    if (is_move_success)
        this->clear_ui();
}

void MainWindow::data_error()
{
    bool is_move_success = MainWindow::move_to_folder(this->app_data.process_file_path, QString(FAILED_IMAGE_PATH));
    if (is_move_success)
        this->clear_ui();
}


QString MainWindow::openfile()
{
    auto file_path = QFileDialog::getOpenFileName(this, "¿ï¨úExcelÀÉ", QDir::homePath(), "ExcelÀÉ (*.xlsx)");
    this->app_data.excel_file_path = file_path;
    return file_path;
}


