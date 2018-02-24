#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include "obj.h"
#include <QImage>
#include <QPixmap>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QString>
#include <QMessageBox>

class DataModel : public QObject
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    int _objCount;
    QImage image() const;
    QPixmap pixmap() const;

    void setImage(const QString &imagePath);

    QString imageFilePath() const;

    void loadObjectsData(const QString &dataImageFilePath);

    QVector<Obj>_objectsOnImage;

    int objCount() const;
    void setObjCount(int objCount);

    QString dataFilePath() const;
    void setDataFilePath(const QString &dataFilePath);

signals:
    void pathImgLoaded(QString newPath);
    void pathDataLoaded(QString newPath);

private:
    QImage _image;
    QString _imageFilePath;
    QString _dataFilePath;
    QString _attrsFilePath;

    void setImagePath(const QString &imageFilePath);

public slots:
};

#endif // DATAMODEL_H
