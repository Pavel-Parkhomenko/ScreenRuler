#include "formfields.h"
#include "dataplayer.h"
#include "qevent.h"
#include "qlineedit.h"
#include "ui_formfields.h"
#include <QCheckBox>
#include <QDebug>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>

FormFields::FormFields(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::FormFields)
{
    ui->setupUi(this);

// #ifdef Q_OS_LINUX
//     setAttribute(Qt::WA_TranslucentBackground);
// #endif

#ifdef Q_OS_WIN
    // setAttribute(Qt::WA_StyledBackground);
    // this->setObjectName("FormFields");
    // this->setStyleSheet("#FormFields { background-color: rgba(0, 0, 0, 0); } ");
#endif

    this->setWindowTitle("Настройка");
    this->setMinimumSize(350, 350);

    readJsonFile("DATA.json");

    QGroupBox *groupBox = new QGroupBox("Настройки выбора");
    nextCheck   = new QRadioButton("Вперед");
    nextCheck->setStyleSheet("color: #fcba03;");
    nextCheck->setChecked(true);
    prevCheck   = new QRadioButton("Назад");
    prevCheck->setStyleSheet("color: red;");
    centerCheck = new QRadioButton("Центр");
    centerCheck->setStyleSheet("color: blue;");

    QHBoxLayout *hlCheck = new QHBoxLayout();
    hlCheck->addWidget(nextCheck);
    hlCheck->addWidget(prevCheck);
    hlCheck->addWidget(centerCheck);

    groupBox->setLayout(hlCheck);

    QListWidget *listWidget = new QListWidget();
    listWidget->setStyleSheet(
        R"(
        QListWidget::item {
            border-bottom : 2px solid #ebdedd;
        }
        QListWidget::item:selected {
            border-bottom: 2px solid #145e16;
        }
        )"
    );

    for (int i = 0; i < dataPlayers.size(); ++i)
    {
        QLabel *label = new QLabel();
        label->setMargin(0);
        const DataPlayer &p = dataPlayers[i];
        label->setText(QString("%1: %2").arg(p.getPlayer(), p.getCoords().getCoordsAsStr()));
        QListWidgetItem *item = new QListWidgetItem(listWidget);


        item->setData(Qt::UserRole + 1, i);

        listWidget->setItemWidget(item, label);
    }

    // connect(listWidget, &QListWidget::itemClicked, this, &FormFields::listItemClickSlot);
    connect(listWidget, &QListWidget::currentItemChanged, this, &FormFields::listItemClickSlot);

    QHBoxLayout *hlEdit = new QHBoxLayout();
    editName = new QLineEdit();
    editName->setFixedWidth(100);

    QCheckBox *newNote = new QCheckBox("new");
    connect(newNote, &QCheckBox::toggled, this, &FormFields::newNoteCheckBoxSlot);

    editCoords = new QLineEdit();
    editCoords->setEnabled(false);

    connect(editName, &QLineEdit::textChanged, this, &FormFields::editNameSlot);

    hlEdit->addWidget(editName);
    hlEdit->addWidget(editCoords);

    QHBoxLayout *hlBtn = new QHBoxLayout();
    QPushButton *btnOk = new QPushButton("Подтвердить");
    QPushButton *btnClose = new QPushButton("Закрыть");
    hlBtn->addWidget(btnOk);
    hlBtn->addWidget(newNote);
    hlBtn->addWidget(btnClose);

    connect(btnOk, &QPushButton::clicked, this, &FormFields::btnOkClickSlot);
    connect(btnClose, &QPushButton::clicked, this, &FormFields::btnCloseClickSlot);

    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->addWidget(groupBox);
    vl->addLayout(hlEdit);
    vl->addWidget(listWidget);
    vl->addLayout(hlBtn);

    if (!curPlayer)
    {
        curPlayer = new DataPlayer();
        curPlayer->setIsNewObj(true);
        curPlayer->setIsEdit(true);
    }
}

void FormFields::listItemClickSlot(QListWidgetItem *item)
{
    int playerDataInd = item->data(Qt::UserRole + 1).toInt();
    curPlayer = &dataPlayers[playerDataInd];
    backupPlayer = *curPlayer;
    curPlayer->setIsNewObj(false);
    curPlayer->setIsEdit(true);

    editName->setText(curPlayer->getPlayer());
    editCoords->setText(curPlayer->getCoords().getCoordsAsStr());

    emit needPaintCircleSig(curPlayer->getCoords());
}

void FormFields::btnOkClickSlot()
{
    savePlayersToJson(dataPlayers, "DATA.json");
    this->close();
    emit userClickCloseSig();
}

void FormFields::btnCloseClickSlot()
{
    this->close();
    emit userClickCloseSig();
}

void FormFields::editNameSlot(const QString &text)
{
    curPlayer->setPlayer(text);
}

void FormFields::newNoteCheckBoxSlot(bool state)
{
    if (state)
    {
        DataPlayer *tmpPlayer = new DataPlayer(*curPlayer);
        *curPlayer = backupPlayer;
        tmpPlayer->setIsNewObj(true);
        tmpPlayer->setIsEdit(true);
        curPlayer = tmpPlayer;
    }
}

void FormFields::setNewCoords(double x, double y)
{
    if (nextCheck->isChecked()) {
        curPlayer->setPartCoords(x, y, POS::NEXT);
    }
    else if (prevCheck->isChecked()) {
        curPlayer->setPartCoords(x, y, POS::PREV);
    }
    else if (centerCheck->isChecked()) {
        curPlayer->setPartCoords(x, y, POS::MOVE);
    }

    editName->setText(curPlayer->getPlayer());
    editCoords->setText(curPlayer->getCoords().getCoordsAsStr());

    emit needPaintCircleSig(curPlayer->getCoords());
}

void FormFields::changeRadioBySpace()
{
    static uint8_t CUR_TR = 1;

    CUR_TR++;

    if (CUR_TR == 4)
        CUR_TR = 1;

    if (CUR_TR == 1)
        nextCheck->setChecked(true);
    else if (CUR_TR == 2)
        prevCheck->setChecked(true);
    else if (CUR_TR == 3)
        centerCheck->setChecked(true);

}

bool FormFields::savePlayersToJson(QVector<DataPlayer> &players, const QString &fileName)
{
    QJsonArray playersArray;
    curPlayer->setIsChecked(true);

    if (curPlayer->getIsNewObj())
    {
        players.push_back(*curPlayer);
    }

    for (const DataPlayer &player : players)
    {
        QJsonObject playerObject;
        playerObject["player"] = player.getPlayer();
        playerObject["coords"] = player.getCoords().getCoordsAsStr();
        playerObject["isChecked"] = player.getIsEdit() ? true : false;
        playersArray.append(playerObject);
    }

    QJsonDocument doc(playersArray);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл для записи:" << file.errorString();
        return false;
    }

    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}

bool FormFields::readJsonFile(const QString &fileName)
{
    // Открываем файл
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Не удалось открыть файл:" << file.errorString();
        return false;
    }

    // Читаем данные из файла
    QByteArray jsonData = file.readAll();
    file.close();

    // Парсим JSON
    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonData, &parseError);

    if (parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "Ошибка парсинга JSON:" << parseError.errorString();
        return false;
    }

    if (jsonDoc.isObject())
    {
        QJsonObject jsonObj = jsonDoc.object();
        processJsonObject(jsonObj);
    }
    else if (jsonDoc.isArray())
    {
        QJsonArray jsonArray = jsonDoc.array();
        processJsonArray(jsonArray);
    }

    return true;
}

void FormFields::processJsonArray(const QJsonArray &array) {
    for (int i = 0; i < array.size(); ++i) {
        QJsonValue value = array[i];

        if (value.isObject()) {
            processJsonObject(value.toObject());
        }
    }
}

void FormFields::processJsonObject(const QJsonObject &obj) {
    DataPlayer dataPlayer = {};

    if (obj.contains("player"))
        dataPlayer.setPlayer(obj["player"].toString());

    if (obj.contains("coords"))
    {
        QString coordsStr = obj["coords"].toString();
        Coords coords = {};
        QStringList lst = coordsStr.split(",");
        coords.xNext = lst.at(0).toDouble();
        coords.yNext = lst.at(1).toDouble();
        coords.xPrev = lst.at(2).toDouble();
        coords.yPrev = lst.at(3).toDouble();
        coords.xMove = lst.at(4).toDouble();
        coords.yMove = lst.at(5).toDouble();
        dataPlayer.setCoords(coords);
    }

    if (obj.contains("isChecked"))
        dataPlayer.setIsChecked(obj["isChecked"].toBool());

    dataPlayers.push_back(dataPlayer);
}

void FormFields::closeEvent(QCloseEvent *event) {
    event->accept();
    emit userClickCloseSig();
}

FormFields::~FormFields()
{
    delete curPlayer;
    delete ui;
}

