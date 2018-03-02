#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "obj.h"

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QString>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QVector>

class DataModel : public QObject
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    QImage image();
    QPixmap pixmap() const;

    void setImage(const QString &imagePath);
    QString imageFilePath() const;

    void loadObjectsData(const QString &dataFilePath);
    int objCount() const;
    QString dataFilePath() const;

    void resizeHeaderItem(int newWidth);

    void pushObject(Obj &ob);

    QStandardItemModel *getStandardItemtModel() const;

    QVector<Obj> getObjectsOnImage() const;

signals:
    void pathImgLoaded(QString newPath);
    void pathDataLoaded(QString newPath);
    void objCountChanged(int newCount);
    void incrementProgress(int newValue);

private:
    QImage _image;

    int _objCount;
    void setObjCount(int objCount);

    QString _imageFilePath;
    void setImagePath(const QString &imageFilePath);

    QString _dataFilePath;
    QString readFileToString(const QString &dataFilePath);
    void setDataFilePath(const QString &dataFilePath);
    QString _attrsFilePath;

    QVector<Obj> _objectsOnImage;

    QStandardItemModel *_model;
    QStandardItemModel *initModel();
    void addObjectRootItem(QStandardItemModel *model, const Obj& ob);
    void addMetaObjectItem(QStandardItem *parentObj, const Obj& ob);
    void addPointsObjectItem(QStandardItem *parentMetaItem, const QVector<QPoint> &points);
public slots:
};

#endif // DATAMODEL_H
