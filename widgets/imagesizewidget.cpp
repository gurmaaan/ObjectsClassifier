#include "imagesizewidget.h"
#include "ui_imagesizewidget.h"

ImageSizeWidget::ImageSizeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageSizeWidget)
{
    ui->setupUi(this);
}

ImageSizeWidget::~ImageSizeWidget()
{
    delete ui;
}

void ImageSizeWidget::setImage(const QImage &image)
{
    int width = image.size().width();
    int height = image.size().height();
    setW(width);
    setH(height);
    setTotal(static_cast<long int>(width * height));
}

void ImageSizeWidget::setH(int h)
{
    _h = h;
    ui->hSpin->setValue(h);
}

void ImageSizeWidget::setW(int w)
{
    _w = w;
    ui->wSpin->setValue(w);
}

void ImageSizeWidget::setTotal(long total)
{
    _total = total;
    ui->totalSpin->setValue(total);
}
