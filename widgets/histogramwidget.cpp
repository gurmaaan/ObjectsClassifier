#include "histogramwidget.h"
#include "ui_histogramwidget.h"

HistogramWidget::HistogramWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HistogramWidget)
{
    ui->setupUi(this);

    connectAll();

    setupDefaults();

    initHist();
}

HistogramWidget::~HistogramWidget()
{
    delete ui;
}

QString HistogramWidget::name() const
{
    return (_name == ui->paramBox->title() ) ? _name : ui->paramBox->title();
}

void HistogramWidget::setName(const QString &name)
{
    ui->paramBox->setTitle(name);
    _name = name;
    emit nameChanged(name);
}

QVariant HistogramWidget::min() const
{
    return _min;
}

void HistogramWidget::setMin(QVariant min)
{
    _min = min;
    emit minChanged(_min);
}

QVariant HistogramWidget::max() const
{
    return _max;
}

void HistogramWidget::setMax(QVariant max)
{
    _max = max;
    emit maxChanged(_max);
}

QVariant HistogramWidget::av() const
{
    return _av;
}

void HistogramWidget::setAv(QVariant av)
{
    _av = av;
    emit avChanged(_av);
}

QVector<QVariant> HistogramWidget::array() const
{
    return _array;
}

//Основной метод
void HistogramWidget::setArray(const QVector<QVariant> &array, Code code)
{
    _array = array;
    setCount(array.count());
    emit countChanged(_count);

    setCode(code);

    setMin( calcMin(array) );
    setMax( calcMax(array) );

    //setAv( calcAv(array) );
    setAv( QVariant(10));
}

QColor HistogramWidget::chartColor() const
{
    return _chartColor;
}

void HistogramWidget::setChartColor(const QColor &chartColor)
{
    _chartColor = chartColor;
    emit chartColorChanged(chartColor);
}

Code HistogramWidget::code() const
{
    return _code;
}

void HistogramWidget::setCode(const Code &code)
{
    _code = code;
    emit codeChanged(code);
}

int HistogramWidget::count() const
{
    return _count;
}

void HistogramWidget::setCount(int count)
{
    _count = count;
    emit countChanged(_count);
}

void HistogramWidget::updateName(QString newName)
{
    ui->paramBox->setTitle(newName);
    bar->setName(newName);
}

void HistogramWidget::updateColor(QColor newColor)
{
    bar->setBrush(QBrush(newColor));
    bar->setPen( QPen( newColor.dark()) );
}

void HistogramWidget::initHist()
{
    ui->histogram->setBackground(QBrush(QColor(255,255,255)));
    bar = new QCPBars(ui->histogram->xAxis, ui->histogram->yAxis);
    bar->setAntialiased(false); //TODO:: check whats happend in case true

}

void HistogramWidget::connectAll()
{
    //Общее количсество объектов
    connect(this, &HistogramWidget::countChanged,
            ui->countAllSpin, &QSpinBox::setValue);

    connect(this, &HistogramWidget::nameChanged,
            ui->paramBox, &QGroupBox::setTitle);

    //-----Значения общего диапазона
//    connect(this, &HistogramWidget::avChanged,
//            ui->valAvSpin, &QSpinBox::setValue);

//    connect(this, &HistogramWidget::minChanged,
//            ui->valMinSpin, &QSpinBox::setValue);

//    connect(this, &HistogramWidget::maxChanged,
//            ui->valMaxSpin, &QSpinBox::setValue);


}

void HistogramWidget::setupDefaults()
{
    setCode(Code::NonAssigned);

    QString newName = Attribute::displayNameOf(Code::NonAssigned);
    setName(newName);
    setMin(0);
    setMax(0);

    setAv(0);
    setCount(0);

    QColor newColor = Attribute::colorOf(Code::NonAssigned);
    setChartColor(newColor);
}
