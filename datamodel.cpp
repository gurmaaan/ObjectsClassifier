 #include "datamodel.h"

#ifndef ROOT_ITEM_SIZE
#define ROOT_ITEM_SIZE QSize(200,58)
#endif

#ifndef META_ITEM_SIZE
#define META_ITEM_SIZE QSize(150,30)
#endif

#ifndef POINT_ITEM_SIZE
#define POINT_ITEM_SIZE QSize(100,30)
#endif

#ifndef ATTR_COUNT
#define ATTR_COUNT 15
#endif

DataModel::DataModel(QObject *parent) : QObject(parent)
{
    _objCount = 0;
}

QStandardItemModel *DataModel::getStandardItemtModel() const
{
    return _model;
}

//Инициализация главной модели (tree виджета)
QStandardItemModel *DataModel::initModel()
{
    QStandardItemModel *model = new QStandardItemModel(0, 4);

    QStringList modelHeaders;
    modelHeaders << "Объект" << "id" << "Ширина" << "Высота";
    model->setHorizontalHeaderLabels(modelHeaders);
    _model = model;

    return model;
}

void DataModel::addObjectRootItem(QStandardItemModel *model, const Obj &ob)
{
    QStringList objStrList;
    objStrList << "id: " << QString::number(ob.id());

    QStandardItem *objectRoot = new QStandardItem(0, 4);

    //TODO: перенести получение иконки в объект
    QRect iconRect = ob.getInternalRect();
    int w = iconRect.width();
    int h = iconRect.height();
    objStrList << QString::number(w) << QString::number(h);
    objectRoot = addMainHeadingItem(model, objStrList);

    QImage icon(w, h, QImage::Format_ARGB32);
    icon.fill(Qt::transparent);

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int deltaX = iconRect.left() + i;
            int deltaY = iconRect.top() + j;
            QColor color(_image.pixelColor(deltaX, deltaY));
            icon.setPixelColor(iconRect.right() - deltaX, j, color);
        }
    }

    QPixmap iconPixmap(QPixmap::fromImage(icon));

    if( (w < 50)&&( h < 50) )
    {
        QPixmap result(50, 50);
        result.fill(Qt::transparent);
        QPainter painter(&result);
        painter.drawPixmap(w, h, iconPixmap);

        objectRoot->setIcon(QIcon(result));
    } else
    {
        objectRoot->setIcon(QIcon(iconPixmap.scaled(50, 50, Qt::KeepAspectRatio)));
    }
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

//Добавление строки вида "название параметра" : "значение параметра" целого числа
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
    intPointsCountItem = addIntegerItem(parent, "Внутренних точек: ", ob.getInternalPoits().count());
    intPointsCountItem->setSizeHint(META_ITEM_SIZE);
    addPointsObjectItem(intPointsCountItem, ob.getInternalPoits());

    QStandardItem *contourPointsItem = new QStandardItem();
    contourPointsItem = addIntegerItem(parent, "Контурных точек: ", ob.getContourPointns().count());
    contourPointsItem->setSizeHint(META_ITEM_SIZE);
    addPointsObjectItem(contourPointsItem, ob.getContourPointns());
}

void DataModel::addPointsObjectItem(QStandardItem *parentMetaItem, const QVector<QPoint> &points)
{
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

//Возвращает модель таблицы файла аттрибутов
QStandardItemModel *DataModel::getAttrModel() const
{
    return _attrModel;
}


int DataModel::objAttrCount() const
{
    return _attrModel->rowCount();
}

void DataModel::pushObjectAttributeById(int id, Attribute atr)
{
    for(auto ob : _objectsOnImage)
        if (ob.id() == id)
            ob.appendDescriptor(atr);
}

void DataModel::pushObjectAttributeById(int id, Code code, int value)
{
    foreach(auto ob, _objectsOnImage)
        if (ob.id() == id)
            ob.appendDescriptor(code, value);

}

//Инициализация модели атрибутов
QStandardItemModel *DataModel::initAttrModel()
{
    QStandardItemModel *model = new QStandardItemModel(0, ATTR_COUNT +1);

    QStringList headerStringList;
    headerStringList << "ID";
    model->setHorizontalHeaderItem(0, new QStandardItem(headerStringList.first()));

    QList<QStandardItem *> itemsList;
    int colNum = 1;
    for(Code c = Code::BrRAv; c != Code::NonAssigned; c++)
    {
        Attribute tempAtr(c);
        QString colName = tempAtr.name();
        headerStringList << colName;
        QStandardItem *it = new QStandardItem(headerStringList.at(colNum));
        it->setData(Qt::AlignHCenter, Qt::TextAlignmentRole);

        model->setHorizontalHeaderItem(colNum, it);
        colNum++;
    }

    //model->setHorizontalHeaderLabels(headerStringList);

    QFont newFont(model->horizontalHeaderItem(0)->font());
    newFont.setBold(true);

    for(int i = 0; i < model->columnCount(); i++)
    {
        model->horizontalHeaderItem(i)->setFont(newFont);
        model->horizontalHeaderItem(i)->setSizeHint(QSize(150,50));
    }

    _attrModel = model;
    return model;
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

        for (int i = 1; i < indOfC; i+=2)
            ob.pushInternalPoint(pointsFullList.at(i).toInt(), pointsFullList.at(i+1).toInt());

        for (int j = indOfC + 1; j < pointsFullList.count(); j+=2)
            ob.pushContourPoint(pointsFullList.at(j).toInt(), pointsFullList.at(j+1).toInt());

        emit incrementProgress(objectsStringList.indexOf(objFullString) + 1);
        pushObject(ob);
    }
}

//Загрузка аттрибутов из файла csv
void DataModel::loadObjectsAttr(const QString &attrFilePath)
{
    //Если объекты загружены (в модели больше одной строк т.е. есть что-то помимо заголовка)
    if ( _model->rowCount() > 1)
    {
        _attrModel = initAttrModel();
        setAttrsFilePath(attrFilePath);

        QFile file(attrFilePath);
        if (!file.open(QIODevice::ReadOnly))
        {
            qDebug() << file.errorString();
        } else
        {
            QTextStream in(&file);
            int rowCounter = 0;
            QString line = in.readLine(); //пропустили первую строку с заголовками
            while (!in.atEnd())
            {
                QString line = in.readLine();
                emit objAttrCountChanged(rowCounter++);

                qDebug() << rowCounter;
                QList<QStandardItem *> standardItemsList;
                QStringList rowList = line.split(";", QString::SkipEmptyParts);

                if (rowList.count() < ATTR_COUNT)
                    return;
                else
                {
                    //Сохраняю нулевой элемент массива содержащий id
                    int id = rowList.first().split(".").first().toInt();
                    QStandardItem *idItem = new QStandardItem(rowList.first());
                    idItem->setData(id, Qt::EditRole);
                    standardItemsList << idItem;

                    //Беру индекс массива как номер колонки в файле. В свою очередь Code = 0 <=> colNum = 1
                    for (Code c = Code::BrRAv; c <= Code::GeomPerim; c++)
                    {
                        Attribute atr(c);

                        QString itemToolTip(rowList.at( atr.colNum()) );
                        QString itemString = itemToolTip.replace(" ", "");
                        QStringList itemStringSeparated = itemString.split(".", QString::SkipEmptyParts);
                        int itemIntVal = itemStringSeparated.first().toInt();
                        double itemDoubleVal = itemString.toDouble();

                        QStandardItem *item = new QStandardItem();
                        item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
                        item->setData(itemToolTip, Qt::ToolTipRole);

                        if ( c >= Code::BrRAv && c <= Code::BrBAv)
                        {
                            QColor bgClr = atr.interpritatedColor(c, itemIntVal);

                            item->setData(itemIntVal, Qt::DisplayRole);
                            item->setData(bgClr, Qt::DecorationRole);

                        }else if (c >= Code::BrRsko && c <= Code::BrBsko)
                        {
                            item->setData(itemDoubleVal, Qt::DisplayRole);

                        }else if (c >= Code::TextRmps && c <= Code::TextBser)
                        {

                            item->setData(itemIntVal, Qt::DisplayRole);
                        }else if (c == Code::GeomForm)
                        {

                            item->setData(itemDoubleVal, Qt::DisplayRole);
                        }else if (c >= Code::GeomSquare)
                        {
                            item->setData(itemIntVal, Qt::DisplayRole);
                        }

                        standardItemsList << item;

                    }
                    emit incrementAttrProgress(rowCounter);
                    _attrModel->appendRow(standardItemsList);
                }

            }
            file.close();

            connect(this, SIGNAL(dataAndAttrFilesMatch), this, SLOT(synchModels));
            bool equalState = (rowCounter == objCount());
            emit dataAndAttrFilesMatch(equalState);
        }
    }
}

void DataModel::pushObject(Obj &ob)
{
    _objectsOnImage.append(ob);
    //qDebug() << ob;

    addObjectRootItem(_model, ob);
}

//Проверяет что строка пути не пустая и устанавливает аттрибут пути модели в переданный параметр
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

//TODO: объединить эти 2 метода в один универсальный

//Проверяет что строка пути к файлу атрибутов не пустая и копирует переданный параметр в поле модели
void DataModel::setAttrsFilePath(const QString &attrsFilePath)
{
    if(!attrsFilePath.isEmpty())
    {
        _attrsFilePath = attrsFilePath;
        emit pathAttrLoaded(_attrsFilePath);
    }
    else
    {
        QMessageBox::critical(0, "Ошибка чтения файла", "Невозможно прочитать файл аттрибутов, строка содержащая путь некорректна");
    }
}

QString DataModel::getAttrsFilePath() const
{
    return _attrsFilePath;
}

//Главный метод - возвращает массив объектов
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
       // TODO: отображение скрытие объектов
        // foreach (auto)
    }
}

//TODO: преобразование строки табличной модели в списочную моодель с вертикальными хедерами
//Добавить в три модель лист
void DataModel::synchModels(bool state)
{
    if(state)
        qDebug() << "Xui";
}

QString DataModel::imageFilePath() const
{
    return _imageFilePath;
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
