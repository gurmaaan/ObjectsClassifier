#ifndef CONST_H
#define CONST_H

#ifndef EXTRA_PIC_PATH
#define EXTRA_PIC_PATH QString("ObjectsClassifier/images")
#endif

#ifndef EXTRA_DAT_PATH
#define EXTRA_DAT_PATH QString("ObjectsClassifier/data")
#endif

#ifndef DAT_EXT
#define DAT_EXT QString("DAT(*.dat)")
#endif

#ifndef EXTRA_ATTR_PATH
#define EXTRA_ATTR_PATH QString("ObjectsClassifier/attributes")
#endif

#ifndef ATTR_EXT
#define ATTR_EXT QString("CSV(*.csv)")
#endif

#define TFS ";;"
//по названию (Отсеивание конкретно этих типов среди остальных )
#define N_F_PNG QString("PNG Images (*.png)")
#define N_F_BMP QString("Bitmap Image (*.bmp)")
#define N_F_JPG QString("JPG Images (*.jpg)")
#define N_F_TIF QString("TIF Images (*.tif)")
#define N_F_ALL (N_F_BMP + TFS + N_F_JPG + TFS + N_F_PNG + TFS + N_F_TIF)

#endif // CONST_H
