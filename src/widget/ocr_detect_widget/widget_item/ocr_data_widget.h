#ifndef OCR_DATA_WIDGET_H
#define OCR_DATA_WIDGET_H

#include <QWidget>

namespace Ui {
class OCR_Data_Widget;
}

class OCR_Data_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit OCR_Data_Widget(QWidget *parent = nullptr);
    ~OCR_Data_Widget();
    void change_lable();

private:
    Ui::OCR_Data_Widget *ui;
};

#endif // OCR_DATA_WIDGET_H
