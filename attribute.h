#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QObject>

//TODO:1 - переименовать понятие атрибут в десккриптор

class Attribute : public QObject
{
    Q_OBJECT
public:
    enum Code
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
        GeomPerim
    };

    Attribute(Code code);
    Attribute(const int code);

    
signals:

public slots:

protected:
    static QString displayName;

private:
    Code _code;
    QString _name;
    int numInCsv;
};

#endif // ATTRIBUTE_H
