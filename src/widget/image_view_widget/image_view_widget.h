

#ifndef CUSTOMIZE_OCR_IMAGE_VIEW_WIDGET_H
#define CUSTOMIZE_OCR_IMAGE_VIEW_WIDGET_H

#include <QLabel>
#include <memory>
#include <QObject>

namespace MousePressEvent {
    const int MoveImage = 2;
    const int PainterPen = 4;
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


    void clear();
    void clear_select_rect();
    void set_select_rect(QRect rect);
    void clear_press_pos();
    QRect get_select_rect();
    [[nodiscard]] int get_mouse_press_event() const;

    QImage get_raw_image();

private:
    QImage original_image;
    qreal zoom_value = 1.0;
    QPoint first_press_pos;
    QPoint second_press_pos;
    int x_pos = 0;              // x 軸的位置
    int y_pos = 0;              // y 的位置
    QPoint old_pos;             // 上一次的滑鼠位置
    bool is_pressed = false;    // 滑鼠是否在按壓
    int mouse_press_mod = MousePressEvent::MoveImage;
    QRect select_rect;


    void display(QPaintEvent *event, int mouse_press_event);
    QRect getReac(QPoint first_pos, QPoint second_pos);



protected:

    void paintEvent(QPaintEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;



signals:
    void pos_change();

private slots:

};

#endif //CUSTOMIZE_OCR_IMAGE_VIEW_WIDGET_H
