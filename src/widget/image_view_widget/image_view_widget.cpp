//
// Created by idk on 2022/6/16.
//

#include "image_view_widget.h"
#include <QDebug>
#include <QPainter>
#include <QWheelEvent>

ImageViewLabel::ImageViewLabel(QWidget *parent) : QLabel(parent) {

}


ImageViewLabel::~ImageViewLabel() = default;


void ImageViewLabel::read_image(const QImage &image) {
    this->original_image = image;
}


void ImageViewLabel::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    // 根据窗視窗計算圖片大小
    int width = qMin(original_image.width(), this->width());
    int height = int(width * 1.0 / (original_image.width() * 1.0 / original_image.height()));
    height = qMin(height, this->height());
    width = int(height * 1.0 * (original_image.width() * 1.0 / original_image.height()));

    // 移動
    painter.translate(int(this->width() / 2 + x_pos), int(this->height() / 2 + y_pos));

    // 縮放
    painter.scale(zoom_value, zoom_value);

    // ?制
    QRect pic_rect(-width / 2, -height / 2, width, height);
    painter.drawImage(pic_rect, this->original_image);
}


void ImageViewLabel::wheelEvent(QWheelEvent *event) {
    int value = event->angleDelta().y();

    if (value > 0)  //放大
        zoom_in_image();
    else            //?小
        zoom_out_image();
}


void ImageViewLabel::mousePressEvent(QMouseEvent *event) {
    switch (this->mouse_press_mod) {

        case MousePressEvent::MoveImage :
            old_pos = event->pos();
            is_pressed = true;
            break;

        case MousePressEvent::PainterPen :
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
            update();
            break;
        }

        case MousePressEvent::PainterPen:
            break;
    }
//    paintEvent(nullptr);
}

//鼠??射事件
void ImageViewLabel::mouseReleaseEvent(QMouseEvent */*event*/) {
    is_pressed = false;
    setCursor(Qt::ArrowCursor);
}

//圖片放大
void ImageViewLabel::zoom_in_image() {
    zoom_value += 0.2;
    update();
}

//圖片?小
void ImageViewLabel::zoom_out_image() {
    zoom_value -= 0.2;
    if (zoom_value <= 0) {
        zoom_value = 0.1;
        return;
    }

    update();
}

//圖片?原
void ImageViewLabel::on_preset_image() {
    zoom_value = 1.0;
    x_pos = 0;
    y_pos = 0;
    update();
}

void ImageViewLabel::set_mouse_press_mode(int mode) {

}


