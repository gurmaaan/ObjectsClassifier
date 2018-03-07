#ifndef OBJ_H
#define OBJ_H

#include <QObject>
#include <QPixmap>
#include <QPoint>
#include <QVector>
#include <QDebug>
#include <QDataStream>
#include <QColor>
#include <QImage>
#include <QIcon>

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

    //Перегрузка оператора равенства
    Obj &operator =(const Obj &ob);

    //Хеширование (получене уникального числа для объекта) по id. id <==> primary key
    uint qHash(const Obj &ob);

    QVector<QPoint> getInternalPoits() const;
    void setInternalPoits(const QVector<QPoint> &internalPoits);

    QVector<QPoint> getContourPointns() const;
    void setContourPointns(const QVector<QPoint> &contourPointns);

    void pushInternalPoint(QPoint point);
    void pushInternalPoint(int x, int y);

    void pushContourPoint(QPoint point);
    void pushContourPoint(int x, int y);

    QRect getInternalRect() const;
    QRect getContourRect() const;

    int id() const;
    void setId(int id);

    //----Ширина контура-------
    int getContourWidth();
    void setContourWidth(int contourWidth);

    QColor getContourColor() const;
    void setContourColor(const QColor &contourColor);

    QColor getInternalColor() const;
    void setInternalColor(const QColor &internalColor);

    QPixmap getInternalPixmap() const;
    void setInternalPixmap(const QPixmap &internalPixmap);

    QPixmap getContourPixmap() const;
    void setContourPixmap(const QPixmap &contourPixmap);

private:
    int _id;

    QVector<QPoint> _internalPoints;
    QVector<QPoint> _contourPoints;

    QRect getAreaRect(QVector<QPoint> points) const;

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
