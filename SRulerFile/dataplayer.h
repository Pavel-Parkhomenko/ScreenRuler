#ifndef DATAPLAYER_H
#define DATAPLAYER_H

#include <QString>
#include <QMetaType>

//1470,1100,100,100,960,540
//x-next,y-next,x-prev,y-prev,x-move,y-move

enum POS {
    NEXT,
    PREV,
    MOVE
};

class Coords {
public:
    Coords() : xNext(0.), yNext(0.), xPrev(0.), yPrev(0.), xMove(0.), yMove(0.) {};
    Coords(double xNext, double yNext, double xPrev, double yPrev, double xMove, double yMove);

    void setPos(double x, double y, POS pos);

    QString getCoordsAsStr();

    double xNext;
    double yNext;
    double xPrev;
    double yPrev;
    double xMove;
    double yMove;
};

class DataPlayer {
public:
    DataPlayer();
    DataPlayer(QString player, Coords *coords, bool isChecked);

    DataPlayer(const DataPlayer &other);
    DataPlayer &operator=(const DataPlayer &other);

    QString getPlayer() const;
    void setPlayer(const QString &newPlayer);
    Coords getCoords() const;
    void setCoords(Coords newCoords);
    void setPartCoords(double x, double y, POS pos);
    bool getIsChecked() const;
    void setIsChecked(bool newIsChecked);

    bool getIsNewObj() const;
    void setIsNewObj(bool newIsNewObj);

    bool getIsEdit() const;
    void setIsEdit(bool newIsEdit);

private:
    QString player;
    Coords coords;
    bool isChecked;
    bool isNewObj = false;
    bool isEdit = false;
};

#endif // DATAPLAYER_H
