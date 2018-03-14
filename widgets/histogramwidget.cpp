#include "histogramwidget.h"
#include "ui_histogramwidget.h"

HistogramWidget::HistogramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogramWidget)
{
    ui->setupUi(this);
}

HistogramWidget::~HistogramWidget()
{
    delete ui;
}

QString HistogramWidget::name() const
{
    return _name;
}

void HistogramWidget::setName(const QString &name)
{
    _name = name;
}

int HistogramWidget::min() const
{
    return _min;
}

void HistogramWidget::setMin(int min)
{
    _min = min;
}

int HistogramWidget::max() const
{
    return _max;
}

void HistogramWidget::setMax(int max)
{
    _max = max;
}

int HistogramWidget::av() const
{
    return _av;
}

void HistogramWidget::setAv(int av)
{
    _av = av;
}

QVector<int> HistogramWidget::array() const
{
    return _array;
}

void HistogramWidget::setArray(const QVector<int> &array)
{
    _array = array;
}
