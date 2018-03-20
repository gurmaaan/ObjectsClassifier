#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>
#include <external/qcustomplot.h>
#include <attribute.h>

namespace Ui {
class HistogramWidget;
}

class HistogramWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HistogramWidget(QWidget *parent = 0);

    ~HistogramWidget();


    QString name() const;
    void setName(const QString &name);

    QVariant min() const;
    void setMin(QVariant min);

    QVariant max() const;
    void setMax(QVariant max);

    QVariant av() const;
    void setAv(QVariant av);

    QVector<QVariant> array() const;
    void setArray(const QVector<QVariant> &array, Code code);

    QColor chartColor() const;
    void setChartColor(const QColor &chartColor);

    Code code() const;
    void setCode(const Code &code);

    int count() const;
    void setCount(int count);

public slots:
   // void updateType(Attribute::Type newTypeCode);
    void updateName(QString newName);
    void updateColor(QColor newColor);

signals:
    void minChanged(QVariant newMin);
    void maxChanged(QVariant newMmax);
    void countChanged(int newCount);
    void avChanged(QVariant newAv);
    void nameChanged(QString newName);
    void typeChanged(Attribute::Type newTypeCode);
    void codeChanged(Code newCode);
    void chartColorChanged(QColor newColor);

private:
    Ui::HistogramWidget *ui;

    template<typename T>
    T calcMin(const QVector<T> &array)
    {
        T min = array.first();
        for(auto elem : array)
            if ( elem < min)
                min = elem;

        return min;
    }

    template<typename T>
    T calcMax(const QVector<T> &array)
    {
        T max = array.first();
        for(auto elem : array)
            if (elem > max)
                max = elem;

        return max;
    }

//    template<typename T>
//    T calcAv(const QVector<T> &array)
//    {
//        T av = 0;
//        T sum = 0;
//        for(auto elem : array)
//            sum += elem;

//        av = static_cast<T>( sum/(array.size()) );
//        return av;
//    }

    QString _name;

    Code _code;

    QVariant _min;
    QVariant _max;
    QVariant _av;
    int _count;

    QColor _chartColor;

    QVector<QVariant> _array;

    QCPBars *bar;
    void initHist();
    void connectAll();
    void setupDefaults();
};

#endif // HISTOGRAMWIDGET_H
