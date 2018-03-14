    inline void setMinOX(int minOX) { _minOX = minOX;}
    inline void setMinOY(int minOY) { _minOY = minOY;}
    inline void setMaxOX(int maxOX) { _maxOX = maxOX;}
    inline void setMaxOY(int maxOY) { _maxOY = maxOY;}

    inline void setMinCX(int minCX) {_minCX = minCX; }
    inline void setMinCY(int minCY) {_minCY = minCY; }
    inline void setMaxCX(int maxCX) {_maxCX = maxCX; }
    inline void setMaxCY(int maxCY) {_maxCY = maxCY; }

    int getMinOX() const;
    void setMinOX(int minOX);
    void 

    int getMinOY() const;
    void setMinOY(int minOY);
    void 

    int getMaxOX() const;
    void setMaxOX(int maxOX);
    void 

    int getMaxOY() const;
    void setMaxOY(int maxOY);
    void 

    setMinOX( getMinX(_internalPoints) );
    setMinOY( getMinY(_internalPoints) );
    setMaxOX( getMaxX(_internalPoints) );
    setMaxOY( getMaxY(_internalPoints) );
