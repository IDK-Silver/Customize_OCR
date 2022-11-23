#ifndef OCR_DETECT_WIDGET_H
#define OCR_DETECT_WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QDebug>
#include <memory>
#include "data_struct/ocr_data_struct.h"
#include "src/widget/ocr_format_create_widget/data_struct/ocr_format_struct.h"
#include <OCR/ImageOCR.h>
#include <XlntControl/XlntControl.h>
#include "widget_item/ocr_data_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class OCR_Detect_Widget; }
QT_END_NAMESPACE

class OCR_Detect_Widget : public QWidget
{
    Q_OBJECT

public:
    QString widget_name = "OCR Detect Widget";

    explicit OCR_Detect_Widget(QWidget *parent = nullptr);
    ~OCR_Detect_Widget() override;

    Ui::OCR_Detect_Widget *ui{};


    void action_open_file();
    void action_open_xlsx_file();


private:
    void connect_setup();

    std::unique_ptr<ImageOCR> ocr_processing;
    std::vector<std::shared_ptr<OCR_Format_Data>> ocr_format_data_list;
    std::vector<std::shared_ptr<OCR_Data_Widget>> ocr_data_widget_list;
    QStringList wait_ocr_path_list;
    std::shared_ptr<XlntControl> x_c;


    void add_list_widget_ocr_data(const OCR_Display_Data &data);




private slots:
    void load_setting_file();
    void ocr_image();
    void choose_images();
    void write_xlsx_data();


};


#endif // OCR_DETECT_WIDGET_H
