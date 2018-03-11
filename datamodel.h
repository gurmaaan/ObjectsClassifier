#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "obj.h"
#include "attribute.h"

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
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QVector>
#include <QFont>

//TODO : Поправить сортировку чисел (вопрос на стековерфлоу)
//TODO : Перенести сюда загрузку файла дескрипторов
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

    void pushObject(Obj &ob);

    QStandardItemModel *getStandardItemtModel() const;

    QVector<Obj> getObjectsOnImage() const;

    QString getAttrsFilePath() const;
    QStandardItemModel *getAttrModel() const;
    void loadObjectsAttr(const QString &attrFilePath);
    int objAttrCount() const;

    void pushObjectAttributeById(int id, Attribute atr);
    void pushObjectAttributeById(int id, Code code, int value);

public slots:
    void setObjectsVisible(bool state);


signals:
    void pathImgLoaded(QString newPath);
    void pathDataLoaded(QString newPath);
    void pathAttrLoaded(QString newPath);

    void objCountChanged(int newCount);
    void incrementProgress(int newValue);

    void objAttrCountChanged(int newValue);
    void incrementAttrProgress(int newValue);

    void dataAndAttrFilesMatch(bool ok);

private:
    QImage _image;

    int _objCount;
    void setObjCount(int objCount);

    //--файл изображения
    QString _imageFilePath;
    void setImagePath(const QString &imageFilePath);

    //Файл точек (данных) от Егора
    QString _dataFilePath;
    QString readFileToString(const QString &dataFilePath);
    void setDataFilePath(const QString &dataFilePath);

    QVector<Obj> _objectsOnImage;

    //Файл аттрибутов (дескрипторов) от Слайка
    QString _attrsFilePath;
    void setAttrsFilePath(const QString &attrsFilePath);

    //----------Модель три вьюхи
    QStandardItemModel *_model;
    QStandardItemModel *initModel();

    void addObjectRootItem(QStandardItemModel *model, const Obj& ob);

    QStandardItem *addHeadingItem(QStandardItem *parent, const QStringList& headerList);
    QStandardItem *addIntegerItem(QStandardItem *parent, const QString& name, const int& value);
    QStandardItem *addMainHeadingItem(QStandardItemModel *parentModel, const QStringList& headerList);

    void addMetaObjectItem(QStandardItem *parentObj, const Obj &ob);
    void addPointsObjectItem(QStandardItem *parentMetaItem, const QVector<QPoint> &points);

    //----------Модель таблицы аттриббутов (дескрипторов)
    QStandardItemModel *_attrModel;
    QStandardItemModel *initAttrModel();
};

#endif // DATAMODEL_H
