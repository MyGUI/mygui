#include "precompiled.h"

#ifdef MYGUI_SAMPLES_INPUT_OIS
#    include "Input/OIS/InputManager.cpp"
#    include "Input/OIS/PointerManager.cpp"
#elif MYGUI_SAMPLES_INPUT_WIN32
#    include "Input/Win32/InputManager.cpp"
#    include "Input/Win32/PointerManager.cpp"
#endif

#ifdef MYGUI_OGRE_PLATFORM
#    include "Base/Ogre/BaseManager.cpp"
#elif  MYGUI_DIRECTX_PLATFORM
#    include "Base/DirectX/BaseManager.cpp"
#elif  MYGUI_OPENGL_PLATFORM
#    include "Base/OpenGL/BaseManager.cpp"
#endif
