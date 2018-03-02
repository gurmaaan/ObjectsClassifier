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
    _contourPoints = other.contourPointns();
    _contourPointsCount = other.contourPointsCount();
    _internalPoints = other.internalPoits();
    _intPointsCount = other.getInternalPointsCount();
}

Obj &Obj::operator = (const Obj &ob)
{
    if (this->_intPointsCount == ob.getInternalPointsCount() && this->_contourPointsCount == ob.getInternalPointsCount())
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
            ^ qHash(ob.getInternalPointsCount()));
}

QVector<QPoint> Obj::internalPoits() const
{
    return _internalPoints;
}

void Obj::setInternalPoits(const QVector<QPoint> &internalPoits)
{
    _internalPoints = internalPoits;
}

QVector<QPoint> Obj::contourPointns() const
{
    return _contourPoints;
}

void Obj::setContourPointns(const QVector<QPoint> &contourPointns)
{
    _contourPoints = contourPointns;
}

void Obj::pushInternalPoint(QPoint point)
{
    _internalPoints.append(point);
}

void Obj::pushInternalPoint(int x, int y)
{
    QPoint tempPoint(x, y);
    _internalPoints.append(tempPoint);
}

void Obj::pushContourPoint(QPoint point)
{
    _contourPoints.append(point);
}

void Obj::pushContourPoint(int x, int y)
{
    QPoint tempPoint(x, y);
    _contourPoints.append(tempPoint);
}

int Obj::id() const
{
    return _id;
}

void Obj::setId(int id)
{
    _id = id;
}

int Obj::getInternalPointsCount() const
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

QColor Obj::getContourColor() const
{
    return _contourColor;
}

void Obj::setContourColor(const QColor &contourColor)
{
    //приравниваем локал к глобалу
    //у добавленной в класс QPen делается сет колор
    //Полигон репаинт
    //Эмит Апдейт Сцена
    _contourColor = contourColor;
}

int Obj::getContourWidth() const
{
    return _contourWidth;
}

void Obj::setContourWidth(int contourWidth)
{
    if (contourWidth <= 10 && contourWidth >= 1)
    {

        _contourWidth = contourWidth;
    }
}

QBrush Obj::getInternalBrush() const
{
    return _internalBrush;
}

void Obj::setInternalBrush(const QBrush &internalBrush)
{
    _internalBrush = internalBrush;
}

QPolygonF Obj::getInternalPolygonF() const
{

    return _internalPolygonF;
}

void Obj::setInternalPolygonF(const QPolygonF &internalPolygonF)
{
    _internalPolygonF = internalPolygonF;
}

QPen Obj::getContourPen() const
{
    return _contourPen;
}

void Obj::setContourPen(const QPen &contourPen)
{
    _contourPen = contourPen;
}

QPolygonF Obj::getContourPolygonF() const
{
    return _contourPolygonF;
}

void Obj::setContourPolygonF(const QPolygonF &contourPolygonF)
{
    _contourPolygonF = contourPolygonF;
}

QDebug operator <<(QDebug dbg, const Obj &ob)
{
    dbg << "Object id: " << ob.id() << endl;
    
    dbg << "\tInternal points count: " << ob.getInternalPointsCount() << endl;
    
    for (int i = 0; i < ob.getInternalPointsCount(); i++)
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
    bool internalPointsCountEqual = ob1.getInternalPointsCount() == ob2.getInternalPointsCount() ? true : false;

    return (idEqual && contourPointsCountEqual && internalPointsCountEqual);
}

