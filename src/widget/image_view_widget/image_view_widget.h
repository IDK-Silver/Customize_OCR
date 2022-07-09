

#ifndef CUSTOMIZE_OCR_IMAGE_VIEW_WIDGET_H
#define CUSTOMIZE_OCR_IMAGE_VIEW_WIDGET_H

#include <QLabel>
#include <memory>
#include <QObject>

namespace MousePressEvent {
    const int MoveImage = 2;
    const int PainterPen = 3;
};

class ImageViewLabel : public QLabel {
Q_OBJECT
public:
    explicit ImageViewLabel(QWidget *parent);

    ~ImageViewLabel() override;

    void read_image(const QImage &image);
    void set_mouse_press_mode(int mode);

    void zoom_in_image();
    void zoom_out_image();
    void on_preset_image();

private:
    QImage original_image;
    qreal zoom_value = 1.0;
    int x_pos = 0;              // x �b����m
    int y_pos = 0;              // y ����m
    QPoint old_pos;             // �W�@�����ƹ���m
    bool is_pressed = false;    // �ƹ��O�_�b����

    QPoint first_press_pos;
    QPoint second_press_pos;
    int mouse_press_mod = MousePressEvent::MoveImage;

    QRect getReac(QPoint first_pos, QPoint second_pos);

protected:

    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;



private slots:

};

#endif //CUSTOMIZE_OCR_IMAGE_VIEW_WIDGET_H
