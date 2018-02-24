#include "obj.h"

Obj::Obj(QObject *parent) : QObject(parent)
{

}

QVector<QPoint> Obj::internalPoits() const
{
    return _internalPoits;
}

void Obj::setInternalPoits(const QVector<QPoint> &internalPoits)
{
    _internalPoits = internalPoits;
}

QVector<QPoint> Obj::contourPointns() const
{
    return _contourPointns;
}

void Obj::setContourPointns(const QVector<QPoint> &contourPointns)
{
    _contourPointns = contourPointns;
}

void Obj::pushInternalPoint(QPoint point)
{
    _internalPoits.append(point);
}

void Obj::pushInternalPoint(int x, int y)
{
    QPoint tempPoint(x, y);
    _internalPoits.append(tempPoint);
}

void Obj::pushContourPoint(QPoint point)
{
    _contourPointns.append(point);
}

void Obj::pushContourPoint(int x, int y)
{
    QPoint tempPoint(x, y);
    _contourPointns.append(tempPoint);
}

int Obj::id() const
{
    return _id;
}

void Obj::setId(int id)
{
    _id = id;
}

int Obj::intPointsCount() const
{
    return _internalPoits.count();
}

void Obj::setIntPointsCount(int intPointsCount)
{
    _intPointsCount = intPointsCount;
}

int Obj::contourPointsCount() const
{
    return _contourPointns.count();
}

void Obj::setContourPointsCount(int contourPointsCount)
{
    _contourPointsCount = contourPointsCount;
}
