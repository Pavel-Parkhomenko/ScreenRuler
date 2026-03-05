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
