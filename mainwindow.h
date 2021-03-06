      #ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QDir>
#include <QRadioButton>
#include <QAbstractSlider>
#include <QStandardItemModel>
#include <QResizeEvent>
#include <QPolygonF>
#include <QColor>
#include <QTreeView>
#include <QStandardPaths>

#include "datamodel.h"
#include "widgets/colorwidget.h"
#include "widgets/histogramwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void updateViewer(const QPixmap &pixmap, double scaleCoeff, Qt::AspectRatioMode mode);
    void resizeEvent(QResizeEvent *event);

    double scaleCoeff() const;
    void setScaleCoeff(double newScaleCoeff);

    void buildHist();

public slots:
    void updateObjColor(QColor clr);
    void updateContourColor(QColor clr);
    void updateTreeModel(QStandardItemModel *newModel);

private slots:
    void on_openDataBut_toggled(bool checked);
    void on_openImgBut_toggled(bool checked);
    void on_openAttrBut_toggled(bool checked);
    void on_closeBut_clicked();

    void on_openDataAct_triggered( );
    void on_openImgAct_triggered( );
    void on_openAttrAct_triggered( );
    void on_closeAct_triggered( );

    void on_keepRatioBut_toggled(bool checked);
    void on_ignoreRatioBut_toggled(bool checked);

    void on_zoomInAct_triggered();
    void on_zoomOutAct_triggered();

    void on_zoomRatioSlider_valueChanged(int value);
    void on_zoomRatioSlider_sliderMoved(int position);
    void on_zoomSpinbox_valueChanged(double newScaleCoeff);

    void on_contourWidthSlider_sliderMoved(int position);

    void on_attrTableViewRadio_toggled(bool checked);
    void on_attrTreeModelRadio_toggled(bool checked);


    void on_calcButton_clicked();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *viewer;

    void initColorWidgets();
    QString requiredPath(QDir currentDir, const QString &redirect);
    void updateAccessState(QAction *ac, QPushButton *bt, bool newEnableState, bool newCheckedState = false);
    void updateAccessState(QAction *ac, QRadioButton *bt, bool newEnableState, bool newCheckedState = false);
    Qt::AspectRatioMode checkedRatio();



    double _scaleCoeff;
    DataModel *model;
};

#endif // MAINWINDOW_H
