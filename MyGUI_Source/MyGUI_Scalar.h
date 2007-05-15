#pragma once

#include "MyGUI_Defines.h"

/*******************************************************
Provides an abstract scalar interface that unifies Real
and pixel coordinates, and other exotic new position
types one can imagine.

All position types are converted to a universal coordinate
system that is square (as opposed to real coordinates which
usually aren't).  The current system in use is the
percentage of the screen height.  This gives the resolution
independance of traditional real coordiantes but avoids
issues with moving from a 4:3 aspect ratio to a widescreen
*******************************************************/
namespace MyGUI
{
    class Scalar
    {
        typedef Real UniversalScalar;
        
        protected:
        UniversalScalar mPosition;
        
        public:
        //Dynamic predefined "constants".
        static Scalar PredefinedEntireScreenWidth();
        static Scalar PredefinedEntireScreenHeight();
        static Scalar PredefinedHalfScreenWidth();
        static Scalar PredefinedHalfScreenHeight();
        static Scalar FromRelativeWidth(const Real Width);
        static Scalar FromRelativeHeight(const Real Height);
        
        //Default ctor: default to the "default" setting
        Scalar();
        
        //implicit conversion AND ctor
        Scalar(int PixelScalar);
        Scalar(unsigned int PixelScalar);
        
        //implicit conversion AND ctor
        Scalar(const Real &PercentScreenHeight);
        
        Scalar operator +(const Scalar &other) const;
        Scalar operator -(const Scalar &other) const;
        
        //Multiplication by actual numbers is well defined
        Scalar operator *(const int number) const;
        Scalar operator *(const unsigned int number) const;
        Scalar operator *(const Real number) const;
        Scalar &operator =(const Scalar &other) const;
        
        Scalar &operator +=(const Scalar &other);
        Scalar &operator -=(const Scalar &other);

        bool operator <  (const Scalar &other) const;
        bool operator <= (const Scalar &other) const;
        bool operator >  (const Scalar &other) const;
        bool operator >= (const Scalar &other) const;
        bool operator == (const Scalar &other) const;
        
        Scalar &Halve();
        Scalar Half();
        
        Scalar &setDefault(bool IsDefaultSet = true);        
        bool isDefault() const;
        
        String ToString() const;
    };
    
    Scalar operator - (const unsigned int &lhs, const Scalar &rhs);
    Scalar operator + (const unsigned int &lhs, const Scalar &rhs);
    Scalar operator - (const int &lhs, const Scalar &rhs);
    Scalar operator + (const int &lhs, const Scalar &rhs);
    Scalar operator - (const Real &lhs, const Scalar &rhs);
    Scalar operator + (const Real &lhs, const Scalar &rhs);
}