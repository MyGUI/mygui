#ifdef MYGUI_OGRE_PLATFORM
#    define MYGUI_CUSTOM_ALLOCATOR
#    include <Ogre.h>
# ifdef OISHEAD
#  include <OIS.h>
# else
#  include <OIS/OIS.h>
# endif
#elif  MYGUI_DIRECTX_PLATFORM
#elif  MYGUI_OPENGL_PLATFORM
#endif

#include <MyGUI.h>
