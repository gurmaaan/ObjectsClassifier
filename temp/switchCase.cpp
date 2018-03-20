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



template< typename T >
class Enum
{
public:
   class Iterator
   {
   public:
      Iterator( int value ) :
         m_value( value )
      { }

      T operator*( void ) const
      {
         return (T)m_value;
      }

      void operator++( void )
      {
         ++m_value;
      }

      bool operator!=( Iterator rhs )
      {
         return m_value != rhs.m_value;
      }

   private:
      int m_value;
   };

};

template< typename T >
typename Enum<T>::Iterator begin( Enum<T> )
{
   return typename Enum<T>::Iterator( (int)T::First );
}

template< typename T >
typename Enum<T>::Iterator end( Enum<T> )
{
   return typename Enum<T>::Iterator( ((int)T::Last) + 1 );
}

enum class Color
{
   Red,
   Green,
   Blue,
   First = Red,
   Last = Blue
};

int main()
{
   for( auto e: Enum<Color>() )
   {
      std::cout << ((int)e) << std::endl;
   }
}