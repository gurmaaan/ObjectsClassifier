#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractSlider>
#include <QResizeEvent>

#ifndef EXTRA_PATH
#define EXTRA_PATH QString("ObjectsClassifier/images")
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new DataModel(this);
    viewer = ui->graphicsView;
    scene = new QGraphicsScene();
    viewer->setScene(scene);

    connect(model, SIGNAL(pathLoaded(QString)), ui->img_path_lineEdit, SLOT(setText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateViewer(const QPixmap &image, bool aspectRatio)
{
    scene->clear();
    viewer->update();

    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(image);
    scene->addItem(item);

    viewer->setAlignment(Qt::AlignCenter);

    Qt::AspectRatioMode mode = aspectRatio ? Qt::IgnoreAspectRatio : Qt::KeepAspectRatioByExpanding;
    viewer->fitInView( scene->sceneRect(), mode);
    viewer->scale(ui->zoomSpinbox->value(), ui->zoomSpinbox->value() );
    viewer->show();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   updateViewer(model->pixmap(), ui->keepRatioBut->isChecked());
}

void MainWindow::on_openDataBut_toggled(bool checked)
{
    if (!ui->openDataBut->isChecked())
        ui->openDataAct->triggered(checked);
}

void MainWindow::on_openImgBut_toggled(bool checked)
{
    if (!ui->openImgBut->isChecked())
        ui->openImgAct->triggered(checked);

}

void MainWindow::on_openAttrBut_toggled(bool checked)
{
    if (!ui->openAttrBut->isChecked())
        ui->openAtrAc->triggered(checked);
}

//Actions
void MainWindow::on_openDataAct_triggered(bool checked)
{
    if (ui->openDataAct->isChecked())
        ui->openDataBut->setChecked(checked);
}

void MainWindow::on_openImgAct_triggered(bool checked)
{
    if (ui->openImgAct->isChecked())
        ui->openImgBut->setChecked(checked);

    QDir dir = QDir::current();
    dir.cdUp();
    dir.cd(EXTRA_PATH);
    QString path = (dir.exists()) ?  dir.absolutePath() : QString(QStandardPaths::PicturesLocation);
    if(dir.exists())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), path);
         if (!fileName.isEmpty())
         {
             QImage image(fileName);
             if (image.isNull())
             {
                 QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
                 return;

             } else
             {
                 model->setImage(fileName);
                 updateViewer( model->pixmap(), ui->ignoreRatioBut->isChecked() );
                 ui->openImgAct->setChecked(true);
                 ui->openImgBut->setChecked(true);
             }
         } else
         {
            qDebug() << "Directory " << dir.absolutePath() << " + " <<  EXTRA_PATH << "doesn't exist";
         }
    }
}

void MainWindow::on_openAtrAc_triggered(bool checked)
{
    if (ui->openAtrAc->isChecked())
        ui->openAttrBut->setChecked(checked);

}

void MainWindow::on_closeAct_triggered(bool checked)
{
    if (checked)
    {
        ui->openDataAct->setChecked(checked);
        ui->openImgBut->setChecked(checked);
    }
}

void MainWindow::on_zoomInAct_triggered()
{
    updateViewer(model->pixmap(), false);
}


void MainWindow::on_zoomOutAct_triggered()
{
    updateViewer(model->pixmap(), true);
}

void MainWindow::on_keepRatioBut_toggled(bool checked)
{
    if (! ui->img_path_lineEdit->text().isEmpty() )
    {
        QImage keepImg(ui->img_path_lineEdit->text());
        updateViewer(QPixmap::fromImage(keepImg), checked);
    }
}

void MainWindow::on_zoomRatioSlider_sliderMoved(int position)
{
    double normilizeVal = (double) position / 100;
    double newSpinVal = 0;

    qDebug() << position << ui->zoomRatioSlider->value() << normilizeVal;
    if (ui->zoomRatioSlider->value() > 0)
    {
        newSpinVal = (double) 1 + normilizeVal;
        ui->zoomInAct->trigger();
    } else {
        newSpinVal = (double) 1 + normilizeVal;
        ui->zoomOutAct->trigger();
    }
    ui->zoomSpinbox->setValue(newSpinVal);
}
