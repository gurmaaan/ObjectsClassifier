#include "datamodel.h"
#include <QVector>

DataModel::DataModel(QObject *parent) : QObject(parent)
{
    _objCount = 0;
      //initModel(_objectsOnImage);
}

//QStandardItemModel *DataModel::getStandardItemtModel() const
//{
//    return _model;
//}

//QStandardItemModel *DataModel::initModel(const QVector<Obj> &objectsOnImage)
//{
//    QStandardItemModel *model = new QStandardItemModel(0, 2);

//    model->setHeaderData(0, Qt::Horizontal, "Object");
//    model->setHeaderData(1, Qt::Horizontal, "Attribute");

//    for (int i = 0; i <= objectsOnImage.count(); i++)
//    {
//        addObjectItem(model, objectsOnImage.at(i));
//    }

//    return model;
//}

//void DataModel::addObjectItem(QStandardItemModel *model, const Obj &ob)
//{

//}

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

void DataModel::pushObject(Obj &ob)
{
    _objectsOnImage.append(ob);
    qDebug() << ob;
}

void DataModel::setDataFilePath(const QString &dataFilePath)
{
    if (!dataFilePath.isEmpty())
    {
        _dataFilePath = dataFilePath;
        emit pathDataLoaded(_dataFilePath);
    }
    else
    {
        QMessageBox::critical(0, "Ошибка чтения файла", "Невозможно прочитать файл данных, строка содержащая путь некорректна");
    }
}

QVector<Obj> DataModel::getObjectsOnImage() const
{
    return _objectsOnImage;
}

QString DataModel::imageFilePath() const
{
    return _imageFilePath;
}

//Загрузка точек из файла
void DataModel::loadObjectsData(const QString &dataFilePath)
{
    setDataFilePath(dataFilePath);
    QString fileString = readFileToString(dataFilePath);

    //Делим исходный файл на строки по ";" содержаще полное описане объектов
    QStringList objectsStringList = fileString.split(";", QString::SkipEmptyParts);
    _objCount = objectsStringList.count();
    emit objCountChanged(_objCount);

    //Обходим каждую строку (объект)
    //Разделяем по точке. Первый элемент - id. Все что до С - внутренние точки. Все что после С - контурные
    //Созданный таким образом объект пушим в общий вектор
    foreach(QString objFullString, objectsStringList)
    {
        QStringList pointsFullList = objFullString.split(".", QString::SkipEmptyParts);

        Obj ob(pointsFullList.at(0).toInt());
        int indOfC = pointsFullList.indexOf("C");

        ob.setIntPointsCount((indOfC - 1) / 2);
        for (int i = 1; i < indOfC; i+=2)
            ob.pushInternalPoint(pointsFullList.at(i).toInt(), pointsFullList.at(i+1).toInt());

        ob.setContourPointsCount( (pointsFullList.count() - indOfC) / 2);
        for (int j = indOfC + 1; j < pointsFullList.count(); j+=2)
            ob.pushContourPoint(pointsFullList.at(j).toInt(), pointsFullList.at(j+1).toInt());

        pushObject(ob);
    }
    //_model = initModel(_objectsOnImage);
}

void DataModel::setImagePath(const QString &imagePath)
{
    if(!imagePath.isEmpty())
    {
        _imageFilePath = imagePath;
        emit pathImgLoaded(_imageFilePath);
    }
}

//Считьы
QString DataModel::readFileToString(const QString &dataFilePath)
{
    QFile datafile(dataFilePath);
    if ( datafile.open( QIODevice::ReadOnly) )
    {
        int t1 = QTime::currentTime().msec();
        //----------------------------------
        QString fileString = "";

        while ( !datafile.atEnd()) {
            fileString = datafile.readLine();
        }
        datafile.close();
        //----------------------------------
        int t2 = QTime::currentTime().msec();

        int delta = t2 - t1;

        qDebug() << "Считыване файла данных - завешено успешно за " << delta << " миллисекунд.";

        return fileString;
    }
    else
    {
        return "";
    }
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
