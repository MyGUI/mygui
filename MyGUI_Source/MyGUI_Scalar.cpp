#include "MyGUI_Scalar.h"

#pragma once

#include "MyGUI_Defines.h"
#include "MyGUI_GUI.h"
#include <OgreStringConverter.h>

using namespace Ogre;

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
    Scalar Scalar::PredefinedEntireScreenWidth()
    {
        return Scalar(GUI::getSingleton()->getAspectRatio());
    }
    
    Scalar Scalar::PredefinedEntireScreenHeight()
    {
        return Scalar(UniversalScalar(1));
    }
    
    Scalar Scalar::PredefinedHalfScreenWidth()
    {
        return Scalar( GUI::getSingleton()->getAspectRatio()).Halve();
    }
    
    Scalar Scalar::PredefinedHalfScreenHeight()
    {
        return PredefinedEntireScreenHeight().Halve();
    }
    
    Scalar FromRelativeWidth(const Real Width)
    {
        return Scalar(Width * GUI::getSingleton()->getAspectRatio());
    }
    
    Scalar FromRelativeHeight(const Real Height)
    {
        return Scalar(Height);
    }
    
    //Default ctor: default to the "default" setting
    Scalar::Scalar() : mPosition(UniversalScalar(-1))
    {
    }
    
    //implicit conversion AND ctor
    Scalar::Scalar(int PixelScalar)
    {
        mPosition = UniversalScalar(mPosition) / UniversalScalar(GUI::getSingleton()->getHeight());
    }
    
    //implicit conversion AND ctor
    Scalar::Scalar(unsigned int PixelScalar)
    {
        mPosition = UniversalScalar(mPosition) / UniversalScalar(GUI::getSingleton()->getHeight());
    }
    
    //implicit conversion AND ctor
    Scalar::Scalar(const Real &PercentScreenHeight)
    {
        mPosition = UniversalScalar(PercentScreenHeight);
    }
    
    Scalar Scalar::operator +(const Scalar &other) const
    {
        return Scalar(other.mPosition + mPosition);
    }
    
    Scalar Scalar::operator -(const Scalar &other) const
    {
        return Scalar(mPosition - other.mPosition);
    }
    
    Scalar Scalar::operator *(const int number) const
    {
        return Scalar(mPosition * UniversalScalar(number));
    }
    
    Scalar Scalar::operator *(const unsigned int number) const
    {
        return Scalar(mPosition * UniversalScalar(number));
    }
    
    Scalar Scalar::operator *(const Real number) const
    {
        return Scalar(mPosition * UniversalScalar(number));
    }
    
    Scalar &Scalar::operator =(const Scalar &other)
    {
        mPosition = other.mPosition;
        return *this;
    }
    
    Scalar &Scalar::operator +=(const Scalar &other)
    {
        return (*this) = (*this) + other;
    }
    
    Scalar &Scalar::operator -=(const Scalar &other)
    {
        return (*this) = (*this) - other;
    }
    
    Scalar &Scalar::Halve()
    {
        mPosition /= UniversalScalar(2);
        return *this;
    }
    
    Scalar Scalar::Half()
    {
        return Scalar(mPosition).Halve();    
    }
    
    Scalar &Scalar::setDefault(bool IsDefaultSet)
    {
        if(IsDefaultSet)
            mPosition = -fabs(mPosition);
        else
            mPosition = fabs(mPosition);
        
        return *this;
    }
    
    bool Scalar::isDefault() const
    {
        return mPosition < UniversalScalar(0) ? true : false;    
    }
    
    String Scalar::ToString() const
    {
        return StringConverter::toString(mPosition);
    }
    
    bool Scalar::operator <  (const Scalar &other) const
    {
        return mPosition < other.mPosition;    
    }
    
    bool Scalar::operator <= (const Scalar &other) const
    {
        return mPosition <= other.mPosition;
    }
    
    bool Scalar::operator > (const Scalar &other) const
    {
        return mPosition >= other.mPosition;
    }
    
    bool Scalar::operator >= (const Scalar &other) const
    {
        return mPosition >= other.mPosition;
    }
    
    bool Scalar::operator == (const Scalar &other) const
    {
        return mPosition == other.mPosition;
    }    
    
    Scalar operator - (const int &lhs, const Scalar &rhs)
    {
        return Scalar(lhs) - rhs;
    }
    Scalar operator + (const int &lhs, const Scalar &rhs)
    {
        return Scalar(lhs) - rhs;
    }
    
    Scalar operator - (const unsigned int &lhs, const Scalar &rhs)
    {
        return Scalar(lhs) - rhs;
    }
    Scalar operator + (const unsigned int &lhs, const Scalar &rhs)
    {
        return Scalar(lhs) - rhs;
    }
    
    Scalar operator - (const Real &lhs, const Scalar &rhs)
    {
        return Scalar(lhs) - rhs;
    }
    Scalar operator + (const Real &lhs, const Scalar &rhs)
    {
        return Scalar(lhs) - rhs;
    }
}