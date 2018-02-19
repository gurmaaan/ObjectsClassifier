#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include "obj.h"
#include <QImage>
#include <QPixmap>

class DataModel : public QObject
{
    Q_OBJECT
public:
    explicit DataModel(QObject *parent = nullptr);

    QImage image() const;
    QPixmap pixmap() const;

    void setImage(const QString &imagePath);

    QString imageFilePath() const;

    QVector<Obj>_objectsOnImage;

signals:
    void pathLoaded(QString newPath);

private:
    QImage _image;
    QString _imageFilePath;
    QString _dataFilePath;
    QString _attrsFilePath;

    void setImagePath(const QString &imageFilePath);

public slots:
};

#endif // DATAMODEL_H
