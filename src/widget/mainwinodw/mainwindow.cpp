#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <qfiledialog.h>

#include "libraries/OCR_Image/OCR_Image.h"
#include "libraries/ImageUtility/ImageUtility.h"
#include <qjsonarray.h>
#include <qjsonobject.h>

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
    
    connect(this->ui->pushButton_start, SIGNAL(pressed()), SLOT(start_ocr()));

    // open file 
    connect(this->ui->actionOpenFiles, &QAction::triggered, this, &MainWindow::openfile);




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

void MainWindow::start_ocr()
{

    // get ocr foldr file list
    QStringList file_paths = get_ocr_files_path(OCR_FOLDER_PATH);

    // check folder have file
    QString file_path;
    if (file_paths.isEmpty())
    {
        qDebug() << "No file, foldr is empty";
        return;
    }
    else
        // set file path is first file path
        file_path = file_paths.at(0);

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
            QString ocr_vector_string = QString::fromStdString(ocr_image.ocr_part_image(vector_crop_image, false));


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
                    QString ocr_check_string = QString::fromStdString(ocr_image.ocr_part_image(check_crop_image, false));
                }

                return false;
            };

            auto ocr_part_image = [&]() {

                std::vector<std::string> ocr_strs;

                std::vector<int> name_crop;
                auto name_obj = type.value(key_word.types.type.name_crop_addres).toArray().toVariantList();
                for (auto pointer : name_obj) { name_crop.push_back(pointer.toInt()); }

                std::vector<int> phone_crop;
                auto phone_obj = type.value(key_word.types.type.phone_crop_addres).toArray().toVariantList();
                for (auto pointer : phone_obj) { phone_crop.push_back(pointer.toInt()); }

                std::vector<int> home_crop;
                auto home_obj = type.value(key_word.types.type.home_crop_addres).toArray().toVariantList();
                for (auto pointer : home_obj) { home_crop.push_back(pointer.toInt()); }
            
                cv::Mat part_image;

                for (auto i : name_crop)
                    qDebug() << i;

                part_image = ImageUtility::crop(src_image, name_crop);
                cv::imshow("123", part_image);
                cv::waitKey();
                auto i = QString::fromStdString(ocr_image.ocr_part_image(part_image, true));
                
               
                


                return ocr_strs;
            };

            // OCR_Part name phone home
            // sec step
            // rotate image to true orientation
            if (vector_check_string == ocr_vector_string)
            {
                ocr_part_image();
            }
            else
            {
                src_image = ImageUtility::rotate(src_image, 180);
                vector_crop_image = ImageUtility::crop(src_image, vector_crop);;
                ocr_vector_string = QString::fromStdString(ocr_image.ocr_part_image(vector_crop_image, false));
                if (vector_check_string == ocr_vector_string)
                {
                    ocr_part_image();
                }
                else
                    // to next type
                    continue;
            }
            
            
            
            

            
        }





        // std::vector<int> crop = QVector<int>(crop_list.begin(), crop_list.end()).toStdVector();


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


QString MainWindow::openfile()
{
    auto file_path = QFileDialog::getOpenFileName(this, "¿ï¨úExcelÀÉ", QDir::homePath(), "ExcelÀÉ (*.xlsx)");
    this->ocr_data.excel_file_path = file_path;
    return file_path;
}


