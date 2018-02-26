#ifndef OBJ_H
#define OBJ_H

#include <QObject>
#include <QPoint>
#include <QVector>
#include <QDebug>
#include <QDataStream>
#include <QGraphicsItem>

class Obj : public QObject
{
    Q_OBJECT
public:
    //Конструктор запрещающей автоприведение типов (стандартный QT)
//    explicit Obj(QObject *parent = nullptr);
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

    int intPointsCount() const;
    void setIntPointsCount(int intPointsCount);

    int contourPointsCount() const;
    void setContourPointsCount(int contourPointsCount);

private:
    int _id;
    int _intPointsCount;
    int _contourPointsCount;
    QVector<QPoint> _internalPoits;
    QVector<QPoint> _contourPointns;
};

Q_DECLARE_METATYPE(Obj);

//Перегрузка оператора вывода в QDebug
QDebug operator <<(QDebug dbg, const Obj &ob);
//Перегрузка оператора сравнения
bool operator ==(const Obj& ob1, const Obj& ob2);

#endif // OBJ_H
