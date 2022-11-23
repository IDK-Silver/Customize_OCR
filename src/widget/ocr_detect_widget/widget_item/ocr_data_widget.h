#ifndef OCR_DATA_WIDGET_H
#define OCR_DATA_WIDGET_H

#include <QWidget>
#include <QString>
#include <QImage>
#include <QPixmap>

namespace Ui {
class OCR_Data_Widget;
}

class OCR_Data_Widget : public QWidget
{
    Q_OBJECT

public:
    explicit OCR_Data_Widget(QWidget *parent = nullptr);
    ~OCR_Data_Widget();

    void change_tag(const QString &text);
    void change_crop_image(const QImage &crop_image);
    void change_crop_image(const QPixmap &crop_image);
    void change_ocr_text(const QString &text);
    void change_xlsx_text(const QString &text);

    QString get_xlsx_text();
    QString get_tag();
    QString get_ocr_text();

private:
    Ui::OCR_Data_Widget *ui;
};

#endif // OCR_DATA_WIDGET_H
