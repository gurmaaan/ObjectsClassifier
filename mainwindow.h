#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDir>

#include "datamodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateViewer(const QPixmap &image, bool aspectRatio);


    void resizeEvent(QResizeEvent *event);

private slots:
    void on_openDataBut_toggled(bool checked);
    void on_openImgBut_toggled(bool checked);
    void on_openAttrBut_toggled(bool checked);
    void on_openDataAct_triggered(bool checked);
    void on_openImgAct_triggered(bool checked);
    void on_openAtrAc_triggered(bool checked);    
    void on_closeAct_triggered(bool checked);
    void on_zoomInAct_triggered();
    void on_zoomOutAct_triggered();
    void on_keepRatioBut_toggled(bool checked);
    void on_zoomRatioSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *viewer;
    DataModel *model;
};

#endif // MAINWINDOW_H
