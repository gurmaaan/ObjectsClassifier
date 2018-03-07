#include "attribute.h"

Attribute::Attribute(QObject *parent) : QObject(parent)
{


}

//static QString Attribute::displayName(AttributeCode code)
//{
//    QString name = "";
//    switch (code) {
//    case AttributeCode::BrRAv:
//       name = "Средняя яркость по R";
//       break;
//    case AttributeCode::BrGAv:
//       name = "Средняя яркость по G";
//       break;
//    case AttributeCode::BrBAv:
//       name = "Средняя яркость по B";
//       break;

//    case AttributeCode::BrRsko:
//       name = "СКО по R";
//       break;
//    case AttributeCode::BrGsko:
//       name = "СКО по G";
//       break;
//    case AttributeCode::BrBsko:
//       name = "СКО по B";
//       break;

//    case AttributeCode::TextRmps:
//       name = "Текстурный признак по МПС для R";
//       break;
//    case AttributeCode::TextGmps:
//       name = "Текстурный признак по МПС для G";
//       break;
//    case AttributeCode::TextBmps:
//       name = "Текстурный признак по МПС для B";
//       break;

//    case AttributeCode::TextRser:
//       name = "Текстурный признак по длине серии R";
//       break;
//    case AttributeCode::TextGser:
//       name = "Текстурный признак по длине серии G";
//       break;
//    case AttributeCode::TextBser:
//       name = "Текстурный признак по длине серии B";
//       break;

//    case AttributeCode::GeomForm:
//       name = "Коэффициент формы";
//       break;
//    case AttributeCode::GeomSquare:
//       name = "Пощадь";
//       break;
//    case AttributeCode::GeomPerim:
//       name = "Периметр";
//       break;
//    default:
//       name = "Дескриптор";
//       break;
//    }
//    return name;
//}
