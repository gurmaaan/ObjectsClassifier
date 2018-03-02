#include "datamodel.h"

DataModel::DataModel(QObject *parent) : QObject(parent)
{
    _objCount = 0;
}

QStandardItemModel *DataModel::getStandardItemtModel() const
{
    return _model;
}

QStandardItemModel *DataModel::initModel()
{
    QStandardItemModel *model = new QStandardItemModel(0, 2);

    model->setHeaderData(0, Qt::Horizontal, "Объект");
    model->setHeaderData(1, Qt::Horizontal, "Атрибут");
    return model;
}

void DataModel::addObjectRootItem(QStandardItemModel *model, const Obj &ob)
{
    QString objHeaderText = "id: " + QString::number(ob.id());
    QStandardItem *item = new QStandardItem(objHeaderText);

    //item->setCheckable(true);
    item->setSizeHint(QSize(200, 30));
    addMetaObjectItem(item, ob);
    model->appendRow(item);
}

void DataModel::addMetaObjectItem(QStandardItem *parent, const Obj &ob)
{
    QString intPointsCountStr = "Internal points count: " + QString::number(ob.getInternalPointsCount());
    QStandardItem *intPointsCountItem = new QStandardItem(intPointsCountStr);
    intPointsCountItem->setSizeHint(QSize(150, 30));
    addPointsObjectItem(intPointsCountItem, ob.internalPoits());

    QString contourPointsCountStr = "Contour points count: " + QString::number(ob.contourPointsCount());
    QStandardItem *contourPointsItem = new QStandardItem(contourPointsCountStr);
    contourPointsItem->setSizeHint(QSize(150, 30));
    addPointsObjectItem(contourPointsItem, ob.contourPointns());

    QList<QStandardItem*> list;
    list.push_back(contourPointsItem);
    list.push_back(intPointsCountItem);
    parent->appendRows(list);
}

void DataModel::addPointsObjectItem(QStandardItem *parentMetaItem, const QVector<QPoint> &points)
{
    QList<QStandardItem*> pointsList;

    for(auto point : points) {
       QString pointStr = "(" + QString::number(point.x()) + " ; " + QString::number(point.y()) + ")";
       QStandardItem *pointItem = new QStandardItem(pointStr);
       pointItem->setSizeHint(QSize(100, 30));
       pointsList.push_back(pointItem);
    }

    parentMetaItem->appendRows(pointsList);
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

void DataModel::pushObject(Obj &ob)
{
    _objectsOnImage.append(ob);
    //qDebug() << ob;
    addObjectRootItem(_model, ob);
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
    _model = initModel();
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

        double progress = static_cast<double>(objectsStringList.indexOf(objFullString)) *
                static_cast<double>(100) / static_cast<double>(objCount()) ;
        //qDebug() << progress;
        emit incrementProgress(static_cast<int>(round(progress)));
        pushObject(ob);
    }
}

//Инициализация поля с путем файла из строки
void DataModel::setImagePath(const QString &imagePath)
{
    if(!imagePath.isEmpty())
    {
        _imageFilePath = imagePath;
        emit pathImgLoaded(_imageFilePath);
    }
}

//Получение строки имени файла
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

QImage DataModel::image()
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
