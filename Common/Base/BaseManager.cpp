#include "precompiled.h"

#include "Input/OIS/InputManager.cpp"

#ifdef MYGUI_OGRE_PLATFORM
#    include "Base/Ogre/BaseManager.cpp"
#elif  MYGUI_DIRECTX_PLATFORM
#    include "Base/DirectX/BaseManager.cpp"
#elif  MYGUI_OPENGL_PLATFORM
#    include "Base/OpenGL/BaseManager.cpp"
#endif
