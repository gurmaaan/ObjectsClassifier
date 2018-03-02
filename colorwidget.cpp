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
    _min = 0;
    _max = 255;
    _color = QColor(Qt::white);

    updateButtonStyleSheet(QColor(Qt::white));
    setAlpha(getAlpha());
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

void ColorWidget::setMinAlpha(const int min)
{
    _min = min;
    ui->minLabel->setText(QString::number(_min));
    ui->slider->setMinimum(_min);
}

void ColorWidget::setMaxAlpha(const int max)
{
    _max = max;
    ui->maxLabel->setText(QString::number(max));
    ui->slider->setMaximum(max);
}

void ColorWidget::on_groupBox_toggled(bool checked)
{
    int val = checked ? getMinAlpha() : getMaxAlpha();
    setAlpha(val);
    ui->slider->setEnabled(checked);
    ui->colroButton->setEnabled(checked);
}

void ColorWidget::on_colroButton_clicked()
{
    QColor clr = QColorDialog::getColor(_color, this);
    clr.setAlpha(ui->slider->value());
    setColor(clr);
    updateButtonStyleSheet(_color);
}

QColor ColorWidget::getColor() const
{
    return _color;
}

void ColorWidget::setColor(QColor value)
{
    _color = value;
    emit colorChenged(_color);
    qDebug() << "Color changed. (R ; G ; B ; A) : ("
             << _color.red()    << " ; "
             << _color.green()  << " ; "
             << _color.blue()   << " ; "
             << _color.alpha()  << " ).";
}

int ColorWidget::getMaxAlpha() const
{
    return _max;
}

int ColorWidget::getMinAlpha() const
{
    return _min;
}

int ColorWidget::getAlpha()
{
    return _alpha;
}

void ColorWidget::setAlpha(int val)
{
    if ( val >= 0 && val <=255)
    {
        _alpha = val;
        ui->slider->setValue(_alpha);
        _color.setAlpha(_alpha);
        emit(colorChenged(_color));
    } else {
        _alpha = 0;
        qDebug() << "Incorrect alpha value. Should be 0 for invisible and 255 for the color without opacity";
    }
}

void ColorWidget::updateButtonStyleSheet(QColor newButtonCOlor)
{
    QString bgStyleSheet = QString(CLR_PCKR_STYLE) + newButtonCOlor.name();
    ui->colroButton->setStyleSheet(bgStyleSheet);
}

void ColorWidget::on_slider_sliderMoved(int position)
{
    _color.setAlpha(position);
    setColor(_color);
    emit(colorChenged(_color));
}

bool ColorWidget::isChecked()
{
    return _checked && ui->groupBox->isChecked();
}

void ColorWidget::setChecked(bool checked)
{
    ui->groupBox->setChecked(checked);
    _checked = checked;
}
