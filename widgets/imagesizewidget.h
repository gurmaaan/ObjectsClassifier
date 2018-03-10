#ifndef IMAGESIZEWIDGET_H
#define IMAGESIZEWIDGET_H

#include <QWidget>

namespace Ui {
class ImageSizeWidget;
}

class ImageSizeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageSizeWidget(QWidget *parent = 0);
    ~ImageSizeWidget();

    void setImage(const QImage &image);

private:
    Ui::ImageSizeWidget *ui;
    int _h;
    int _w;
    long int _total;

    void setH(int h);
    void setW(int w);
    void setTotal(long total);
};

#endif // IMAGESIZEVIEWER_H
