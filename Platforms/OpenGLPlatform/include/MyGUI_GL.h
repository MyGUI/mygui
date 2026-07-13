#if defined(__APPLE__)
	#include <OpenGL/gl.h>
#elif defined(_WIN32)
	#include <glad/glad.h>
#else
	#include <GL/gl.h>
#endif
