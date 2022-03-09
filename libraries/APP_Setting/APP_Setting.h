#ifndef APP_Setting_LIBS
#define APP_Setting_LIBS

#include <qjsonarray.h>
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonvalue.h>
#include <qfile.h>

#include <vector>

#define SHOW_DEBUG_IMAGE false

#define SETTING_FILENAME "result.json"
#define OCR_FOLDER_PATH "ocr_folder"


#define orientation_horizontal "orientation_horizontal"
#define orientation_vertical "orientation_vertical"


struct App_Data
{
    QString excel_file_path = nullptr;
};

struct APP_Setting_Key
{
    struct types_obj
    {
        QString key = "types";

        QString type_name = nullptr;

        struct type_struct
        {
            QString name_crop_addres = "name_crop_addres";
            QString phone_crop_addres = "phone_crop_addres";
            QString home_crop_addres = "home_crop_addres";

            struct check_struct
            {
                QString key = "check_data";

                struct check_data
                {
                    QString check_string = "check_name";
                    QString check_crop_addres = "check_crop_addres";
                };

                check_data data;

            };
            check_struct check;


            struct vector_struct
            {
                QString key = "vector_data";

                struct vector_data
                {
                    QString vector_string = "vector_name";
                    QString vector_crop_addres = "vector_crop_addres";
                    QString image_orientation = "vector_orientation";
                };

                vector_data data;

            };
            vector_struct vector;

        };
        type_struct type;

    };
    types_obj types;


    struct excel_obj
    {
        QString key = "excel";
    };
    excel_obj excel;
};


class APP_Setting
{
public:
	APP_Setting();
	~APP_Setting();

	void generate_default();
	
	void load_file(QString file_path);

	QJsonValue get_obj(QString Key);
	QJsonValue get_obj();

private:
	QJsonObject json_file;
	QString json_file_path;
	std::vector<QString> object_path;
};




#endif // !APP_Setting
