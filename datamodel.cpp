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

    QList<QStandardItem*> headerItemList;

    QStringList modelHeaders;
    modelHeaders << "Объект" << "ID" << "Ширина" << "Высота";
    foreach (QString colHeader, modelHeaders)
    {
        QStandardItem *colHeaderItem = new QStandardItem(colHeader);


        QFont newFont(colHeaderItem->font());
        newFont.setBold(true);
        colHeaderItem->setData(newFont, Qt::FontRole);

        colHeaderItem->setData(ROOT_ITEM_SIZE, Qt::SizeHintRole);

        colHeaderItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);

        model->setHorizontalHeaderItem( modelHeaders.indexOf(colHeader), colHeaderItem);
    }

    _model = model;

    return _model;
}

void DataModel::addObjectRootItem(QStandardItemModel *model, const Obj &ob)
{
    for (int i = 0; i < model->rowCount(); i++)
    {
        if (model->item(i, 1)->data(Qt::EditRole).toInt() == ob.id())
        {
            model->removeRow(i);
        }
    }
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
        //BUG : иконка не в центре а хз есть ли вообще
        QPixmap result(50, 50);
        result.fill(Qt::transparent);
        QPainter painter(&result);
        painter.drawPixmap(w, h, iconPixmap);

        objectRoot->setData(iconPixmap, Qt::DecorationRole);
    } else
    {
        objectRoot->setData(QIcon(iconPixmap.scaled(50, 50, Qt::KeepAspectRatioByExpanding)), Qt::DecorationRole);
    }

    for(auto descr : ob.getDescriptors())
    {
        if (Attribute::isInt(descr->getCode()))
        {
            addIntegerItem(objectRoot, descr->name(), descr->value().toInt());
        } else {
            addDoubleItem(objectRoot, descr->name(), descr->value().toDouble());
        }
    }

    addPointsHeadingItem(objectRoot, ob);

    emit modelUpdated(model);
}


//Добавление строки вида "название параметра" : "значение параметра" - целое число
QStandardItem *DataModel::addIntegerItem(QStandardItem *parent, const QString &name, const int &value)
{
    QList<QStandardItem*> itemsList;

    itemsList << genDescrItem(name, Qt::AlignRight);

    QStandardItem *dataItem = new QStandardItem();
    dataItem->setData(value, Qt::DisplayRole);
    dataItem->setData(META_ITEM_SIZE, Qt::SizeHintRole);
    dataItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    itemsList << dataItem;

    parent->appendRow(itemsList);

    return itemsList.first();
}

//Добавление строки вида "название параметра" : "значение параметра" - дробное число
QStandardItem *DataModel::addDoubleItem(QStandardItem *parent, const QString &name, const double &value)
{
    QList<QStandardItem*> itemsList;

    itemsList << genDescrItem(name, Qt::AlignRight);

    QStandardItem *dataItem = new QStandardItem();
    dataItem->setData(value, Qt::EditRole);
    dataItem->setData( QString::number(value, 'f', 2), Qt::DisplayRole);
    dataItem->setData(META_ITEM_SIZE, Qt::SizeHintRole);
    dataItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    itemsList << dataItem;

    parent->appendRow(itemsList);

    return itemsList.first();
}


//Добавление строки вида "название параметра" : "значение парметра" в корень модели
QStandardItem *DataModel::addMainHeadingItem(QStandardItemModel *parentModel, const QStringList& headerList)
{
    QList<QStandardItem*> itemsList;
    foreach (QString itemText, headerList)
    {
        QStandardItem *item = new QStandardItem(itemText);
        item->setData(Qt::AscendingOrder, Qt::InitialSortOrderRole);

        QFont newFont(item->font());
        newFont.setBold(true);
        item->setData(newFont, Qt::FontRole);

        item->setData(ROOT_ITEM_SIZE, Qt::SizeHintRole);
        itemsList.push_back(item);
    }

    parentModel->appendRow(itemsList);

    return itemsList.first();
}

//Возвращает QStandartItem просто с надписью для описания параметра
QStandardItem *DataModel::genDescrItem(QString itemText, Qt::AlignmentFlag textAlignment, QFont::Style fontStyle)
{
    QStandardItem *descriptionItem = new QStandardItem(itemText);
    descriptionItem->setData(textAlignment, Qt::TextAlignmentRole);
    descriptionItem->setData(Qt::AlignVCenter, Qt::TextAlignmentRole);

    QFont newFont(descriptionItem->font());
    newFont.setStyle(fontStyle);
    descriptionItem->setData(newFont, Qt::FontRole);

    return descriptionItem;
}

void DataModel::addPointsHeadingItem(QStandardItem *parent, const Obj &ob)
{
    QStandardItem *intPointsCountItem = new QStandardItem();
    intPointsCountItem = addIntegerItem(parent, "Внутренних точек: ", ob.getInternalPoits().count());

    addPointsObjectItem(intPointsCountItem, ob.getInternalPoits());

    QStandardItem *contourPointsItem = new QStandardItem();
    contourPointsItem = addIntegerItem(parent, "Контурных точек: ", ob.getContourPointns().count());

    addPointsObjectItem(contourPointsItem, ob.getContourPointns());
}

void DataModel::addPointsObjectItem(QStandardItem *parentMetaItem, const QVector<QPoint> &points)
{
    QList<QStandardItem*> pointsList;
    int i = 1; //введено т. к. операция взятия индекса очень долгая
    for(auto point : points)
    {
        QString pointStr = "Точка: " + QString::number(i) +" : ( " + QString::number(point.x()) + " ; " + QString::number(point.y()) + " )";

        QStandardItem *pointItem = new QStandardItem();
        pointItem->setData(pointStr, Qt::DisplayRole);
        pointItem->setData(POINT_ITEM_SIZE, Qt::SizeHintRole);

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

void DataModel::pushObjectAttributeById(int id, Code code, QVariant value)
{
    foreach(auto ob, _objectsOnImage)
        if (ob.id() == id)
        {
            Attribute newAtr(code, value);
            pushObjectAttributeById(id, newAtr);
        }

}

//Инициализация модели атрибутов
QStandardItemModel *DataModel::initAttrModel()
{
    QStandardItemModel *model = new QStandardItemModel(0, ATTR_COUNT +1);

    QStringList headerStringList;
    headerStringList << "ID";
    model->setHorizontalHeaderItem(0, new QStandardItem(headerStringList.first()));

    QList<QStandardItem *> itemsList;

    for(Code c = Code::BrRAv; c < Code::NonAssigned; c++)
    {
        Attribute tempAtr(c);
        QString colName = tempAtr.name();
        headerStringList << colName;
        QStandardItem *it = new QStandardItem(headerStringList.at(static_cast<int>(c) + 1));
        it->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
        model->setHorizontalHeaderItem(static_cast<int>(c) +1, it);
    }

    QFont newFont(model->horizontalHeaderItem(0)->font());
    newFont.setBold(true);

    for(int i = 0; i < model->columnCount(); i++)
    {
        model->horizontalHeaderItem(i)->setData(newFont, Qt::FontRole);
        model->horizontalHeaderItem(i)->setData(META_ITEM_SIZE, Qt::SizeHintRole);
    }

    _attrModel = model;
    return model;
}

QStandardItem *DataModel::genCenteredStItWithToolTip(QString toolTip)
{
    QStandardItem *item = new QStandardItem();
    item->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
    item->setData(toolTip, Qt::ToolTipRole);

    return item;
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
    emit modelUpdated(_model);

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

                QList<QStandardItem *> standardItemsList;
                QStringList rowList = line.split(";", QString::SkipEmptyParts);

                if (rowList.count() < ATTR_COUNT)
                    return;
                else
                {
                    //Сохраняю нулевой элемент массива содержащий id
                    int id = rowList.first().split(".").first().toInt();
                    QStandardItem *idItem = new QStandardItem(rowList.first());
                    idItem->setData(id, Qt::DisplayRole);
                    idItem->setData(Qt::AlignCenter, Qt::TextAlignmentRole);
                    standardItemsList << idItem;

                    //Беру индекс массива как номер колонки в файле. В свою очередь Code = 0 <=> colNum = 1
                    for (Code c = Code::BrRAv; c <= Code::GeomPerim; c++)
                    {
                        Attribute atr(c);

                        QString itemToolTip(rowList.at( atr.colNum()) );

                        QString itemString = itemToolTip.replace(" ", "");
                        double itemDoubleVal = itemString.toDouble();
                        QString itemDoubleStrRound = QString::number(itemDoubleVal, 'f', 2);

                        QStringList itemStringSeparated = itemString.split(".", QString::SkipEmptyParts);
                        int itemIntVal = itemStringSeparated.first().toInt();

                        QStandardItem *item = genCenteredStItWithToolTip(itemToolTip);

                        if ( c >= Code::BrRAv && c <= Code::BrBAv)
                        {
                            QColor bgClr = atr.interpritatedColor(c, itemIntVal);

                            item->setData(itemIntVal, Qt::DisplayRole);
                            item->setData(bgClr, Qt::DecorationRole);
                            
                            atr.setValue(itemIntVal);

                        }else
                        {
                            if (Attribute::isInt(c))
                            {
                                item->setData(itemIntVal, Qt::DisplayRole);
                                atr.setValue(itemIntVal);
                            }
                            else
                            {
                                item->setData(itemDoubleVal, Qt::EditRole);
                                item->setData(itemDoubleStrRound, Qt::DisplayRole);
                                
                                atr.setValue(itemDoubleVal);
                            }
                        }
                        standardItemsList << item;
                        pushObjectAttributeById(id, atr);
                    }
                    emit incrementAttrProgress(rowCounter);
                    _attrModel->appendRow(standardItemsList);
                    _model->appendRow(standardItemsList);
                    
                }

            }
            file.close();

            emit dataAndAttrFilesMatch((rowCounter == objCount()));
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

//QStandardItemModel *DataModel::getModel() const
//{
//    return _model;
//}

void DataModel::setModel(QStandardItemModel *model)
{
    _model = model;
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
//    if (state && (_attrModel->rowCount() > 1) )
//    {
//        _attrModel->sort(0, Qt::AscendingOrder);

//        for(int i = 0; i < _attrModel->rowCount(); i++)
//        {
//            bool convertationStatus = false;
//            int idInAttr = _attrModel->item(i, 0)->data(Qt::EditRole).toInt(&convertationStatus);

//            if ((idInAttr == i) && convertationStatus)
//            {
//                QList<QStandardItem*> attrsModelRow;
//                QStandardItem *rootTreeObj = _model->item(i, 1);

//                for(Code c = Code::BrRAv; c < Code::NonAssigned; c++)
//                {
//                    QStandardItem *attrInCurrentRow = new QStandardItem();
//                    //(_attrModel->columnCount() <= static_cast<int>(Code::NonAssigned)) &&
//                    if(  _objCount >= 0)
//                    {
//                        attrInCurrentRow = _attrModel->item(i, static_cast<int>(c));
//                        attrsModelRow << attrInCurrentRow;

//                        QVariant dataFromAttr = attrInCurrentRow->data(Qt::EditRole);
//                        Attribute newAtr(c, dataFromAttr);
//                        pushObjectAttributeById(idInAttr, newAtr);

////                       //bool converted;
////                        if (Attribute::isInt(c))
////                        {
////                            addIntegerItem(rootTreeObj, newAtr.name(), newAtr.value().toInt(&converted));
////                            qDebug() << rootTreeObj->data(Qt::DisplayRole);
//////                            qDebug() << idInAttr << " : i = " << i << "; C = " << static_cast<int>(c) << "\n"
//////                                     <<  "Integer added. Root item: " << rootTreeObj->data(Qt::DisplayRole)
//////                                      << ". Status of convertation: " << converted;
////                        }
////                        else
////                        {
////                            addDoubleItem(rootTreeObj, newAtr.name(), newAtr.value().toDouble(&converted));
//////                            qDebug() << idInAttr << " : i = " << i << "; C = " << static_cast<int>(c) << "\n"
//////                                     <<  "Double added. Root item: " << rootTreeObj->data(Qt::DisplayRole)
//////                                      << ". Status of convertation: " << converted;
////                        }


////                        if (!converted)
////                           // qDebug() <<  "DataModel Error"<< "Unable to convert DataModel to TreeModel";

//                    }// Если колонок не больше чем кодов аттрибутов

//                } //обошли все столбцы

//            } //Если таблица правильно и по порядку отсортирована

//            emitebug() << _objectsOnImage.at(idInAttr);
//            //addObjectRootItem(_model, _objectsOnImage.at(idInAttr));
//        } //обошли все строки
//    } //Если сигнал был эмитирован и модель не пустая

 //`   for(auto ob : )
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
    QFileInfo fi(dataFilePath);
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

        qDebug() << "Считыване файла " << fi.completeBaseName() << " - завешено успешно за " << delta << " миллисекунд.";

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

//template<typename T>
QVector<int> DataModel::getRangeByCode(Code code)
{
    QVector<int> range;
    for(auto object : getObjectsOnImage())
    {
        qDebug() << object.id();
        int rOfO = object.getDescriptors().at(static_cast<int>(code))->value().toInt();
        range << rOfO;
    }

    qDebug() << range;
    return range;
}
