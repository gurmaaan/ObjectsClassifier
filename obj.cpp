#include "obj.h"

Obj::Obj(const int id)
    : _id(id)
{
}

Obj::~Obj()
{

}

Obj::Obj(const Obj &other)
{
    _id = other.id();
    _contourPointns = other.contourPointns();
    _contourPointsCount = other.contourPointsCount();
    _internalPoits = other.internalPoits();
    _intPointsCount = other.intPointsCount();
}

Obj &Obj::operator = (const Obj &ob)
{
    if (this->_intPointsCount == ob.intPointsCount() && this->_contourPointsCount == ob.intPointsCount())
    {
        setId(ob.id());
        setInternalPoits(ob.internalPoits());
        setContourPointns(ob.contourPointns());
        return *this;
    }
    else
    {
        return *this;
    }
}

uint Obj::qHash(const Obj &ob)
{
    return (qHash(ob.id())
            ^ qHash(ob.contourPointsCount())
            ^ qHash(ob.intPointsCount()));
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
    return _intPointsCount;
}

void Obj::setIntPointsCount(int intPointsCount)
{
    _intPointsCount = intPointsCount;
}

int Obj::contourPointsCount() const
{
    return _contourPointsCount;
}

void Obj::setContourPointsCount(int contourPointsCount)
{
    _contourPointsCount = contourPointsCount;
}

QDebug operator <<(QDebug dbg, const Obj &ob)
{
    dbg << "Object id: " << ob.id() << endl;

    dbg << "\tInternal points count: " << ob.intPointsCount() << endl;

    for (int i = 0; i < ob.intPointsCount(); i++)
        dbg << "\t\t" << i <<":"
            <<" (" << ob.internalPoits().at(i).x() << " ; "
            << ob.internalPoits().at(i).y() << ");" << endl;


    dbg << "\tContour points count: " << ob.contourPointsCount() << endl;

    for (int j = 0; j < ob.contourPointsCount(); j++)
        dbg << "\t\t" << j <<":"
            <<" (" << ob.contourPointns().at(j).x() << " ; "
            << ob.contourPointns().at(j).y() << ");" <<endl;

    return dbg;
}

bool operator ==(const Obj &ob1, const Obj &ob2)
{
    bool idEqual = ob1.id() == ob2.id() ? true : false;
    bool contourPointsCountEqual = ob1.contourPointsCount() == ob2.contourPointsCount() ? true : false;
    bool internalPointsCountEqual = ob1.intPointsCount() == ob2.intPointsCount() ? true : false;

    return (idEqual && contourPointsCountEqual && internalPointsCountEqual);
}

