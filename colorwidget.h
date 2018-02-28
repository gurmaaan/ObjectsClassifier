#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QColor>
#include <QColorDialog>
#include <QDebug>

namespace Ui {
class ColorWidget;
}

class ColorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ColorWidget(QWidget *parent = 0);
    ~ColorWidget();

    void setTitle(const QString title);
    void setParamName(const QString name);

    void setMinVal(const double min);
    double getMin() const;

    void setMaxVal(const double max);
    double getMax() const;

    QColor getColor() const;
    void setColor(QColor value);

    double getVal();
    void setVal(double val);

signals:
    void colorChenged(QColor color);

private slots:
    void on_groupBox_toggled(bool checked);
    void on_colroButton_clicked();
    void on_slider_sliderMoved(int position);

private:
    Ui::ColorWidget *ui;

    QColor _color;
    QString _title;
    QString _paramName;

    double _min;
    double _max;
    double _val;

    void updateButtonStyleSheet(QColor newButtonCOlor);

};

#endif // COLORWIDGET_H
