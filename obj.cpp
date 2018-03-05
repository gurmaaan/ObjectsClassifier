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
    //TODO внутренние: проброска обновления цвета от виджета
    _internalColor = internalColor;
}

QColor Obj::getContourColor()
{
    return _contourColor;
}

void Obj::setContourColor(QColor &contourColor)
{
    //TODO контурные: проброска обновления цвета от виджета
    _contourColor = contourColor;
}

int Obj::getContourWidth()
{
    return _contourWidth;
}

void Obj::setContourWidth(int contourWidth)
{
    _contourWidth = contourWidth;
    qDebug() << contourWidth;
}

QPixmap Obj::getInternalPixmap()
{
    setMinOX( getMinX(_contourPoints) );
    setMinOY( getMinY(_contourPoints) );
    setMaxOX( getMaxX(_contourPoints) );
    setMaxOY( getMaxY(_contourPoints) );

    QSize tempSize( getAreaWidth(_internalPoints), getAreaHeight(_internalPoints) );

    QPixmap cntPixMap(tempSize);

    cntPixMap.fill(qRgba(0, 255, 0, 180));
    return _internalPixmap;
}

void Obj::setInternalPixmap(QPixmap &internalPixmap)
{
    _internalPixmap = internalPixmap;
}

QPixmap Obj::getObjectIcon(QVector<QPoint> points)
{
    QPixmap copy;
    copy = getInternalPixmap().copy(_minOX, _maxOY, getAreaWidth(points), getAreaHeight(points));

    return copy;
}

int Obj::getAreaWidth(QVector<QPoint> points)
{
    //minX = самая левая координата области
    //maxX = самая правая
    //их разница даст ширину нового изображения

    int minX = getMinX(points);
    int maxX = getMaxX(points);
    int w = maxX - minX;
    qDebug() << "Ширина: " << w;
    return w;
}

int Obj::getAreaHeight(QVector<QPoint> points)
{
    //minY = нижня координата области
    //maxY = самая верхняя
    //их разница даст высотуц создаваемого изображения

    int minY = getMinY(points);
    int maxY = getMaxY(points);
    int h = maxY - minY;
    qDebug() << "Высота: " <<h;
    return h;
}

int Obj::getMinX(QVector<QPoint> points)
{
    return std::min_element(points.begin()->y(), points.end()->y());
}

int Obj::getMaxX(QVector<QPoint> points)
{
    return std::max_element(points.begin()->x(), points.end()->x());
}

int Obj::getMinY(QVector<QPoint> points)
{
    return std::min_element(points.begin()->y(), points.end()->y());
}

int Obj::getMaxY(QVector<QPoint> points)
{
    return std::max_element(points.begin()->y(), points.end()->y());
}

QPixmap Obj::getContourPixmap()
{
    setMinСX( getMinX(_contourPoints) );
    setMinСY( getMinY(_contourPoints) );
    setMaxСX( getMaxX(_contourPoints) );
    setMaxСY( getMaxY(_contourPoints) );

    QSize tempSize( getAreaWidth(_contourPoints), getAreaHeight(_contourPoints) );
    QPixmap cntPixMap(tempSize);
    cntPixMap.fill(qRgba(255 ,0, 0, 170));
    return _contourPixmap;
}

void Obj::setContourPixmap(QPixmap &contourPixmap)
{
    _contourPixmap = contourPixmap;
}

void Obj::setMinСX(int minСX)
{
    _minСX = minСX;
}

void Obj::setMinСY(int minСY)
{
    _minСY = minСY;
}

void Obj::setMaxСX(int maxСX)
{
    _maxСX = maxСX;
}

void Obj::setMaxСY(int maxСY)
{
    _maxСY = maxСY;
}

void Obj::setMinOX(int minOX)
{
    _minOX = minOX;
}

void Obj::setMinOY(int minOY)
{
    _minOY = minOY;
}

void Obj::setMaxOX(int maxOX)
{
    _maxOX = maxOX;
}

void Obj::setMaxOY(int maxOY)
{
    _maxOY = maxOY;
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


