#ifndef IMAGESIZEVIEWER_H
#define IMAGESIZEVIEWER_H

#include <QWidget>

namespace Ui {
class ImageSizeViewer;
}

class ImageSizeViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageSizeViewer(QWidget *parent = 0);
    ~ImageSizeViewer();

    void setImage(const QImage &image);

private:
    Ui::ImageSizeViewer *ui;
    int _h;
    int _w;
    long int _total;

    void setH(int h);
    void setW(int w);
    void setTotal(long total);
};

#endif // IMAGESIZEVIEWER_H
