
#pragma once

#include <cstdlib>

namespace msdfgen {

typedef unsigned char byte;

/// Reference to a 2D image bitmap or a buffer acting as one. Pixel storage not owned or managed by the object.
template <typename T, int N = 1>
struct BitmapRef {

    T *pixels;
    int width, height;

    inline BitmapRef() : pixels(NULL), width(0), height(0) { }
    inline BitmapRef(T *_pixels, int _width, int _height) : pixels(_pixels), width(_width), height(_height) { }

    inline T * operator()(int x, int y) const {
        return pixels+N*(width*y+x);
    }

};

/// Constant reference to a 2D image bitmap or a buffer acting as one. Pixel storage not owned or managed by the object.
template <typename T, int N = 1>
struct BitmapConstRef {

    const T *pixels;
    int width, height;

    inline BitmapConstRef() : pixels(NULL), width(0), height(0) { }
    inline BitmapConstRef(const T *_pixels, int _width, int _height) : pixels(_pixels), width(_width), height(_height) { }
    inline BitmapConstRef(const BitmapRef<T, N> &orig) : pixels(orig.pixels), width(orig.width), height(orig.height) { }

    inline const T * operator()(int x, int y) const {
        return pixels+N*(width*y+x);
    }

};

}
