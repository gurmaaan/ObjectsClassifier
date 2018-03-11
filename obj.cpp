#include "obj.h"

Obj::Obj(const int id)

    : _id(id),
      _internalColor(Qt::green), _contourColor(Qt::red),
      _contourWidth(0)
{

}
//NOTE: Посмотреть про список инициализации и тд
Obj::Obj(const Obj &other)
{
    _id = other.id();
}

Obj::~Obj()
{

}

Obj &Obj::operator = (const Obj &ob)
{
    if (this->_internalPoints.count() == ob.getInternalPoits().count()
            && this->_contourPoints.count() == ob.getInternalPoits().count() )
    {
        setId(ob.id());

        setInternalPoits(ob.getInternalPoits());
        setContourPointns(ob.getContourPointns());

        setInternalColor(ob.getInternalColor());
        setContourColor(ob.getInternalColor());

        setInternalPixmap(ob.getInternalPixmap());
        setContourPixmap(ob.getContourPixmap());

        return *this;
    }
    else
    {
        return *this;
    }
}

QVector<QPoint> Obj::getInternalPoits() const
{
    return _internalPoints;
}

void Obj::setInternalPoits(const QVector<QPoint> &internalPoits)
{
    _internalPoints = internalPoits;
}

QVector<QPoint> Obj::getContourPointns() const
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
    _internalPoints.append(QPoint(x, y));
}

void Obj::pushContourPoint(QPoint point)
{
    _contourPoints.append(point);
}

void Obj::pushContourPoint(int x, int y)
{
    _contourPoints.append(QPoint(x, y));
}

QRect Obj::getInternalRect() const
{
    QRect rect(getAreaRect(_internalPoints));
    return rect;
}

QRect Obj::getContourRect() const
{
    QRect rect(getAreaRect(_contourPoints));
    return rect;
}

int Obj::id() const
{
    return _id;
}

void Obj::setId(int id)
{
    _id = id;
}

int Obj::getContourWidth()
{
    return _contourWidth;
}

void Obj::setContourWidth(int contourWidth)
{
    _contourWidth = contourWidth;
}

QColor Obj::getContourColor() const
{
    return _contourColor;
}

void Obj::setContourColor(const QColor &contourColor)
{
    _contourColor = contourColor;
}

QColor Obj::getInternalColor() const
{
    return _internalColor;
}

void Obj::setInternalColor(const QColor &internalColor)
{
    _internalColor = internalColor;
}

QPixmap Obj::getInternalPixmap() const
{
    QRect intRect = getInternalRect();

    QImage intImg(intRect.size(), QImage::Format_ARGB32);

    for (int i = 0; i < intRect.width(); i++) {
        for (int j = 0; j < intRect.height(); j++) {
            intImg.setPixelColor(i, j, getInternalColor());
        }
    }

    return QPixmap::fromImage(intImg);
}

void Obj::setInternalPixmap(const QPixmap &internalPixmap)
{
    _internalPixmap = internalPixmap;
}

QPixmap Obj::getContourPixmap() const
{

    return _contourPixmap;
}

void Obj::setContourPixmap(const QPixmap &contourPixmap)
{
    _contourPixmap = contourPixmap;
}

void Obj::appendDescriptor(Code code, int val)
{
    if(code != Code::NonAssigned)
    {
        Attribute attr(code, val);
        _descriptors.append(attr);
    }
}

void Obj::appendDescriptor(Attribute attribute)
{
    if (attribute.getCode() != Code::NonAssigned)
        _descriptors.append(attribute);
}

QRect Obj::getAreaRect(QVector<QPoint> points) const
{
    int minX = points.first().x();
    int maxX = minX;
    int minY = points.first().y();
    int maxY = minY;

    int w = 0, h = 0;

    foreach(auto point, points)
    {
        int x = point.x();
        int y = point.y();

        if (x > maxX)
            maxX = x;

        if (y > maxY)
            maxY = y;

        if (x < minX)
            minX = x;

        if (y < minY)
            minY = y;
    }

    w = maxX - minX + 1;
    h = maxY - minY + 1;

    QRect rect(minX, minY, w, h);

    return rect;
}

QDebug operator <<(QDebug dbg, const Obj &ob)
{
    dbg << "Object id: " << ob.id() << endl;
    int ic = ob.getInternalPoits().count();
    int cc = ob.getContourPointns().count();
    dbg << "\tInternal points count: " << ic << endl;
    
    for (int i = 0; i < ic; i++)
        dbg << "\t\t" << i <<":"
            <<" (" << ob.getInternalPoits().at(i).x() << " ; "
            << ob.getInternalPoits().at(i).y() << ");" << endl;


    dbg << "\tContour points count: " << cc << endl;

    for (int j = 0; j < cc; j++)
        dbg << "\t\t" << j <<":"
            <<" (" << ob.getContourPointns().at(j).x() << " ; "
            << ob.getContourPointns().at(j).y() << ");" <<endl;

    return dbg;
}

bool operator ==(const Obj &ob1, const Obj &ob2)
{
    bool idEqual = ob1.id() == ob2.id() ? true : false;

    int ic1 = ob1.getInternalPoits().count();
    int cc1 = ob1.getContourPointns().count();
    int ic2 = ob2.getInternalPoits().count();
    int cc2 = ob2.getContourPointns().count();

    bool contourPointsCountEqual = cc1 == cc2 ? true : false;
    bool internalPointsCountEqual = ic1 == ic2 ? true : false;

    return (idEqual && contourPointsCountEqual && internalPointsCountEqual);
}
