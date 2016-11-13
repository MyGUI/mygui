#ifndef CHECK_GL_ERROR_DEBUG//()
#define CHECK_GL_ERROR_DEBUG() \
do { \
GLenum __error = glGetError(); \
if(__error) { \
MYGUI_PLATFORM_LOG(Info, "OpenGLES error 0x%04X in " << __error << " " << __FILE__<< " " << __FUNCTION__<< " "<< __LINE__); \
} \
} while (false)
#endif
