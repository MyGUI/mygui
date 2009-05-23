#ifdef BASE_PLATFORM_OGRE
#    define MYGUI_CUSTOM_ALLOCATOR
#    include <Ogre.h>
#    include <OIS/OIS.h>
#elif  BASE_PLATFORM_DIRECTX
#elif  BASE_PLATFORM_OPENGL
#endif

#include <MyGUI.h>
