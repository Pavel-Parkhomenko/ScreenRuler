#include "mainwindow.h"
#include "qnamespace.h"
#include "qpainter.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QScreen>
#include <QString>
#include <cmath>
#include "formfields.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

#ifdef Q_OS_LINUX
    setAttribute(Qt::WA_TranslucentBackground);
#endif

#ifdef Q_OS_WIN
    setWindowOpacity(0.2);
#endif

    QLabel *screenGeometry = new QLabel(this);
    QScreen *screen = QGuiApplication::primaryScreen();

    if (screen)
    {
        QRect geometry = screen->geometry();
        screenW = geometry.width();
        screenH = geometry.height();
        screenGeometry->setText(QString("%1x%2").arg(screenW).arg(screenH));
    }
    else
    {
        screenGeometry->setText("none");
    }

    screenGeometry->adjustSize();
    screenGeometry->setStyleSheet("background-color: green; color: white;");

    QLabel *textClose = new QLabel(this);
    textClose->setText("Use Alt+F4 for close");
    textClose->adjustSize();
    textClose->setStyleSheet("background-color: red; color: white;");

    QLabel *textCtrl = new QLabel(this);
    textCtrl->setText("Use ctrl");
    textCtrl->adjustSize();
    textCtrl->setStyleSheet("background-color: red; color: white;");

    screenGeometry->move(textClose->width() + 10, 0);
    textCtrl->move(screenGeometry->x() + screenGeometry->width() + 10, 0);

    textPos = new QLabel(this);
    textPos->setStyleSheet(
        "padding: 0 3px 0 3px; color: red; background-color: rgba(255, 255, 255, 128);");
    textPos->setText(QString("(%1,%2)").arg(0).arg(0));
    textPos->adjustSize();

    // setFocusPolicy(Qt::StrongFocus);
    showFullScreen();
    // setFocus();

    formFields = new FormFields();
    formFields->show();

    connect(formFields, &FormFields::userClickCloseSig, this, &MainWindow::userClickCloseSlot);
    connect(formFields, &FormFields::needPaintCircleSig, this, &MainWindow::needPaintCircleSlot);

    x = screenW / 2;
    y = screenH / 2;

    textPos->setText(QString("(%1,%2)").arg(x).arg(y));
    textPos->adjustSize();
    checkMove();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && (event->modifiers() & Qt::ControlModifier))
    {
        ctrlPress = true;
    }
    else
        ctrlPress = false;

    if (event->button() == Qt::LeftButton)
    {
        this->x = event->pos().x();
        this->y = event->pos().y();
        textPos->setText(QString("(%1,%2)").arg(x).arg(y));
        textPos->adjustSize();

        formFields->setNewCoords(x, y);

        checkMove();
    }
}

void MainWindow::checkMove()
{
    if (x + textPos->width() >= screenW)
    {
        if (y + textPos->height() + 5 >= screenH)
            textPos->move(x - textPos->width() - 5, y - textPos->height() - 5);
        else
            textPos->move(x - textPos->width() - 5, y + 5);
    }
    else if (y - textPos->height() <= 0)
    {
        textPos->move(x + 5, y + 5);
    }
    else
        textPos->move(x + 5, y - textPos->height() - 5);

    update();
}

void MainWindow::userClickCloseSlot()
{
    this->close();
}

void MainWindow::paintCircles()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 1));

    const int h = 10;
    const int w = 10;

    painter.setBrush(QBrush(QColor(252, 186, 3)));
    painter.drawEllipse(coords.xNext - w / 2, coords.yNext - h / 2, w, h);

    painter.setBrush(QBrush(Qt::red));
    painter.drawEllipse(coords.xPrev - w / 2, coords.yPrev - h / 2, w, h);

    painter.setBrush(QBrush(Qt::blue));
    painter.drawEllipse(coords.xMove - w / 2, coords.yMove - h / 2, w, h);

    update();
}


void MainWindow::needPaintCircleSlot(Coords coords)
{
    this->coords = coords;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setPen(QPen(Qt::blue, 1));
    painter.drawLine(0, y, screenW, y);
    painter.drawLine(x, 0, x, screenH);

    if (ctrlPress)
    {
        QPainter painter(this);
        painter.setPen(QPen(Qt::red, 1));

        int pointX = 0;
        int pointY = 0;
        if (x < prevX)
            pointX = prevX - (std::abs(prevX - x) / 2);
        else
            pointX = prevX + (std::abs(prevX - x) / 2);

        if (y < prevY)
            pointY = y + (std::abs(prevY - y) / 2);
        else
            pointY = y - (std::abs(prevY - y) / 2);

        painter.drawLine(0, prevY, screenW, prevY);
        int a = std::abs(x - prevX);
        painter.drawText(pointX, y, QString("%1").arg(a));

        painter.drawLine(prevX, 0, prevX, screenH);
        int b = std::abs(y - prevY);
        painter.drawText(x, pointY, QString("%1").arg(b));

        QPen pen(Qt::green, 1, Qt::DashLine);
        painter.setPen(pen);
        painter.drawLine(prevX, prevY, x, y);
        int gipD = std::sqrt(std::pow(std::abs(x - prevX), 2) + std::pow(std::abs(y - prevY), 2));

        painter.save();
        QPoint center(pointX, pointY);
        painter.translate(center);
        if (a != 0 || b != 0)
        {
            painter.rotate(0);
        }
        painter.drawText(0, 0, QString("%1").arg(gipD));
        painter.restore();
        return;
    }

    prevY = y;
    prevX = x;

    paintCircles();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    bool moved = false;
    switch (event->key())
    {
    case Qt::Key_Left:
        this->x -= 1;
        textPos->move(textPos->x() - 1, textPos->y());
        moved = true;
        break;
    case Qt::Key_Right:
        this->x += 1;
        textPos->move(textPos->x() + 1, textPos->y());
        moved = true;
        break;
    case Qt::Key_Up:
        this->y -= 1;
        textPos->move(textPos->x(), textPos->y() - 1);
        moved = true;
        break;
    case Qt::Key_Down:
        this->y += 1;
        textPos->move(textPos->x(), textPos->y() + 1);
        moved = true;
        break;
    case Qt::Key_Space:
        formFields->changeRadioBySpace();
        break;
    default:
        QMainWindow::keyPressEvent(event);
    }

    if (moved)
    {
        textPos->setText(QString("(%1,%2)").arg(x).arg(y));
        formFields->setNewCoords(x, y);
        checkMove();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
