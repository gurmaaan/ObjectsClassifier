#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "const.h"

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

    initColorWidgets();
    showMaximized();

    //TODO : сделать активной первую табу с масштабом
    tabifyDockWidget(ui->dock_scale, ui->dock_object);
    tabifyDockWidget(ui->dock_scale, ui->dock_contour);

    connect(model, &DataModel::pathImgLoaded, ui->img_path_lineEdit, &QLineEdit::setText);
    connect(model, &DataModel::pathDataLoaded, ui->dataPathLineEdit, &QLineEdit::setText);
    connect(model, &DataModel::objCountChanged, ui->dataObjectsCountSpinBox, &QSpinBox::setValue);
    connect(model, &DataModel::incrementProgress, ui->dataFileProgressBar, &QProgressBar::setValue);

    connect(ui->objectColorWidget, &ColorWidget::viewStateChanged, model, &DataModel::setObjectsVisible);
    connect(ui->objectColorWidget, &ColorWidget::colorChenged, this, &MainWindow::updateObjColor);
    connect(ui->contourColorWidget, &ColorWidget::colorChenged, this, &MainWindow::updateContourColor);

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

    //FIXME: деление на объект - костыль тк ресайзилось при увелечение коэффициента и тд
    double newW = static_cast<double>(oldW) / scaleCoeff;
    double newH = static_cast<double>(oldH) / scaleCoeff;

    int w = static_cast<int>(newW);
    int h = static_cast<int>(newH);

    //QPixmap newPixMap(pixmap.scaled(QSize(w, h), mode));
    QPixmap newPixMap(pixmap);
    QGraphicsPixmapItem *pItem = scene->addPixmap(newPixMap);
    pItem->setPos(0, 0);

    viewer->fitInView(QRectF(scene->width() / 2, scene->height() / 2, w, h), mode);
    viewer->setScene(scene);
    viewer->show();
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

//Действие закрытия
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
    updateAccessState(ui->openImgAct, ui->openImgBut, false, true);
    updateAccessState(ui->openDataAct, ui->openDataBut, true);
    updateAccessState(ui->closeAct, ui->closeBut, true);

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
            ui->dock_scale->setEnabled(true);
            ui->imageSizeWidget->setImage(QImage(fileName));
            ui->tabWidget->setCurrentIndex(0);
        }
     }

}
//Действие открытия файла с точками
void MainWindow::on_openDataAct_triggered(bool checked)
{
    //TODO: добавить иконки в качестве части изображения
    updateAccessState(ui->openDataAct, ui->openDataBut, false, true);
    updateAccessState(ui->openAttrAct, ui->openAttrBut, true);
    ui->dock_contour->setEnabled(true);
    ui->dock_object->setEnabled(true);
    ui->tabWidget->setCurrentIndex(1);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select data file"),
                                                    requiredPath(QDir::current(),
                                                    EXTRA_DAT_PATH), "DAT(*.dat)");
    if (!fileName.isEmpty())
    {
        model->loadObjectsData(fileName);
        if (model->dataFilePath() != fileName || model->objCount() == 0)
        {
            QMessageBox::information(this, tr("Error"), tr("Cannot load %1.").arg(fileName));
            return;

        } else
        {
            ui->dataFileProgressBar->setMaximum(model->objCount());

            ui->tree_data->setModel( model->getStandardItemtModel() );
            ui->tree_data->resizeColumnToContents(0);
            ui->tree_data->resizeColumnToContents(1);

            ui->dataFileProgressBar->setValue(model->objCount());
        }
    }
}

void MainWindow::initColorWidgets()
{
    ui->objectColorWidget->setChecked(false);
    ui->objectColorWidget->setColor(Qt::green);

    ui->contourColorWidget->setChecked(false);
    ui->contourColorWidget->setColor(Qt::red);
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

//При любом изменении цвета виджета происходит перерисовка полигона на сцене
//За то отображается и впринципе объект отвечает прозрачность
//
void MainWindow::updateObjColor(QColor clr)
{

}

void MainWindow::updateContourColor(QColor clr)
{
    //qDebug() << "Updated contour Color: " << clr << ui->contourColorWidget->getColor();
}

void MainWindow::on_zoomSpinbox_valueChanged(double newScaleCoeff)
{
    //BUG: когда меняется содержимое spin box'a , слайдер не двигается
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
        //TODO: Опитимизировать изменение enabled \ disabled  интерфейса (см метод ниже)
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
    //TODO: Опитимизировать выбор типа ресайзинга картинки внутри виджета
    Qt::AspectRatioMode mode;
    if (ui->keepRatioBut->isChecked())
    {
        updateAccessState(ui->keepRatioAct, ui->keepRatioBut, true, true);
        updateAccessState(ui->ignoreRatioAct, ui->ignoreRatioBut, true, false);
        mode =  Qt::KeepAspectRatio;
    } else if( ui->ignoreRatioBut->isChecked())
    {
        updateAccessState(ui->ignoreRatioAct, ui->ignoreRatioBut, true, true);
        updateAccessState(ui->keepRatioAct, ui->keepRatioBut, true, false);
        mode =  Qt::IgnoreAspectRatio;
    }

    return mode;
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

//Файл атрибутов - парсинг .csv файла
//TODo : пренести в файл модели
void MainWindow::on_openAttrAct_triggered(bool checked)
{
    updateAccessState(ui->openAttrAct, ui->openAttrBut, !checked, checked);

    ui->tabWidget->setCurrentIndex(2);
    csvModel = new QStandardItemModel(this);
    csvModel->setColumnCount(18);
    //TODO: добавить заголовки из проксирования
    //csvModel->setHorizontalHeaderLabels(QStringList() << "Марка" << "Модель" << "Цена");
    ui->attrTbleView->setModel(csvModel);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Select attributes file"), requiredPath(QDir::current(), EXTRA_ATTR_PATH), "CSV(*.csv)");

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << file.errorString();
    }
    else {
        QTextStream in(&file);
        // Считываем данные до конца файла
        while (!in.atEnd())
        {
        // ... построчно
            QString line = in.readLine();
            QList<QStandardItem *> standardItemsList;
            for (QString item : line.split(";"))
            {
                standardItemsList.append(new QStandardItem(item));
            }
            csvModel->insertRow(csvModel->rowCount(), standardItemsList);
            file.close();
        }
    }
}

void MainWindow::on_contourWidthSlider_sliderMoved(int position)
{
    //TODO: Связать со слотом объекта на обновление ширины контура
}

void MainWindow::on_pushButton_clicked()
{
    foreach (auto objOnImg, model->getObjectsOnImage())
    {
        scene->addPixmap(objOnImg.getContourPixmap());
    }
}
