#include "precompiled.h"
#ifdef BASE_PLATFORM_OGRE
#    include "Base/Ogre/BaseManager.cpp"
#elif  BASE_PLATFORM_DIRECTX
#    include "Base/DirectX/BaseManager.cpp"
#elif  BASE_PLATFORM_OPENGL
#    include "Base/OpenGL/BaseManager.cpp"
#endif
