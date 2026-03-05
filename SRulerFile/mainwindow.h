#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "formfields.h"
#include <QDebug>
#include <QLabel>
#include <QMainWindow>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MainWindow *ui;
    QLabel *textPos;
    FormFields *formFields;
    int x;
    int y;
    int screenW;
    int screenH;
    bool ctrlPress = false;

    int prevX;
    int prevY;

    void checkMove();

private slots:
    void userClickCloseSlot();
};
#endif // MAINWINDOW_H
