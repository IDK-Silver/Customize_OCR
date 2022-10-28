#ifndef OCR_DETECT_WIDGET_H
#define OCR_DETECT_WIDGET_H

#include <QWidget>
#include "data_struct/ocr_data_struct.h"
#include "src/widget/ocr_format_create_widget/data_struct/ocr_format_struct.h"

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



    

private:
    std::vector<std::shared_ptr<OCR_Format_Data>> ocr_format_data_list;

    std::vector<QString> wait_ocr_path_list;

    void add_list_widget_ocr_data(const OCR_Display_Data &data);




private slots:
    void load_setting_file();
    void ocr_image();


};


#endif // OCR_DETECT_WIDGET_H
