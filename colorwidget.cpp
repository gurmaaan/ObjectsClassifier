#include "colorwidget.h"
#include "ui_colorwidget.h"

#ifndef CLR_PCKR_STYLE
#define CLR_PCKR_STYLE "border: 1px solid black; background-color: "
#endif

ColorWidget::ColorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorWidget)
{
    ui->setupUi(this);
    _min = 0.0;
    _max = 1.0;
    _color = QColor(Qt::white);

    updateButtonStyleSheet(QColor(Qt::white));
    setVal(getVal());
    emit colorChenged(_color);
}

ColorWidget::~ColorWidget()
{
    delete ui;
}

void ColorWidget::setTitle(const QString title)
{
    _title = title;
    ui->groupBox->setTitle(_title);
}

void ColorWidget::setParamName(const QString name)
{
    _paramName = name;
    ui->colorLabel->setText(name);
}

void ColorWidget::setMinVal(const double min)
{
    _min = min;
    ui->minLabel->setText(QString::number(min));
    int sliderMin;
    sliderMin = static_cast<int>(min*100);
    ui->slider->setMinimum(sliderMin);
}

void ColorWidget::setMaxVal(const double max)
{
    _max = max;
    ui->maxLabel->setText(QString::number(max));
    int sliderMax;
    sliderMax = static_cast<int>(max*100);
    ui->slider->setMaximum(sliderMax);
}

void ColorWidget::on_groupBox_toggled(bool checked)
{
    double val = checked ? getMin() : getMax();
    setVal(val);
    ui->slider->setEnabled(checked);
    ui->colroButton->setEnabled(checked);
}

void ColorWidget::on_colroButton_clicked()
{
    QColor clr = QColorDialog::getColor(_color, this);
    clr.setAlphaF(getVal());
    _color = clr;
    updateButtonStyleSheet(_color);
    emit colorChenged(clr);
}

QColor ColorWidget::getColor() const
{
    return _color;
}

double ColorWidget::getMax() const
{
    return _max;
}

double ColorWidget::getMin() const
{
    return _min;
}

double ColorWidget::getVal()
{
    static double sliderVal = static_cast<double>(ui->slider->value()) / static_cast<double>(100);
    setVal(sliderVal);
    return _val;
}

void ColorWidget::setVal(double val)
{
    _val = val;
    emit colorChenged(getColor());
    int sliderVal;
    sliderVal = static_cast<int>(val * 100);
    _color.setAlpha(val);
    ui->slider->setValue(sliderVal);

    emit(colorChenged(_color));
}

void ColorWidget::updateButtonStyleSheet(QColor newButtonCOlor)
{
    //BUG: разобраться с этой прозрачностью черт знает что во всем файле
    QString bgStyleSheet = QString(CLR_PCKR_STYLE) + newButtonCOlor.name();
    ui->colroButton->setStyleSheet(bgStyleSheet);
}

void ColorWidget::on_slider_sliderMoved(int position)
{
    //static qreal sliderVal = static_cast<qreal>(static_cast<qreal>(position) / static_cast<qreal>(100));
    qDebug() << position <<_min << _max;
    _color.setAlpha(position);
    emit(colorChenged(_color));
}
