  Attribute::Type type = Type::NonAssigned

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
  }

  return type;