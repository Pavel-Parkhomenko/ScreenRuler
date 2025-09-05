#include "mainwindow.h"
#include "qnamespace.h"
#include "qpainter.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QString>
#include <QScreen>
#include <QHBoxLayout>
#include <cmath>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  setAttribute(Qt::WA_TranslucentBackground);
//  setWindowFlags(windowFlags() | Qt::FramelessWindowHint);

  QLabel *screenGeometry = new QLabel(this);
  QScreen *screen = QGuiApplication::primaryScreen();
  if (screen) {
    QRect geometry = screen->geometry();
    screenW = geometry.width();
    screenH = geometry.height();
    screenGeometry->setText(QString("%1x%2").arg(screenW).arg(screenH));
  } else {
    screenGeometry->setText("none");
  }

  screenGeometry->adjustSize();
  screenGeometry->setStyleSheet("background-color: green; color: white;");

  QLabel *textClose = new QLabel(this);
  textClose->setText("Use Alt+F4 for close");
  textClose->adjustSize();
  textClose->setStyleSheet("background-color: red; color: white;");

  screenGeometry->move(textClose->width() + 10, 0);

  textPos = new QLabel(this);
  textPos->setStyleSheet("padding: 0 3px 0 3px; color: red; background-color: rgba(255, 255, 255, 128);");
  textPos->setText(QString("(%1,%2)").arg(0).arg(0));
  textPos->adjustSize();

  setFocusPolicy(Qt::StrongFocus);
  showFullScreen();
  setFocus();
}

void MainWindow::mousePressEvent(QMouseEvent *event) {

  if(event->button() == Qt::LeftButton && (event->modifiers() & Qt::ControlModifier)) {
    ctrlPress = true;
  } else ctrlPress = false;

  if (event->button() == Qt::LeftButton) {
    this->x = event->pos().x();
    this->y = event->pos().y();
    textPos->setText(QString("(%1,%2)").arg(x).arg(y));
    textPos->adjustSize();

    checkMove();
  }
}

void MainWindow::checkMove() {
  if(x + textPos->width() >= screenW) {
    if(y + textPos->height() + 5 >= screenH) textPos->move(x - textPos->width() - 5, y - textPos->height() - 5);
    else textPos->move(x - textPos->width() - 5, y + 5);
  }
  else if(y - textPos->height() <= 0) {
    textPos->move(x + 5, y + 5);
  }
  else textPos->move(x + 5, y - textPos->height() - 5);

  update();
}

int cnt = 0;
void MainWindow::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  painter.setPen(QPen(Qt::blue, 1));
  painter.drawLine(0, y, screenW, y);
  painter.drawLine(x, 0, x, screenH);

  qDebug() << __FUNCTION__ << cnt++;

  if(ctrlPress) {
    qDebug() << "CTRL" << ctrlPress;
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 1));

    int pointX = 0;
    int pointY = 0;
    if(x < prevX)  pointX = prevX - (std::abs(prevX - x) / 2);
    else pointX = prevX + (std::abs(prevX - x) / 2);

    if(y < prevY)  pointY = y + (std::abs(prevY - y) / 2);
    else pointY = y - (std::abs(prevY - y) / 2);

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
    if(a != 0 || b != 0) {
//      double ang1 = std::atan(a / b) * 180.0 / 3.14;
//      double ang2 = 90 - ang1;
      painter.rotate(0);//ang1 < ang2 ? ang2 : ang1
    }
    painter.drawText(0, 0, QString("%1").arg(gipD));
    painter.restore();
    return;
  }

  prevY = y;
  prevX = x;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  bool moved = false;
  switch (event->key()) {
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
    default:
      QMainWindow::keyPressEvent(event);
  }

  if(moved) {
//    ctrlPress = false;
    textPos->setText(QString("(%1,%2)").arg(x).arg(y));
    checkMove();
  }
}

MainWindow::~MainWindow()
{
  delete ui;
}

