#include "imagesizeviewer.h"
#include "ui_imagesizeviewer.h"

ImageSizeViewer::ImageSizeViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageSizeViewer)
{
    ui->setupUi(this);
}

ImageSizeViewer::~ImageSizeViewer()
{
    delete ui;
}

void ImageSizeViewer::setImage(const QImage &image)
{
    int width = image.size().width();
    int height = image.size().height();
    setW(width);
    setH(height);
    setTotal(static_cast<long int>(width * height));
}

void ImageSizeViewer::setH(int h)
{
    _h = h;
    ui->hSpin->setValue(h);
}

void ImageSizeViewer::setW(int w)
{
    _w = w;
    ui->wSpin->setValue(w);
}

void ImageSizeViewer::setTotal(long total)
{
    _total = total;
    ui->totalSpin->setValue(total);
}
