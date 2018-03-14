#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>
#include <QDebug>
#include <QColor>
#include <QVariant>

enum class Code
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

    //Геометрические признаки. 12-14
    // S = площадь, P = периметр, K = P^2 / S
    GeomForm,
    GeomSquare,
    GeomPerim,

    NonAssigned
};

Code &operator++(Code& c);
Code operator++(Code& c, int);


bool operator>(const Code c, const int i);
bool operator>(const int i, const Code c);

bool operator>=(const Code c1, const Code c2);
bool operator<=(const Code c1, const Code c2);

bool operator<(const Code c, const int i);
bool operator<(const int i, const Code c);

class Attribute : public QObject
{
    Q_OBJECT
public:   
    enum Type
    {
        NonAssigned,
        Brightness,
        Texture,
        Geometrical
    };

    Attribute(Code code = Code::NonAssigned);
    Attribute(const Attribute &other);

    Attribute(const int code);
    Attribute(Code code, int value);
    Attribute(Code code, QVariant value);
    Attribute(const int code, int value);

    ~Attribute();

    Attribute &operator =(const Attribute &at);
    QString name() const;
    int colNum() const;


    QVariant value() const;

    QString getTypeName() const;


    Code getCode() const;

    void setValue(QVariant value);

    Attribute::Type getType() const;

    void setCode(const int &code);

    QColor interpritatedColor(Code c, int v);
    void setAtrColor(const QColor &value);

    QColor getAtrColor();

    static int columnFromCode(Code code);
    //возвращает истину если аттрибут целочисленный, ложь если дробный
    static bool isInt(Code code);

signals:

public slots:

private:
    Code _code;
    QString _name;
    int _colNum;
    Attribute::Type _type;
    QVariant _value;


    QColor _atrColor;
    QString genDisplayName(Code code);
    int genColumnNumber(Code code);
    Attribute::Type genAttrType(Code code);
};

Q_DECLARE_METATYPE(Attribute);

bool operator ==(const Attribute &at1, const Attribute &at2);
QDebug operator <<(QDebug dbg, const Attribute &at);
#endif // ATTRIBUTE_H
