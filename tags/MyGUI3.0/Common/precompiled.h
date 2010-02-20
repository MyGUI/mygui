#ifdef MYGUI_OGRE_PLATFORM
//#  define MYGUI_CUSTOM_ALLOCATOR
//#  include <Ogre.h>

#ifdef MYGUI_SAMPLES_INPUT_OIS
#  include <OIS.h>
#elif MYGUI_SAMPLES_INPUT_WIN32
//#  include <OIS.h>
#elif MYGUI_SAMPLES_INPUT_WIN32_OIS
#  include <OIS.h>
#endif

#elif  MYGUI_DIRECTX_PLATFORM
#elif  MYGUI_OPENGL_PLATFORM
#endif

#include <MyGUI.h>
