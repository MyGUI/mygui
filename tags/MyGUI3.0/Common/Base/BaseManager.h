
#ifdef MYGUI_SAMPLES_INPUT_OIS
#    include "Input/OIS/InputManager.h"
#    include "Input/OIS/PointerManager.h"
#elif MYGUI_SAMPLES_INPUT_WIN32
#    include "Input/Win32/InputManager.h"
#    include "Input/Win32/PointerManager.h"
#elif MYGUI_SAMPLES_INPUT_WIN32_OIS
#    include "Input/Win32_OIS/InputManager.h"
#    include "Input/Win32_OIS/PointerManager.h"
#endif

#ifdef MYGUI_OGRE_PLATFORM
#    include "Base/Ogre/BaseManager.h"
#elif  MYGUI_DIRECTX_PLATFORM
#    include "Base/DirectX/BaseManager.h"
#elif  MYGUI_OPENGL_PLATFORM
#    include "Base/OpenGL/BaseManager.h"
#endif
