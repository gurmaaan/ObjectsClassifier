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

    tabifyDockWidget(ui->dock_container, ui->dock_object);
    tabifyDockWidget(ui->dock_container, ui->dock_contour);
    tabifyDockWidget(ui->dock_container, ui->dock_scale);

    //line Edit'ы с путями файлом
    connect(model, &DataModel::pathImgLoaded,
            ui->img_path_lineEdit, &QLineEdit::setText);
    connect(model, &DataModel::pathDataLoaded,
            ui->dataPathLineEdit, &QLineEdit::setText);
    connect(model, &DataModel::pathAttrLoaded,
            ui->attrPathLineEdit, &QLineEdit::setText);

    //Файл точек
    connect(model, &DataModel::objCountChanged,
            ui->dataObjectsCountSpinBox, &QSpinBox::setMaximum);
    connect(model, &DataModel::objCountChanged,
            ui->dataFileProgressBar, &QProgressBar::setMaximum);
    connect(model, &DataModel::incrementProgress,
            ui->dataFileProgressBar, &QProgressBar::setValue);
    connect(model, &DataModel::incrementProgress,
            ui->dataObjectsCountSpinBox, &QSpinBox::setValue);

    //Файл аттрибутов
    connect(model, &DataModel::objCountChanged,
            ui->attrCountSpinbox, &QSpinBox::setMaximum);
    connect(model, &DataModel::objCountChanged,
            ui->attrFileProgressBar, &QProgressBar::setMaximum);
    connect(model, &DataModel::incrementAttrProgress,
            ui->attrCountSpinbox,&QSpinBox::setValue);
    connect(model, &DataModel::incrementAttrProgress,
           ui->attrFileProgressBar, &QProgressBar::setValue);

    //Цвета контура и объекта
    connect(ui->objectColorWidget, &ColorWidget::viewStateChanged,
            model, &DataModel::setObjectsVisible);
    connect(ui->objectColorWidget, &ColorWidget::colorChenged,
            this, &MainWindow::updateObjColor);
    connect(ui->contourColorWidget, &ColorWidget::colorChenged,
            this, &MainWindow::updateContourColor);

    //Совпадение файлов
    connect(model, &DataModel::dataAndAttrFilesMatch,
            ui->filesMatchCheckBox, &QCheckBox::setChecked);
    connect(model, &DataModel::dataAndAttrFilesMatch,
            model, &DataModel::synchModels);
    connect(model, &DataModel::modelUpdated,
            ui->tree_data, &QTreeView::setModel);
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

    //NOTE: деление на объект - костыль тк ресайзилось при увелечение коэффициента и тд
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
    //Запуск автоматизации открытия разных файлов
    ui->openDataAct->triggered(checked);
    ui->openAttrAct->triggered(checked);
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
void MainWindow::on_closeAct_triggered()
{
    //FIXME : ничерта не закрывает, картинка вообще так и остается
    updateAccessState(ui->closeAct, ui->closeBut, false);
    updateAccessState(ui->openImgAct, ui->openImgBut, true);
    updateAccessState(ui->openDataAct, ui->openDataBut, false);
    updateAccessState(ui->openAttrAct, ui->openAttrBut, false);

    ui->tabWidget->setCurrentIndex(0);

    QPixmap pixmap(ui->graphicsView->width(), ui->graphicsView->height());
    pixmap.fill(QColor(Qt::white));
    updateViewer(pixmap, 0, Qt::IgnoreAspectRatio);
}

//Открытие самого изображения
void MainWindow::on_openImgAct_triggered( )
{
    updateAccessState(ui->openImgAct, ui->openImgBut, false, true);
    updateAccessState(ui->openDataAct, ui->openDataBut, true);
    updateAccessState(ui->closeAct, ui->closeBut, true);

    ui->tabWidget->setCurrentIndex(0);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select image file"),
                                                    requiredPath(QDir::current(), EXTRA_PIC_PATH),
                                                    N_F_ALL);
    if (!fileName.isEmpty())
    {
        QImage image(fileName);
        if (image.isNull())
        {
             QMessageBox::information(this,
                                      tr("Image Viewer"),
                                      tr("Cannot load %1.").arg(fileName));
             return;
        }
        else
        {
            updateViewer(QPixmap(fileName), ui->zoomSpinbox->value(), checkedRatio());
            model->setImage(fileName);

            ui->dock_scale->setEnabled(true);
            ui->tabWidget->setCurrentIndex(0);
        }
     }

}
//Действие открытия файла с точками
void MainWindow::on_openDataAct_triggered( )
{
    //TODO: добавить иконки в качестве части изображения
    //(updated) поправить чтобы ровненько по центру
    updateAccessState(ui->openDataAct, ui->openDataBut, false);
    updateAccessState(ui->openAttrAct, ui->openAttrBut, true);

    ui->dock_contour->setEnabled(true);
    ui->dock_object->setEnabled(true);
    ui->dock_scale->setEnabled(true);

    ui->tabWidget->setCurrentIndex(1);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select data file"),
                                                    requiredPath(QDir::current(),EXTRA_DAT_PATH),
                                                    DAT_EXT);
    if (!fileName.isEmpty())
    {
        model->loadObjectsData(fileName);
        if (model->dataFilePath() != fileName || model->objCount() == 0)
        {
            QMessageBox::information(this,
                                     tr("Error"),
                                     tr("Cannot load %1.").arg(fileName));
            return;

        } else
        {
            ui->dataFileProgressBar->setMaximum(model->objCount());
            ui->dataObjectsCountSpinBox->setMaximum(model->objCount());
            ui->tree_data->setModel( model->getStandardItemtModel() );
            ui->tree_data->header()->setSectionResizeMode(QHeaderView::Interactive);

            foreach(auto obj, model->getObjectsOnImage())
            {
                //FIXME: Assert QLista
//                scene->addPixmap(obj.getInternalPixmap());
//                viewer->show();
            }
        }
    }
}

void MainWindow::on_openAttrAct_triggered()
{
    updateAccessState(ui->openAttrAct, ui->openAttrBut, false);

    ui->tabWidget->setCurrentIndex(2);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Select Attributes file"),
                                                    requiredPath(QDir::current(), EXTRA_ATTR_PATH),
                                                    ATTR_EXT);
    if (!fileName.isEmpty())
    {
        model->loadObjectsAttr(fileName);
        if (model->getAttrsFilePath() != fileName)
        {
            QMessageBox::critical(this,
                                  tr("Error"),
                                  tr("Cannot load %1").arg(fileName));
            return;
        } else
        {
            ui->attrTbleView->setModel( model->getAttrModel() );
            //Позволяет пользователю ресайзить
            ui->attrTbleView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
            //То в каком месте появляются точки сокращения
            ui->attrTbleView->horizontalHeader()->setTextElideMode(Qt::ElideMiddle);
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
    setScaleCoeff(sameNonIntPos);
    ui->zoomSpinbox->setValue(sameNonIntPos);
}

void MainWindow::on_zoomRatioSlider_valueChanged(int value)
{
    double sameNonIntVal = static_cast<double>(value) / static_cast<double>(100);
    setScaleCoeff(sameNonIntVal);
    ui->zoomSpinbox->setValue(sameNonIntVal);
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
void MainWindow::updateObjColor(QColor clr)
{
    qDebug() << "I would like to fix warnings with" << clr.red();
}

void MainWindow::updateContourColor(QColor clr)
{
    qDebug() << "Updated contour Color: " << clr << ui->contourColorWidget->getColor();
}

//После синхронизации файлов обновляется виджет дерева
void MainWindow::updateTreeModel(QStandardItemModel *newModel)
{
    if (newModel->rowCount() > 1)
    {
        ui->tree_data->setModel(model->getStandardItemtModel());
        qDebug() << model->getStandardItemtModel() << newModel;

    }
}

void MainWindow::on_zoomSpinbox_valueChanged(double newScaleCoeff)
{
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

void MainWindow::on_contourWidthSlider_sliderMoved(int position)
{
    qDebug() << "Когда я буду двигать слайдеро контур станет вот таклой ширины" << position;
}

//переключение отображения модели атрибутов
void MainWindow::on_attrTableViewRadio_toggled(bool checked)
{
    int activeTab = checked ? 2 : 1;
    ui->tabWidget->setCurrentIndex(activeTab);
}

void MainWindow::on_attrTreeModelRadio_toggled(bool checked)
{
    int activeTab = checked ? 1 : 2;
    ui->tabWidget->setCurrentIndex(activeTab);
}

void MainWindow::on_calcButton_clicked()
{
//    QVector<int> rAvVector;

//    for (int i = 0; i < ui->attrTbleView->model()->rowCount(); i++)
//    {
//        int avRatIRow = ui->attrTbleView->model()->
//    }
//    ui->widget->setArray( )

    QVector<int> ravVect;
    ravVect = model->getRangeByCode(Code::BrRAv);
}
