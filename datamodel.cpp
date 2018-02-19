#include "datamodel.h"

DataModel::DataModel(QObject *parent) : QObject(parent)
{

}

QString DataModel::imageFilePath() const
{
    return _imageFilePath;
}

void DataModel::setImagePath(const QString &imagePath)
{
    _imageFilePath = imagePath;
    emit pathLoaded(_imageFilePath);
}

QImage DataModel::image() const
{
    return _image;
}

QPixmap DataModel::pixmap() const
{
    return QPixmap::fromImage(_image);
}

void DataModel::setImage(const QString &imagePath)
{
    if (!(imagePath.isEmpty() && imagePath.isNull()))
    {
        _image.load(imagePath);
        setImagePath(imagePath);
    } else
    {
        setImagePath("");
    }
}
