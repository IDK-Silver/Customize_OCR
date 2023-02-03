//
// Created by idk on 2023/1/17.
//

#include "Widget_Config.h"

extern const QString Config::AppConfig::FilePath = QString::fromStdString(
        Utility::Files_Manager::absolute_path_join(
        Config::Files_Path::get_config_path(), Config::AppConfig::FileName.toStdString()
        ));

Config::AppConfig::OCRDetectWidgetConfig::OCRDetectWidgetConfig() {

    // to check file is exist
    file_generation();

    // read json string
    QString json_str;
    {
        QFile file(Config::AppConfig::FilePath);

        // check the json file read state
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            if (Config::Global::is_output_io_error_message)
            {
                std::cerr << "Can't Read the json file : " << Config::AppConfig::FilePath.toStdString() << "\n";
            }
        }
        json_str = file.readAll();
        file.close();
    }

    // to json document
    this->json_doc = QJsonDocument::fromJson(json_str.toUtf8());
}

QString Config::AppConfig::OCRDetectWidgetConfig::get_xlsx_write_mode() {
    QString mode = this->json_doc[this->Key][this->XlsxOption.Key][this->XlsxOption.WriteMode.Key].toString();
    return mode;
}

void Config::AppConfig::OCRDetectWidgetConfig::set_xlsx_write_mode(const QString& mode) {

    // if the json obj is null to creat default setting
    if (this->json_doc[this->Key].toObject().isEmpty())
    {
        if (Config::Global::is_output_log_message)
        {
            std::cout << "generation OCRDetectWidget default json.\n";
        }

        // create main json obj
        QJsonObject json_obj;

        // create widget option part
        QJsonObject widget_obj;

        // create widget - xlsx option
        QJsonObject xlsx_option_obj;

        // create option - xlsx option -write mode
        xlsx_option_obj.insert(this->XlsxOption.WriteMode.Key, this->XlsxOption.WriteMode.add);

        // insert xlsx option to widget obj
        widget_obj.insert(this->XlsxOption.Key, xlsx_option_obj);

        // insert widget option to main json
        json_obj.insert(this->Key, widget_obj);

        // set to the json doc
        this->json_doc.setObject(json_obj);
    }

    // get to root obj
    QJsonObject root_obj = this->json_doc.object();

    // find the widget ref
    QJsonValueRef widget_ref = root_obj.find(this->Key).value();
    QJsonObject widget_obj = widget_ref.toObject();

    // find xlsx option
    QJsonValueRef xlsx_option_ref = widget_obj.find(this->XlsxOption.Key).value();
    QJsonObject xlsx_option_obj = xlsx_option_ref.toObject();

    // change the xlsx write mode
    xlsx_option_obj.insert(this->XlsxOption.WriteMode.Key, mode);

    // replace json obj
    xlsx_option_ref = xlsx_option_obj;
    widget_ref = widget_obj;
    this->json_doc.setObject(root_obj);

    // open json file to write
    QFile file(Config::AppConfig::FilePath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "file error!";
        return;
    }

    // set file stream
    QTextStream out_stream(&file);

    // json document object to string and write to file
    QByteArray write_json_str = this->json_doc.toJson();
    out_stream << write_json_str;
    file.close();
}

Config::AppConfig::OCRDetectWidgetConfig::~OCRDetectWidgetConfig() {

}

void Config::AppConfig::file_generation() {


    // get the file path
    QFile file(Config::AppConfig::FilePath);

    // if file exist to not generation file
    if (Utility::Files_Manager::is_exists_file(Config::AppConfig::FilePath.toStdString()))
    {
        return;
    }

    if (Config::Global::is_output_log_message) {
        std::cout << "app config is not exists. generation blank json file.\n";
    }

    // create file
    if(!file.open(QIODevice::ReadWrite|QIODevice::Text)){
        qDebug() << "file error!";
    }

    // close file
    file.close();
}
