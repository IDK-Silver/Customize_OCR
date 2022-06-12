//
// Created by idk on 2022/5/24.
//

#ifndef CUSTOMIZE_OCR_MAIN_WINDOW_H
#define CUSTOMIZE_OCR_MAIN_WINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;


private:
    Ui::MainWindow *ui{};

    void tab_widget_init();


private
    slots:


};

#endif //CUSTOMIZE_OCR_MAIN_WINDOW_H
