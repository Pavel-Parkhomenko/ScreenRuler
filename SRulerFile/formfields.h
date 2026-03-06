#ifndef FORMFIELDS_H
#define FORMFIELDS_H

#include "dataplayer.h"
#include "qlistwidget.h"
#include <QWidget>
#include <QRadioButton>

namespace Ui {
class FormFields;
}

class FormFields : public QWidget
{
    Q_OBJECT

public:
    explicit FormFields(QWidget *parent = nullptr);
    ~FormFields();

    void setNewCoords(double x, double y);
    void changeRadioBySpace();

private:
    Ui::FormFields *ui;

    QLineEdit *editName;
    QLineEdit *editCoords;
    QRadioButton *nextCheck;
    QRadioButton *prevCheck;
    QRadioButton *centerCheck;

    DataPlayer *curPlayer = nullptr;

    QVector<DataPlayer> dataPlayers;

    bool readJsonFile(const QString &fileName);
    void processJsonObject(const QJsonObject &obj);
    void processJsonArray(const QJsonArray &array);
    bool savePlayersToJson(QVector<DataPlayer> &players, const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void listItemClickSlot(QListWidgetItem *item);
    void btnOkClickSlot();
    void btnCloseClickSlot();
    void editNameSlot(const QString &text);

signals:
    void userClickCloseSig();
    void needPaintCircleSig(Coords coords);
};

#endif // FORMFIELDS_H
