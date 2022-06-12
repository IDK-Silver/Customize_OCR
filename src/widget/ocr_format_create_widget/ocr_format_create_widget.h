#ifndef OCR_FORMAT_CREATE_WIDGET_H
#define OCR_FORMAT_CREATE_WIDGET_H

#include <QWidget>

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

};

#endif // OCR_FORMAT_CREATE_WIDGET_H
