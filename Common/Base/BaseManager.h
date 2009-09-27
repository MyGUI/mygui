
#include "Input/OIS/InputManager.h"

#ifdef MYGUI_OGRE_PLATFORM
#    include "Base/Ogre/BaseManager.h"
#elif  MYGUI_DIRECTX_PLATFORM
#    include "Base/DirectX/BaseManager.h"
#elif  MYGUI_OPENGL_PLATFORM
#    include "Base/OpenGL/BaseManager.h"
#endif
