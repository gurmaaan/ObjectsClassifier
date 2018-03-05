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
    _internalPoints = other.getInternalPoits();
    _intPointsCount = other.getInternalPointsCount();
}

Obj &Obj::operator = (const Obj &ob)
{
    if (this->_intPointsCount == ob.getInternalPointsCount() && this->_contourPointsCount == ob.getInternalPointsCount())
    {
        setId(ob.id());
        setInternalPoits(ob.getInternalPoits());
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

QVector<QPoint> Obj::getInternalPoits() const
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

void Obj::setInternalColor(QColor &internalColor)
{
    if (_internalPolyItem->polygon().count() !=0)
    {
        QBrush brush(internalColor);
        setInternalBrush(brush);
        _internalColor = internalColor;
    }
}

QColor Obj::getContourColor()
{
    return _contourColor;
}

void Obj::setContourColor(QColor &contourColor)
{
    if (_contourPolyItem->polygon().count() != 0)
    {
        QPen pen(contourColor);
        _contourColor = contourColor;
        setContourPen(pen);
    }
}

int Obj::getContourWidth()
{
    return _contourWidth;
}

void Obj::setContourWidth(int contourWidth)
{
    if (contourWidth <= 10 && contourWidth >= 1)
    {
        _contourPen.setWidth(contourWidth);
        _contourWidth = contourWidth;

    }
}

QBrush Obj::getInternalBrush()
{
    return _internalBrush;
}

void Obj::setInternalBrush(QBrush &internalBrush)
{
    _internalBrush = internalBrush;
}

QVector<QPointF> Obj::convertToF(QVector<QPoint> &vectorI)
{
    QVector<QPointF> floatVector;

    foreach (auto point, vectorI) {
        floatVector.append(QPointF(point));
    }
    return floatVector;
}

QGraphicsPolygonItem *Obj::initInternalPolyItem(QVector<QPoint> &vectorI)
{
    QVector<QPointF> vectorF;
    vectorF = convertToF(vectorI);
    QPolygonF internalPolygonF(vectorF);
    _internalPolyItem = new QGraphicsPolygonItem(internalPolygonF);
    return _internalPolyItem;
}

QGraphicsPolygonItem *Obj::initContourPolyItem(QVector<QPoint> &vectorI)
{
    QVector<QPointF> vectorF;
    vectorF = convertToF(vectorI);
    QPolygonF contourPolygonF(vectorF);
    _contourPolyItem = new QGraphicsPolygonItem(contourPolygonF);
    return _internalPolyItem;
}

QPen Obj::getContourPen()
{
    return _contourPen;
}

void Obj::setContourPen(QPen &contourPen)
{
    _contourPen = contourPen;
}

QGraphicsPolygonItem *Obj::getContourPolyItem()
{
    return _contourPolyItem;
}

QGraphicsPolygonItem *Obj::getInternalPolyItem()
{
    return _internalPolyItem;
}


QDebug operator <<(QDebug dbg, const Obj &ob)
{
    dbg << "Object id: " << ob.id() << endl;
    
    dbg << "\tInternal points count: " << ob.getInternalPointsCount() << endl;
    
    for (int i = 0; i < ob.getInternalPointsCount(); i++)
        dbg << "\t\t" << i <<":"
            <<" (" << ob.getInternalPoits().at(i).x() << " ; "
            << ob.getInternalPoits().at(i).y() << ");" << endl;


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


