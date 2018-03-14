#ifndef HISTOGRAMWIDGET_H
#define HISTOGRAMWIDGET_H

#include <QWidget>

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

    int min() const;
    void setMin(int min);

    int max() const;
    void setMax(int max);

    int av() const;
    void setAv(int av);

    //void setIntArray(QVector)

    QVector<int> array() const;
    void setArray(const QVector<int> &array);

private:
    Ui::HistogramWidget *ui;

    QString _name;

    int _min;
    int _max;
    int _av;
    int _count;

    int calcMin(const QVector<int> &array);

    QVector<int> _array;
};

#endif // HISTOGRAMWIDGET_H
