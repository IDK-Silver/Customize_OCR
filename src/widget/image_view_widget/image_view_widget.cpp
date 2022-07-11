//
// Created by idk on 2022/6/16.
//

#include "image_view_widget.h"
#include <QDebug>
#include <QPainter>
#include <QWheelEvent>
#include "lib/Config/Global_Config/Global_Config.h"

ImageViewLabel::ImageViewLabel(QWidget *parent) : QLabel(parent) {

}

ImageViewLabel::~ImageViewLabel() = default;


void ImageViewLabel::read_image(const QImage &image) {
    this->original_image = image;
}


void ImageViewLabel::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    switch (mouse_press_mod) {
        case MousePressEvent::MoveImage:
        {
            // 根据窗視窗計算圖片大小
            int width = qMin(original_image.width(), this->width());
            int height = int(width * 1.0 / (original_image.width() * 1.0 / original_image.height()));
            height = qMin(height, this->height());
            width = int(height * 1.0 * (original_image.width() * 1.0 / original_image.height()));

            // 移動
            painter.translate(int(this->width() / 2 + x_pos), int(this->height() / 2 + y_pos));

            // 縮放
            painter.scale(zoom_value, zoom_value);

            // 繪圖
            QRect pic_rect(-width / 2, -height / 2, width, height);
            painter.drawImage(pic_rect, this->original_image);
            break;
        }
        case MousePressEvent::PainterPen:

            int center_point_x = this->width() / 2;
            int center_point_y = this->height() / 2;


            // 圖片置中顯示
            auto match_image = [=](QImage image, int width, int height) {
                int image_width = 0, image_height = 0;
                float match_scale_width = float(width) / float(image.width());
                float match_scale_height = float(height) / float(image.height());
                float match_scale_value = qMin(match_scale_width, match_scale_height);
                image_width = image.width() * match_scale_value;
                image_height = image.height() * match_scale_value;
                return QPoint(image_width, image_height);
            };

            // 裁切縮放
            int image_zoom_size_width = this->original_image.width() * zoom_value;
            int image_zoom_size_height = this->original_image.height() * zoom_value;
            int overflow_zoom_size_width = image_zoom_size_width - this->original_image.width();
            int overflow_zoom_size_height = image_zoom_size_height - this->original_image.height();



            QRect image_crop_rect(overflow_zoom_size_width / 2 - x_pos, overflow_zoom_size_height / 2 - y_pos,
                                   original_image.width() - overflow_zoom_size_width, original_image.height() - overflow_zoom_size_height);


            QImage crop_image;
            if (image_crop_rect.width() >= 1 or image_crop_rect.height() >= 1)
                crop_image = this->original_image.copy(image_crop_rect);
            else {
                zoom_out_image();
                break;
            }

            if (Config::Global::is_output_log_message) {
                qDebug() << "Image View Info";
                qDebug() << "x pos" << x_pos << "y pos" << y_pos;
                qDebug() << "crop size" << crop_image.width() << crop_image.height();
                qDebug() << "LX-Pad" << overflow_zoom_size_width / 2 - x_pos << "RX-Pad " << original_image.width() - overflow_zoom_size_width;
                qDebug() << "LY-Pad" << overflow_zoom_size_height / 2 - y_pos<< "RY-Pad " << original_image.height() - overflow_zoom_size_height;
                qDebug() << "Overflow x " << overflow_zoom_size_width << "Overflow y " << overflow_zoom_size_height;
                qDebug() << (overflow_zoom_size_width / 2 - x_pos) + (original_image.width() - overflow_zoom_size_width);
            }


            // 消除黑邊, 把超出圖片的範圍繪製白色
            if ((overflow_zoom_size_width / 2 - x_pos) > overflow_zoom_size_width) {
                // Crop Image Width - (Left Padding - OverFlow Width)
                for (int index_width = crop_image.width() - ((overflow_zoom_size_width / 2 - x_pos) - overflow_zoom_size_width);
                     index_width < crop_image.width(); index_width++) {
                    for (int index_height = 0; index_height < crop_image.height(); index_height++)
                    {
                        QRgb value = qRgb(255, 255, 255);
                        crop_image.setPixel(index_width, index_height, value);
                    }
                }
            }

            if ((overflow_zoom_size_height / 2 - y_pos) > overflow_zoom_size_height) {
                for (int index_height = crop_image.height() - ((overflow_zoom_size_height / 2 - y_pos) - overflow_zoom_size_height);
                    index_height < crop_image.height(); index_height++) {
                    for (int index_width = 0; index_width < crop_image.width(); index_width++)
                    {
                        QRgb value = qRgb(255, 255, 255);
                        crop_image.setPixel(index_width, index_height, value);
                    }
                }
            }


            if ((overflow_zoom_size_width / 2 - x_pos) < 0) {
                for (int index_width = 0; index_width < abs(overflow_zoom_size_width / 2 - x_pos); index_width++) {
                    for (int index_height = 0; index_height < crop_image.height(); index_height++)
                    {
                        QRgb value = qRgb(255, 255, 255);
                        crop_image.setPixel(index_width, index_height, value);
                    }
                }
            }


            if ((overflow_zoom_size_height / 2 - y_pos) < 0) {
                for (int index_height = 0; index_height < abs(overflow_zoom_size_height / 2 - y_pos); index_height++) {
                    for (int index_width = 0; index_width < crop_image.width(); index_width++)
                    {
                        QRgb value = qRgb(255, 255, 255);
                        crop_image.setPixel(index_width, index_height, value);
                    }
                }
            }

            // 繪製時預留邊寬, 讓圖片可以置中顯示

            int image_padding_width = (this->width() - match_image(crop_image, this->width(), this->height()).x()) / 2;
            int image_padding_height = (this->height() - match_image(crop_image, this->width(), this->height()).y()) / 2;
            QRect image_rect(image_padding_width, image_padding_height,
                             match_image(crop_image, this->width(), this->height()).x(),
                             match_image(crop_image, this->width(), this->height()).y());

            painter.drawImage(image_rect, crop_image);





            //   繪製選取框
            float match_scale_value = match_image(crop_image, this->width(), this->height()).x() / float(crop_image.width());
            QPoint rect_l((first_press_pos.x() - image_padding_width) / match_scale_value + overflow_zoom_size_width / 2 - x_pos,
                          (first_press_pos.y() - image_padding_height) / match_scale_value + overflow_zoom_size_height / 2 - y_pos) ;
            QPoint rect_r((second_press_pos.x() - image_padding_width) / match_scale_value + overflow_zoom_size_width / 2 - x_pos,
                          (second_press_pos.y() - image_padding_height) / match_scale_value + overflow_zoom_size_height / 2 - y_pos);


            auto check_select_range = [] (QPoint& point, QImage image) {
                if (point.x() > image.width())
                    point.setX(image.width());
                else if (point.x() < 0)
                    point.setX(0);

                if (point.y() > image.height())
                    point.setY(image.height());
                else if (point.y() < 0)
                    point.setY(0);
            };


            check_select_range(rect_l, original_image);
            check_select_range(rect_r, original_image);



            QRect select_rect(rect_l, rect_r);

            if (Config::Global::is_output_log_message) {
                qDebug() << match_scale_value << select_rect;
            }

            QImage save_image = original_image.copy(select_rect);
            save_image.save("C:\\Users\\idk\\Desktop\\crop_save.jpg");

            painter.drawRect(QRect(first_press_pos, second_press_pos));



            break;
    }
}


void ImageViewLabel::wheelEvent(QWheelEvent *event) {
    int value = event->angleDelta().y();
    first_press_pos = QPoint();
    second_press_pos = QPoint();
    if (value > 0)  // 放大
        zoom_in_image();
    else            // 縮小
        zoom_out_image();
}


void ImageViewLabel::mousePressEvent(QMouseEvent *event) {
    switch (this->mouse_press_mod) {

        case MousePressEvent::MoveImage :
            old_pos = event->pos();
            is_pressed = true;
            break;

        case MousePressEvent::PainterPen :
            first_press_pos = event->pos();
            second_press_pos = event->pos();
            is_pressed = true;
            break;
    }
}


void ImageViewLabel::mouseMoveEvent(QMouseEvent *event) {

    if (!is_pressed)
        return QWidget::mouseMoveEvent(event);

    switch (this->mouse_press_mod) {

        case MousePressEvent::MoveImage : {
            this->setCursor(Qt::SizeAllCursor);
            QPoint pos = event->pos();
            int xPtInterval = pos.x() - old_pos.x();
            int yPtInterval = pos.y() - old_pos.y();
            x_pos += xPtInterval;
            y_pos += yPtInterval;
            old_pos = pos;
            break;
        }

        case MousePressEvent::PainterPen:
            second_press_pos = event->pos();
            break;
    }
    update();
//    paintEvent(nullptr);
}

//滑鼠發射事件
void ImageViewLabel::mouseReleaseEvent(QMouseEvent */*event*/) {
    is_pressed = false;
    switch (mouse_press_mod) {

        case MousePressEvent::MoveImage:
            setCursor(Qt::ArrowCursor);
            break;

        case MousePressEvent::PainterPen:
            QRect select_reac = getReac(first_press_pos, second_press_pos);
            break;
    }
}

//圖片放大
void ImageViewLabel::zoom_in_image() {
    zoom_value += 0.015;
    update();
}

//圖片縮小
void ImageViewLabel::zoom_out_image() {
    zoom_value -= 0.015;
    if (zoom_value <= 0) {
        zoom_value = 0.1;
        return;
    }

    update();
}

//圖片原始大小
void ImageViewLabel::on_preset_image() {
    zoom_value = 1.0;
    x_pos = 0;
    y_pos = 0;
    update();
}

void ImageViewLabel::set_mouse_press_mode(int mode) {

}

QRect ImageViewLabel::getReac(QPoint first_pos, QPoint second_pos) {
    int x = 0, y = 0, width = 0, height = 0;
    width = qAbs(first_pos.x() - second_pos.x());
    height = qAbs((first_pos.y() - first_pos.y()));
    x = fmin(first_pos.x(), second_pos.x());
    y = fmin(first_pos.y(), second_pos.y());

    auto select_reac = QRect(x, y, width, height);

    if (select_reac.width() == 0) {
        select_reac.setWidth(1);
    }

    if (select_reac.height() == 0) {
        select_reac.setHeight(0);
    }


    return select_reac;
}


