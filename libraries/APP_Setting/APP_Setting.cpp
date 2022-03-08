#include "APP_Setting.h"

#pragma execution_character_set("utf-8")


APP_Setting::APP_Setting()
{   
    generate_default();
}

APP_Setting::~APP_Setting()
{
}

void APP_Setting::generate_default()
{
    QJsonObject main_json;


    QJsonObject types;

    QJsonObject type_data;

    QJsonArray home_crop_addres = { 540, 227, 785, 275 };

    APP_Setting_Key key_word;

    type_data.insert(key_word.types.type.home_crop_addres, home_crop_addres);

    QJsonArray phone_crop_addres = { 1, 2, 3, 4 };
    type_data.insert(key_word.types.type.phone_crop_addres, phone_crop_addres);

    QJsonArray name_crop_addres = { 540, 227, 785, 275 };
    type_data.insert(key_word.types.type.name_crop_addres, name_crop_addres);

    QJsonArray check_list;

    QJsonObject check_data;
    QJsonArray check_crop_addres = {335, 150, 435, 205};
    check_data.insert(key_word.types.type.check.data.check_crop_addres, check_crop_addres);

    QString check_string = "None";
    check_data.insert(key_word.types.type.check.data.check_string, check_string);

    check_list.append(check_data);
    type_data.insert(key_word.types.type.check.key, check_list);


    QJsonArray vector_list;

    QJsonObject vector_data;

    QJsonArray vector_crop_addres = {55, 3075, 245, 3135};
    vector_data.insert(key_word.types.type.vector.data.vector_crop_addres, vector_crop_addres);

    QString vector_string = "¤É¯d¯Å";
    vector_data.insert(key_word.types.type.vector.data.vector_string, vector_string);

    QString vector_orientation = orientation_vertical;
    vector_data.insert(key_word.types.type.vector.data.image_orientation, vector_orientation);

    vector_list.append(vector_data);
    type_data.insert(key_word.types.type.vector.key, vector_list);



    types.insert("91_t2", type_data);



    main_json.insert(key_word.types.key, types);


    QJsonDocument jsonDoc(main_json);
    QByteArray ba = jsonDoc.toJson();
    QFile file("result.json");
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "write json file failed";
    }
    file.write(ba);
    file.close();

}

void APP_Setting::load_file(QString file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "read json file failed";
    }
    QByteArray ba = file.readAll();
    qDebug() << ba;
    QJsonParseError e;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(ba, &e);
    if (e.error == QJsonParseError::NoError && !jsonDoc.isNull())
    {
        qDebug() << jsonDoc;
    }

    this->json_file_path = file_path;
    this->json_file = jsonDoc.object();
}

QJsonValue APP_Setting::get_obj(QString key)
{
    return this->json_file.value(key);
}

QJsonValue APP_Setting::get_obj()
{
    return this->json_file;
}
