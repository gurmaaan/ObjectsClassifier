#ifndef DATAMODEL_H
#define DATAMODEL_H

#include "obj.h"
#include "attribute.h"

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QFileInfo>
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
    void pushObjectAttributeById(int id, Code code, QVariant value);

    //template<typename T>
    QVector<int> getRangeByCode(Code code);

    //QStandardItemModel *getModel() const;
    void setModel(QStandardItemModel *model);

public slots:
    void setObjectsVisible(bool state);
    void synchModels(bool state);

signals:
    void pathImgLoaded(QString newPath);
    void pathDataLoaded(QString newPath);
    void pathAttrLoaded(QString newPath);

    void objCountChanged(int newCount);
    void incrementProgress(int newValue);

    void objAttrCountChanged(int newValue);
    void incrementAttrProgress(int newValue);

    void dataAndAttrFilesMatch(bool ok);

    void modelUpdated(QStandardItemModel *model);

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
    QStandardItem *addMainHeadingItem(QStandardItemModel *parentModel, const QStringList& headerList);

    QStandardItem *genDescrItem(QString itemText, Qt::AlignmentFlag textAlignment = Qt::AlignRight,  QFont::Style fontStyle = QFont::StyleItalic );

    QStandardItem *addIntegerItem(QStandardItem *parent, const QString& name, const int& value);
    QStandardItem *addDoubleItem(QStandardItem *parent, const QString& name, const double& value);

    void addPointsHeadingItem(QStandardItem *parentObj, const Obj &ob);
    void addPointsObjectItem(QStandardItem *parentMetaItem, const QVector<QPoint> &points);

    //----------Модель таблицы аттриббутов (дескрипторов)
    QStandardItemModel *_attrModel;
    QStandardItemModel *initAttrModel();
    QStandardItem *genCenteredStItWithToolTip(QString toolTip);
};

#endif // DATAMODEL_H
