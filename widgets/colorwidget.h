#ifndef COLORWIDGET_H
#define COLORWIDGET_H

#include <QWidget>
#include <QColor>
#include <QRgb>
#include <QColorDialog>
#include <QDebug>

namespace Ui {
class ColorWidget;
}

class ColorWidget : public QWidget
{
    Q_OBJECT

public:
    //0 = полностью прозрачный объект
    //255 = абсолютно непрозрачный (видно в оригинале)
    explicit ColorWidget(QWidget *parent = 0);
    ~ColorWidget();

    void setTitle(const QString title);
    void setParamName(const QString name);


    void setMinAlpha(const int min);
    int getMinAlpha() const;

    void setMaxAlpha(const int max);
    int getMaxAlpha() const;

    QColor getColor() const;
    void setColor(QColor value);

    int getAlpha();
    void setAlpha(int val);

    bool isChecked();
    void setChecked(bool checked);

signals:
    void viewStateChanged(bool state);
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
    bool _checked;

    int _min;
    int _max;
    int _alpha;

    void updateButtonStyleSheet(QColor newButtonCOlor);

};

#endif // COLORWIDGET_H
