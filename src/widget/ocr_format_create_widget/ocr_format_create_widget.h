#ifndef OCR_FORMAT_CREATE_WIDGET_H
#define OCR_FORMAT_CREATE_WIDGET_H

#include <QWidget>
#include <QMessageBox>
#include <QtWidgets/QFileDialog>
#include <src/struct/OCR_Data/OCR_Data_ListWidget.h>
#include <memory>
#include <vector>
#include <Utility/Files_Manager/Files_Manager.h>

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

    struct Status {
        bool image_range = false;
        bool load_file = false;
        bool load_image = false;
        int current_format_index = 0;
        bool is_crop = false;
        bool is_create_cb = false;
    };
    Status status;

    std::shared_ptr<OCR_Data> current_format;
    OCR_Data_List format_datas;

    void widget_init();
//    void widget_refresh(const OCR_Data data);
    void widget_refresh();

    void crop_label_refresh(QRect input_rect) const;

//    void tag_choose_refresh();
//    void change_image_crop_label(QRect input_rect) const;


public slots:

    // loading format from file
    void load_file();

    // add new format file
    void new_file();

    // save format file
    void save_file();

    // select, clear image crop range
    void crop_range();
    void clear_crop_range();

    // add tag
    void add_tag();

    // cb select
    void cb_tag_select(int);

    // le tag name change
    void le_tag_name_change(const QString &);

    void cb_is_search(bool);

//    void choose_image_crop_range();
//    void clear_image_crop_range();
//    void cb_tag_choose_index_change(int index);
//    void le_tag_name_change(const QString & text) const;
};

#endif // OCR_FORMAT_CREATE_WIDGET_H
