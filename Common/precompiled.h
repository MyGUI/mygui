#ifdef MYGUI_OGRE_PLATFORM
#    define MYGUI_CUSTOM_ALLOCATOR
#    include <Ogre.h>
#    include <OIS/OIS.h>
#elif  MYGUI_DIRECTX_PLATFORM
#elif  MYGUI_OPENGL_PLATFORM
#endif

#include <MyGUI.h>
