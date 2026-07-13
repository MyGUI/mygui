#if defined(__APPLE__)
	#include <OpenGL/gl3.h>
#elif defined(_WIN32)
	#include <glad/glad.h>
#else
	#include <GL/gl.h>
#endif
