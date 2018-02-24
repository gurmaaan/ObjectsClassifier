#include "datamodel.h"

#include <QString>
#include <QTimer>


DataModel::DataModel(QObject *parent) : QObject(parent)
{
    _objCount = 0;
}

int DataModel::objCount() const
{
    return _objCount;
}

void DataModel::setObjCount(int objCount)
{
    _objCount = objCount;
}

QString DataModel::dataFilePath() const
{
    return _dataFilePath;
}

void DataModel::setDataFilePath(const QString &dataFilePath)
{
    if (!dataFilePath.isEmpty())
    {
        _dataFilePath = dataFilePath;
        emit pathDataLoaded(_dataFilePath);
    }
}

QString DataModel::imageFilePath() const
{
    return _imageFilePath;
}

void DataModel::loadObjectsData(const QString &dataImageFilePath)
{
    QString fileString;
    setDataFilePath(dataImageFilePath);

    if (!dataImageFilePath.isNull())
    {
        QFile datafile(dataImageFilePath);
        if ( !datafile.open( QIODevice::ReadOnly) )
            return;

        int t1 = QTime::currentTime().msec();
        while ( !datafile.atEnd()) {
            fileString = datafile.readLine();
        }
        datafile.close();
        int t2 = QTime::currentTime().msec();

        int delta = t2 - t1;

        qDebug() << "Считыване файла данных - завешено успешно за " << delta << " миллисекунд.";

        //Делим исходный файл на строки содержаще полное описане объектов
        QStringList objectsStringList = fileString.split(";", QString::SkipEmptyParts);
        int t3 = QTime::currentTime().msec();
        delta = t3 - t2;
        _objCount = objectsStringList.count();
        qDebug() << "Найдено " <<  _objCount << " объектов за " << delta << " миллисекунд.";

        foreach(QString objFullString, objectsStringList)
        {
            //Пробегаюсь по полученному массиву строк.
            //Создаю локальную копию каждого объекта и делаю пушбек в вектор
            Obj *ob = new Obj();
            QStringList pointsFullList = objFullString.split(".", QString::SkipEmptyParts);

            ob->setId(pointsFullList.at(0).toInt());

            qDebug() << "id: " << ob->id();

            int cPos = pointsFullList.indexOf("C");
            for (int i = 1; i < cPos; i+=2)
            {
                ob->pushInternalPoint(pointsFullList.at(i).toInt(), pointsFullList.at(i+1).toInt());
            }

            qDebug() << "\tВнутренних точек: " << ob->intPointsCount();

            for (int j = cPos + 1; j < pointsFullList.count(); j+=2)
            {
                ob->pushContourPoint(pointsFullList.at(j).toInt(), pointsFullList.at(j+1).toInt());
            }

            qDebug() << "\tКонтурных точек: " << ob->contourPointsCount();
        }

    } else {
        QMessageBox::critical(0, "Ошибка чтения файла", "Невозможно прочитать файл данных, строка содержащая путь некорректна");
    }
}

void DataModel::setImagePath(const QString &imagePath)
{
    _imageFilePath = imagePath;
    emit pathImgLoaded(_imageFilePath);
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
