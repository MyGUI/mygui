#pragma once

#include <OgreVector2.h>

namespace MyGUI
{

struct Coord
{
    int x,y;
    
    Coord(int X, int Y) : x(X), y(Y) { }
    
    const Coord operator * (const int Multiplier) {
        return Coord(x * Multiplier, y * Multiplier);
    }
    
    //Implicit conversion
    Coord(const Ogre::Vector2 &Coordinate)
        : x((int)Coordinate.x), y((int)Coordinate.y) { }        
};
}