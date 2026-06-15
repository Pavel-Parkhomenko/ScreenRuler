#include "dataplayer.h"

Coords::Coords(double xNext, double yNext, double xPrev, double yPrev, double xMove, double yMove)
{
    this->xNext = xNext;
    this->yNext = yNext;
    this->xPrev = xPrev;
    this->yPrev = yPrev;
    this->xMove = xMove;
    this->yMove = yMove;
}

QString Coords::getCoordsAsStr()
{
    return QString("%1,%2,%3,%4,%5,%6")
        .arg(this->xNext)
        .arg(this->yNext)
        .arg(this->xPrev)
        .arg(this->yPrev)
        .arg(this->xMove)
        .arg(this->yMove);
}

void Coords::setPos(double x, double y, POS pos)
{
    switch (pos)
    {
    case NEXT: {
        this->xNext = x;
        this->yNext = y;
        break;
    }
    case PREV: {
        this->xPrev = x;
        this->yPrev = y;
        break;
    }
    case MOVE: {
        this->xMove = x;
        this->yMove = y;
        break;
    }
    }
}

DataPlayer::DataPlayer()
{
    this->player = "player_0";
    this->coords = Coords();
    this->isChecked = false;
}

DataPlayer::DataPlayer(const DataPlayer &other) {
    this->player = other.player;
    this->coords = other.coords;
    this->isChecked = other.isChecked;
}

DataPlayer &DataPlayer::operator=(const DataPlayer &other) {
    if (this != &other) {
        this->player = other.player;
        this->coords = other.coords;
        this->isChecked = other.isChecked;
    }
    return *this;
}

void DataPlayer::setPartCoords(double x, double y, POS pos)
{
    coords.setPos(x, y, pos);
}

QString DataPlayer::getPlayer() const
{
    return player;
}

void DataPlayer::setPlayer(const QString &newPlayer)
{
    player = newPlayer;
}

Coords DataPlayer::getCoords() const
{
    return coords;
}

void DataPlayer::setCoords(Coords newCoords)
{
    coords = newCoords;
}

bool DataPlayer::getIsChecked() const
{
    return isChecked;
}

void DataPlayer::setIsChecked(bool newIsChecked)
{
    isChecked = newIsChecked;
}

bool DataPlayer::getIsNewObj() const
{
    return isNewObj;
}

void DataPlayer::setIsNewObj(bool newIsNewObj)
{
    this->isNewObj = newIsNewObj;
}

bool DataPlayer::getIsEdit() const
{
    return isEdit;
}

void DataPlayer::setIsEdit(bool newIsEdit)
{
    isEdit = newIsEdit;
}
