#if defined(__APPLE__)
	#include <OpenGL/gl.h>
#elif defined(_WIN32)
	#include <glad/glad.h>
#else
	#include <GL/gl.h>
#endif

// GLAD may omit extension tokens. Support is checked at runtime before using them.
#ifndef GL_VERTEX_PROGRAM_ARB
	#define GL_VERTEX_PROGRAM_ARB 0x8620
#endif
#ifndef GL_FRAGMENT_PROGRAM_ARB
	#define GL_FRAGMENT_PROGRAM_ARB 0x8804
#endif
#ifndef GL_RASTERIZER_DISCARD_EXT
	#define GL_RASTERIZER_DISCARD_EXT 0x8C89
#endif
#ifndef GL_TEXTURE_RECTANGLE_ARB
	#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#endif
