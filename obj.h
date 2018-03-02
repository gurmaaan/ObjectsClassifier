#ifndef OBJ_H
#define OBJ_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QDebug>
#include <QDataStream>
#include <QPolygon>
#include <QPen>
#include <QBrush>

//1 - поменять на норм табы в главном окне док виджеты
//2 - в объекте прописать методы возвращающие полигоны, кисти, цвета и тд
//3 - в моделе прописать создание полигон айтемов, формирование вектора полигон айтемов и их апдейт по сигналам главного окна
//4 - в главном окне прописать метод обновления сцены = вьюхи по сигналам обновления из модели
//5 - проксирование по коду параметра всех нужных объектов
//6 - сверка атрибутов с данными и их мерж
//7 - кастомный виджет с 3-мя гистограммами и инфой

//8 фильтрация
//9 группировка
enum class Attribute
{
    //Яркостные признаки 0-2. Средняя яркость по компонентам
    //Вычисляется как суммарная яркость / кол-во пикселей
    BrRAv,
    BrGAv,
    BrBAv,

    //Яркостные признаки 3-5. СКО яркости по компонентам.
    //sqrt { sum[ (сред ярк - ярк в i)^2 / N ] }
    BrRsko,
    BrGsko,
    BrBsko,

    //Текстурные признаки 6-8. МПС  по компонентам - энергия.
    //По факту - среднее значение квадрата яркости компонента
    TextRmps,
    TextGmps,
    TextBmps,

    //Текстурные признаки 9-11. Длина серии по компонентам - энтропия.
    //По факту - среднее значение величины R * ln (R)
    TextRser,
    TextGser,
    TextBser,

    //Геометрические признаки.
    // S = площадь, P = периметр, K = P^2 / S
    GeomForm,
    GeomSquare,
    GeomPerim
};

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
    Obj& operator = (const Obj &ob);

    //Хеширование (получене уникального числа для объекта) по id. id <==> primary key
    inline uint qHash(const Obj &ob);

    QVector<QPoint> internalPoits() const;
    void setInternalPoits(const QVector<QPoint> &internalPoits);

    QVector<QPoint> contourPointns() const;
    void setContourPointns(const QVector<QPoint> &contourPointns);

    void pushInternalPoint(QPoint point);
    void pushInternalPoint(int x, int y);

    void pushContourPoint(QPoint point);
    void pushContourPoint(int x, int y);

    int id() const;
    void setId(int id);

    int getInternalPointsCount() const;
    void setIntPointsCount(int getInternalPointsCount);

    int contourPointsCount() const;
    void setContourPointsCount(int contourPointsCount);

    QColor getInternalObjectColor() const;
    void setInternalObjectColor(const QColor &internalObjColor);

    QColor getContourColor() const;
    void setContourColor(const QColor &contourColor);

    int getContourWidth() const;
    void setContourWidth(int contourWidth);

    QBrush getInternalBrush() const;
    void setInternalBrush(const QBrush &internalBrush);

    QPolygonF getInternalPolygonF() const;
    void setInternalPolygonF(const QPolygonF &internalPolygonF);

    QPen getContourPen() const;
    void setContourPen(const QPen &contourPen);

    QPolygonF getContourPolygonF() const;
    void setContourPolygonF(const QPolygonF &contourPolygonF);

private:
    int _id;
    int _intPointsCount;
    int _contourPointsCount;

    QVector<QPoint> _internalPoints;
    QVector<QPoint> _contourPoints;

    QColor _internalColor;
    QBrush _internalBrush;
    QPolygonF _internalPolygonF;

    QColor _contourColor;
    QPen _contourPen;
    int _contourWidth;
    QPolygonF _contourPolygonF;
};

Q_DECLARE_METATYPE(Obj);

//Перегрузка оператора вывода в QDebug
QDebug operator <<(QDebug dbg, const Obj &ob);
//Перегрузка оператора сравнения
bool operator ==(const Obj& ob1, const Obj& ob2);

#endif // OBJ_H
