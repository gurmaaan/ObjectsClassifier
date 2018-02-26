#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QAbstractSlider>
#include <QResizeEvent>

//TODO: Виджет цвет + слайдер прозрачности
#ifndef EXTRA_PIC_PATH
#define EXTRA_PIC_PATH QString("ObjectsClassifier/images")
#endif

#ifndef EXTRA_DAT_PATH
#define EXTRA_DAT_PATH QString("ObjectsClassifier/data")
#endif

#define TFS ";;"
//по названию (Отсеивание конкретно этих типов среди остальных )
#define N_F_PNG QString("PNG Images (*.png)")
#define N_F_BMP QString("Bitmap Image (*.bmp)")
#define N_F_JPG QString("JPG Images (*.jpg)")
#define N_F_TIF QString("TIF Images (*.tif)")
#define N_F_ALL (N_F_BMP + TFS + N_F_JPG + TFS + N_F_PNG + TFS + N_F_TIF)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new DataModel(this);
    setScaleCoeff(1.0);
    viewer = ui->graphicsView;
    scene = new QGraphicsScene();
    viewer->setScene(scene);

    connect(model, SIGNAL(pathImgLoaded(QString)), ui->img_path_lineEdit, SLOT(setText(QString)));
    connect(model, SIGNAL(pathDataLoaded(QString)), ui->dataPathLineEdit, SLOT(setText(QString)));
    connect(model, SIGNAL(objCountChanged(int)), ui->ojectsCountSpinBox, SLOT(setValue(int)));
    connect(model, SIGNAL(incrementProgress(int)), ui->progressBar, SLOT(setValue(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateViewer(const QPixmap &pixmap, double scaleCoeff, Qt::AspectRatioMode mode)
{
    scene->clear();
    viewer->update();

    int oldW = pixmap.size().width();
    int oldH = pixmap.size().height();

    double newW = static_cast<double>(oldW) * scaleCoeff;
    double newH = static_cast<double>(oldH) * scaleCoeff;

    int w = static_cast<int>(newW);
    int h = static_cast<int>(newH);

    QPixmap newPixMap(pixmap.scaled(QSize(w, h), mode));
    QGraphicsPixmapItem *pItem = scene->addPixmap(newPixMap);
    pItem->setPos(0, 0);

    //ui->graphicsView->fitInView(QRectF(scene->width() / 2, scene->height() / 2, w, h), mode);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   double resizedCoeff = static_cast<double>(model->pixmap().width()) / static_cast<double>(scene->width());
   updateViewer(model->pixmap(), resizedCoeff, checkedRatio());
}

void MainWindow::on_openImgBut_toggled(bool checked)
{
    ui->openImgAct->triggered(checked);
}

void MainWindow::on_openDataBut_toggled(bool checked)
{
    ui->openDataAct->triggered(checked);
}

void MainWindow::on_openAttrBut_toggled(bool checked)
{
    ui->openAttrAct->triggered(checked);
}

void MainWindow::on_closeBut_clicked()
{
    ui->closeAct->triggered(true);
}

//Дейстьвие закрытия
void MainWindow::on_closeAct_triggered(bool checked)
{
    updateAccessState(ui->closeAct, ui->closeBut, !checked);
    updateAccessState(ui->openImgAct, ui->openImgBut, checked);
    updateAccessState(ui->openDataAct, ui->openDataBut, !checked);
    updateAccessState(ui->openAttrAct, ui->openAttrBut, !checked);

    ui->tabWidget->setCurrentIndex(0);

    QPixmap pixmap(ui->graphicsView->width(), ui->graphicsView->height());
    pixmap.fill(QColor(Qt::white));
    updateViewer(pixmap, 0, Qt::IgnoreAspectRatio);
}

//Открытие самого изображения
void MainWindow::on_openImgAct_triggered(bool checked)
{
    updateAccessState(ui->openImgAct, ui->openImgBut, !checked, checked);
    updateAccessState(ui->openDataAct, ui->openDataBut, checked);
    updateAccessState(ui->closeAct, ui->closeBut, checked);

    ui->tabWidget->setCurrentIndex(0);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Select image file"), requiredPath(QDir::current(), EXTRA_PIC_PATH), N_F_ALL);
    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        if (image.isNull())
        {
             QMessageBox::information(this, tr("Image Viewer"), tr("Cannot load %1.").arg(fileName));
             return;
        }
        else
        {
            updateViewer(QPixmap(fileName), ui->zoomSpinbox->value(), checkedRatio());
            model->setImage(fileName);
            ui->view_dock->setEnabled(true);

            ui->tabWidget->setCurrentIndex(0);
        }
     }

}
//Действие открытия файла с точками
void MainWindow::on_openDataAct_triggered(bool checked)
{
    updateAccessState(ui->openDataAct, ui->openDataBut, !checked, checked);
    updateAccessState(ui->openAttrAct, ui->openAttrBut, checked);

    ui->tabWidget->setCurrentIndex(1);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Select data file"), requiredPath(QDir::current(), EXTRA_DAT_PATH), "DAT(*.dat)");
    if (!fileName.isEmpty())
    {
        model->loadObjectsData(fileName);
        if (model->dataFilePath() != fileName || model->objCount() == 0)
        {
            QMessageBox::information(this, tr("Error"), tr("Cannot load %1.").arg(fileName));
            return;

        } else
        {
            ui->progressBar->setMaximum(model->objCount());

            ui->treeView->setModel( model->getStandardItemtModel() );
            ui->treeView->resizeColumnToContents(0);
            ui->treeView->resizeColumnToContents(1);

            ui->progressBar->setValue(model->objCount());
        }
    }
}

//Файл атрибутов
void MainWindow::on_openAttrAct_toggled(bool arg1)
{
    //TODO: загиузка csv файла
}

//Скейлинг и прочие действия с зумом
void MainWindow::on_zoomRatioSlider_sliderMoved(int position)
{
    double sameNonIntPos = static_cast<double>(position) / static_cast<double>(100);

    int compare = static_cast<int>(position - scaleCoeff() * 100);

    if (compare != 0)
    {
        setScaleCoeff(sameNonIntPos);
        ui->zoomSpinbox->setValue(sameNonIntPos);
    }
}

double MainWindow::scaleCoeff() const
{
    return _scaleCoeff;
}

void MainWindow::setScaleCoeff(double newScaleCoeff)
{
    _scaleCoeff = newScaleCoeff;
}

void MainWindow::on_zoomSpinbox_valueChanged(double newScaleCoeff)
{
    int compare = static_cast<int>(newScaleCoeff - scaleCoeff());
    if (compare != 0)
        setScaleCoeff(newScaleCoeff);

    updateViewer(model->pixmap(), scaleCoeff(), checkedRatio());
}

void MainWindow::on_keepRatioBut_toggled(bool checked)
{
    if(checked)
        updateViewer(model->pixmap(), ui->zoomSpinbox->value(), checkedRatio());
}

void MainWindow::on_ignoreRatioBut_toggled(bool checked)
{
    if(checked)
        updateViewer(model->pixmap(), ui->zoomSpinbox->value(), checkedRatio());

}

QString MainWindow::requiredPath(QDir currentDir, const QString &redirect)
{
    currentDir.cdUp();
    currentDir.cd(redirect);
    QString path = (currentDir.exists()) ?  currentDir.absolutePath() : QString(QStandardPaths::PicturesLocation);

    if(currentDir.exists())
    {
        return path;
    } else
    {
       qDebug() << "Directory " << currentDir.absolutePath() << " + " <<  redirect << "doesn't exist";
       return "";
    }
}

void MainWindow::updateAccessState(QAction *ac, QPushButton *bt, bool newEnableState, bool newCheckedState)
{
    ac->setEnabled(newEnableState);
    bt->setEnabled(newEnableState);

    ac->setChecked(newCheckedState);
    bt->setChecked(newCheckedState);
}

void MainWindow::updateAccessState(QAction *ac, QRadioButton *bt, bool newEnableState, bool newCheckedState)
{
    ac->setEnabled(newEnableState);
    bt->setEnabled(newEnableState);

    ac->setChecked(newCheckedState);
    bt->setChecked(newCheckedState);
}

Qt::AspectRatioMode MainWindow::checkedRatio()
{
    if (ui->keepRatioBut->isChecked())
    {
        updateAccessState(ui->keepRatioAct, ui->keepRatioBut, true, true);
        updateAccessState(ui->ignoreRatioAct, ui->ignoreRatioBut, true, false);
        return Qt::KeepAspectRatio;
    } else if(ui->ignoreRatioBut->isChecked())
    {
        updateAccessState(ui->ignoreRatioAct, ui->ignoreRatioBut, true, true);
        updateAccessState(ui->keepRatioAct, ui->keepRatioBut, true, false);
        return Qt::IgnoreAspectRatio;
    }
}

void MainWindow::on_zoomInAct_triggered()
{
    double newCoeff = scaleCoeff() - 0.05;
    setScaleCoeff(newCoeff);
    ui->zoomSpinbox->setValue(newCoeff);
}

void MainWindow::on_zoomOutAct_triggered()
{
    double newCoeff = scaleCoeff() + 0.05;
    setScaleCoeff(newCoeff);
    ui->zoomSpinbox->setValue(newCoeff);
}

void MainWindow::on_zoomRatioSlider_valueChanged(int value)
{
    double sameNonIntVal = static_cast<double>(value) / static_cast<double>(100);

    int compare = static_cast<int>(sameNonIntVal - scaleCoeff());

    if (compare != 0)
    {
        setScaleCoeff(sameNonIntVal);
        ui->zoomSpinbox->setValue(sameNonIntVal);
    }
}

