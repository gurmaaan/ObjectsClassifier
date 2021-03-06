#include "attribute.h"

#ifndef TRANSP
#define TRANSP QColor(255, 255, 255, 255)
#endif




Attribute::Attribute(Code code)
    : _code(code)
{
    _name = genDisplayName(code);
    _colNum = genColumnNumber(code);
    _type = genAttrType(code);
    setAtrColor(TRANSP);
    setValue(0);
}

Attribute::Attribute(const Attribute &other)
{
    _code = other.getCode();
    _name = genDisplayName(_code);
    _colNum = genColumnNumber(_code);
    _type = genAttrType(_code);
    setAtrColor(TRANSP);
    _value = other.value();
}

Attribute::Attribute(const int code)
{
    _code = static_cast<Code>(code);
    _name = genDisplayName(_code);
    _colNum = genColumnNumber(_code);
    _type = genAttrType(_code);
    setAtrColor(TRANSP);
    setValue(0);
}

Attribute::Attribute(Code code, int value)
    : _code(code),
      _value(value)
{
    _name = genDisplayName(code);
    _colNum = genColumnNumber(code);
    setAtrColor(TRANSP);;
    _type = genAttrType(code);
}

Attribute::Attribute(Code code, QVariant value)
    : _code(code),
      _value(value)
{
    _name = genDisplayName(code);
    _colNum = genColumnNumber(code);
    setAtrColor(TRANSP);
    _type = genAttrType(code);
}

Attribute::Attribute(const int code, int value)
    : _value(value)
{
    _code = static_cast<Code>(code);
    _name = genDisplayName(_code);
    _colNum = genColumnNumber(_code);
    setAtrColor(TRANSP);
    _type = genAttrType(_code);
}

Attribute::~Attribute()
{

}

Attribute &Attribute::operator =(const Attribute &at)
{
    if ( this->_code == at.getCode())
    {
        setValue(at.value());
        return *this;

    } else
    {
        return *this;
    }
}

QString Attribute::name() const
{
    return _name;
}

int Attribute::colNum() const
{
    return _colNum;
}

QVariant Attribute::value() const
{
    return _value;
}

QString Attribute::getTypeName() const
{
    QString typeName = "Non assigned";
    Attribute::Type type = this->getType();
    switch ( type )
    {
        case Attribute::Type::Brightness:
            typeName = "Яркостный";
            break;
        case Attribute::Type::Texture:
            typeName = "Текстурный";
            break;
        case Attribute::Type::Geometrical:
            typeName = "Геометрический";
            break;
        default:
            typeName = "Не определено";
            break;
    }
    return  typeName;
}

Code Attribute::getCode() const
{
    return _code;
}

void Attribute::setValue(QVariant value)
{
    _value = value;
}

Attribute::Type Attribute::getType() const
{
    return _type;
}

void Attribute::setCode(const int &code)
{
    _code = static_cast<Code>(code);
    _name = genDisplayName(_code);
    _colNum = genColumnNumber(_code);
    _type = genAttrType(_code);
}

QColor Attribute::interpritatedColor(Code c, int v)
{
    QColor newColor = TRANSP;
    switch (c) {
    case Code::BrRAv:
        newColor.setRed( v );
        break;

    case Code::BrGAv:
        newColor.setGreen( v );
        break;

    case Code::BrBAv:
        newColor.setBlue( v );
        break;

    default:
        break;
    }

    setAtrColor(newColor);
    return getAtrColor();
}

void Attribute::setAtrColor(const QColor &value)
{
    _atrColor = value;
}

QColor Attribute::getAtrColor()
{
    return _atrColor;
}

int Attribute::columnFromCode(Code code)
{
    int colNum = 0;

    switch (code) {
    case Code::BrRAv:
       colNum = 1;
       break;
    case Code::BrGAv:
       colNum = 2;
       break;
    case Code::BrBAv:
       colNum = 3;
       break;

    case Code::BrRsko:
       colNum = 3;
       break;
    case Code::BrGsko:
       colNum = 4;
       break;
    case Code::BrBsko:
       colNum = 5;
       break;

    case Code::TextRmps:
       colNum = 10;
       break;
    case Code::TextGmps:
       colNum = 11;
       break;
    case Code::TextBmps:
       colNum = 12;
       break;

    case Code::TextRser:
       colNum = 13;
       break;
    case Code::TextGser:
       colNum = 14;
       break;
    case Code::TextBser:
       colNum = 15;
       break;

    case Code::GeomForm:
       colNum = 16;
       break;
    case Code::GeomSquare:
       colNum = 17;
       break;
    case Code::GeomPerim:
       colNum = 18;
       break;
    default:
        colNum = 0;
        break;
    }

    return colNum;

}

bool Attribute::isInt(Code code)
{
    bool integerValue = false;

    switch (code)
    {
        case Code::BrRAv:
            integerValue = true;
            break;
        case Code::BrGAv:
            integerValue = true;
            break;
        case Code::BrBAv:
            integerValue = true;
            break;

        case Code::BrRsko:
            integerValue = false;
            break;
        case Code::BrGsko:
            integerValue = false;
            break;
        case Code::BrBsko:
            integerValue = false;
            break;

        case Code::TextRmps:
            integerValue = true;
            break;
        case Code::TextGmps:
            integerValue = true;
            break;
        case Code::TextBmps:
            integerValue = true;
            break;

        case Code::TextRser:
            integerValue = true;
            break;
        case Code::TextGser:
            integerValue = true;
            break;
        case Code::TextBser:
            integerValue = true;
            break;

        case Code::GeomForm:
            integerValue = false;
            break;
        case Code::GeomSquare:
            integerValue = true;
            break;
        case Code::GeomPerim:
            integerValue = true;
            break;

        default:
            integerValue = false;
            break;
    }

    return integerValue;
}

QColor Attribute::colorOf(Code code)
{
    QColor codeColor = Qt::transparent;

    switch (code) {
    case Code::BrRAv: //RED
       codeColor = QColor(Qt::red).lighter(150);
       break;
    case Code::BrGAv: //GREEN
       codeColor = QColor(Qt::green).lighter(150);
       break;
    case Code::BrBAv: //BLUE
       codeColor = QColor(Qt::blue).lighter(150);
       break;

    case Code::BrRsko: //RED
       codeColor =  QColor(Qt::red).lighter(150);
       break;
    case Code::BrGsko: //GREEN
       codeColor =  QColor(Qt::green).lighter(150);
       break;
    case Code::BrBsko: //BLUE
       codeColor =  QColor(Qt::blue).lighter(150);
       break;

    case Code::TextRmps: //RED
       codeColor = QColor(Qt::red).lighter(150);
       break;
    case Code::TextGmps: //GREEN
       codeColor = QColor(Qt::green).lighter(150);
       break;
    case Code::TextBmps: //BLUE
       codeColor = QColor(Qt::blue).lighter(150);
       break;

    case Code::TextRser: //RED
       codeColor = QColor(Qt::red).lighter(150);
       break;
    case Code::TextGser: //GREEN
       codeColor = QColor(Qt::green).lighter(150);
       break;
    case Code::TextBser: //BLUE
       codeColor = QColor(Qt::blue).lighter(150);
       break;

    case Code::GeomForm: //GRAY
       codeColor = QColor(Qt::lightGray);
       break;
    case Code::GeomSquare:
       codeColor = QColor(Qt::lightGray);
       break;
    case Code::GeomPerim:
       codeColor = QColor(Qt::lightGray);
       break;

    default:
      codeColor = QColor(Qt::transparent);
      break;
    }

    return codeColor;
}

//Attribute::Type Attribute::typeOfCode(Code code)
//{
//    Attribute::Type type = Type::NonAssigned;

//    switch (code) {
//    case Code::BrRAv:
//       type = Type::Brightness;
//       break;
//    case Code::BrGAv:
//       type = Type::Brightness;
//       break;
//    case Code::BrBAv:
//       type = Type::Brightness;
//       break;

//    case Code::BrRsko:
//       type = Type::Brightness;
//       break;
//    case Code::BrGsko:
//       type = Type::Brightness;
//       break;
//    case Code::BrBsko:
//       type = Type::Brightness;
//       break;

//    case Code::TextRmps:
//       type = Type::Texture;
//       break;
//    case Code::TextGmps:
//       type = Type::Texture;
//       break;
//    case Code::TextBmps:
//       type = Type::Texture;
//       break;

//    case Code::TextRser:
//       type = Type::Texture;
//       break;
//    case Code::TextGser:
//       type = Type::Texture;
//       break;
//    case Code::TextBser:
//       type = Type::Texture;
//       break;

//    case Code::GeomForm:
//       type = Type::Geometrical;
//       break;
//    case Code::GeomSquare:
//       type = Type::Geometrical;
//       break;
//    case Code::GeomPerim:
//       type = Type::Geometrical;
//       break;
//    default:
//        type = Type::NonAssigned;
//        break;
//    }

//    return type;
//}

QString Attribute::displayNameOf(Code code)
{
    QString name = "";

    switch (code) {
    case Code::BrRAv:
        name = "Средняя яркость по R";
       break;
    case Code::BrGAv:
       name = "Средняя яркость по G";
       break;
    case Code::BrBAv:
       name = "Средняя яркость по B";
       break;

    case Code::BrRsko:
       name = "СКО по R";
       break;
    case Code::BrGsko:
       name = "СКО по G";
       break;
    case Code::BrBsko:
       name = "СКО по B";
       break;

    case Code::TextRmps:
       name = "Текстурный признак по МПС для R";
       break;
    case Code::TextGmps:
       name = "Текстурный признак по МПС для G";
       break;
    case Code::TextBmps:
       name = "Текстурный признак по МПС для B";
       break;

    case Code::TextRser:
       name = "Текстурный признак по длине серии R";
       break;
    case Code::TextGser:
       name = "Текстурный признак по длине серии G";
       break;
    case Code::TextBser:
       name = "Текстурный признак по длине серии B";
       break;

    case Code::GeomForm:
       name = "Коэффициент формы";
       break;
    case Code::GeomSquare:
       name = "Пощадь";
       break;
    case Code::GeomPerim:
       name = "Периметр";
       break;
    default:
        name = "Неизвестный параметр";
        break;
    }

    return name;
}

QString Attribute::genDisplayName(Code code)
{
    QString name = "";

    switch (code) {
    case Code::BrRAv:
        name = "Средняя яркость \nпо R";
       break;
    case Code::BrGAv:
       name = "Средняя яркость \nпо G";
       break;
    case Code::BrBAv:
       name = "Средняя яркость \nпо B";
       break;

    case Code::BrRsko:
       name = "СКО по R";
       break;
    case Code::BrGsko:
       name = "СКО по G";
       break;
    case Code::BrBsko:
       name = "СКО по B";
       break;

    case Code::TextRmps:
       name = "Текстурный признак \nпо МПС \nдля R";
       break;
    case Code::TextGmps:
       name = "Текстурный признак \nпо МПС \nдля G";
       break;
    case Code::TextBmps:
       name = "Текстурный признак \nпо МПС \nдля B";
       break;

    case Code::TextRser:
       name = "Текстурный признак \nпо длине \nсерии R";
       break;
    case Code::TextGser:
       name = "Текстурный признак \nпо длине \nсерии G";
       break;
    case Code::TextBser:
       name = "Текстурный признак \nпо длине \nсерии B";
       break;

    case Code::GeomForm:
       name = "Коэффициент \nформы";
       break;
    case Code::GeomSquare:
       name = "Пощадь";
       break;
    case Code::GeomPerim:
       name = "Периметр";
       break;
    default:
        name = "Неизвестный параметр";
        break;
    }

    return name;
}

int Attribute::genColumnNumber(Code code)
{
    int colNum = 0;

    switch (code) {
    case Code::BrRAv:
       colNum = 1;
       break;
    case Code::BrGAv:
       colNum = 2;
       break;
    case Code::BrBAv:
       colNum = 3;
       break;

    case Code::BrRsko:
       colNum = 3;
       break;
    case Code::BrGsko:
       colNum = 4;
       break;
    case Code::BrBsko:
       colNum = 5;
       break;

    case Code::TextRmps:
       colNum = 10;
       break;
    case Code::TextGmps:
       colNum = 11;
       break;
    case Code::TextBmps:
       colNum = 12;
       break;

    case Code::TextRser:
       colNum = 13;
       break;
    case Code::TextGser:
       colNum = 14;
       break;
    case Code::TextBser:
       colNum = 15;
       break;

    case Code::GeomForm:
       colNum = 16;
       break;
    case Code::GeomSquare:
       colNum = 17;
       break;
    case Code::GeomPerim:
       colNum = 18;
       break;
    default:
        colNum = 0;
        break;
    }

    return colNum;
}

Attribute::Type Attribute::genAttrType(Code code)
{
    Attribute::Type type = Type::NonAssigned;

    switch (code) {
    case Code::BrRAv:
       type = Type::Brightness;
       break;
    case Code::BrGAv:
       type = Type::Brightness;
       break;
    case Code::BrBAv:
       type = Type::Brightness;
       break;

    case Code::BrRsko:
       type = Type::Brightness;
       break;
    case Code::BrGsko:
       type = Type::Brightness;
       break;
    case Code::BrBsko:
       type = Type::Brightness;
       break;

    case Code::TextRmps:
       type = Type::Texture;
       break;
    case Code::TextGmps:
       type = Type::Texture;
       break;
    case Code::TextBmps:
       type = Type::Texture;
       break;

    case Code::TextRser:
       type = Type::Texture;
       break;
    case Code::TextGser:
       type = Type::Texture;
       break;
    case Code::TextBser:
       type = Type::Texture;
       break;

    case Code::GeomForm:
       type = Type::Geometrical;
       break;
    case Code::GeomSquare:
       type = Type::Geometrical;
       break;
    case Code::GeomPerim:
       type = Type::Geometrical;
       break;
    default:
        type = Type::NonAssigned;
        break;
    }

    return type;
}

QDebug operator <<(QDebug dbg, const Attribute &at)
{
    dbg << "Attribute";
    dbg << "\tCode:  " << static_cast<int>(at.getCode()) << endl;
    dbg << "\tType:  " << at.getTypeName();
    dbg << "\tName:  " << at.name();
    dbg << "\tValue: " << at.value() << endl;
    dbg << "---------" << endl;
    return dbg;
}

bool operator ==(const Attribute &at1, const Attribute &at2)
{
    bool codeEqual = at1.getCode() == at2.getCode() ? true : false;
    bool valueEqual = at1.value() == at2.value() ? true : false;

    return (codeEqual && valueEqual);
}

bool operator>(const Code c, const int i)
{
    if ( static_cast<int>(c) > i )
        return true;
    else
        return false;
}

bool operator>(const int i, const Code c)
{
    if(i > static_cast<int>(c))
        return operator<(c, i);
    else
        return operator>(c, i);
}

bool operator<(const Code c, const int i)
{
    if ( static_cast<int>(c) < i )
        return true;
    else
        return false;
}

bool operator<(const int i, const Code c)
{
    if(i < static_cast<int>(c))
        return operator>(c, i);
    else
        return operator<(c, i);

}

bool operator>=(const Code c1, const Code c2)
{
    int i1 = static_cast<int>(c1);
    int i2 = static_cast<int>(c2);

    return (i1>=i2);
}

bool operator<=(const Code c1, const Code c2)
{
    int i1 = static_cast<int>(c1);
    int i2 = static_cast<int>(c2);

    return (i1<=i2);
}

Code &operator++(Code &c) //++code;
{
    c = static_cast<Code>(static_cast<int>(c) + 1);
    return c;
}

Code operator++(Code& c, int) //code++;
{
    Code old = c;
    c = static_cast<Code>(static_cast<int>(c)+1);
    return old;
}

Code operator*(Code c)
{
    return c;
}

Code begin(Code c)
{
    return Code::First;
}

Code end(Code c)
{
    Code lastc = Code::Last;
    return lastc++;
}
