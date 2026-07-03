#ifndef CHECK_GL_ERROR_DEBUG //()
#define CHECK_GL_ERROR_DEBUG() \
	do \
	{ \
		GLenum _error = glGetError(); \
		if (_error) \
		{ \
			MYGUI_PLATFORM_LOG( \
				Info, \
				"OpenGLES error 0x%04X in " << _error << " " << __FILE__ << " " << __FUNCTION__ << " " << __LINE__); \
		} \
	} while (false)
#endif
