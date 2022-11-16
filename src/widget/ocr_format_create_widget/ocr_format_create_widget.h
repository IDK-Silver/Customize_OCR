#ifndef OCR_FORMAT_CREATE_WIDGET_H
#define OCR_FORMAT_CREATE_WIDGET_H


#include <QWidget>
#include <QtWidgets/QFileDialog>
#include <src/widget/ocr_format_create_widget/data_struct/ocr_format_struct.h>
#include <memory>
#include <vector>

namespace Ui {
class OCR_Format_Create_Widget;
}

class OCR_Format_Create_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit OCR_Format_Create_Widget(QWidget *parent = nullptr);
    ~OCR_Format_Create_Widget() override;

    Ui::OCR_Format_Create_Widget *ui;
    QString widget_name = "Format Create Widget";



private:

    bool is_crop_image_range = false;
    bool is_import_image = false;


    std::shared_ptr<OCR_Format_Data> now_edit_format;
    std::vector<std::shared_ptr<OCR_Format_Data>> format_list;
    int now_edit_format_index = 0;

    [[nodiscard]] QRect get_crop_image_rect() const;


    void widget_init();

    void tag_choose_refresh();
    void change_image_crop_label(QRect input_rect) const;


public slots:
    void load_setting_file();
    void add_tag();
    void add_image();
    void choose_image_crop_range();
    void clear_image_crop_range();
    void cb_tag_choose_index_change(int index);
    void le_tag_name_change(const QString & text) const;
};

#endif // OCR_FORMAT_CREATE_WIDGET_H
