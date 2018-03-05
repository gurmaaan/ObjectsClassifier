#include "datamodel.h"

#ifndef ROOT_ITEM_SIZE
#define ROOT_ITEM_SIZE QSize(200,30)
#endif

#ifndef META_ITEM_SIZE
#define META_ITEM_SIZE QSize(150,30)
#endif

#ifndef POINT_ITEM_SIZE
#define POINT_ITEM_SIZE QSize(100,30)
#endif

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

    QStringList modelHeaders;
    modelHeaders << "Название атрибута" << "Значение";
    model->setHorizontalHeaderLabels(modelHeaders);
    _model = model;

    return model;
}

void DataModel::addObjectRootItem(QStandardItemModel *model, const Obj &ob)
{
    QStringList objStrList;
    objStrList << "id: " << QString::number(ob.id());

    QStandardItem *objectRoot = new QStandardItem(0, 2);

//    item->setText( "Объект " + QString::number( ob.id() ) );

    //Добавление в корень строку id : номер
    objectRoot = addMainHeadingItem(model, objStrList);

    addMetaObjectItem(objectRoot, ob);
    //model->appendRow();
}

//Добавление строки вида "название параметра" : "значение парметра" в качестве ребенка переданного item
QStandardItem *DataModel::addHeadingItem(QStandardItem *parent, const QStringList &headerList)
{
    QList<QStandardItem*> itemsList;
    foreach (QString itemText, headerList) {
        QStandardItem *item = new QStandardItem(itemText);
        itemsList.push_back(item);
    }
    parent->appendRow(itemsList);
    return itemsList.first();
}

QStandardItem *DataModel::addIntegerItem(QStandardItem *parent, const QString &name, const int &value)
{
    QStringList itemsStringList;
    itemsStringList << name << QString::number(value);

    return addHeadingItem(parent, itemsStringList);
}

//Добавление строки вида "название параметра" : "значение парметра" в корень модели
QStandardItem *DataModel::addMainHeadingItem(QStandardItemModel *parentModel, const QStringList& headerList)
{
    QList<QStandardItem*> itemsList;
    foreach (QString itemText, headerList) {
        QStandardItem *item = new QStandardItem(itemText);

        QFont newFont(item->font());
        newFont.setBold(true);
        item->setFont(newFont);

        item->setSizeHint(ROOT_ITEM_SIZE);

        itemsList.push_back(item);
    }
    parentModel->appendRow(itemsList);

    return itemsList.first();
}

void DataModel::addMetaObjectItem(QStandardItem *parent, const Obj &ob)
{
    QStandardItem *intPointsCountItem = new QStandardItem();
    intPointsCountItem = addIntegerItem(parent, "Внутренних точек: ", ob.getInternalPointsCount());
    intPointsCountItem->setSizeHint(META_ITEM_SIZE);
    addPointsObjectItem(intPointsCountItem, ob.getInternalPoits());

    QStandardItem *contourPointsItem = new QStandardItem();
    contourPointsItem = addIntegerItem(parent, "Контурных точек: ", ob.contourPointsCount() );
    contourPointsItem->setSizeHint(META_ITEM_SIZE);
    addPointsObjectItem(contourPointsItem, ob.contourPointns());
}

void DataModel::addPointsObjectItem(QStandardItem *parentMetaItem, const QVector<QPoint> &points)
{
    //TODO: сделать отображение в модели как отдельные числа а не строка

    QList<QStandardItem*> pointsList;
    int i = 1; //введено т. к. операция взятия индекса очень долгая
    for(auto point : points)
    {
        QString pointStr = "Точка: " + QString::number(i) +" : (" + QString::number(point.x()) + " ; " + QString::number(point.y()) + ")";
        QStandardItem *pointItem = new QStandardItem(pointStr);
        i++;
        pointItem->setSizeHint(POINT_ITEM_SIZE);
        addIntegerItem(pointItem, "X: ", point.x() );
        addIntegerItem(pointItem, "Y: ", point.y() );
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

void DataModel::resizeHeaderItem(int newWidth)
{
    //TODO: Выравнивание ширины строк по ширине виджета
    //this->getStandardItemtModel()->ro
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

//Слот реагирующий на сигнал виджета отображения объектов
//Если приъходит истина, посылает виджету главного окна QGraphicsPolygonItem сигнал обновления вьювера с ново   й сцйеной
void DataModel::setObjectsVisible(bool state)
{
    if (state && !_objectsOnImage.isEmpty())
    {
       // foreach (auto)
    }
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
