#ifndef OBJ_H
#define OBJ_H

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QVector>
#include <QDebug>
#include <QDataStream>

//TODO:2 - в объекте прописать методы возвращающие полигоны, кисти, цвета и тд
//TODO:3 - в моделе прописать создание полигон айтемов, формирование вектора полигон айтемов и их апдейт по сигналам главного окна
//TODO:4 - в главном окне прописать метод обновления сцены = вьюхи по сигналам обновления из модели
//TODO:5 - проксирование по коду параметра всех нужных объектов
//TODO:6 - сверка атрибутов с данными и их мерж
//TODO:7 - кастомный виджет с 3-мя гистограммами и инфой
//TODO:
//TODO:8 фильтрация
//TODO:9 группировка

class Obj : public QObject
{
    Q_OBJECT
public:
    //Конструктор копирования
    Obj(const Obj &other);
    //Конструктор по умолчанию
    Obj(const int id = 0);

    //Диструктор по умолчанию
    ~Obj();

    int _minOX = 0, _minOY = 0, _maxOX = 0, _maxOY = 0;
    int _minСX = 0, _minСY = 0, _maxСX = 0, _maxСY = 0;
    //Перегрузка оператора равенства
    Obj& operator = (const Obj &ob);

    //Хеширование (получене уникального числа для объекта) по id. id <==> primary key
    inline uint qHash(const Obj &ob);

    QVector<QPoint> getInternalPoits() const;
    void setInternalPoits(const QVector<QPoint> &internalPoits);

    QVector<QPoint> contourPointns() const;
    void setContourPointns(const QVector<QPoint> &contourPointns);

    void pushInternalPoint(QPoint point);
    void pushInternalPoint(int x, int y);

    void pushContourPoint(QPoint point);
    void pushContourPoint(int x, int y);

    int id() const;
    void setId(int id);

    //-----количество точек------
    int getInternalPointsCount() const;
    void setIntPointsCount(int getInternalPointsCount);

    int contourPointsCount() const;
    void setContourPointsCount(int contourPointsCount);

    //----Цвет подсветки
    QColor getInternalColor();
    void setInternalColor(QColor &internalColor);

    QColor getContourColor();
    void setContourColor(QColor &contourColor);

    //----Ширина контура-------
    int getContourWidth();
    void setContourWidth(int contourWidth);

    //---Куски изображения содержащие объект
    QPixmap getInternalPixmap() ;
    void setInternalPixmap(QPixmap &internalPixmap);

    QPixmap getObjectIcon(QVector<QPoint> points);

    int getAreaWidth(QVector<QPoint> points);
    int getAreaHeight(QVector<QPoint> points);
    int getMinX(QVector<QPoint> points);
    int getMaxX(QVector<QPoint> points);
    int getMinY(QVector<QPoint> points);
    int getMaxY(QVector<QPoint> points);

    QPixmap getContourPixmap();
    void setContourPixmap(QPixmap &contourPixmap);

    void setMinСX(int minСX);
    void setMinСY(int minСY);
    void setMaxСX(int maxСX);
    void setMaxСY(int maxСY);

    void setMinOX(int minOX);
    void setMinOY(int minOY);
    void setMaxOX(int maxOX);
    void setMaxOY(int maxOY);

private:
    int _id;
    int _intPointsCount;
    int _contourPointsCount;

    QVector<QPoint> _internalPoints;
    QVector<QPoint> _contourPoints;

    //Отображение внутренних точек
    QColor _internalColor;
    QPixmap _internalPixmap;


    //Отображение контурных точек
    QColor _contourColor;
    int _contourWidth;
    QPixmap _contourPixmap;

};

Q_DECLARE_METATYPE(Obj);

//Перегрузка оператора вывода в QDebug
QDebug operator <<(QDebug dbg, const Obj &ob);
//Перегрузка оператора сравнения
bool operator ==(const Obj& ob1, const Obj& ob2);

#endif // OBJ_H
