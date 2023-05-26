/*
** The OpenGL Extension Wrangler Library
** Copyright (C) 2002-2008, Milan Ikits <milan ikits[]ieee org>
** Copyright (C) 2002-2008, Marcelo E. Magallon <mmagallo[]debian org>
** Copyright (C) 2002, Lev Povalahev
** All rights reserved.
** 
** Redistribution and use in source and binary forms, with or without 
** modification, are permitted provided that the following conditions are met:
** 
** * Redistributions of source code must retain the above copyright notice, 
**   this list of conditions and the following disclaimer.
** * Redistributions in binary form must reproduce the above copyright notice, 
**   this list of conditions and the following disclaimer in the documentation 
**   and/or other materials provided with the distribution.
** * The name of the author may be used to endorse or promote products 
**   derived from this software without specific prior written permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
** AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
** IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
** ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
** LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
** CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
** SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
** INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
** CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
** ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
** THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * Mesa 3-D graphics library
 * Version:  7.0
 *
 * Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
** Copyright (c) 2007 The Khronos Group Inc.
** 
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
** 
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
** 
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

#ifndef G_glew_h__
#define G_glew_h__
#define G_GLEW_H__

#if defined(__gl_h_) || defined(__GL_H__) || defined(__X_GL_H)
#error gl.h included before glew.h
#endif
#if defined(__REGAL_H__)
#error Regal.h included before glew.h
#endif
#if defined(__glext_h_) || defined(__GLEXT_H_)
#error glext.h included before glew.h
#endif
#if defined(__gl_ATI_h_)
#error glATI.h included before glew.h
#endif

#define __gl_h_
#define __GL_H__
#define __REGAL_H__
#define __X_GL_H
#define __glext_h_
#define __GLEXT_H_
#define __gl_ATI_h_

#if defined(_WIN32)

/*
 * GLEW does not include <windows.h> to avoid name space pollution.
 * GL needs GLAPI and GLAPIENTRY, GLU needs APIENTRY, CALLBACK, and wchar_t
 * defined properly.
 */
/* <windef.h> */
#ifndef APIENTRY
#define GLEW_APIENTRY_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define APIENTRY __stdcall
#  elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#    define APIENTRY __stdcall
#  else
#    define APIENTRY
#  endif
#endif
#ifndef GLAPI
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define GLAPI extern
#  endif
#endif
/* <winnt.h> */
#ifndef CALLBACK
#define GLEW_CALLBACK_DEFINED
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define CALLBACK __attribute__ ((__stdcall__))
#  elif (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS)
#    define CALLBACK __stdcall
#  else
#    define CALLBACK
#  endif
#endif
/* <wingdi.h> and <winnt.h> */
#ifndef WINGDIAPI
#define GLEW_WINGDIAPI_DEFINED
#define WINGDIAPI __declspec(dllimport)
#endif
/* <ctype.h> */
#if (defined(_MSC_VER) || defined(__BORLANDC__)) && !defined(_WCHAR_T_DEFINED)
typedef unsigned short wchar_t;
#  define _WCHAR_T_DEFINED
#endif
/* <stddef.h> */
#if !defined(_W64)
#  if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && defined(_MSC_VER) && _MSC_VER >= 1300
#    define _W64 __w64
#  else
#    define _W64
#  endif
#endif
#if !defined(_PTRDIFF_T_DEFINED) && !defined(_PTRDIFF_T_) && !defined(__MINGW64__)
#  ifdef _WIN64
typedef __int64 ptrdiff_t;
#  else
typedef _W64 int ptrdiff_t;
#  endif
#  define _PTRDIFF_T_DEFINED
#  define _PTRDIFF_T_
#endif

#ifndef GLAPI
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#    define GLAPI extern
#  else
#    define GLAPI WINGDIAPI
#  endif
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifndef GLEWAPIENTRY
#define GLEWAPIENTRY APIENTRY
#endif

/*
 * GLEW_STATIC is defined for static library.
 * GLEW_BUILD  is defined for building the DLL library.
 */

#ifdef GLEW_STATIC
#  define GLEWAPI extern
#else
#  ifdef GLEW_BUILD
#    define GLEWAPI extern __declspec(dllexport)
#  else
#    define GLEWAPI extern __declspec(dllimport)
#  endif
#endif

#else /* _UNIX */

/*
 * Needed for ptrdiff_t in turn needed by VBO.  This is defined by ISO
 * C.  On my system, this amounts to _3 lines_ of included code, all of
 * them pretty much harmless.  If you know of a way of detecting 32 vs
 * 64 _targets_ at compile time you are free to replace this with
 * something that's portable.  For now, _this_ is the portable solution.
 * (mem, 2004-01-04)
 */

#include <stddef.h>

/* SGI MIPSPro doesn't like stdint.h in C++ mode          */
/* ID: 3376260 Solaris 9 has inttypes.h, but not stdint.h */

#if (defined(__sgi) || defined(__sun)) && !defined(__GNUC__)
#include <inttypes.h>
#else
#include <stdint.h>
#endif

#define GLEW_APIENTRY_DEFINED
#define APIENTRY

/*
 * GLEW_STATIC is defined for static library.
 */

#ifdef GLEW_STATIC
#  define GLEWAPI extern
#else
#  if defined(__GNUC__) && __GNUC__>=4
#   define GLEWAPI extern __attribute__ ((visibility("default")))
#  elif defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#   define GLEWAPI extern __global
#  else
#   define GLEWAPI extern
#  endif
#endif

/* <glu.h> */
#ifndef GLAPI
#define GLAPI extern
#endif

#ifndef GLAPIENTRY
#define GLAPIENTRY
#endif

#ifndef GLEWAPIENTRY
#define GLEWAPIENTRY
#endif

#endif /* _WIN32 */

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------- GL_VERSION_1_1 ---------------------------- */

#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1 1

typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
#if defined(_MSC_VER) && _MSC_VER < 1400
typedef __int64 GLint64EXT;
typedef unsigned __int64 GLuint64EXT;
#elif defined(_MSC_VER) || defined(__BORLANDC__)
typedef signed long long GLint64EXT;
typedef unsigned long long GLuint64EXT;
#else
#  if defined(__MINGW32__) || defined(__CYGWIN__)
#include <inttypes.h>
#  endif
typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
#endif
typedef GLint64EXT  GLint64;
typedef GLuint64EXT GLuint64;
typedef struct __GLsync *GLsync;

typedef char GLchar;

#define GL_ZERO 0
#define GL_FALSE 0
#define GL_LOGIC_OP 0x0BF1
#define GL_NONE 0
#define GL_TEXTURE_COMPONENTS 0x1003
#define GL_NO_ERROR 0
#define GL_POINTS 0x0000
#define GL_CURRENT_BIT 0x00000001
#define GL_TRUE 1
#define GL_ONE 1
#define GL_CLIENT_PIXEL_STORE_BIT 0x00000001
#define GL_LINES 0x0001
#define GL_LINE_LOOP 0x0002
#define GL_POINT_BIT 0x00000002
#define GL_CLIENT_VERTEX_ARRAY_BIT 0x00000002
#define GL_LINE_STRIP 0x0003
#define GL_LINE_BIT 0x00000004
#define GL_TRIANGLES 0x0004
#define GL_TRIANGLE_STRIP 0x0005
#define GL_TRIANGLE_FAN 0x0006
#define GL_QUADS 0x0007
#define GL_QUAD_STRIP 0x0008
#define GL_POLYGON_BIT 0x00000008
#define GL_POLYGON 0x0009
#define GL_POLYGON_STIPPLE_BIT 0x00000010
#define GL_PIXEL_MODE_BIT 0x00000020
#define GL_LIGHTING_BIT 0x00000040
#define GL_FOG_BIT 0x00000080
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_ACCUM 0x0100
#define GL_LOAD 0x0101
#define GL_RETURN 0x0102
#define GL_MULT 0x0103
#define GL_ADD 0x0104
#define GL_NEVER 0x0200
#define GL_ACCUM_BUFFER_BIT 0x00000200
#define GL_LESS 0x0201
#define GL_EQUAL 0x0202
#define GL_LEQUAL 0x0203
#define GL_GREATER 0x0204
#define GL_NOTEQUAL 0x0205
#define GL_GEQUAL 0x0206
#define GL_ALWAYS 0x0207
#define GL_SRC_COLOR 0x0300
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_SRC_ALPHA 0x0302
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_DST_ALPHA 0x0304
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_DST_COLOR 0x0306
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_FRONT_LEFT 0x0400
#define GL_FRONT_RIGHT 0x0401
#define GL_BACK_LEFT 0x0402
#define GL_BACK_RIGHT 0x0403
#define GL_FRONT 0x0404
#define GL_BACK 0x0405
#define GL_LEFT 0x0406
#define GL_RIGHT 0x0407
#define GL_FRONT_AND_BACK 0x0408
#define GL_AUX0 0x0409
#define GL_AUX1 0x040A
#define GL_AUX2 0x040B
#define GL_AUX3 0x040C
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE 0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_OUT_OF_MEMORY 0x0505
#define GL_2D 0x0600
#define GL_3D 0x0601
#define GL_3D_COLOR 0x0602
#define GL_3D_COLOR_TEXTURE 0x0603
#define GL_4D_COLOR_TEXTURE 0x0604
#define GL_PASS_THROUGH_TOKEN 0x0700
#define GL_POINT_TOKEN 0x0701
#define GL_LINE_TOKEN 0x0702
#define GL_POLYGON_TOKEN 0x0703
#define GL_BITMAP_TOKEN 0x0704
#define GL_DRAW_PIXEL_TOKEN 0x0705
#define GL_COPY_PIXEL_TOKEN 0x0706
#define GL_LINE_RESET_TOKEN 0x0707
#define GL_EXP 0x0800
#define GL_VIEWPORT_BIT 0x00000800
#define GL_EXP2 0x0801
#define GL_CW 0x0900
#define GL_CCW 0x0901
#define GL_COEFF 0x0A00
#define GL_ORDER 0x0A01
#define GL_DOMAIN 0x0A02
#define GL_CURRENT_COLOR 0x0B00
#define GL_CURRENT_INDEX 0x0B01
#define GL_CURRENT_NORMAL 0x0B02
#define GL_CURRENT_TEXTURE_COORDS 0x0B03
#define GL_CURRENT_RASTER_COLOR 0x0B04
#define GL_CURRENT_RASTER_INDEX 0x0B05
#define GL_CURRENT_RASTER_TEXTURE_COORDS 0x0B06
#define GL_CURRENT_RASTER_POSITION 0x0B07
#define GL_CURRENT_RASTER_POSITION_VALID 0x0B08
#define GL_CURRENT_RASTER_DISTANCE 0x0B09
#define GL_POINT_SMOOTH 0x0B10
#define GL_POINT_SIZE 0x0B11
#define GL_POINT_SIZE_RANGE 0x0B12
#define GL_POINT_SIZE_GRANULARITY 0x0B13
#define GL_LINE_SMOOTH 0x0B20
#define GL_LINE_WIDTH 0x0B21
#define GL_LINE_WIDTH_RANGE 0x0B22
#define GL_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_LINE_STIPPLE 0x0B24
#define GL_LINE_STIPPLE_PATTERN 0x0B25
#define GL_LINE_STIPPLE_REPEAT 0x0B26
#define GL_LIST_MODE 0x0B30
#define GL_MAX_LIST_NESTING 0x0B31
#define GL_LIST_BASE 0x0B32
#define GL_LIST_INDEX 0x0B33
#define GL_POLYGON_MODE 0x0B40
#define GL_POLYGON_SMOOTH 0x0B41
#define GL_POLYGON_STIPPLE 0x0B42
#define GL_EDGE_FLAG 0x0B43
#define GL_CULL_FACE 0x0B44
#define GL_CULL_FACE_MODE 0x0B45
#define GL_FRONT_FACE 0x0B46
#define GL_LIGHTING 0x0B50
#define GL_LIGHT_MODEL_LOCAL_VIEWER 0x0B51
#define GL_LIGHT_MODEL_TWO_SIDE 0x0B52
#define GL_LIGHT_MODEL_AMBIENT 0x0B53
#define GL_SHADE_MODEL 0x0B54
#define GL_COLOR_MATERIAL_FACE 0x0B55
#define GL_COLOR_MATERIAL_PARAMETER 0x0B56
#define GL_COLOR_MATERIAL 0x0B57
#define GL_FOG 0x0B60
#define GL_FOG_INDEX 0x0B61
#define GL_FOG_DENSITY 0x0B62
#define GL_FOG_START 0x0B63
#define GL_FOG_END 0x0B64
#define GL_FOG_MODE 0x0B65
#define GL_FOG_COLOR 0x0B66
#define GL_DEPTH_RANGE 0x0B70
#define GL_DEPTH_TEST 0x0B71
#define GL_DEPTH_WRITEMASK 0x0B72
#define GL_DEPTH_CLEAR_VALUE 0x0B73
#define GL_DEPTH_FUNC 0x0B74
#define GL_ACCUM_CLEAR_VALUE 0x0B80
#define GL_STENCIL_TEST 0x0B90
#define GL_STENCIL_CLEAR_VALUE 0x0B91
#define GL_STENCIL_FUNC 0x0B92
#define GL_STENCIL_VALUE_MASK 0x0B93
#define GL_STENCIL_FAIL 0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL 0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS 0x0B96
#define GL_STENCIL_REF 0x0B97
#define GL_STENCIL_WRITEMASK 0x0B98
#define GL_MATRIX_MODE 0x0BA0
#define GL_NORMALIZE 0x0BA1
#define GL_VIEWPORT 0x0BA2
#define GL_MODELVIEW_STACK_DEPTH 0x0BA3
#define GL_PROJECTION_STACK_DEPTH 0x0BA4
#define GL_TEXTURE_STACK_DEPTH 0x0BA5
#define GL_MODELVIEW_MATRIX 0x0BA6
#define GL_PROJECTION_MATRIX 0x0BA7
#define GL_TEXTURE_MATRIX 0x0BA8
#define GL_ATTRIB_STACK_DEPTH 0x0BB0
#define GL_CLIENT_ATTRIB_STACK_DEPTH 0x0BB1
#define GL_ALPHA_TEST 0x0BC0
#define GL_ALPHA_TEST_FUNC 0x0BC1
#define GL_ALPHA_TEST_REF 0x0BC2
#define GL_DITHER 0x0BD0
#define GL_BLEND_DST 0x0BE0
#define GL_BLEND_SRC 0x0BE1
#define GL_BLEND 0x0BE2
#define GL_LOGIC_OP_MODE 0x0BF0
#define GL_INDEX_LOGIC_OP 0x0BF1
#define GL_COLOR_LOGIC_OP 0x0BF2
#define GL_AUX_BUFFERS 0x0C00
#define GL_DRAW_BUFFER 0x0C01
#define GL_READ_BUFFER 0x0C02
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_INDEX_CLEAR_VALUE 0x0C20
#define GL_INDEX_WRITEMASK 0x0C21
#define GL_COLOR_CLEAR_VALUE 0x0C22
#define GL_COLOR_WRITEMASK 0x0C23
#define GL_INDEX_MODE 0x0C30
#define GL_RGBA_MODE 0x0C31
#define GL_DOUBLEBUFFER 0x0C32
#define GL_STEREO 0x0C33
#define GL_RENDER_MODE 0x0C40
#define GL_PERSPECTIVE_CORRECTION_HINT 0x0C50
#define GL_POINT_SMOOTH_HINT 0x0C51
#define GL_LINE_SMOOTH_HINT 0x0C52
#define GL_POLYGON_SMOOTH_HINT 0x0C53
#define GL_FOG_HINT 0x0C54
#define GL_TEXTURE_GEN_S 0x0C60
#define GL_TEXTURE_GEN_T 0x0C61
#define GL_TEXTURE_GEN_R 0x0C62
#define GL_TEXTURE_GEN_Q 0x0C63
#define GL_PIXEL_MAP_I_TO_I 0x0C70
#define GL_PIXEL_MAP_S_TO_S 0x0C71
#define GL_PIXEL_MAP_I_TO_R 0x0C72
#define GL_PIXEL_MAP_I_TO_G 0x0C73
#define GL_PIXEL_MAP_I_TO_B 0x0C74
#define GL_PIXEL_MAP_I_TO_A 0x0C75
#define GL_PIXEL_MAP_R_TO_R 0x0C76
#define GL_PIXEL_MAP_G_TO_G 0x0C77
#define GL_PIXEL_MAP_B_TO_B 0x0C78
#define GL_PIXEL_MAP_A_TO_A 0x0C79
#define GL_PIXEL_MAP_I_TO_I_SIZE 0x0CB0
#define GL_PIXEL_MAP_S_TO_S_SIZE 0x0CB1
#define GL_PIXEL_MAP_I_TO_R_SIZE 0x0CB2
#define GL_PIXEL_MAP_I_TO_G_SIZE 0x0CB3
#define GL_PIXEL_MAP_I_TO_B_SIZE 0x0CB4
#define GL_PIXEL_MAP_I_TO_A_SIZE 0x0CB5
#define GL_PIXEL_MAP_R_TO_R_SIZE 0x0CB6
#define GL_PIXEL_MAP_G_TO_G_SIZE 0x0CB7
#define GL_PIXEL_MAP_B_TO_B_SIZE 0x0CB8
#define GL_PIXEL_MAP_A_TO_A_SIZE 0x0CB9
#define GL_UNPACK_SWAP_BYTES 0x0CF0
#define GL_UNPACK_LSB_FIRST 0x0CF1
#define GL_UNPACK_ROW_LENGTH 0x0CF2
#define GL_UNPACK_SKIP_ROWS 0x0CF3
#define GL_UNPACK_SKIP_PIXELS 0x0CF4
#define GL_UNPACK_ALIGNMENT 0x0CF5
#define GL_PACK_SWAP_BYTES 0x0D00
#define GL_PACK_LSB_FIRST 0x0D01
#define GL_PACK_ROW_LENGTH 0x0D02
#define GL_PACK_SKIP_ROWS 0x0D03
#define GL_PACK_SKIP_PIXELS 0x0D04
#define GL_PACK_ALIGNMENT 0x0D05
#define GL_MAP_COLOR 0x0D10
#define GL_MAP_STENCIL 0x0D11
#define GL_INDEX_SHIFT 0x0D12
#define GL_INDEX_OFFSET 0x0D13
#define GL_RED_SCALE 0x0D14
#define GL_RED_BIAS 0x0D15
#define GL_ZOOM_X 0x0D16
#define GL_ZOOM_Y 0x0D17
#define GL_GREEN_SCALE 0x0D18
#define GL_GREEN_BIAS 0x0D19
#define GL_BLUE_SCALE 0x0D1A
#define GL_BLUE_BIAS 0x0D1B
#define GL_ALPHA_SCALE 0x0D1C
#define GL_ALPHA_BIAS 0x0D1D
#define GL_DEPTH_SCALE 0x0D1E
#define GL_DEPTH_BIAS 0x0D1F
#define GL_MAX_EVAL_ORDER 0x0D30
#define GL_MAX_LIGHTS 0x0D31
#define GL_MAX_CLIP_PLANES 0x0D32
#define GL_MAX_TEXTURE_SIZE 0x0D33
#define GL_MAX_PIXEL_MAP_TABLE 0x0D34
#define GL_MAX_ATTRIB_STACK_DEPTH 0x0D35
#define GL_MAX_MODELVIEW_STACK_DEPTH 0x0D36
#define GL_MAX_NAME_STACK_DEPTH 0x0D37
#define GL_MAX_PROJECTION_STACK_DEPTH 0x0D38
#define GL_MAX_TEXTURE_STACK_DEPTH 0x0D39
#define GL_MAX_VIEWPORT_DIMS 0x0D3A
#define GL_MAX_CLIENT_ATTRIB_STACK_DEPTH 0x0D3B
#define GL_SUBPIXEL_BITS 0x0D50
#define GL_INDEX_BITS 0x0D51
#define GL_RED_BITS 0x0D52
#define GL_GREEN_BITS 0x0D53
#define GL_BLUE_BITS 0x0D54
#define GL_ALPHA_BITS 0x0D55
#define GL_DEPTH_BITS 0x0D56
#define GL_STENCIL_BITS 0x0D57
#define GL_ACCUM_RED_BITS 0x0D58
#define GL_ACCUM_GREEN_BITS 0x0D59
#define GL_ACCUM_BLUE_BITS 0x0D5A
#define GL_ACCUM_ALPHA_BITS 0x0D5B
#define GL_NAME_STACK_DEPTH 0x0D70
#define GL_AUTO_NORMAL 0x0D80
#define GL_MAP1_COLOR_4 0x0D90
#define GL_MAP1_INDEX 0x0D91
#define GL_MAP1_NORMAL 0x0D92
#define GL_MAP1_TEXTURE_COORD_1 0x0D93
#define GL_MAP1_TEXTURE_COORD_2 0x0D94
#define GL_MAP1_TEXTURE_COORD_3 0x0D95
#define GL_MAP1_TEXTURE_COORD_4 0x0D96
#define GL_MAP1_VERTEX_3 0x0D97
#define GL_MAP1_VERTEX_4 0x0D98
#define GL_MAP2_COLOR_4 0x0DB0
#define GL_MAP2_INDEX 0x0DB1
#define GL_MAP2_NORMAL 0x0DB2
#define GL_MAP2_TEXTURE_COORD_1 0x0DB3
#define GL_MAP2_TEXTURE_COORD_2 0x0DB4
#define GL_MAP2_TEXTURE_COORD_3 0x0DB5
#define GL_MAP2_TEXTURE_COORD_4 0x0DB6
#define GL_MAP2_VERTEX_3 0x0DB7
#define GL_MAP2_VERTEX_4 0x0DB8
#define GL_MAP1_GRID_DOMAIN 0x0DD0
#define GL_MAP1_GRID_SEGMENTS 0x0DD1
#define GL_MAP2_GRID_DOMAIN 0x0DD2
#define GL_MAP2_GRID_SEGMENTS 0x0DD3
#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_FEEDBACK_BUFFER_POINTER 0x0DF0
#define GL_FEEDBACK_BUFFER_SIZE 0x0DF1
#define GL_FEEDBACK_BUFFER_TYPE 0x0DF2
#define GL_SELECTION_BUFFER_POINTER 0x0DF3
#define GL_SELECTION_BUFFER_SIZE 0x0DF4
#define GL_TEXTURE_WIDTH 0x1000
#define GL_TRANSFORM_BIT 0x00001000
#define GL_TEXTURE_HEIGHT 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT 0x1003
#define GL_TEXTURE_BORDER_COLOR 0x1004
#define GL_TEXTURE_BORDER 0x1005
#define GL_DONT_CARE 0x1100
#define GL_FASTEST 0x1101
#define GL_NICEST 0x1102
#define GL_AMBIENT 0x1200
#define GL_DIFFUSE 0x1201
#define GL_SPECULAR 0x1202
#define GL_POSITION 0x1203
#define GL_SPOT_DIRECTION 0x1204
#define GL_SPOT_EXPONENT 0x1205
#define GL_SPOT_CUTOFF 0x1206
#define GL_CONSTANT_ATTENUATION 0x1207
#define GL_LINEAR_ATTENUATION 0x1208
#define GL_QUADRATIC_ATTENUATION 0x1209
#define GL_COMPILE 0x1300
#define GL_COMPILE_AND_EXECUTE 0x1301
#define GL_BYTE 0x1400
#define GL_UNSIGNED_BYTE 0x1401
#define GL_SHORT 0x1402
#define GL_UNSIGNED_SHORT 0x1403
#define GL_INT 0x1404
#define GL_UNSIGNED_INT 0x1405
#define GL_FLOAT 0x1406
#define GL_2_BYTES 0x1407
#define GL_3_BYTES 0x1408
#define GL_4_BYTES 0x1409
#define GL_DOUBLE 0x140A
#define GL_CLEAR 0x1500
#define GL_AND 0x1501
#define GL_AND_REVERSE 0x1502
#define GL_COPY 0x1503
#define GL_AND_INVERTED 0x1504
#define GL_NOOP 0x1505
#define GL_XOR 0x1506
#define GL_OR 0x1507
#define GL_NOR 0x1508
#define GL_EQUIV 0x1509
#define GL_INVERT 0x150A
#define GL_OR_REVERSE 0x150B
#define GL_COPY_INVERTED 0x150C
#define GL_OR_INVERTED 0x150D
#define GL_NAND 0x150E
#define GL_SET 0x150F
#define GL_EMISSION 0x1600
#define GL_SHININESS 0x1601
#define GL_AMBIENT_AND_DIFFUSE 0x1602
#define GL_COLOR_INDEXES 0x1603
#define GL_MODELVIEW 0x1700
#define GL_PROJECTION 0x1701
#define GL_TEXTURE 0x1702
#define GL_COLOR 0x1800
#define GL_DEPTH 0x1801
#define GL_STENCIL 0x1802
#define GL_COLOR_INDEX 0x1900
#define GL_STENCIL_INDEX 0x1901
#define GL_DEPTH_COMPONENT 0x1902
#define GL_RED 0x1903
#define GL_GREEN 0x1904
#define GL_BLUE 0x1905
#define GL_ALPHA 0x1906
#define GL_RGB 0x1907
#define GL_RGBA 0x1908
#define GL_LUMINANCE 0x1909
#define GL_LUMINANCE_ALPHA 0x190A
#define GL_BITMAP 0x1A00
#define GL_POINT 0x1B00
#define GL_LINE 0x1B01
#define GL_FILL 0x1B02
#define GL_RENDER 0x1C00
#define GL_FEEDBACK 0x1C01
#define GL_SELECT 0x1C02
#define GL_FLAT 0x1D00
#define GL_SMOOTH 0x1D01
#define GL_KEEP 0x1E00
#define GL_REPLACE 0x1E01
#define GL_INCR 0x1E02
#define GL_DECR 0x1E03
#define GL_VENDOR 0x1F00
#define GL_RENDERER 0x1F01
#define GL_VERSION 0x1F02
#define GL_EXTENSIONS 0x1F03
#define GL_S 0x2000
#define GL_ENABLE_BIT 0x00002000
#define GL_T 0x2001
#define GL_R 0x2002
#define GL_Q 0x2003
#define GL_MODULATE 0x2100
#define GL_DECAL 0x2101
#define GL_TEXTURE_ENV_MODE 0x2200
#define GL_TEXTURE_ENV_COLOR 0x2201
#define GL_TEXTURE_ENV 0x2300
#define GL_EYE_LINEAR 0x2400
#define GL_OBJECT_LINEAR 0x2401
#define GL_SPHERE_MAP 0x2402
#define GL_TEXTURE_GEN_MODE 0x2500
#define GL_OBJECT_PLANE 0x2501
#define GL_EYE_PLANE 0x2502
#define GL_NEAREST 0x2600
#define GL_LINEAR 0x2601
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_CLAMP 0x2900
#define GL_REPEAT 0x2901
#define GL_POLYGON_OFFSET_UNITS 0x2A00
#define GL_POLYGON_OFFSET_POINT 0x2A01
#define GL_POLYGON_OFFSET_LINE 0x2A02
#define GL_R3_G3_B2 0x2A10
#define GL_V2F 0x2A20
#define GL_V3F 0x2A21
#define GL_C4UB_V2F 0x2A22
#define GL_C4UB_V3F 0x2A23
#define GL_C3F_V3F 0x2A24
#define GL_N3F_V3F 0x2A25
#define GL_C4F_N3F_V3F 0x2A26
#define GL_T2F_V3F 0x2A27
#define GL_T4F_V4F 0x2A28
#define GL_T2F_C4UB_V3F 0x2A29
#define GL_T2F_C3F_V3F 0x2A2A
#define GL_T2F_N3F_V3F 0x2A2B
#define GL_T2F_C4F_N3F_V3F 0x2A2C
#define GL_T4F_C4F_N3F_V4F 0x2A2D
#define GL_CLIP_PLANE0 0x3000
#define GL_CLIP_PLANE1 0x3001
#define GL_CLIP_PLANE2 0x3002
#define GL_CLIP_PLANE3 0x3003
#define GL_CLIP_PLANE4 0x3004
#define GL_CLIP_PLANE5 0x3005
#define GL_LIGHT0 0x4000
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_LIGHT1 0x4001
#define GL_LIGHT2 0x4002
#define GL_LIGHT3 0x4003
#define GL_LIGHT4 0x4004
#define GL_LIGHT5 0x4005
#define GL_LIGHT6 0x4006
#define GL_LIGHT7 0x4007
#define GL_HINT_BIT 0x00008000
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_POLYGON_OFFSET_FACTOR 0x8038
#define GL_ALPHA4 0x803B
#define GL_ALPHA8 0x803C
#define GL_ALPHA12 0x803D
#define GL_ALPHA16 0x803E
#define GL_LUMINANCE4 0x803F
#define GL_LUMINANCE8 0x8040
#define GL_LUMINANCE12 0x8041
#define GL_LUMINANCE16 0x8042
#define GL_LUMINANCE4_ALPHA4 0x8043
#define GL_LUMINANCE6_ALPHA2 0x8044
#define GL_LUMINANCE8_ALPHA8 0x8045
#define GL_LUMINANCE12_ALPHA4 0x8046
#define GL_LUMINANCE12_ALPHA12 0x8047
#define GL_LUMINANCE16_ALPHA16 0x8048
#define GL_INTENSITY 0x8049
#define GL_INTENSITY4 0x804A
#define GL_INTENSITY8 0x804B
#define GL_INTENSITY12 0x804C
#define GL_INTENSITY16 0x804D
#define GL_RGB4 0x804F
#define GL_RGB5 0x8050
#define GL_RGB8 0x8051
#define GL_RGB10 0x8052
#define GL_RGB12 0x8053
#define GL_RGB16 0x8054
#define GL_RGBA2 0x8055
#define GL_RGBA4 0x8056
#define GL_RGB5_A1 0x8057
#define GL_RGBA8 0x8058
#define GL_RGB10_A2 0x8059
#define GL_RGBA12 0x805A
#define GL_RGBA16 0x805B
#define GL_TEXTURE_RED_SIZE 0x805C
#define GL_TEXTURE_GREEN_SIZE 0x805D
#define GL_TEXTURE_BLUE_SIZE 0x805E
#define GL_TEXTURE_ALPHA_SIZE 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE 0x8060
#define GL_TEXTURE_INTENSITY_SIZE 0x8061
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_TEXTURE_PRIORITY 0x8066
#define GL_TEXTURE_RESIDENT 0x8067
#define GL_TEXTURE_BINDING_1D 0x8068
#define GL_TEXTURE_BINDING_2D 0x8069
#define GL_VERTEX_ARRAY 0x8074
#define GL_NORMAL_ARRAY 0x8075
#define GL_COLOR_ARRAY 0x8076
#define GL_INDEX_ARRAY 0x8077
#define GL_TEXTURE_COORD_ARRAY 0x8078
#define GL_EDGE_FLAG_ARRAY 0x8079
#define GL_VERTEX_ARRAY_SIZE 0x807A
#define GL_VERTEX_ARRAY_TYPE 0x807B
#define GL_VERTEX_ARRAY_STRIDE 0x807C
#define GL_NORMAL_ARRAY_TYPE 0x807E
#define GL_NORMAL_ARRAY_STRIDE 0x807F
#define GL_COLOR_ARRAY_SIZE 0x8081
#define GL_COLOR_ARRAY_TYPE 0x8082
#define GL_COLOR_ARRAY_STRIDE 0x8083
#define GL_INDEX_ARRAY_TYPE 0x8085
#define GL_INDEX_ARRAY_STRIDE 0x8086
#define GL_TEXTURE_COORD_ARRAY_SIZE 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE 0x808A
#define GL_EDGE_FLAG_ARRAY_STRIDE 0x808C
#define GL_VERTEX_ARRAY_POINTER 0x808E
#define GL_NORMAL_ARRAY_POINTER 0x808F
#define GL_COLOR_ARRAY_POINTER 0x8090
#define GL_INDEX_ARRAY_POINTER 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER 0x8093
#define GL_COLOR_INDEX1_EXT 0x80E2
#define GL_COLOR_INDEX2_EXT 0x80E3
#define GL_COLOR_INDEX4_EXT 0x80E4
#define GL_COLOR_INDEX8_EXT 0x80E5
#define GL_COLOR_INDEX12_EXT 0x80E6
#define GL_COLOR_INDEX16_EXT 0x80E7
#define GL_EVAL_BIT 0x00010000
#define GL_LIST_BIT 0x00020000
#define GL_TEXTURE_BIT 0x00040000
#define GL_SCISSOR_BIT 0x00080000
#define GL_ALL_ATTRIB_BITS 0x000fffff
#define GL_CLIENT_ALL_ATTRIB_BITS 0xffffffff

GLAPI void GLAPIENTRY glAccum (GLenum op, GLfloat value);
GLAPI void GLAPIENTRY glAlphaFunc (GLenum func, GLclampf ref);
GLAPI GLboolean GLAPIENTRY glAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences);
GLAPI void GLAPIENTRY glArrayElement (GLint i);
GLAPI void GLAPIENTRY glBegin (GLenum mode);
GLAPI void GLAPIENTRY glBindTexture (GLenum target, GLuint texture);
GLAPI void GLAPIENTRY glBitmap (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
GLAPI void GLAPIENTRY glBlendFunc (GLenum sfactor, GLenum dfactor);
GLAPI void GLAPIENTRY glCallList (GLuint list);
GLAPI void GLAPIENTRY glCallLists (GLsizei n, GLenum type, const GLvoid *lists);
GLAPI void GLAPIENTRY glClear (GLbitfield mask);
GLAPI void GLAPIENTRY glClearAccum (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void GLAPIENTRY glClearColor (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
GLAPI void GLAPIENTRY glClearDepth (GLclampd depth);
GLAPI void GLAPIENTRY glClearIndex (GLfloat c);
GLAPI void GLAPIENTRY glClearStencil (GLint s);
GLAPI void GLAPIENTRY glClipPlane (GLenum plane, const GLdouble *equation);
GLAPI void GLAPIENTRY glColor3b (GLbyte red, GLbyte green, GLbyte blue);
GLAPI void GLAPIENTRY glColor3bv (const GLbyte *v);
GLAPI void GLAPIENTRY glColor3d (GLdouble red, GLdouble green, GLdouble blue);
GLAPI void GLAPIENTRY glColor3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glColor3f (GLfloat red, GLfloat green, GLfloat blue);
GLAPI void GLAPIENTRY glColor3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glColor3i (GLint red, GLint green, GLint blue);
GLAPI void GLAPIENTRY glColor3iv (const GLint *v);
GLAPI void GLAPIENTRY glColor3s (GLshort red, GLshort green, GLshort blue);
GLAPI void GLAPIENTRY glColor3sv (const GLshort *v);
GLAPI void GLAPIENTRY glColor3ub (GLubyte red, GLubyte green, GLubyte blue);
GLAPI void GLAPIENTRY glColor3ubv (const GLubyte *v);
GLAPI void GLAPIENTRY glColor3ui (GLuint red, GLuint green, GLuint blue);
GLAPI void GLAPIENTRY glColor3uiv (const GLuint *v);
GLAPI void GLAPIENTRY glColor3us (GLushort red, GLushort green, GLushort blue);
GLAPI void GLAPIENTRY glColor3usv (const GLushort *v);
GLAPI void GLAPIENTRY glColor4b (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
GLAPI void GLAPIENTRY glColor4bv (const GLbyte *v);
GLAPI void GLAPIENTRY glColor4d (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
GLAPI void GLAPIENTRY glColor4dv (const GLdouble *v);
GLAPI void GLAPIENTRY glColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GLAPI void GLAPIENTRY glColor4fv (const GLfloat *v);
GLAPI void GLAPIENTRY glColor4i (GLint red, GLint green, GLint blue, GLint alpha);
GLAPI void GLAPIENTRY glColor4iv (const GLint *v);
GLAPI void GLAPIENTRY glColor4s (GLshort red, GLshort green, GLshort blue, GLshort alpha);
GLAPI void GLAPIENTRY glColor4sv (const GLshort *v);
GLAPI void GLAPIENTRY glColor4ub (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
GLAPI void GLAPIENTRY glColor4ubv (const GLubyte *v);
GLAPI void GLAPIENTRY glColor4ui (GLuint red, GLuint green, GLuint blue, GLuint alpha);
GLAPI void GLAPIENTRY glColor4uiv (const GLuint *v);
GLAPI void GLAPIENTRY glColor4us (GLushort red, GLushort green, GLushort blue, GLushort alpha);
GLAPI void GLAPIENTRY glColor4usv (const GLushort *v);
GLAPI void GLAPIENTRY glColorMask (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
GLAPI void GLAPIENTRY glColorMaterial (GLenum face, GLenum mode);
GLAPI void GLAPIENTRY glColorPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void GLAPIENTRY glCopyPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
GLAPI void GLAPIENTRY glCopyTexImage1D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
GLAPI void GLAPIENTRY glCopyTexImage2D (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
GLAPI void GLAPIENTRY glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
GLAPI void GLAPIENTRY glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void GLAPIENTRY glCullFace (GLenum mode);
GLAPI void GLAPIENTRY glDeleteLists (GLuint list, GLsizei range);
GLAPI void GLAPIENTRY glDeleteTextures (GLsizei n, const GLuint *textures);
GLAPI void GLAPIENTRY glDepthFunc (GLenum func);
GLAPI void GLAPIENTRY glDepthMask (GLboolean flag);
GLAPI void GLAPIENTRY glDepthRange (GLclampd zNear, GLclampd zFar);
GLAPI void GLAPIENTRY glDisable (GLenum cap);
GLAPI void GLAPIENTRY glDisableClientState (GLenum array);
GLAPI void GLAPIENTRY glDrawArrays (GLenum mode, GLint first, GLsizei count);
GLAPI void GLAPIENTRY glDrawBuffer (GLenum mode);
GLAPI void GLAPIENTRY glDrawElements (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
GLAPI void GLAPIENTRY glDrawPixels (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void GLAPIENTRY glEdgeFlag (GLboolean flag);
GLAPI void GLAPIENTRY glEdgeFlagPointer (GLsizei stride, const GLvoid *pointer);
GLAPI void GLAPIENTRY glEdgeFlagv (const GLboolean *flag);
GLAPI void GLAPIENTRY glEnable (GLenum cap);
GLAPI void GLAPIENTRY glEnableClientState (GLenum array);
GLAPI void GLAPIENTRY glEnd (void);
GLAPI void GLAPIENTRY glEndList (void);
GLAPI void GLAPIENTRY glEvalCoord1d (GLdouble u);
GLAPI void GLAPIENTRY glEvalCoord1dv (const GLdouble *u);
GLAPI void GLAPIENTRY glEvalCoord1f (GLfloat u);
GLAPI void GLAPIENTRY glEvalCoord1fv (const GLfloat *u);
GLAPI void GLAPIENTRY glEvalCoord2d (GLdouble u, GLdouble v);
GLAPI void GLAPIENTRY glEvalCoord2dv (const GLdouble *u);
GLAPI void GLAPIENTRY glEvalCoord2f (GLfloat u, GLfloat v);
GLAPI void GLAPIENTRY glEvalCoord2fv (const GLfloat *u);
GLAPI void GLAPIENTRY glEvalMesh1 (GLenum mode, GLint i1, GLint i2);
GLAPI void GLAPIENTRY glEvalMesh2 (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
GLAPI void GLAPIENTRY glEvalPoint1 (GLint i);
GLAPI void GLAPIENTRY glEvalPoint2 (GLint i, GLint j);
GLAPI void GLAPIENTRY glFeedbackBuffer (GLsizei size, GLenum type, GLfloat *buffer);
GLAPI void GLAPIENTRY glFinish (void);
GLAPI void GLAPIENTRY glFlush (void);
GLAPI void GLAPIENTRY glFogf (GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glFogfv (GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glFogi (GLenum pname, GLint param);
GLAPI void GLAPIENTRY glFogiv (GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glFrontFace (GLenum mode);
GLAPI void GLAPIENTRY glFrustum (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLAPI GLuint GLAPIENTRY glGenLists (GLsizei range);
GLAPI void GLAPIENTRY glGenTextures (GLsizei n, GLuint *textures);
GLAPI void GLAPIENTRY glGetBooleanv (GLenum pname, GLboolean *params);
GLAPI void GLAPIENTRY glGetClipPlane (GLenum plane, GLdouble *equation);
GLAPI void GLAPIENTRY glGetDoublev (GLenum pname, GLdouble *params);
GLAPI GLenum GLAPIENTRY glGetError (void);
GLAPI void GLAPIENTRY glGetFloatv (GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetIntegerv (GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetLightfv (GLenum light, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetLightiv (GLenum light, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetMapdv (GLenum target, GLenum query, GLdouble *v);
GLAPI void GLAPIENTRY glGetMapfv (GLenum target, GLenum query, GLfloat *v);
GLAPI void GLAPIENTRY glGetMapiv (GLenum target, GLenum query, GLint *v);
GLAPI void GLAPIENTRY glGetMaterialfv (GLenum face, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetMaterialiv (GLenum face, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetPixelMapfv (GLenum map, GLfloat *values);
GLAPI void GLAPIENTRY glGetPixelMapuiv (GLenum map, GLuint *values);
GLAPI void GLAPIENTRY glGetPixelMapusv (GLenum map, GLushort *values);
GLAPI void GLAPIENTRY glGetPointerv (GLenum pname, GLvoid* *params);
GLAPI void GLAPIENTRY glGetPolygonStipple (GLubyte *mask);
GLAPI const GLubyte * GLAPIENTRY glGetString (GLenum name);
GLAPI void GLAPIENTRY glGetTexEnvfv (GLenum target, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexEnviv (GLenum target, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetTexGendv (GLenum coord, GLenum pname, GLdouble *params);
GLAPI void GLAPIENTRY glGetTexGenfv (GLenum coord, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexGeniv (GLenum coord, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetTexImage (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
GLAPI void GLAPIENTRY glGetTexLevelParameterfv (GLenum target, GLint level, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexLevelParameteriv (GLenum target, GLint level, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glGetTexParameterfv (GLenum target, GLenum pname, GLfloat *params);
GLAPI void GLAPIENTRY glGetTexParameteriv (GLenum target, GLenum pname, GLint *params);
GLAPI void GLAPIENTRY glHint (GLenum target, GLenum mode);
GLAPI void GLAPIENTRY glIndexMask (GLuint mask);
GLAPI void GLAPIENTRY glIndexPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void GLAPIENTRY glIndexd (GLdouble c);
GLAPI void GLAPIENTRY glIndexdv (const GLdouble *c);
GLAPI void GLAPIENTRY glIndexf (GLfloat c);
GLAPI void GLAPIENTRY glIndexfv (const GLfloat *c);
GLAPI void GLAPIENTRY glIndexi (GLint c);
GLAPI void GLAPIENTRY glIndexiv (const GLint *c);
GLAPI void GLAPIENTRY glIndexs (GLshort c);
GLAPI void GLAPIENTRY glIndexsv (const GLshort *c);
GLAPI void GLAPIENTRY glIndexub (GLubyte c);
GLAPI void GLAPIENTRY glIndexubv (const GLubyte *c);
GLAPI void GLAPIENTRY glInitNames (void);
GLAPI void GLAPIENTRY glInterleavedArrays (GLenum format, GLsizei stride, const GLvoid *pointer);
GLAPI GLboolean GLAPIENTRY glIsEnabled (GLenum cap);
GLAPI GLboolean GLAPIENTRY glIsList (GLuint list);
GLAPI GLboolean GLAPIENTRY glIsTexture (GLuint texture);
GLAPI void GLAPIENTRY glLightModelf (GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glLightModelfv (GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glLightModeli (GLenum pname, GLint param);
GLAPI void GLAPIENTRY glLightModeliv (GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glLightf (GLenum light, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glLightfv (GLenum light, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glLighti (GLenum light, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glLightiv (GLenum light, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glLineStipple (GLint factor, GLushort pattern);
GLAPI void GLAPIENTRY glLineWidth (GLfloat width);
GLAPI void GLAPIENTRY glListBase (GLuint base);
GLAPI void GLAPIENTRY glLoadIdentity (void);
GLAPI void GLAPIENTRY glLoadMatrixd (const GLdouble *m);
GLAPI void GLAPIENTRY glLoadMatrixf (const GLfloat *m);
GLAPI void GLAPIENTRY glLoadName (GLuint name);
GLAPI void GLAPIENTRY glLogicOp (GLenum opcode);
GLAPI void GLAPIENTRY glMap1d (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
GLAPI void GLAPIENTRY glMap1f (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
GLAPI void GLAPIENTRY glMap2d (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
GLAPI void GLAPIENTRY glMap2f (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
GLAPI void GLAPIENTRY glMapGrid1d (GLint un, GLdouble u1, GLdouble u2);
GLAPI void GLAPIENTRY glMapGrid1f (GLint un, GLfloat u1, GLfloat u2);
GLAPI void GLAPIENTRY glMapGrid2d (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
GLAPI void GLAPIENTRY glMapGrid2f (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
GLAPI void GLAPIENTRY glMaterialf (GLenum face, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glMaterialfv (GLenum face, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glMateriali (GLenum face, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glMaterialiv (GLenum face, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glMatrixMode (GLenum mode);
GLAPI void GLAPIENTRY glMultMatrixd (const GLdouble *m);
GLAPI void GLAPIENTRY glMultMatrixf (const GLfloat *m);
GLAPI void GLAPIENTRY glNewList (GLuint list, GLenum mode);
GLAPI void GLAPIENTRY glNormal3b (GLbyte nx, GLbyte ny, GLbyte nz);
GLAPI void GLAPIENTRY glNormal3bv (const GLbyte *v);
GLAPI void GLAPIENTRY glNormal3d (GLdouble nx, GLdouble ny, GLdouble nz);
GLAPI void GLAPIENTRY glNormal3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glNormal3f (GLfloat nx, GLfloat ny, GLfloat nz);
GLAPI void GLAPIENTRY glNormal3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glNormal3i (GLint nx, GLint ny, GLint nz);
GLAPI void GLAPIENTRY glNormal3iv (const GLint *v);
GLAPI void GLAPIENTRY glNormal3s (GLshort nx, GLshort ny, GLshort nz);
GLAPI void GLAPIENTRY glNormal3sv (const GLshort *v);
GLAPI void GLAPIENTRY glNormalPointer (GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void GLAPIENTRY glOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
GLAPI void GLAPIENTRY glPassThrough (GLfloat token);
GLAPI void GLAPIENTRY glPixelMapfv (GLenum map, GLsizei mapsize, const GLfloat *values);
GLAPI void GLAPIENTRY glPixelMapuiv (GLenum map, GLsizei mapsize, const GLuint *values);
GLAPI void GLAPIENTRY glPixelMapusv (GLenum map, GLsizei mapsize, const GLushort *values);
GLAPI void GLAPIENTRY glPixelStoref (GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glPixelStorei (GLenum pname, GLint param);
GLAPI void GLAPIENTRY glPixelTransferf (GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glPixelTransferi (GLenum pname, GLint param);
GLAPI void GLAPIENTRY glPixelZoom (GLfloat xfactor, GLfloat yfactor);
GLAPI void GLAPIENTRY glPointSize (GLfloat size);
GLAPI void GLAPIENTRY glPolygonMode (GLenum face, GLenum mode);
GLAPI void GLAPIENTRY glPolygonOffset (GLfloat factor, GLfloat units);
GLAPI void GLAPIENTRY glPolygonStipple (const GLubyte *mask);
GLAPI void GLAPIENTRY glPopAttrib (void);
GLAPI void GLAPIENTRY glPopClientAttrib (void);
GLAPI void GLAPIENTRY glPopMatrix (void);
GLAPI void GLAPIENTRY glPopName (void);
GLAPI void GLAPIENTRY glPrioritizeTextures (GLsizei n, const GLuint *textures, const GLclampf *priorities);
GLAPI void GLAPIENTRY glPushAttrib (GLbitfield mask);
GLAPI void GLAPIENTRY glPushClientAttrib (GLbitfield mask);
GLAPI void GLAPIENTRY glPushMatrix (void);
GLAPI void GLAPIENTRY glPushName (GLuint name);
GLAPI void GLAPIENTRY glRasterPos2d (GLdouble x, GLdouble y);
GLAPI void GLAPIENTRY glRasterPos2dv (const GLdouble *v);
GLAPI void GLAPIENTRY glRasterPos2f (GLfloat x, GLfloat y);
GLAPI void GLAPIENTRY glRasterPos2fv (const GLfloat *v);
GLAPI void GLAPIENTRY glRasterPos2i (GLint x, GLint y);
GLAPI void GLAPIENTRY glRasterPos2iv (const GLint *v);
GLAPI void GLAPIENTRY glRasterPos2s (GLshort x, GLshort y);
GLAPI void GLAPIENTRY glRasterPos2sv (const GLshort *v);
GLAPI void GLAPIENTRY glRasterPos3d (GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glRasterPos3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glRasterPos3f (GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glRasterPos3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glRasterPos3i (GLint x, GLint y, GLint z);
GLAPI void GLAPIENTRY glRasterPos3iv (const GLint *v);
GLAPI void GLAPIENTRY glRasterPos3s (GLshort x, GLshort y, GLshort z);
GLAPI void GLAPIENTRY glRasterPos3sv (const GLshort *v);
GLAPI void GLAPIENTRY glRasterPos4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void GLAPIENTRY glRasterPos4dv (const GLdouble *v);
GLAPI void GLAPIENTRY glRasterPos4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void GLAPIENTRY glRasterPos4fv (const GLfloat *v);
GLAPI void GLAPIENTRY glRasterPos4i (GLint x, GLint y, GLint z, GLint w);
GLAPI void GLAPIENTRY glRasterPos4iv (const GLint *v);
GLAPI void GLAPIENTRY glRasterPos4s (GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void GLAPIENTRY glRasterPos4sv (const GLshort *v);
GLAPI void GLAPIENTRY glReadBuffer (GLenum mode);
GLAPI void GLAPIENTRY glReadPixels (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
GLAPI void GLAPIENTRY glRectd (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
GLAPI void GLAPIENTRY glRectdv (const GLdouble *v1, const GLdouble *v2);
GLAPI void GLAPIENTRY glRectf (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
GLAPI void GLAPIENTRY glRectfv (const GLfloat *v1, const GLfloat *v2);
GLAPI void GLAPIENTRY glRecti (GLint x1, GLint y1, GLint x2, GLint y2);
GLAPI void GLAPIENTRY glRectiv (const GLint *v1, const GLint *v2);
GLAPI void GLAPIENTRY glRects (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
GLAPI void GLAPIENTRY glRectsv (const GLshort *v1, const GLshort *v2);
GLAPI GLint GLAPIENTRY glRenderMode (GLenum mode);
GLAPI void GLAPIENTRY glRotated (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glRotatef (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glScaled (GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glScalef (GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glScissor (GLint x, GLint y, GLsizei width, GLsizei height);
GLAPI void GLAPIENTRY glSelectBuffer (GLsizei size, GLuint *buffer);
GLAPI void GLAPIENTRY glShadeModel (GLenum mode);
GLAPI void GLAPIENTRY glStencilFunc (GLenum func, GLint ref, GLuint mask);
GLAPI void GLAPIENTRY glStencilMask (GLuint mask);
GLAPI void GLAPIENTRY glStencilOp (GLenum fail, GLenum zfail, GLenum zpass);
GLAPI void GLAPIENTRY glTexCoord1d (GLdouble s);
GLAPI void GLAPIENTRY glTexCoord1dv (const GLdouble *v);
GLAPI void GLAPIENTRY glTexCoord1f (GLfloat s);
GLAPI void GLAPIENTRY glTexCoord1fv (const GLfloat *v);
GLAPI void GLAPIENTRY glTexCoord1i (GLint s);
GLAPI void GLAPIENTRY glTexCoord1iv (const GLint *v);
GLAPI void GLAPIENTRY glTexCoord1s (GLshort s);
GLAPI void GLAPIENTRY glTexCoord1sv (const GLshort *v);
GLAPI void GLAPIENTRY glTexCoord2d (GLdouble s, GLdouble t);
GLAPI void GLAPIENTRY glTexCoord2dv (const GLdouble *v);
GLAPI void GLAPIENTRY glTexCoord2f (GLfloat s, GLfloat t);
GLAPI void GLAPIENTRY glTexCoord2fv (const GLfloat *v);
GLAPI void GLAPIENTRY glTexCoord2i (GLint s, GLint t);
GLAPI void GLAPIENTRY glTexCoord2iv (const GLint *v);
GLAPI void GLAPIENTRY glTexCoord2s (GLshort s, GLshort t);
GLAPI void GLAPIENTRY glTexCoord2sv (const GLshort *v);
GLAPI void GLAPIENTRY glTexCoord3d (GLdouble s, GLdouble t, GLdouble r);
GLAPI void GLAPIENTRY glTexCoord3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glTexCoord3f (GLfloat s, GLfloat t, GLfloat r);
GLAPI void GLAPIENTRY glTexCoord3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glTexCoord3i (GLint s, GLint t, GLint r);
GLAPI void GLAPIENTRY glTexCoord3iv (const GLint *v);
GLAPI void GLAPIENTRY glTexCoord3s (GLshort s, GLshort t, GLshort r);
GLAPI void GLAPIENTRY glTexCoord3sv (const GLshort *v);
GLAPI void GLAPIENTRY glTexCoord4d (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
GLAPI void GLAPIENTRY glTexCoord4dv (const GLdouble *v);
GLAPI void GLAPIENTRY glTexCoord4f (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
GLAPI void GLAPIENTRY glTexCoord4fv (const GLfloat *v);
GLAPI void GLAPIENTRY glTexCoord4i (GLint s, GLint t, GLint r, GLint q);
GLAPI void GLAPIENTRY glTexCoord4iv (const GLint *v);
GLAPI void GLAPIENTRY glTexCoord4s (GLshort s, GLshort t, GLshort r, GLshort q);
GLAPI void GLAPIENTRY glTexCoord4sv (const GLshort *v);
GLAPI void GLAPIENTRY glTexCoordPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void GLAPIENTRY glTexEnvf (GLenum target, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glTexEnvfv (GLenum target, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glTexEnvi (GLenum target, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glTexEnviv (GLenum target, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glTexGend (GLenum coord, GLenum pname, GLdouble param);
GLAPI void GLAPIENTRY glTexGendv (GLenum coord, GLenum pname, const GLdouble *params);
GLAPI void GLAPIENTRY glTexGenf (GLenum coord, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glTexGenfv (GLenum coord, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glTexGeni (GLenum coord, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glTexGeniv (GLenum coord, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void GLAPIENTRY glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void GLAPIENTRY glTexParameterf (GLenum target, GLenum pname, GLfloat param);
GLAPI void GLAPIENTRY glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
GLAPI void GLAPIENTRY glTexParameteri (GLenum target, GLenum pname, GLint param);
GLAPI void GLAPIENTRY glTexParameteriv (GLenum target, GLenum pname, const GLint *params);
GLAPI void GLAPIENTRY glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void GLAPIENTRY glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
GLAPI void GLAPIENTRY glTranslated (GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glTranslatef (GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glVertex2d (GLdouble x, GLdouble y);
GLAPI void GLAPIENTRY glVertex2dv (const GLdouble *v);
GLAPI void GLAPIENTRY glVertex2f (GLfloat x, GLfloat y);
GLAPI void GLAPIENTRY glVertex2fv (const GLfloat *v);
GLAPI void GLAPIENTRY glVertex2i (GLint x, GLint y);
GLAPI void GLAPIENTRY glVertex2iv (const GLint *v);
GLAPI void GLAPIENTRY glVertex2s (GLshort x, GLshort y);
GLAPI void GLAPIENTRY glVertex2sv (const GLshort *v);
GLAPI void GLAPIENTRY glVertex3d (GLdouble x, GLdouble y, GLdouble z);
GLAPI void GLAPIENTRY glVertex3dv (const GLdouble *v);
GLAPI void GLAPIENTRY glVertex3f (GLfloat x, GLfloat y, GLfloat z);
GLAPI void GLAPIENTRY glVertex3fv (const GLfloat *v);
GLAPI void GLAPIENTRY glVertex3i (GLint x, GLint y, GLint z);
GLAPI void GLAPIENTRY glVertex3iv (const GLint *v);
GLAPI void GLAPIENTRY glVertex3s (GLshort x, GLshort y, GLshort z);
GLAPI void GLAPIENTRY glVertex3sv (const GLshort *v);
GLAPI void GLAPIENTRY glVertex4d (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
GLAPI void GLAPIENTRY glVertex4dv (const GLdouble *v);
GLAPI void GLAPIENTRY glVertex4f (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
GLAPI void GLAPIENTRY glVertex4fv (const GLfloat *v);
GLAPI void GLAPIENTRY glVertex4i (GLint x, GLint y, GLint z, GLint w);
GLAPI void GLAPIENTRY glVertex4iv (const GLint *v);
GLAPI void GLAPIENTRY glVertex4s (GLshort x, GLshort y, GLshort z, GLshort w);
GLAPI void GLAPIENTRY glVertex4sv (const GLshort *v);
GLAPI void GLAPIENTRY glVertexPointer (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
GLAPI void GLAPIENTRY glViewport (GLint x, GLint y, GLsizei width, GLsizei height);

#define GLEW_VERSION_1_1 GLEW_GET_VAR(G_GLEW_VERSION_1_1)

#endif /* GL_VERSION_1_1 */

/* ---------------------------------- GLU ---------------------------------- */

#ifndef GLEW_NO_GLU
/* this is where we can safely include GLU */
#  if defined(__APPLE__) && defined(__MACH__)
#    include <OpenGL/glu.h>
#  else
#    include <GL/glu.h>
#  endif
#endif

/* ----------------------------- GL_VERSION_1_2 ---------------------------- */

#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2 1

#define GL_SMOOTH_POINT_SIZE_RANGE 0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY 0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE 0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY 0x0B23
#define GL_UNSIGNED_BYTE_3_3_2 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_UNSIGNED_INT_8_8_8_8 0x8035
#define GL_UNSIGNED_INT_10_10_10_2 0x8036
#define GL_RESCALE_NORMAL 0x803A
#define GL_TEXTURE_BINDING_3D 0x806A
#define GL_PACK_SKIP_IMAGES 0x806B
#define GL_PACK_IMAGE_HEIGHT 0x806C
#define GL_UNPACK_SKIP_IMAGES 0x806D
#define GL_UNPACK_IMAGE_HEIGHT 0x806E
#define GL_TEXTURE_3D 0x806F
#define GL_PROXY_TEXTURE_3D 0x8070
#define GL_TEXTURE_DEPTH 0x8071
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_MAX_3D_TEXTURE_SIZE 0x8073
#define GL_BGR 0x80E0
#define GL_BGRA 0x80E1
#define GL_MAX_ELEMENTS_VERTICES 0x80E8
#define GL_MAX_ELEMENTS_INDICES 0x80E9
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_TEXTURE_BASE_LEVEL 0x813C
#define GL_TEXTURE_MAX_LEVEL 0x813D
#define GL_LIGHT_MODEL_COLOR_CONTROL 0x81F8
#define GL_SINGLE_COLOR 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR 0x81FA
#define GL_UNSIGNED_BYTE_2_3_3_REV 0x8362
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV 0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV 0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV 0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_ALIASED_POINT_SIZE_RANGE 0x846D
#define GL_ALIASED_LINE_WIDTH_RANGE 0x846E

typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DPROC) (GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);

#define glCopyTexSubImage3D GLEW_GET_FUN(G_glewCopyTexSubImage3D)
#define glDrawRangeElements GLEW_GET_FUN(G_glewDrawRangeElements)
#define glTexImage3D GLEW_GET_FUN(G_glewTexImage3D)
#define glTexSubImage3D GLEW_GET_FUN(G_glewTexSubImage3D)

#define GLEW_VERSION_1_2 GLEW_GET_VAR(G_GLEW_VERSION_1_2)

#endif /* GL_VERSION_1_2 */

/* ---------------------------- GL_VERSION_1_2_1 --------------------------- */

#ifndef GL_VERSION_1_2_1
#define GL_VERSION_1_2_1 1

#define GLEW_VERSION_1_2_1 GLEW_GET_VAR(G_GLEW_VERSION_1_2_1)

#endif /* GL_VERSION_1_2_1 */

/* ----------------------------- GL_VERSION_1_3 ---------------------------- */

#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1

#define GL_MULTISAMPLE 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE 0x809F
#define GL_SAMPLE_COVERAGE 0x80A0
#define GL_SAMPLE_BUFFERS 0x80A8
#define GL_SAMPLES 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT 0x80AB
#define GL_CLAMP_TO_BORDER 0x812D
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE1 0x84C1
#define GL_TEXTURE2 0x84C2
#define GL_TEXTURE3 0x84C3
#define GL_TEXTURE4 0x84C4
#define GL_TEXTURE5 0x84C5
#define GL_TEXTURE6 0x84C6
#define GL_TEXTURE7 0x84C7
#define GL_TEXTURE8 0x84C8
#define GL_TEXTURE9 0x84C9
#define GL_TEXTURE10 0x84CA
#define GL_TEXTURE11 0x84CB
#define GL_TEXTURE12 0x84CC
#define GL_TEXTURE13 0x84CD
#define GL_TEXTURE14 0x84CE
#define GL_TEXTURE15 0x84CF
#define GL_TEXTURE16 0x84D0
#define GL_TEXTURE17 0x84D1
#define GL_TEXTURE18 0x84D2
#define GL_TEXTURE19 0x84D3
#define GL_TEXTURE20 0x84D4
#define GL_TEXTURE21 0x84D5
#define GL_TEXTURE22 0x84D6
#define GL_TEXTURE23 0x84D7
#define GL_TEXTURE24 0x84D8
#define GL_TEXTURE25 0x84D9
#define GL_TEXTURE26 0x84DA
#define GL_TEXTURE27 0x84DB
#define GL_TEXTURE28 0x84DC
#define GL_TEXTURE29 0x84DD
#define GL_TEXTURE30 0x84DE
#define GL_TEXTURE31 0x84DF
#define GL_ACTIVE_TEXTURE 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE 0x84E1
#define GL_MAX_TEXTURE_UNITS 0x84E2
#define GL_TRANSPOSE_MODELVIEW_MATRIX 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX 0x84E6
#define GL_SUBTRACT 0x84E7
#define GL_COMPRESSED_ALPHA 0x84E9
#define GL_COMPRESSED_LUMINANCE 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA 0x84EB
#define GL_COMPRESSED_INTENSITY 0x84EC
#define GL_COMPRESSED_RGB 0x84ED
#define GL_COMPRESSED_RGBA 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#define GL_NORMAL_MAP 0x8511
#define GL_REFLECTION_MAP 0x8512
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE 0x851C
#define GL_COMBINE 0x8570
#define GL_COMBINE_RGB 0x8571
#define GL_COMBINE_ALPHA 0x8572
#define GL_RGB_SCALE 0x8573
#define GL_ADD_SIGNED 0x8574
#define GL_INTERPOLATE 0x8575
#define GL_CONSTANT 0x8576
#define GL_PRIMARY_COLOR 0x8577
#define GL_PREVIOUS 0x8578
#define GL_SOURCE0_RGB 0x8580
#define GL_SOURCE1_RGB 0x8581
#define GL_SOURCE2_RGB 0x8582
#define GL_SOURCE0_ALPHA 0x8588
#define GL_SOURCE1_ALPHA 0x8589
#define GL_SOURCE2_ALPHA 0x858A
#define GL_OPERAND0_RGB 0x8590
#define GL_OPERAND1_RGB 0x8591
#define GL_OPERAND2_RGB 0x8592
#define GL_OPERAND0_ALPHA 0x8598
#define GL_OPERAND1_ALPHA 0x8599
#define GL_OPERAND2_ALPHA 0x859A
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE 0x86A0
#define GL_TEXTURE_COMPRESSED 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS 0x86A3
#define GL_DOT3_RGB 0x86AE
#define GL_DOT3_RGBA 0x86AF
#define GL_MULTISAMPLE_BIT 0x20000000

typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE1DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXIMAGEPROC) (GLenum target, GLint lod, GLvoid *img);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXDPROC) (const GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXFPROC) (const GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXDPROC) (const GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXFPROC) (const GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DPROC) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FPROC) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IPROC) (GLenum target, GLint s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SPROC) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IPROC) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SPROC) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DVPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FVPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IVPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SVPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSAMPLECOVERAGEPROC) (GLclampf value, GLboolean invert);

#define glActiveTexture GLEW_GET_FUN(G_glewActiveTexture)
#define glClientActiveTexture GLEW_GET_FUN(G_glewClientActiveTexture)
#define glCompressedTexImage1D GLEW_GET_FUN(G_glewCompressedTexImage1D)
#define glCompressedTexImage2D GLEW_GET_FUN(G_glewCompressedTexImage2D)
#define glCompressedTexImage3D GLEW_GET_FUN(G_glewCompressedTexImage3D)
#define glCompressedTexSubImage1D GLEW_GET_FUN(G_glewCompressedTexSubImage1D)
#define glCompressedTexSubImage2D GLEW_GET_FUN(G_glewCompressedTexSubImage2D)
#define glCompressedTexSubImage3D GLEW_GET_FUN(G_glewCompressedTexSubImage3D)
#define glGetCompressedTexImage GLEW_GET_FUN(G_glewGetCompressedTexImage)
#define glLoadTransposeMatrixd GLEW_GET_FUN(G_glewLoadTransposeMatrixd)
#define glLoadTransposeMatrixf GLEW_GET_FUN(G_glewLoadTransposeMatrixf)
#define glMultTransposeMatrixd GLEW_GET_FUN(G_glewMultTransposeMatrixd)
#define glMultTransposeMatrixf GLEW_GET_FUN(G_glewMultTransposeMatrixf)
#define glMultiTexCoord1d GLEW_GET_FUN(G_glewMultiTexCoord1d)
#define glMultiTexCoord1dv GLEW_GET_FUN(G_glewMultiTexCoord1dv)
#define glMultiTexCoord1f GLEW_GET_FUN(G_glewMultiTexCoord1f)
#define glMultiTexCoord1fv GLEW_GET_FUN(G_glewMultiTexCoord1fv)
#define glMultiTexCoord1i GLEW_GET_FUN(G_glewMultiTexCoord1i)
#define glMultiTexCoord1iv GLEW_GET_FUN(G_glewMultiTexCoord1iv)
#define glMultiTexCoord1s GLEW_GET_FUN(G_glewMultiTexCoord1s)
#define glMultiTexCoord1sv GLEW_GET_FUN(G_glewMultiTexCoord1sv)
#define glMultiTexCoord2d GLEW_GET_FUN(G_glewMultiTexCoord2d)
#define glMultiTexCoord2dv GLEW_GET_FUN(G_glewMultiTexCoord2dv)
#define glMultiTexCoord2f GLEW_GET_FUN(G_glewMultiTexCoord2f)
#define glMultiTexCoord2fv GLEW_GET_FUN(G_glewMultiTexCoord2fv)
#define glMultiTexCoord2i GLEW_GET_FUN(G_glewMultiTexCoord2i)
#define glMultiTexCoord2iv GLEW_GET_FUN(G_glewMultiTexCoord2iv)
#define glMultiTexCoord2s GLEW_GET_FUN(G_glewMultiTexCoord2s)
#define glMultiTexCoord2sv GLEW_GET_FUN(G_glewMultiTexCoord2sv)
#define glMultiTexCoord3d GLEW_GET_FUN(G_glewMultiTexCoord3d)
#define glMultiTexCoord3dv GLEW_GET_FUN(G_glewMultiTexCoord3dv)
#define glMultiTexCoord3f GLEW_GET_FUN(G_glewMultiTexCoord3f)
#define glMultiTexCoord3fv GLEW_GET_FUN(G_glewMultiTexCoord3fv)
#define glMultiTexCoord3i GLEW_GET_FUN(G_glewMultiTexCoord3i)
#define glMultiTexCoord3iv GLEW_GET_FUN(G_glewMultiTexCoord3iv)
#define glMultiTexCoord3s GLEW_GET_FUN(G_glewMultiTexCoord3s)
#define glMultiTexCoord3sv GLEW_GET_FUN(G_glewMultiTexCoord3sv)
#define glMultiTexCoord4d GLEW_GET_FUN(G_glewMultiTexCoord4d)
#define glMultiTexCoord4dv GLEW_GET_FUN(G_glewMultiTexCoord4dv)
#define glMultiTexCoord4f GLEW_GET_FUN(G_glewMultiTexCoord4f)
#define glMultiTexCoord4fv GLEW_GET_FUN(G_glewMultiTexCoord4fv)
#define glMultiTexCoord4i GLEW_GET_FUN(G_glewMultiTexCoord4i)
#define glMultiTexCoord4iv GLEW_GET_FUN(G_glewMultiTexCoord4iv)
#define glMultiTexCoord4s GLEW_GET_FUN(G_glewMultiTexCoord4s)
#define glMultiTexCoord4sv GLEW_GET_FUN(G_glewMultiTexCoord4sv)
#define glSampleCoverage GLEW_GET_FUN(G_glewSampleCoverage)

#define GLEW_VERSION_1_3 GLEW_GET_VAR(G_GLEW_VERSION_1_3)

#endif /* GL_VERSION_1_3 */

/* ----------------------------- GL_VERSION_1_4 ---------------------------- */

#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4 1

#define GL_BLEND_DST_RGB 0x80C8
#define GL_BLEND_SRC_RGB 0x80C9
#define GL_BLEND_DST_ALPHA 0x80CA
#define GL_BLEND_SRC_ALPHA 0x80CB
#define GL_POINT_SIZE_MIN 0x8126
#define GL_POINT_SIZE_MAX 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE 0x8128
#define GL_POINT_DISTANCE_ATTENUATION 0x8129
#define GL_GENERATE_MIPMAP 0x8191
#define GL_GENERATE_MIPMAP_HINT 0x8192
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_DEPTH_COMPONENT24 0x81A6
#define GL_DEPTH_COMPONENT32 0x81A7
#define GL_MIRRORED_REPEAT 0x8370
#define GL_FOG_COORDINATE_SOURCE 0x8450
#define GL_FOG_COORDINATE 0x8451
#define GL_FRAGMENT_DEPTH 0x8452
#define GL_CURRENT_FOG_COORDINATE 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER 0x8456
#define GL_FOG_COORDINATE_ARRAY 0x8457
#define GL_COLOR_SUM 0x8458
#define GL_CURRENT_SECONDARY_COLOR 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER 0x845D
#define GL_SECONDARY_COLOR_ARRAY 0x845E
#define GL_MAX_TEXTURE_LOD_BIAS 0x84FD
#define GL_TEXTURE_FILTER_CONTROL 0x8500
#define GL_TEXTURE_LOD_BIAS 0x8501
#define GL_INCR_WRAP 0x8507
#define GL_DECR_WRAP 0x8508
#define GL_TEXTURE_DEPTH_SIZE 0x884A
#define GL_DEPTH_TEXTURE_MODE 0x884B
#define GL_TEXTURE_COMPARE_MODE 0x884C
#define GL_TEXTURE_COMPARE_FUNC 0x884D
#define GL_COMPARE_R_TO_TEXTURE 0x884E

typedef void (GLAPIENTRY * PFNGLBLENDCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTERPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDPROC) (GLdouble coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDVPROC) (const GLdouble *coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFPROC) (GLfloat coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFVPROC) (const GLfloat *coord);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSPROC) (GLenum mode, const GLsizei *count, GLenum type, const GLvoid **indices, GLsizei drawcount);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVPROC) (GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIVPROC) (GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BVPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DVPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FVPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IVPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SVPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBVPROC) (const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIVPROC) (const GLuint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USVPROC) (const GLushort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTERPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVPROC) (const GLdouble *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVPROC) (const GLfloat *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVPROC) (const GLint *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVPROC) (const GLshort *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVPROC) (const GLdouble *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVPROC) (const GLfloat *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVPROC) (const GLint *p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVPROC) (const GLshort *p);

#define glBlendColor GLEW_GET_FUN(G_glewBlendColor)
#define glBlendEquation GLEW_GET_FUN(G_glewBlendEquation)
#define glBlendFuncSeparate GLEW_GET_FUN(G_glewBlendFuncSeparate)
#define glFogCoordPointer GLEW_GET_FUN(G_glewFogCoordPointer)
#define glFogCoordd GLEW_GET_FUN(G_glewFogCoordd)
#define glFogCoorddv GLEW_GET_FUN(G_glewFogCoorddv)
#define glFogCoordf GLEW_GET_FUN(G_glewFogCoordf)
#define glFogCoordfv GLEW_GET_FUN(G_glewFogCoordfv)
#define glMultiDrawArrays GLEW_GET_FUN(G_glewMultiDrawArrays)
#define glMultiDrawElements GLEW_GET_FUN(G_glewMultiDrawElements)
#define glPointParameterf GLEW_GET_FUN(G_glewPointParameterf)
#define glPointParameterfv GLEW_GET_FUN(G_glewPointParameterfv)
#define glPointParameteri GLEW_GET_FUN(G_glewPointParameteri)
#define glPointParameteriv GLEW_GET_FUN(G_glewPointParameteriv)
#define glSecondaryColor3b GLEW_GET_FUN(G_glewSecondaryColor3b)
#define glSecondaryColor3bv GLEW_GET_FUN(G_glewSecondaryColor3bv)
#define glSecondaryColor3d GLEW_GET_FUN(G_glewSecondaryColor3d)
#define glSecondaryColor3dv GLEW_GET_FUN(G_glewSecondaryColor3dv)
#define glSecondaryColor3f GLEW_GET_FUN(G_glewSecondaryColor3f)
#define glSecondaryColor3fv GLEW_GET_FUN(G_glewSecondaryColor3fv)
#define glSecondaryColor3i GLEW_GET_FUN(G_glewSecondaryColor3i)
#define glSecondaryColor3iv GLEW_GET_FUN(G_glewSecondaryColor3iv)
#define glSecondaryColor3s GLEW_GET_FUN(G_glewSecondaryColor3s)
#define glSecondaryColor3sv GLEW_GET_FUN(G_glewSecondaryColor3sv)
#define glSecondaryColor3ub GLEW_GET_FUN(G_glewSecondaryColor3ub)
#define glSecondaryColor3ubv GLEW_GET_FUN(G_glewSecondaryColor3ubv)
#define glSecondaryColor3ui GLEW_GET_FUN(G_glewSecondaryColor3ui)
#define glSecondaryColor3uiv GLEW_GET_FUN(G_glewSecondaryColor3uiv)
#define glSecondaryColor3us GLEW_GET_FUN(G_glewSecondaryColor3us)
#define glSecondaryColor3usv GLEW_GET_FUN(G_glewSecondaryColor3usv)
#define glSecondaryColorPointer GLEW_GET_FUN(G_glewSecondaryColorPointer)
#define glWindowPos2d GLEW_GET_FUN(G_glewWindowPos2d)
#define glWindowPos2dv GLEW_GET_FUN(G_glewWindowPos2dv)
#define glWindowPos2f GLEW_GET_FUN(G_glewWindowPos2f)
#define glWindowPos2fv GLEW_GET_FUN(G_glewWindowPos2fv)
#define glWindowPos2i GLEW_GET_FUN(G_glewWindowPos2i)
#define glWindowPos2iv GLEW_GET_FUN(G_glewWindowPos2iv)
#define glWindowPos2s GLEW_GET_FUN(G_glewWindowPos2s)
#define glWindowPos2sv GLEW_GET_FUN(G_glewWindowPos2sv)
#define glWindowPos3d GLEW_GET_FUN(G_glewWindowPos3d)
#define glWindowPos3dv GLEW_GET_FUN(G_glewWindowPos3dv)
#define glWindowPos3f GLEW_GET_FUN(G_glewWindowPos3f)
#define glWindowPos3fv GLEW_GET_FUN(G_glewWindowPos3fv)
#define glWindowPos3i GLEW_GET_FUN(G_glewWindowPos3i)
#define glWindowPos3iv GLEW_GET_FUN(G_glewWindowPos3iv)
#define glWindowPos3s GLEW_GET_FUN(G_glewWindowPos3s)
#define glWindowPos3sv GLEW_GET_FUN(G_glewWindowPos3sv)

#define GLEW_VERSION_1_4 GLEW_GET_VAR(G_GLEW_VERSION_1_4)

#endif /* GL_VERSION_1_4 */

/* ----------------------------- GL_VERSION_1_5 ---------------------------- */

#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1

#define GL_FOG_COORD_SRC GL_FOG_COORDINATE_SOURCE
#define GL_FOG_COORD GL_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY GL_FOG_COORDINATE_ARRAY
#define GL_SRC0_RGB GL_SOURCE0_RGB
#define GL_FOG_COORD_ARRAY_POINTER GL_FOG_COORDINATE_ARRAY_POINTER
#define GL_FOG_COORD_ARRAY_TYPE GL_FOG_COORDINATE_ARRAY_TYPE
#define GL_SRC1_ALPHA GL_SOURCE1_ALPHA
#define GL_CURRENT_FOG_COORD GL_CURRENT_FOG_COORDINATE
#define GL_FOG_COORD_ARRAY_STRIDE GL_FOG_COORDINATE_ARRAY_STRIDE
#define GL_SRC0_ALPHA GL_SOURCE0_ALPHA
#define GL_SRC1_RGB GL_SOURCE1_RGB
#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING
#define GL_SRC2_ALPHA GL_SOURCE2_ALPHA
#define GL_SRC2_RGB GL_SOURCE2_RGB
#define GL_BUFFER_SIZE 0x8764
#define GL_BUFFER_USAGE 0x8765
#define GL_QUERY_COUNTER_BITS 0x8864
#define GL_CURRENT_QUERY 0x8865
#define GL_QUERY_RESULT 0x8866
#define GL_QUERY_RESULT_AVAILABLE 0x8867
#define GL_ARRAY_BUFFER 0x8892
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_ARRAY_BUFFER_BINDING 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY 0x88B8
#define GL_WRITE_ONLY 0x88B9
#define GL_READ_WRITE 0x88BA
#define GL_BUFFER_ACCESS 0x88BB
#define GL_BUFFER_MAPPED 0x88BC
#define GL_BUFFER_MAP_POINTER 0x88BD
#define GL_STREAM_DRAW 0x88E0
#define GL_STREAM_READ 0x88E1
#define GL_STREAM_COPY 0x88E2
#define GL_STATIC_DRAW 0x88E4
#define GL_STATIC_READ 0x88E5
#define GL_STATIC_COPY 0x88E6
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_DYNAMIC_READ 0x88E9
#define GL_DYNAMIC_COPY 0x88EA
#define GL_SAMPLES_PASSED 0x8914

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;

typedef void (GLAPIENTRY * PFNGLBEGINQUERYPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERPROC) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBUFFERDATAPROC) (GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERSPROC) (GLsizei n, const GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLDELETEQUERIESPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLENDQUERYPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSPROC) (GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGENQUERIESPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPOINTERVPROC) (GLenum target, GLenum pname, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERSUBDATAPROC) (GLenum target, GLintptr offset, GLsizeiptr size, GLvoid* data);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTIVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUIVPROC) (GLuint id, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERPROC) (GLuint buffer);
typedef GLboolean (GLAPIENTRY * PFNGLISQUERYPROC) (GLuint id);
typedef GLvoid* (GLAPIENTRY * PFNGLMAPBUFFERPROC) (GLenum target, GLenum access);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPBUFFERPROC) (GLenum target);

#define glBeginQuery GLEW_GET_FUN(G_glewBeginQuery)
#define glBindBuffer GLEW_GET_FUN(G_glewBindBuffer)
#define glBufferData GLEW_GET_FUN(G_glewBufferData)
#define glBufferSubData GLEW_GET_FUN(G_glewBufferSubData)
#define glDeleteBuffers GLEW_GET_FUN(G_glewDeleteBuffers)
#define glDeleteQueries GLEW_GET_FUN(G_glewDeleteQueries)
#define glEndQuery GLEW_GET_FUN(G_glewEndQuery)
#define glGenBuffers GLEW_GET_FUN(G_glewGenBuffers)
#define glGenQueries GLEW_GET_FUN(G_glewGenQueries)
#define glGetBufferParameteriv GLEW_GET_FUN(G_glewGetBufferParameteriv)
#define glGetBufferPointerv GLEW_GET_FUN(G_glewGetBufferPointerv)
#define glGetBufferSubData GLEW_GET_FUN(G_glewGetBufferSubData)
#define glGetQueryObjectiv GLEW_GET_FUN(G_glewGetQueryObjectiv)
#define glGetQueryObjectuiv GLEW_GET_FUN(G_glewGetQueryObjectuiv)
#define glGetQueryiv GLEW_GET_FUN(G_glewGetQueryiv)
#define glIsBuffer GLEW_GET_FUN(G_glewIsBuffer)
#define glIsQuery GLEW_GET_FUN(G_glewIsQuery)
#define glMapBuffer GLEW_GET_FUN(G_glewMapBuffer)
#define glUnmapBuffer GLEW_GET_FUN(G_glewUnmapBuffer)

#define GLEW_VERSION_1_5 GLEW_GET_VAR(G_GLEW_VERSION_1_5)

#endif /* GL_VERSION_1_5 */

/* ----------------------------- GL_VERSION_2_0 ---------------------------- */

#ifndef GL_VERSION_2_0
#define GL_VERSION_2_0 1

#define GL_BLEND_EQUATION_RGB GL_BLEND_EQUATION
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE 0x8625
#define GL_CURRENT_VERTEX_ATTRIB 0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE 0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER 0x8645
#define GL_STENCIL_BACK_FUNC 0x8800
#define GL_STENCIL_BACK_FAIL 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS 0x8803
#define GL_MAX_DRAW_BUFFERS 0x8824
#define GL_DRAW_BUFFER0 0x8825
#define GL_DRAW_BUFFER1 0x8826
#define GL_DRAW_BUFFER2 0x8827
#define GL_DRAW_BUFFER3 0x8828
#define GL_DRAW_BUFFER4 0x8829
#define GL_DRAW_BUFFER5 0x882A
#define GL_DRAW_BUFFER6 0x882B
#define GL_DRAW_BUFFER7 0x882C
#define GL_DRAW_BUFFER8 0x882D
#define GL_DRAW_BUFFER9 0x882E
#define GL_DRAW_BUFFER10 0x882F
#define GL_DRAW_BUFFER11 0x8830
#define GL_DRAW_BUFFER12 0x8831
#define GL_DRAW_BUFFER13 0x8832
#define GL_DRAW_BUFFER14 0x8833
#define GL_DRAW_BUFFER15 0x8834
#define GL_BLEND_EQUATION_ALPHA 0x883D
#define GL_POINT_SPRITE 0x8861
#define GL_COORD_REPLACE 0x8862
#define GL_MAX_VERTEX_ATTRIBS 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_COORDS 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS 0x8872
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_VERTEX_SHADER 0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS 0x8B4A
#define GL_MAX_VARYING_FLOATS 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
#define GL_SHADER_TYPE 0x8B4F
#define GL_FLOAT_VEC2 0x8B50
#define GL_FLOAT_VEC3 0x8B51
#define GL_FLOAT_VEC4 0x8B52
#define GL_INT_VEC2 0x8B53
#define GL_INT_VEC3 0x8B54
#define GL_INT_VEC4 0x8B55
#define GL_BOOL 0x8B56
#define GL_BOOL_VEC2 0x8B57
#define GL_BOOL_VEC3 0x8B58
#define GL_BOOL_VEC4 0x8B59
#define GL_FLOAT_MAT2 0x8B5A
#define GL_FLOAT_MAT3 0x8B5B
#define GL_FLOAT_MAT4 0x8B5C
#define GL_SAMPLER_1D 0x8B5D
#define GL_SAMPLER_2D 0x8B5E
#define GL_SAMPLER_3D 0x8B5F
#define GL_SAMPLER_CUBE 0x8B60
#define GL_SAMPLER_1D_SHADOW 0x8B61
#define GL_SAMPLER_2D_SHADOW 0x8B62
#define GL_DELETE_STATUS 0x8B80
#define GL_COMPILE_STATUS 0x8B81
#define GL_LINK_STATUS 0x8B82
#define GL_VALIDATE_STATUS 0x8B83
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_ATTACHED_SHADERS 0x8B85
#define GL_ACTIVE_UNIFORMS 0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH 0x8B87
#define GL_SHADER_SOURCE_LENGTH 0x8B88
#define GL_ACTIVE_ATTRIBUTES 0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH 0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#define GL_CURRENT_PROGRAM 0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN 0x8CA0
#define GL_LOWER_LEFT 0x8CA1
#define GL_UPPER_LEFT 0x8CA2
#define GL_STENCIL_BACK_REF 0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK 0x8CA4
#define GL_STENCIL_BACK_WRITEMASK 0x8CA5

typedef void (GLAPIENTRY * PFNGLATTACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONPROC) (GLuint program, GLuint index, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEPROC) (GLenum, GLenum);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERPROC) (GLuint shader);
typedef GLuint (GLAPIENTRY * PFNGLCREATEPROGRAMPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROC) (GLenum type);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLDELETESHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLDETACHSHADERPROC) (GLuint program, GLuint shader);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSPROC) (GLsizei n, const GLenum* bufs);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBARRAYPROC) (GLuint);
typedef void (GLAPIENTRY * PFNGLGETACTIVEATTRIBPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMPROC) (GLuint program, GLuint index, GLsizei maxLength, GLsizei* length, GLint* size, GLenum* type, GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETATTACHEDSHADERSPROC) (GLuint program, GLsizei maxCount, GLsizei* count, GLuint* shaders);
typedef GLint (GLAPIENTRY * PFNGLGETATTRIBLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMINFOLOGPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVPROC) (GLuint program, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETSHADERINFOLOGPROC) (GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
typedef void (GLAPIENTRY * PFNGLGETSHADERSOURCEPROC) (GLuint obj, GLsizei maxLength, GLsizei* length, GLchar* source);
typedef void (GLAPIENTRY * PFNGLGETSHADERIVPROC) (GLuint shader, GLenum pname, GLint* param);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONPROC) (GLuint program, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMFVPROC) (GLuint program, GLint location, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMIVPROC) (GLuint program, GLint location, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVPROC) (GLuint, GLenum, GLvoid**);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVPROC) (GLuint, GLenum, GLdouble*);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVPROC) (GLuint, GLenum, GLfloat*);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVPROC) (GLuint, GLenum, GLint*);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMPROC) (GLuint program);
typedef GLboolean (GLAPIENTRY * PFNGLISSHADERPROC) (GLuint shader);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEPROC) (GLuint shader, GLsizei count, const GLchar** strings, const GLint* lengths);
typedef void (GLAPIENTRY * PFNGLSTENCILFUNCSEPARATEPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILMASKSEPARATEPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLSTENCILOPSEPARATEPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IPROC) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IVPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NBVPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NIVPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NSVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBVPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUIVPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUSVPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4BVPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4IVPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UIVPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4USVPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);

#define glAttachShader GLEW_GET_FUN(G_glewAttachShader)
#define glBindAttribLocation GLEW_GET_FUN(G_glewBindAttribLocation)
#define glBlendEquationSeparate GLEW_GET_FUN(G_glewBlendEquationSeparate)
#define glCompileShader GLEW_GET_FUN(G_glewCompileShader)
#define glCreateProgram GLEW_GET_FUN(G_glewCreateProgram)
#define glCreateShader GLEW_GET_FUN(G_glewCreateShader)
#define glDeleteProgram GLEW_GET_FUN(G_glewDeleteProgram)
#define glDeleteShader GLEW_GET_FUN(G_glewDeleteShader)
#define glDetachShader GLEW_GET_FUN(G_glewDetachShader)
#define glDisableVertexAttribArray GLEW_GET_FUN(G_glewDisableVertexAttribArray)
#define glDrawBuffers GLEW_GET_FUN(G_glewDrawBuffers)
#define glEnableVertexAttribArray GLEW_GET_FUN(G_glewEnableVertexAttribArray)
#define glGetActiveAttrib GLEW_GET_FUN(G_glewGetActiveAttrib)
#define glGetActiveUniform GLEW_GET_FUN(G_glewGetActiveUniform)
#define glGetAttachedShaders GLEW_GET_FUN(G_glewGetAttachedShaders)
#define glGetAttribLocation GLEW_GET_FUN(G_glewGetAttribLocation)
#define glGetProgramInfoLog GLEW_GET_FUN(G_glewGetProgramInfoLog)
#define glGetProgramiv GLEW_GET_FUN(G_glewGetProgramiv)
#define glGetShaderInfoLog GLEW_GET_FUN(G_glewGetShaderInfoLog)
#define glGetShaderSource GLEW_GET_FUN(G_glewGetShaderSource)
#define glGetShaderiv GLEW_GET_FUN(G_glewGetShaderiv)
#define glGetUniformLocation GLEW_GET_FUN(G_glewGetUniformLocation)
#define glGetUniformfv GLEW_GET_FUN(G_glewGetUniformfv)
#define glGetUniformiv GLEW_GET_FUN(G_glewGetUniformiv)
#define glGetVertexAttribPointerv GLEW_GET_FUN(G_glewGetVertexAttribPointerv)
#define glGetVertexAttribdv GLEW_GET_FUN(G_glewGetVertexAttribdv)
#define glGetVertexAttribfv GLEW_GET_FUN(G_glewGetVertexAttribfv)
#define glGetVertexAttribiv GLEW_GET_FUN(G_glewGetVertexAttribiv)
#define glIsProgram GLEW_GET_FUN(G_glewIsProgram)
#define glIsShader GLEW_GET_FUN(G_glewIsShader)
#define glLinkProgram GLEW_GET_FUN(G_glewLinkProgram)
#define glShaderSource GLEW_GET_FUN(G_glewShaderSource)
#define glStencilFuncSeparate GLEW_GET_FUN(G_glewStencilFuncSeparate)
#define glStencilMaskSeparate GLEW_GET_FUN(G_glewStencilMaskSeparate)
#define glStencilOpSeparate GLEW_GET_FUN(G_glewStencilOpSeparate)
#define glUniform1f GLEW_GET_FUN(G_glewUniform1f)
#define glUniform1fv GLEW_GET_FUN(G_glewUniform1fv)
#define glUniform1i GLEW_GET_FUN(G_glewUniform1i)
#define glUniform1iv GLEW_GET_FUN(G_glewUniform1iv)
#define glUniform2f GLEW_GET_FUN(G_glewUniform2f)
#define glUniform2fv GLEW_GET_FUN(G_glewUniform2fv)
#define glUniform2i GLEW_GET_FUN(G_glewUniform2i)
#define glUniform2iv GLEW_GET_FUN(G_glewUniform2iv)
#define glUniform3f GLEW_GET_FUN(G_glewUniform3f)
#define glUniform3fv GLEW_GET_FUN(G_glewUniform3fv)
#define glUniform3i GLEW_GET_FUN(G_glewUniform3i)
#define glUniform3iv GLEW_GET_FUN(G_glewUniform3iv)
#define glUniform4f GLEW_GET_FUN(G_glewUniform4f)
#define glUniform4fv GLEW_GET_FUN(G_glewUniform4fv)
#define glUniform4i GLEW_GET_FUN(G_glewUniform4i)
#define glUniform4iv GLEW_GET_FUN(G_glewUniform4iv)
#define glUniformMatrix2fv GLEW_GET_FUN(G_glewUniformMatrix2fv)
#define glUniformMatrix3fv GLEW_GET_FUN(G_glewUniformMatrix3fv)
#define glUniformMatrix4fv GLEW_GET_FUN(G_glewUniformMatrix4fv)
#define glUseProgram GLEW_GET_FUN(G_glewUseProgram)
#define glValidateProgram GLEW_GET_FUN(G_glewValidateProgram)
#define glVertexAttrib1d GLEW_GET_FUN(G_glewVertexAttrib1d)
#define glVertexAttrib1dv GLEW_GET_FUN(G_glewVertexAttrib1dv)
#define glVertexAttrib1f GLEW_GET_FUN(G_glewVertexAttrib1f)
#define glVertexAttrib1fv GLEW_GET_FUN(G_glewVertexAttrib1fv)
#define glVertexAttrib1s GLEW_GET_FUN(G_glewVertexAttrib1s)
#define glVertexAttrib1sv GLEW_GET_FUN(G_glewVertexAttrib1sv)
#define glVertexAttrib2d GLEW_GET_FUN(G_glewVertexAttrib2d)
#define glVertexAttrib2dv GLEW_GET_FUN(G_glewVertexAttrib2dv)
#define glVertexAttrib2f GLEW_GET_FUN(G_glewVertexAttrib2f)
#define glVertexAttrib2fv GLEW_GET_FUN(G_glewVertexAttrib2fv)
#define glVertexAttrib2s GLEW_GET_FUN(G_glewVertexAttrib2s)
#define glVertexAttrib2sv GLEW_GET_FUN(G_glewVertexAttrib2sv)
#define glVertexAttrib3d GLEW_GET_FUN(G_glewVertexAttrib3d)
#define glVertexAttrib3dv GLEW_GET_FUN(G_glewVertexAttrib3dv)
#define glVertexAttrib3f GLEW_GET_FUN(G_glewVertexAttrib3f)
#define glVertexAttrib3fv GLEW_GET_FUN(G_glewVertexAttrib3fv)
#define glVertexAttrib3s GLEW_GET_FUN(G_glewVertexAttrib3s)
#define glVertexAttrib3sv GLEW_GET_FUN(G_glewVertexAttrib3sv)
#define glVertexAttrib4Nbv GLEW_GET_FUN(G_glewVertexAttrib4Nbv)
#define glVertexAttrib4Niv GLEW_GET_FUN(G_glewVertexAttrib4Niv)
#define glVertexAttrib4Nsv GLEW_GET_FUN(G_glewVertexAttrib4Nsv)
#define glVertexAttrib4Nub GLEW_GET_FUN(G_glewVertexAttrib4Nub)
#define glVertexAttrib4Nubv GLEW_GET_FUN(G_glewVertexAttrib4Nubv)
#define glVertexAttrib4Nuiv GLEW_GET_FUN(G_glewVertexAttrib4Nuiv)
#define glVertexAttrib4Nusv GLEW_GET_FUN(G_glewVertexAttrib4Nusv)
#define glVertexAttrib4bv GLEW_GET_FUN(G_glewVertexAttrib4bv)
#define glVertexAttrib4d GLEW_GET_FUN(G_glewVertexAttrib4d)
#define glVertexAttrib4dv GLEW_GET_FUN(G_glewVertexAttrib4dv)
#define glVertexAttrib4f GLEW_GET_FUN(G_glewVertexAttrib4f)
#define glVertexAttrib4fv GLEW_GET_FUN(G_glewVertexAttrib4fv)
#define glVertexAttrib4iv GLEW_GET_FUN(G_glewVertexAttrib4iv)
#define glVertexAttrib4s GLEW_GET_FUN(G_glewVertexAttrib4s)
#define glVertexAttrib4sv GLEW_GET_FUN(G_glewVertexAttrib4sv)
#define glVertexAttrib4ubv GLEW_GET_FUN(G_glewVertexAttrib4ubv)
#define glVertexAttrib4uiv GLEW_GET_FUN(G_glewVertexAttrib4uiv)
#define glVertexAttrib4usv GLEW_GET_FUN(G_glewVertexAttrib4usv)
#define glVertexAttribPointer GLEW_GET_FUN(G_glewVertexAttribPointer)

#define GLEW_VERSION_2_0 GLEW_GET_VAR(G_GLEW_VERSION_2_0)

#endif /* GL_VERSION_2_0 */

/* ----------------------------- GL_VERSION_2_1 ---------------------------- */

#ifndef GL_VERSION_2_1
#define GL_VERSION_2_1 1

#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F
#define GL_PIXEL_PACK_BUFFER 0x88EB
#define GL_PIXEL_UNPACK_BUFFER 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING 0x88EF
#define GL_FLOAT_MAT2x3 0x8B65
#define GL_FLOAT_MAT2x4 0x8B66
#define GL_FLOAT_MAT3x2 0x8B67
#define GL_FLOAT_MAT3x4 0x8B68
#define GL_FLOAT_MAT4x2 0x8B69
#define GL_FLOAT_MAT4x3 0x8B6A
#define GL_SRGB 0x8C40
#define GL_SRGB8 0x8C41
#define GL_SRGB_ALPHA 0x8C42
#define GL_SRGB8_ALPHA8 0x8C43
#define GL_SLUMINANCE_ALPHA 0x8C44
#define GL_SLUMINANCE8_ALPHA8 0x8C45
#define GL_SLUMINANCE 0x8C46
#define GL_SLUMINANCE8 0x8C47
#define GL_COMPRESSED_SRGB 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA 0x8C49
#define GL_COMPRESSED_SLUMINANCE 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA 0x8C4B

typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);

#define glUniformMatrix2x3fv GLEW_GET_FUN(G_glewUniformMatrix2x3fv)
#define glUniformMatrix2x4fv GLEW_GET_FUN(G_glewUniformMatrix2x4fv)
#define glUniformMatrix3x2fv GLEW_GET_FUN(G_glewUniformMatrix3x2fv)
#define glUniformMatrix3x4fv GLEW_GET_FUN(G_glewUniformMatrix3x4fv)
#define glUniformMatrix4x2fv GLEW_GET_FUN(G_glewUniformMatrix4x2fv)
#define glUniformMatrix4x3fv GLEW_GET_FUN(G_glewUniformMatrix4x3fv)

#define GLEW_VERSION_2_1 GLEW_GET_VAR(G_GLEW_VERSION_2_1)

#endif /* GL_VERSION_2_1 */

/* ----------------------------- GL_VERSION_3_0 ---------------------------- */

#ifndef GL_VERSION_3_0
#define GL_VERSION_3_0 1

#define GL_MAX_CLIP_DISTANCES GL_MAX_CLIP_PLANES
#define GL_CLIP_DISTANCE5 GL_CLIP_PLANE5
#define GL_CLIP_DISTANCE1 GL_CLIP_PLANE1
#define GL_CLIP_DISTANCE3 GL_CLIP_PLANE3
#define GL_COMPARE_REF_TO_TEXTURE GL_COMPARE_R_TO_TEXTURE_ARB
#define GL_CLIP_DISTANCE0 GL_CLIP_PLANE0
#define GL_CLIP_DISTANCE4 GL_CLIP_PLANE4
#define GL_CLIP_DISTANCE2 GL_CLIP_PLANE2
#define GL_MAX_VARYING_COMPONENTS GL_MAX_VARYING_FLOATS
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
#define GL_MAJOR_VERSION 0x821B
#define GL_MINOR_VERSION 0x821C
#define GL_NUM_EXTENSIONS 0x821D
#define GL_CONTEXT_FLAGS 0x821E
#define GL_DEPTH_BUFFER 0x8223
#define GL_STENCIL_BUFFER 0x8224
#define GL_COMPRESSED_RED 0x8225
#define GL_COMPRESSED_RG 0x8226
#define GL_RGBA32F 0x8814
#define GL_RGB32F 0x8815
#define GL_RGBA16F 0x881A
#define GL_RGB16F 0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER 0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS 0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET 0x8905
#define GL_CLAMP_VERTEX_COLOR 0x891A
#define GL_CLAMP_FRAGMENT_COLOR 0x891B
#define GL_CLAMP_READ_COLOR 0x891C
#define GL_FIXED_ONLY 0x891D
#define GL_TEXTURE_RED_TYPE 0x8C10
#define GL_TEXTURE_GREEN_TYPE 0x8C11
#define GL_TEXTURE_BLUE_TYPE 0x8C12
#define GL_TEXTURE_ALPHA_TYPE 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE 0x8C15
#define GL_TEXTURE_DEPTH_TYPE 0x8C16
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY 0x8C19
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY 0x8C1D
#define GL_R11F_G11F_B10F 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV 0x8C3B
#define GL_RGB9_E5 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV 0x8C3E
#define GL_TEXTURE_SHARED_SIZE 0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS 0x8C8C
#define GL_SEPARATE_ATTRIBS 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI 0x8D70
#define GL_RGB32UI 0x8D71
#define GL_RGBA16UI 0x8D76
#define GL_RGB16UI 0x8D77
#define GL_RGBA8UI 0x8D7C
#define GL_RGB8UI 0x8D7D
#define GL_RGBA32I 0x8D82
#define GL_RGB32I 0x8D83
#define GL_RGBA16I 0x8D88
#define GL_RGB16I 0x8D89
#define GL_RGBA8I 0x8D8E
#define GL_RGB8I 0x8D8F
#define GL_RED_INTEGER 0x8D94
#define GL_GREEN_INTEGER 0x8D95
#define GL_BLUE_INTEGER 0x8D96
#define GL_ALPHA_INTEGER 0x8D97
#define GL_RGB_INTEGER 0x8D98
#define GL_RGBA_INTEGER 0x8D99
#define GL_BGR_INTEGER 0x8D9A
#define GL_BGRA_INTEGER 0x8D9B
#define GL_SAMPLER_1D_ARRAY 0x8DC0
#define GL_SAMPLER_2D_ARRAY 0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW 0x8DC5
#define GL_UNSIGNED_INT_VEC2 0x8DC6
#define GL_UNSIGNED_INT_VEC3 0x8DC7
#define GL_UNSIGNED_INT_VEC4 0x8DC8
#define GL_INT_SAMPLER_1D 0x8DC9
#define GL_INT_SAMPLER_2D 0x8DCA
#define GL_INT_SAMPLER_3D 0x8DCB
#define GL_INT_SAMPLER_CUBE 0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY 0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY 0x8DD7
#define GL_QUERY_WAIT 0x8E13
#define GL_QUERY_NO_WAIT 0x8E14
#define GL_QUERY_BY_REGION_WAIT 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT 0x8E16

typedef void (GLAPIENTRY * PFNGLBEGINCONDITIONALRENDERPROC) (GLuint, GLenum);
typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKPROC) (GLenum);
typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONPROC) (GLuint, GLuint, const GLchar*);
typedef void (GLAPIENTRY * PFNGLCLAMPCOLORPROC) (GLenum, GLenum);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERFIPROC) (GLenum, GLint, GLfloat, GLint);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERFVPROC) (GLenum, GLint, const GLfloat*);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERIVPROC) (GLenum, GLint, const GLint*);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERUIVPROC) (GLenum, GLint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLCOLORMASKIPROC) (GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
typedef void (GLAPIENTRY * PFNGLDISABLEIPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLENABLEIPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLENDCONDITIONALRENDERPROC) (void);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANI_VPROC) (GLenum, GLuint, GLboolean*);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATALOCATIONPROC) (GLuint, const GLchar*);
typedef const GLubyte* (GLAPIENTRY * PFNGLGETSTRINGIPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIIVPROC) (GLenum, GLenum, GLint*);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIUIVPROC) (GLenum, GLenum, GLuint*);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGPROC) (GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUIVPROC) (GLuint, GLint, GLuint*);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIIVPROC) (GLuint, GLenum, GLint*);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIUIVPROC) (GLuint, GLenum, GLuint*);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDIPROC) (GLenum, GLuint);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIIVPROC) (GLenum, GLenum, const GLint*);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIUIVPROC) (GLenum, GLenum, const GLuint*);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSPROC) (GLuint, GLsizei, const GLchar **, GLenum);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIPROC) (GLint, GLuint);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIPROC) (GLint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIPROC) (GLint, GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIPROC) (GLint, GLuint, GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIVPROC) (GLint, GLsizei, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IPROC) (GLuint, GLint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IVPROC) (GLuint, const GLint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIPROC) (GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIVPROC) (GLuint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IPROC) (GLuint, GLint, GLint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IVPROC) (GLuint, const GLint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIPROC) (GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIVPROC) (GLuint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IPROC) (GLuint, GLint, GLint, GLint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IVPROC) (GLuint, const GLint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIPROC) (GLuint, GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIVPROC) (GLuint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4BVPROC) (GLuint, const GLbyte*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IPROC) (GLuint, GLint, GLint, GLint, GLint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IVPROC) (GLuint, const GLint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4SVPROC) (GLuint, const GLshort*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UBVPROC) (GLuint, const GLubyte*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIPROC) (GLuint, GLuint, GLuint, GLuint, GLuint);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIVPROC) (GLuint, const GLuint*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4USVPROC) (GLuint, const GLushort*);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIPOINTERPROC) (GLuint, GLint, GLenum, GLsizei, const GLvoid*);

#define glBeginConditionalRender GLEW_GET_FUN(G_glewBeginConditionalRender)
#define glBeginTransformFeedback GLEW_GET_FUN(G_glewBeginTransformFeedback)
#define glBindFragDataLocation GLEW_GET_FUN(G_glewBindFragDataLocation)
#define glClampColor GLEW_GET_FUN(G_glewClampColor)
#define glClearBufferfi GLEW_GET_FUN(G_glewClearBufferfi)
#define glClearBufferfv GLEW_GET_FUN(G_glewClearBufferfv)
#define glClearBufferiv GLEW_GET_FUN(G_glewClearBufferiv)
#define glClearBufferuiv GLEW_GET_FUN(G_glewClearBufferuiv)
#define glColorMaski GLEW_GET_FUN(G_glewColorMaski)
#define glDisablei GLEW_GET_FUN(G_glewDisablei)
#define glEnablei GLEW_GET_FUN(G_glewEnablei)
#define glEndConditionalRender GLEW_GET_FUN(G_glewEndConditionalRender)
#define glEndTransformFeedback GLEW_GET_FUN(G_glewEndTransformFeedback)
#define glGetBooleani_v GLEW_GET_FUN(G_glewGetBooleani_v)
#define glGetFragDataLocation GLEW_GET_FUN(G_glewGetFragDataLocation)
#define glGetStringi GLEW_GET_FUN(G_glewGetStringi)
#define glGetTexParameterIiv GLEW_GET_FUN(G_glewGetTexParameterIiv)
#define glGetTexParameterIuiv GLEW_GET_FUN(G_glewGetTexParameterIuiv)
#define glGetTransformFeedbackVarying GLEW_GET_FUN(G_glewGetTransformFeedbackVarying)
#define glGetUniformuiv GLEW_GET_FUN(G_glewGetUniformuiv)
#define glGetVertexAttribIiv GLEW_GET_FUN(G_glewGetVertexAttribIiv)
#define glGetVertexAttribIuiv GLEW_GET_FUN(G_glewGetVertexAttribIuiv)
#define glIsEnabledi GLEW_GET_FUN(G_glewIsEnabledi)
#define glTexParameterIiv GLEW_GET_FUN(G_glewTexParameterIiv)
#define glTexParameterIuiv GLEW_GET_FUN(G_glewTexParameterIuiv)
#define glTransformFeedbackVaryings GLEW_GET_FUN(G_glewTransformFeedbackVaryings)
#define glUniform1ui GLEW_GET_FUN(G_glewUniform1ui)
#define glUniform1uiv GLEW_GET_FUN(G_glewUniform1uiv)
#define glUniform2ui GLEW_GET_FUN(G_glewUniform2ui)
#define glUniform2uiv GLEW_GET_FUN(G_glewUniform2uiv)
#define glUniform3ui GLEW_GET_FUN(G_glewUniform3ui)
#define glUniform3uiv GLEW_GET_FUN(G_glewUniform3uiv)
#define glUniform4ui GLEW_GET_FUN(G_glewUniform4ui)
#define glUniform4uiv GLEW_GET_FUN(G_glewUniform4uiv)
#define glVertexAttribI1i GLEW_GET_FUN(G_glewVertexAttribI1i)
#define glVertexAttribI1iv GLEW_GET_FUN(G_glewVertexAttribI1iv)
#define glVertexAttribI1ui GLEW_GET_FUN(G_glewVertexAttribI1ui)
#define glVertexAttribI1uiv GLEW_GET_FUN(G_glewVertexAttribI1uiv)
#define glVertexAttribI2i GLEW_GET_FUN(G_glewVertexAttribI2i)
#define glVertexAttribI2iv GLEW_GET_FUN(G_glewVertexAttribI2iv)
#define glVertexAttribI2ui GLEW_GET_FUN(G_glewVertexAttribI2ui)
#define glVertexAttribI2uiv GLEW_GET_FUN(G_glewVertexAttribI2uiv)
#define glVertexAttribI3i GLEW_GET_FUN(G_glewVertexAttribI3i)
#define glVertexAttribI3iv GLEW_GET_FUN(G_glewVertexAttribI3iv)
#define glVertexAttribI3ui GLEW_GET_FUN(G_glewVertexAttribI3ui)
#define glVertexAttribI3uiv GLEW_GET_FUN(G_glewVertexAttribI3uiv)
#define glVertexAttribI4bv GLEW_GET_FUN(G_glewVertexAttribI4bv)
#define glVertexAttribI4i GLEW_GET_FUN(G_glewVertexAttribI4i)
#define glVertexAttribI4iv GLEW_GET_FUN(G_glewVertexAttribI4iv)
#define glVertexAttribI4sv GLEW_GET_FUN(G_glewVertexAttribI4sv)
#define glVertexAttribI4ubv GLEW_GET_FUN(G_glewVertexAttribI4ubv)
#define glVertexAttribI4ui GLEW_GET_FUN(G_glewVertexAttribI4ui)
#define glVertexAttribI4uiv GLEW_GET_FUN(G_glewVertexAttribI4uiv)
#define glVertexAttribI4usv GLEW_GET_FUN(G_glewVertexAttribI4usv)
#define glVertexAttribIPointer GLEW_GET_FUN(G_glewVertexAttribIPointer)

#define GLEW_VERSION_3_0 GLEW_GET_VAR(G_GLEW_VERSION_3_0)

#endif /* GL_VERSION_3_0 */

/* ----------------------------- GL_VERSION_3_1 ---------------------------- */

#ifndef GL_VERSION_3_1
#define GL_VERSION_3_1 1

#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE 0x84F8
#define GL_SAMPLER_2D_RECT 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW 0x8B64
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT 0x8C2E
#define GL_SAMPLER_BUFFER 0x8DC2
#define GL_INT_SAMPLER_2D_RECT 0x8DCD
#define GL_INT_SAMPLER_BUFFER 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER 0x8DD8
#define GL_RED_SNORM 0x8F90
#define GL_RG_SNORM 0x8F91
#define GL_RGB_SNORM 0x8F92
#define GL_RGBA_SNORM 0x8F93
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_PRIMITIVE_RESTART 0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX 0x8F9E
#define GL_BUFFER_ACCESS_FLAGS 0x911F
#define GL_BUFFER_MAP_LENGTH 0x9120
#define GL_BUFFER_MAP_OFFSET 0x9121

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDPROC) (GLenum, GLint, GLsizei, GLsizei);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDPROC) (GLenum, GLsizei, GLenum, const GLvoid*, GLsizei);
typedef void (GLAPIENTRY * PFNGLPRIMITIVERESTARTINDEXPROC) (GLuint);
typedef void (GLAPIENTRY * PFNGLTEXBUFFERPROC) (GLenum, GLenum, GLuint);

#define glDrawArraysInstanced GLEW_GET_FUN(G_glewDrawArraysInstanced)
#define glDrawElementsInstanced GLEW_GET_FUN(G_glewDrawElementsInstanced)
#define glPrimitiveRestartIndex GLEW_GET_FUN(G_glewPrimitiveRestartIndex)
#define glTexBuffer GLEW_GET_FUN(G_glewTexBuffer)

#define GLEW_VERSION_3_1 GLEW_GET_VAR(G_GLEW_VERSION_3_1)

#endif /* GL_VERSION_3_1 */

/* ----------------------------- GL_VERSION_3_2 ---------------------------- */

#ifndef GL_VERSION_3_2
#define GL_VERSION_3_2 1

#define GL_CONTEXT_CORE_PROFILE_BIT 0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY 0x000A
#define GL_LINE_STRIP_ADJACENCY 0x000B
#define GL_TRIANGLES_ADJACENCY 0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY 0x000D
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_GEOMETRY_VERTICES_OUT 0x8916
#define GL_GEOMETRY_INPUT_TYPE 0x8917
#define GL_GEOMETRY_OUTPUT_TYPE 0x8918
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER 0x8DD9
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS 0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS 0x9125
#define GL_CONTEXT_PROFILE_MASK 0x9126

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREPROC) (GLenum, GLenum, GLuint, GLint);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERI64VPROC) (GLenum, GLenum, GLint64 *);
typedef void (GLAPIENTRY * PFNGLGETINTEGER64I_VPROC) (GLenum, GLuint, GLint64 *);

#define glFramebufferTexture GLEW_GET_FUN(G_glewFramebufferTexture)
#define glGetBufferParameteri64v GLEW_GET_FUN(G_glewGetBufferParameteri64v)
#define glGetInteger64i_v GLEW_GET_FUN(G_glewGetInteger64i_v)

#define GLEW_VERSION_3_2 GLEW_GET_VAR(G_GLEW_VERSION_3_2)

#endif /* GL_VERSION_3_2 */

/* ----------------------------- GL_VERSION_3_3 ---------------------------- */

#ifndef GL_VERSION_3_3
#define GL_VERSION_3_3 1

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR 0x88FE
#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46
#define GL_RGB10_A2UI 0x906F

typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBDIVISORPROC) (GLuint index, GLuint divisor);

#define glVertexAttribDivisor GLEW_GET_FUN(G_glewVertexAttribDivisor)

#define GLEW_VERSION_3_3 GLEW_GET_VAR(G_GLEW_VERSION_3_3)

#endif /* GL_VERSION_3_3 */

/* ----------------------------- GL_VERSION_4_0 ---------------------------- */

#ifndef GL_VERSION_4_0
#define GL_VERSION_4_0 1

#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F
#define GL_SAMPLE_SHADING 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE 0x8C37
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS 0x8F9F
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEIPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONIPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEIPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCIPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (GLAPIENTRY * PFNGLMINSAMPLESHADINGPROC) (GLclampf value);

#define glBlendEquationSeparatei GLEW_GET_FUN(G_glewBlendEquationSeparatei)
#define glBlendEquationi GLEW_GET_FUN(G_glewBlendEquationi)
#define glBlendFuncSeparatei GLEW_GET_FUN(G_glewBlendFuncSeparatei)
#define glBlendFunci GLEW_GET_FUN(G_glewBlendFunci)
#define glMinSampleShading GLEW_GET_FUN(G_glewMinSampleShading)

#define GLEW_VERSION_4_0 GLEW_GET_VAR(G_GLEW_VERSION_4_0)

#endif /* GL_VERSION_4_0 */

/* ----------------------------- GL_VERSION_4_1 ---------------------------- */

#ifndef GL_VERSION_4_1
#define GL_VERSION_4_1 1

#define GLEW_VERSION_4_1 GLEW_GET_VAR(G_GLEW_VERSION_4_1)

#endif /* GL_VERSION_4_1 */

/* ----------------------------- GL_VERSION_4_2 ---------------------------- */

#ifndef GL_VERSION_4_2
#define GL_VERSION_4_2 1

#define GL_COMPRESSED_RGBA_BPTC_UNORM 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT 0x8E8F

#define GLEW_VERSION_4_2 GLEW_GET_VAR(G_GLEW_VERSION_4_2)

#endif /* GL_VERSION_4_2 */

/* ----------------------------- GL_VERSION_4_3 ---------------------------- */

#ifndef GL_VERSION_4_3
#define GL_VERSION_4_3 1

#define GL_NUM_SHADING_LANGUAGE_VERSIONS 0x82E9
#define GL_VERTEX_ATTRIB_ARRAY_LONG 0x874E

#define GLEW_VERSION_4_3 GLEW_GET_VAR(G_GLEW_VERSION_4_3)

#endif /* GL_VERSION_4_3 */

/* -------------------------- GL_3DFX_multisample -------------------------- */

#ifndef GL_3DFX_multisample
#define GL_3DFX_multisample 1

#define GL_MULTISAMPLE_3DFX 0x86B2
#define GL_SAMPLE_BUFFERS_3DFX 0x86B3
#define GL_SAMPLES_3DFX 0x86B4
#define GL_MULTISAMPLE_BIT_3DFX 0x20000000

#define GLEW_3DFX_multisample GLEW_GET_VAR(G_GLEW_3DFX_multisample)

#endif /* GL_3DFX_multisample */

/* ---------------------------- GL_3DFX_tbuffer ---------------------------- */

#ifndef GL_3DFX_tbuffer
#define GL_3DFX_tbuffer 1

typedef void (GLAPIENTRY * PFNGLTBUFFERMASK3DFXPROC) (GLuint mask);

#define glTbufferMask3DFX GLEW_GET_FUN(G_glewTbufferMask3DFX)

#define GLEW_3DFX_tbuffer GLEW_GET_VAR(G_GLEW_3DFX_tbuffer)

#endif /* GL_3DFX_tbuffer */

/* -------------------- GL_3DFX_texture_compression_FXT1 ------------------- */

#ifndef GL_3DFX_texture_compression_FXT1
#define GL_3DFX_texture_compression_FXT1 1

#define GL_COMPRESSED_RGB_FXT1_3DFX 0x86B0
#define GL_COMPRESSED_RGBA_FXT1_3DFX 0x86B1

#define GLEW_3DFX_texture_compression_FXT1 GLEW_GET_VAR(G_GLEW_3DFX_texture_compression_FXT1)

#endif /* GL_3DFX_texture_compression_FXT1 */

/* ----------------------- GL_AMD_blend_minmax_factor ---------------------- */

#ifndef GL_AMD_blend_minmax_factor
#define GL_AMD_blend_minmax_factor 1

#define GL_FACTOR_MIN_AMD 0x901C
#define GL_FACTOR_MAX_AMD 0x901D

#define GLEW_AMD_blend_minmax_factor GLEW_GET_VAR(G_GLEW_AMD_blend_minmax_factor)

#endif /* GL_AMD_blend_minmax_factor */

/* ----------------------- GL_AMD_conservative_depth ----------------------- */

#ifndef GL_AMD_conservative_depth
#define GL_AMD_conservative_depth 1

#define GLEW_AMD_conservative_depth GLEW_GET_VAR(G_GLEW_AMD_conservative_depth)

#endif /* GL_AMD_conservative_depth */

/* -------------------------- GL_AMD_debug_output -------------------------- */

#ifndef GL_AMD_debug_output
#define GL_AMD_debug_output 1

#define GL_MAX_DEBUG_MESSAGE_LENGTH_AMD 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_AMD 0x9144
#define GL_DEBUG_LOGGED_MESSAGES_AMD 0x9145
#define GL_DEBUG_SEVERITY_HIGH_AMD 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_AMD 0x9147
#define GL_DEBUG_SEVERITY_LOW_AMD 0x9148
#define GL_DEBUG_CATEGORY_API_ERROR_AMD 0x9149
#define GL_DEBUG_CATEGORY_WINDOW_SYSTEM_AMD 0x914A
#define GL_DEBUG_CATEGORY_DEPRECATION_AMD 0x914B
#define GL_DEBUG_CATEGORY_UNDEFINED_BEHAVIOR_AMD 0x914C
#define GL_DEBUG_CATEGORY_PERFORMANCE_AMD 0x914D
#define GL_DEBUG_CATEGORY_SHADER_COMPILER_AMD 0x914E
#define GL_DEBUG_CATEGORY_APPLICATION_AMD 0x914F
#define GL_DEBUG_CATEGORY_OTHER_AMD 0x9150

typedef void (APIENTRY *GLDEBUGPROCAMD)(GLuint id, GLenum category, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECALLBACKAMDPROC) (GLDEBUGPROCAMD callback, void* userParam);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEENABLEAMDPROC) (GLenum category, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEINSERTAMDPROC) (GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar* buf);
typedef GLuint (GLAPIENTRY * PFNGLGETDEBUGMESSAGELOGAMDPROC) (GLuint count, GLsizei bufsize, GLenum* categories, GLuint* severities, GLuint* ids, GLsizei* lengths, GLchar* message);

#define glDebugMessageCallbackAMD GLEW_GET_FUN(G_glewDebugMessageCallbackAMD)
#define glDebugMessageEnableAMD GLEW_GET_FUN(G_glewDebugMessageEnableAMD)
#define glDebugMessageInsertAMD GLEW_GET_FUN(G_glewDebugMessageInsertAMD)
#define glGetDebugMessageLogAMD GLEW_GET_FUN(G_glewGetDebugMessageLogAMD)

#define GLEW_AMD_debug_output GLEW_GET_VAR(G_GLEW_AMD_debug_output)

#endif /* GL_AMD_debug_output */

/* ---------------------- GL_AMD_depth_clamp_separate ---------------------- */

#ifndef GL_AMD_depth_clamp_separate
#define GL_AMD_depth_clamp_separate 1

#define GL_DEPTH_CLAMP_NEAR_AMD 0x901E
#define GL_DEPTH_CLAMP_FAR_AMD 0x901F

#define GLEW_AMD_depth_clamp_separate GLEW_GET_VAR(G_GLEW_AMD_depth_clamp_separate)

#endif /* GL_AMD_depth_clamp_separate */

/* ----------------------- GL_AMD_draw_buffers_blend ----------------------- */

#ifndef GL_AMD_draw_buffers_blend
#define GL_AMD_draw_buffers_blend 1

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONINDEXEDAMDPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCINDEXEDAMDPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);

#define glBlendEquationIndexedAMD GLEW_GET_FUN(G_glewBlendEquationIndexedAMD)
#define glBlendEquationSeparateIndexedAMD GLEW_GET_FUN(G_glewBlendEquationSeparateIndexedAMD)
#define glBlendFuncIndexedAMD GLEW_GET_FUN(G_glewBlendFuncIndexedAMD)
#define glBlendFuncSeparateIndexedAMD GLEW_GET_FUN(G_glewBlendFuncSeparateIndexedAMD)

#define GLEW_AMD_draw_buffers_blend GLEW_GET_VAR(G_GLEW_AMD_draw_buffers_blend)

#endif /* GL_AMD_draw_buffers_blend */

/* ----------------------- GL_AMD_multi_draw_indirect ---------------------- */

#ifndef GL_AMD_multi_draw_indirect
#define GL_AMD_multi_draw_indirect 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSINDIRECTAMDPROC) (GLenum mode, const void* indirect, GLsizei primcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSINDIRECTAMDPROC) (GLenum mode, GLenum type, const void* indirect, GLsizei primcount, GLsizei stride);

#define glMultiDrawArraysIndirectAMD GLEW_GET_FUN(G_glewMultiDrawArraysIndirectAMD)
#define glMultiDrawElementsIndirectAMD GLEW_GET_FUN(G_glewMultiDrawElementsIndirectAMD)

#define GLEW_AMD_multi_draw_indirect GLEW_GET_VAR(G_GLEW_AMD_multi_draw_indirect)

#endif /* GL_AMD_multi_draw_indirect */

/* ------------------------- GL_AMD_name_gen_delete ------------------------ */

#ifndef GL_AMD_name_gen_delete
#define GL_AMD_name_gen_delete 1

#define GL_DATA_BUFFER_AMD 0x9151
#define GL_PERFORMANCE_MONITOR_AMD 0x9152
#define GL_QUERY_OBJECT_AMD 0x9153
#define GL_VERTEX_ARRAY_OBJECT_AMD 0x9154
#define GL_SAMPLER_OBJECT_AMD 0x9155

typedef void (GLAPIENTRY * PFNGLDELETENAMESAMDPROC) (GLenum identifier, GLuint num, const GLuint* names);
typedef void (GLAPIENTRY * PFNGLGENNAMESAMDPROC) (GLenum identifier, GLuint num, GLuint* names);
typedef GLboolean (GLAPIENTRY * PFNGLISNAMEAMDPROC) (GLenum identifier, GLuint name);

#define glDeleteNamesAMD GLEW_GET_FUN(G_glewDeleteNamesAMD)
#define glGenNamesAMD GLEW_GET_FUN(G_glewGenNamesAMD)
#define glIsNameAMD GLEW_GET_FUN(G_glewIsNameAMD)

#define GLEW_AMD_name_gen_delete GLEW_GET_VAR(G_GLEW_AMD_name_gen_delete)

#endif /* GL_AMD_name_gen_delete */

/* ----------------------- GL_AMD_performance_monitor ---------------------- */

#ifndef GL_AMD_performance_monitor
#define GL_AMD_performance_monitor 1

#define GL_COUNTER_TYPE_AMD 0x8BC0
#define GL_COUNTER_RANGE_AMD 0x8BC1
#define GL_UNSIGNED_INT64_AMD 0x8BC2
#define GL_PERCENTAGE_AMD 0x8BC3
#define GL_PERFMON_RESULT_AVAILABLE_AMD 0x8BC4
#define GL_PERFMON_RESULT_SIZE_AMD 0x8BC5
#define GL_PERFMON_RESULT_AMD 0x8BC6

typedef void (GLAPIENTRY * PFNGLBEGINPERFMONITORAMDPROC) (GLuint monitor);
typedef void (GLAPIENTRY * PFNGLDELETEPERFMONITORSAMDPROC) (GLsizei n, GLuint* monitors);
typedef void (GLAPIENTRY * PFNGLENDPERFMONITORAMDPROC) (GLuint monitor);
typedef void (GLAPIENTRY * PFNGLGENPERFMONITORSAMDPROC) (GLsizei n, GLuint* monitors);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORCOUNTERDATAAMDPROC) (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint* data, GLint *bytesWritten);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORCOUNTERINFOAMDPROC) (GLuint group, GLuint counter, GLenum pname, void* data);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC) (GLuint group, GLuint counter, GLsizei bufSize, GLsizei* length, GLchar *counterString);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORCOUNTERSAMDPROC) (GLuint group, GLint* numCounters, GLint *maxActiveCounters, GLsizei countersSize, GLuint *counters);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORGROUPSTRINGAMDPROC) (GLuint group, GLsizei bufSize, GLsizei* length, GLchar *groupString);
typedef void (GLAPIENTRY * PFNGLGETPERFMONITORGROUPSAMDPROC) (GLint* numGroups, GLsizei groupsSize, GLuint *groups);
typedef void (GLAPIENTRY * PFNGLSELECTPERFMONITORCOUNTERSAMDPROC) (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint* counterList);

#define glBeginPerfMonitorAMD GLEW_GET_FUN(G_glewBeginPerfMonitorAMD)
#define glDeletePerfMonitorsAMD GLEW_GET_FUN(G_glewDeletePerfMonitorsAMD)
#define glEndPerfMonitorAMD GLEW_GET_FUN(G_glewEndPerfMonitorAMD)
#define glGenPerfMonitorsAMD GLEW_GET_FUN(G_glewGenPerfMonitorsAMD)
#define glGetPerfMonitorCounterDataAMD GLEW_GET_FUN(G_glewGetPerfMonitorCounterDataAMD)
#define glGetPerfMonitorCounterInfoAMD GLEW_GET_FUN(G_glewGetPerfMonitorCounterInfoAMD)
#define glGetPerfMonitorCounterStringAMD GLEW_GET_FUN(G_glewGetPerfMonitorCounterStringAMD)
#define glGetPerfMonitorCountersAMD GLEW_GET_FUN(G_glewGetPerfMonitorCountersAMD)
#define glGetPerfMonitorGroupStringAMD GLEW_GET_FUN(G_glewGetPerfMonitorGroupStringAMD)
#define glGetPerfMonitorGroupsAMD GLEW_GET_FUN(G_glewGetPerfMonitorGroupsAMD)
#define glSelectPerfMonitorCountersAMD GLEW_GET_FUN(G_glewSelectPerfMonitorCountersAMD)

#define GLEW_AMD_performance_monitor GLEW_GET_VAR(G_GLEW_AMD_performance_monitor)

#endif /* GL_AMD_performance_monitor */

/* -------------------------- GL_AMD_pinned_memory ------------------------- */

#ifndef GL_AMD_pinned_memory
#define GL_AMD_pinned_memory 1

#define GL_EXTERNAL_VIRTUAL_MEMORY_BUFFER_AMD 0x9160

#define GLEW_AMD_pinned_memory GLEW_GET_VAR(G_GLEW_AMD_pinned_memory)

#endif /* GL_AMD_pinned_memory */

/* ----------------------- GL_AMD_query_buffer_object ---------------------- */

#ifndef GL_AMD_query_buffer_object
#define GL_AMD_query_buffer_object 1

#define GL_QUERY_BUFFER_AMD 0x9192
#define GL_QUERY_BUFFER_BINDING_AMD 0x9193
#define GL_QUERY_RESULT_NO_WAIT_AMD 0x9194

#define GLEW_AMD_query_buffer_object GLEW_GET_VAR(G_GLEW_AMD_query_buffer_object)

#endif /* GL_AMD_query_buffer_object */

/* ------------------------ GL_AMD_sample_positions ------------------------ */

#ifndef GL_AMD_sample_positions
#define GL_AMD_sample_positions 1

#define GL_SUBSAMPLE_DISTANCE_AMD 0x883F

typedef void (GLAPIENTRY * PFNGLSETMULTISAMPLEFVAMDPROC) (GLenum pname, GLuint index, const GLfloat* val);

#define glSetMultisamplefvAMD GLEW_GET_FUN(G_glewSetMultisamplefvAMD)

#define GLEW_AMD_sample_positions GLEW_GET_VAR(G_GLEW_AMD_sample_positions)

#endif /* GL_AMD_sample_positions */

/* ------------------ GL_AMD_seamless_cubemap_per_texture ------------------ */

#ifndef GL_AMD_seamless_cubemap_per_texture
#define GL_AMD_seamless_cubemap_per_texture 1

#define GL_TEXTURE_CUBE_MAP_SEAMLESS_ARB 0x884F

#define GLEW_AMD_seamless_cubemap_per_texture GLEW_GET_VAR(G_GLEW_AMD_seamless_cubemap_per_texture)

#endif /* GL_AMD_seamless_cubemap_per_texture */

/* ---------------------- GL_AMD_shader_stencil_export --------------------- */

#ifndef GL_AMD_shader_stencil_export
#define GL_AMD_shader_stencil_export 1

#define GLEW_AMD_shader_stencil_export GLEW_GET_VAR(G_GLEW_AMD_shader_stencil_export)

#endif /* GL_AMD_shader_stencil_export */

/* ------------------- GL_AMD_stencil_operation_extended ------------------- */

#ifndef GL_AMD_stencil_operation_extended
#define GL_AMD_stencil_operation_extended 1

#define GL_SET_AMD 0x874A
#define GL_REPLACE_VALUE_AMD 0x874B
#define GL_STENCIL_OP_VALUE_AMD 0x874C
#define GL_STENCIL_BACK_OP_VALUE_AMD 0x874D

typedef void (GLAPIENTRY * PFNGLSTENCILOPVALUEAMDPROC) (GLenum face, GLuint value);

#define glStencilOpValueAMD GLEW_GET_FUN(G_glewStencilOpValueAMD)

#define GLEW_AMD_stencil_operation_extended GLEW_GET_VAR(G_GLEW_AMD_stencil_operation_extended)

#endif /* GL_AMD_stencil_operation_extended */

/* ------------------------ GL_AMD_texture_texture4 ------------------------ */

#ifndef GL_AMD_texture_texture4
#define GL_AMD_texture_texture4 1

#define GLEW_AMD_texture_texture4 GLEW_GET_VAR(G_GLEW_AMD_texture_texture4)

#endif /* GL_AMD_texture_texture4 */

/* --------------- GL_AMD_transform_feedback3_lines_triangles -------------- */

#ifndef GL_AMD_transform_feedback3_lines_triangles
#define GL_AMD_transform_feedback3_lines_triangles 1

#define GLEW_AMD_transform_feedback3_lines_triangles GLEW_GET_VAR(G_GLEW_AMD_transform_feedback3_lines_triangles)

#endif /* GL_AMD_transform_feedback3_lines_triangles */

/* ----------------------- GL_AMD_vertex_shader_layer ---------------------- */

#ifndef GL_AMD_vertex_shader_layer
#define GL_AMD_vertex_shader_layer 1

#define GLEW_AMD_vertex_shader_layer GLEW_GET_VAR(G_GLEW_AMD_vertex_shader_layer)

#endif /* GL_AMD_vertex_shader_layer */

/* -------------------- GL_AMD_vertex_shader_tessellator ------------------- */

#ifndef GL_AMD_vertex_shader_tessellator
#define GL_AMD_vertex_shader_tessellator 1

#define GL_SAMPLER_BUFFER_AMD 0x9001
#define GL_INT_SAMPLER_BUFFER_AMD 0x9002
#define GL_UNSIGNED_INT_SAMPLER_BUFFER_AMD 0x9003
#define GL_TESSELLATION_MODE_AMD 0x9004
#define GL_TESSELLATION_FACTOR_AMD 0x9005
#define GL_DISCRETE_AMD 0x9006
#define GL_CONTINUOUS_AMD 0x9007

typedef void (GLAPIENTRY * PFNGLTESSELLATIONFACTORAMDPROC) (GLfloat factor);
typedef void (GLAPIENTRY * PFNGLTESSELLATIONMODEAMDPROC) (GLenum mode);

#define glTessellationFactorAMD GLEW_GET_FUN(G_glewTessellationFactorAMD)
#define glTessellationModeAMD GLEW_GET_FUN(G_glewTessellationModeAMD)

#define GLEW_AMD_vertex_shader_tessellator GLEW_GET_VAR(G_GLEW_AMD_vertex_shader_tessellator)

#endif /* GL_AMD_vertex_shader_tessellator */

/* ------------------ GL_AMD_vertex_shader_viewport_index ------------------ */

#ifndef GL_AMD_vertex_shader_viewport_index
#define GL_AMD_vertex_shader_viewport_index 1

#define GLEW_AMD_vertex_shader_viewport_index GLEW_GET_VAR(G_GLEW_AMD_vertex_shader_viewport_index)

#endif /* GL_AMD_vertex_shader_viewport_index */

/* ----------------------- GL_APPLE_aux_depth_stencil ---------------------- */

#ifndef GL_APPLE_aux_depth_stencil
#define GL_APPLE_aux_depth_stencil 1

#define GL_AUX_DEPTH_STENCIL_APPLE 0x8A14

#define GLEW_APPLE_aux_depth_stencil GLEW_GET_VAR(G_GLEW_APPLE_aux_depth_stencil)

#endif /* GL_APPLE_aux_depth_stencil */

/* ------------------------ GL_APPLE_client_storage ------------------------ */

#ifndef GL_APPLE_client_storage
#define GL_APPLE_client_storage 1

#define GL_UNPACK_CLIENT_STORAGE_APPLE 0x85B2

#define GLEW_APPLE_client_storage GLEW_GET_VAR(G_GLEW_APPLE_client_storage)

#endif /* GL_APPLE_client_storage */

/* ------------------------- GL_APPLE_element_array ------------------------ */

#ifndef GL_APPLE_element_array
#define GL_APPLE_element_array 1

#define GL_ELEMENT_ARRAY_APPLE 0x8A0C
#define GL_ELEMENT_ARRAY_TYPE_APPLE 0x8A0D
#define GL_ELEMENT_ARRAY_POINTER_APPLE 0x8A0E

typedef void (GLAPIENTRY * PFNGLDRAWELEMENTARRAYAPPLEPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC) (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLELEMENTPOINTERAPPLEPROC) (GLenum type, const void* pointer);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC) (GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC) (GLenum mode, GLuint start, GLuint end, const GLint* first, const GLsizei *count, GLsizei primcount);

#define glDrawElementArrayAPPLE GLEW_GET_FUN(G_glewDrawElementArrayAPPLE)
#define glDrawRangeElementArrayAPPLE GLEW_GET_FUN(G_glewDrawRangeElementArrayAPPLE)
#define glElementPointerAPPLE GLEW_GET_FUN(G_glewElementPointerAPPLE)
#define glMultiDrawElementArrayAPPLE GLEW_GET_FUN(G_glewMultiDrawElementArrayAPPLE)
#define glMultiDrawRangeElementArrayAPPLE GLEW_GET_FUN(G_glewMultiDrawRangeElementArrayAPPLE)

#define GLEW_APPLE_element_array GLEW_GET_VAR(G_GLEW_APPLE_element_array)

#endif /* GL_APPLE_element_array */

/* ----------------------------- GL_APPLE_fence ---------------------------- */

#ifndef GL_APPLE_fence
#define GL_APPLE_fence 1

#define GL_DRAW_PIXELS_APPLE 0x8A0A
#define GL_FENCE_APPLE 0x8A0B

typedef void (GLAPIENTRY * PFNGLDELETEFENCESAPPLEPROC) (GLsizei n, const GLuint* fences);
typedef void (GLAPIENTRY * PFNGLFINISHFENCEAPPLEPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLFINISHOBJECTAPPLEPROC) (GLenum object, GLint name);
typedef void (GLAPIENTRY * PFNGLGENFENCESAPPLEPROC) (GLsizei n, GLuint* fences);
typedef GLboolean (GLAPIENTRY * PFNGLISFENCEAPPLEPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLSETFENCEAPPLEPROC) (GLuint fence);
typedef GLboolean (GLAPIENTRY * PFNGLTESTFENCEAPPLEPROC) (GLuint fence);
typedef GLboolean (GLAPIENTRY * PFNGLTESTOBJECTAPPLEPROC) (GLenum object, GLuint name);

#define glDeleteFencesAPPLE GLEW_GET_FUN(G_glewDeleteFencesAPPLE)
#define glFinishFenceAPPLE GLEW_GET_FUN(G_glewFinishFenceAPPLE)
#define glFinishObjectAPPLE GLEW_GET_FUN(G_glewFinishObjectAPPLE)
#define glGenFencesAPPLE GLEW_GET_FUN(G_glewGenFencesAPPLE)
#define glIsFenceAPPLE GLEW_GET_FUN(G_glewIsFenceAPPLE)
#define glSetFenceAPPLE GLEW_GET_FUN(G_glewSetFenceAPPLE)
#define glTestFenceAPPLE GLEW_GET_FUN(G_glewTestFenceAPPLE)
#define glTestObjectAPPLE GLEW_GET_FUN(G_glewTestObjectAPPLE)

#define GLEW_APPLE_fence GLEW_GET_VAR(G_GLEW_APPLE_fence)

#endif /* GL_APPLE_fence */

/* ------------------------- GL_APPLE_float_pixels ------------------------- */

#ifndef GL_APPLE_float_pixels
#define GL_APPLE_float_pixels 1

#define GL_HALF_APPLE 0x140B
#define GL_RGBA_FLOAT32_APPLE 0x8814
#define GL_RGB_FLOAT32_APPLE 0x8815
#define GL_ALPHA_FLOAT32_APPLE 0x8816
#define GL_INTENSITY_FLOAT32_APPLE 0x8817
#define GL_LUMINANCE_FLOAT32_APPLE 0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_APPLE 0x8819
#define GL_RGBA_FLOAT16_APPLE 0x881A
#define GL_RGB_FLOAT16_APPLE 0x881B
#define GL_ALPHA_FLOAT16_APPLE 0x881C
#define GL_INTENSITY_FLOAT16_APPLE 0x881D
#define GL_LUMINANCE_FLOAT16_APPLE 0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_APPLE 0x881F
#define GL_COLOR_FLOAT_APPLE 0x8A0F

#define GLEW_APPLE_float_pixels GLEW_GET_VAR(G_GLEW_APPLE_float_pixels)

#endif /* GL_APPLE_float_pixels */

/* ---------------------- GL_APPLE_flush_buffer_range ---------------------- */

#ifndef GL_APPLE_flush_buffer_range
#define GL_APPLE_flush_buffer_range 1

#define GL_BUFFER_SERIALIZED_MODIFY_APPLE 0x8A12
#define GL_BUFFER_FLUSHING_UNMAP_APPLE 0x8A13

typedef void (GLAPIENTRY * PFNGLBUFFERPARAMETERIAPPLEPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC) (GLenum target, GLintptr offset, GLsizeiptr size);

#define glBufferParameteriAPPLE GLEW_GET_FUN(G_glewBufferParameteriAPPLE)
#define glFlushMappedBufferRangeAPPLE GLEW_GET_FUN(G_glewFlushMappedBufferRangeAPPLE)

#define GLEW_APPLE_flush_buffer_range GLEW_GET_VAR(G_GLEW_APPLE_flush_buffer_range)

#endif /* GL_APPLE_flush_buffer_range */

/* ----------------------- GL_APPLE_object_purgeable ----------------------- */

#ifndef GL_APPLE_object_purgeable
#define GL_APPLE_object_purgeable 1

#define GL_BUFFER_OBJECT_APPLE 0x85B3
#define GL_RELEASED_APPLE 0x8A19
#define GL_VOLATILE_APPLE 0x8A1A
#define GL_RETAINED_APPLE 0x8A1B
#define GL_UNDEFINED_APPLE 0x8A1C
#define GL_PURGEABLE_APPLE 0x8A1D

typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERIVAPPLEPROC) (GLenum objectType, GLuint name, GLenum pname, GLint* params);
typedef GLenum (GLAPIENTRY * PFNGLOBJECTPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);
typedef GLenum (GLAPIENTRY * PFNGLOBJECTUNPURGEABLEAPPLEPROC) (GLenum objectType, GLuint name, GLenum option);

#define glGetObjectParameterivAPPLE GLEW_GET_FUN(G_glewGetObjectParameterivAPPLE)
#define glObjectPurgeableAPPLE GLEW_GET_FUN(G_glewObjectPurgeableAPPLE)
#define glObjectUnpurgeableAPPLE GLEW_GET_FUN(G_glewObjectUnpurgeableAPPLE)

#define GLEW_APPLE_object_purgeable GLEW_GET_VAR(G_GLEW_APPLE_object_purgeable)

#endif /* GL_APPLE_object_purgeable */

/* ------------------------- GL_APPLE_pixel_buffer ------------------------- */

#ifndef GL_APPLE_pixel_buffer
#define GL_APPLE_pixel_buffer 1

#define GL_MIN_PBUFFER_VIEWPORT_DIMS_APPLE 0x8A10

#define GLEW_APPLE_pixel_buffer GLEW_GET_VAR(G_GLEW_APPLE_pixel_buffer)

#endif /* GL_APPLE_pixel_buffer */

/* ---------------------------- GL_APPLE_rgb_422 --------------------------- */

#ifndef GL_APPLE_rgb_422
#define GL_APPLE_rgb_422 1

#define GL_UNSIGNED_SHORT_8_8_APPLE 0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_APPLE 0x85BB
#define GL_RGB_422_APPLE 0x8A1F

#define GLEW_APPLE_rgb_422 GLEW_GET_VAR(G_GLEW_APPLE_rgb_422)

#endif /* GL_APPLE_rgb_422 */

/* --------------------------- GL_APPLE_row_bytes -------------------------- */

#ifndef GL_APPLE_row_bytes
#define GL_APPLE_row_bytes 1

#define GL_PACK_ROW_BYTES_APPLE 0x8A15
#define GL_UNPACK_ROW_BYTES_APPLE 0x8A16

#define GLEW_APPLE_row_bytes GLEW_GET_VAR(G_GLEW_APPLE_row_bytes)

#endif /* GL_APPLE_row_bytes */

/* ------------------------ GL_APPLE_specular_vector ----------------------- */

#ifndef GL_APPLE_specular_vector
#define GL_APPLE_specular_vector 1

#define GL_LIGHT_MODEL_SPECULAR_VECTOR_APPLE 0x85B0

#define GLEW_APPLE_specular_vector GLEW_GET_VAR(G_GLEW_APPLE_specular_vector)

#endif /* GL_APPLE_specular_vector */

/* ------------------------- GL_APPLE_texture_range ------------------------ */

#ifndef GL_APPLE_texture_range
#define GL_APPLE_texture_range 1

#define GL_TEXTURE_RANGE_LENGTH_APPLE 0x85B7
#define GL_TEXTURE_RANGE_POINTER_APPLE 0x85B8
#define GL_TEXTURE_STORAGE_HINT_APPLE 0x85BC
#define GL_STORAGE_PRIVATE_APPLE 0x85BD
#define GL_STORAGE_CACHED_APPLE 0x85BE
#define GL_STORAGE_SHARED_APPLE 0x85BF

typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC) (GLenum target, GLenum pname, GLvoid **params);
typedef void (GLAPIENTRY * PFNGLTEXTURERANGEAPPLEPROC) (GLenum target, GLsizei length, GLvoid *pointer);

#define glGetTexParameterPointervAPPLE GLEW_GET_FUN(G_glewGetTexParameterPointervAPPLE)
#define glTextureRangeAPPLE GLEW_GET_FUN(G_glewTextureRangeAPPLE)

#define GLEW_APPLE_texture_range GLEW_GET_VAR(G_GLEW_APPLE_texture_range)

#endif /* GL_APPLE_texture_range */

/* ------------------------ GL_APPLE_transform_hint ------------------------ */

#ifndef GL_APPLE_transform_hint
#define GL_APPLE_transform_hint 1

#define GL_TRANSFORM_HINT_APPLE 0x85B1

#define GLEW_APPLE_transform_hint GLEW_GET_VAR(G_GLEW_APPLE_transform_hint)

#endif /* GL_APPLE_transform_hint */

/* ---------------------- GL_APPLE_vertex_array_object --------------------- */

#ifndef GL_APPLE_vertex_array_object
#define GL_APPLE_vertex_array_object 1

#define GL_VERTEX_ARRAY_BINDING_APPLE 0x85B5

typedef void (GLAPIENTRY * PFNGLBINDVERTEXARRAYAPPLEPROC) (GLuint array);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXARRAYSAPPLEPROC) (GLsizei n, const GLuint* arrays);
typedef void (GLAPIENTRY * PFNGLGENVERTEXARRAYSAPPLEPROC) (GLsizei n, const GLuint* arrays);
typedef GLboolean (GLAPIENTRY * PFNGLISVERTEXARRAYAPPLEPROC) (GLuint array);

#define glBindVertexArrayAPPLE GLEW_GET_FUN(G_glewBindVertexArrayAPPLE)
#define glDeleteVertexArraysAPPLE GLEW_GET_FUN(G_glewDeleteVertexArraysAPPLE)
#define glGenVertexArraysAPPLE GLEW_GET_FUN(G_glewGenVertexArraysAPPLE)
#define glIsVertexArrayAPPLE GLEW_GET_FUN(G_glewIsVertexArrayAPPLE)

#define GLEW_APPLE_vertex_array_object GLEW_GET_VAR(G_GLEW_APPLE_vertex_array_object)

#endif /* GL_APPLE_vertex_array_object */

/* ---------------------- GL_APPLE_vertex_array_range ---------------------- */

#ifndef GL_APPLE_vertex_array_range
#define GL_APPLE_vertex_array_range 1

#define GL_VERTEX_ARRAY_RANGE_APPLE 0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE 0x851E
#define GL_VERTEX_ARRAY_STORAGE_HINT_APPLE 0x851F
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_APPLE 0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_APPLE 0x8521
#define GL_STORAGE_CLIENT_APPLE 0x85B4
#define GL_STORAGE_CACHED_APPLE 0x85BE
#define GL_STORAGE_SHARED_APPLE 0x85BF

typedef void (GLAPIENTRY * PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC) (GLsizei length, void* pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYPARAMETERIAPPLEPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYRANGEAPPLEPROC) (GLsizei length, void* pointer);

#define glFlushVertexArrayRangeAPPLE GLEW_GET_FUN(G_glewFlushVertexArrayRangeAPPLE)
#define glVertexArrayParameteriAPPLE GLEW_GET_FUN(G_glewVertexArrayParameteriAPPLE)
#define glVertexArrayRangeAPPLE GLEW_GET_FUN(G_glewVertexArrayRangeAPPLE)

#define GLEW_APPLE_vertex_array_range GLEW_GET_VAR(G_GLEW_APPLE_vertex_array_range)

#endif /* GL_APPLE_vertex_array_range */

/* ------------------- GL_APPLE_vertex_program_evaluators ------------------ */

#ifndef GL_APPLE_vertex_program_evaluators
#define GL_APPLE_vertex_program_evaluators 1

#define GL_VERTEX_ATTRIB_MAP1_APPLE 0x8A00
#define GL_VERTEX_ATTRIB_MAP2_APPLE 0x8A01
#define GL_VERTEX_ATTRIB_MAP1_SIZE_APPLE 0x8A02
#define GL_VERTEX_ATTRIB_MAP1_COEFF_APPLE 0x8A03
#define GL_VERTEX_ATTRIB_MAP1_ORDER_APPLE 0x8A04
#define GL_VERTEX_ATTRIB_MAP1_DOMAIN_APPLE 0x8A05
#define GL_VERTEX_ATTRIB_MAP2_SIZE_APPLE 0x8A06
#define GL_VERTEX_ATTRIB_MAP2_COEFF_APPLE 0x8A07
#define GL_VERTEX_ATTRIB_MAP2_ORDER_APPLE 0x8A08
#define GL_VERTEX_ATTRIB_MAP2_DOMAIN_APPLE 0x8A09

typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBAPPLEPROC) (GLuint index, GLenum pname);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBAPPLEPROC) (GLuint index, GLenum pname);
typedef GLboolean (GLAPIENTRY * PFNGLISVERTEXATTRIBENABLEDAPPLEPROC) (GLuint index, GLenum pname);
typedef void (GLAPIENTRY * PFNGLMAPVERTEXATTRIB1DAPPLEPROC) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble* points);
typedef void (GLAPIENTRY * PFNGLMAPVERTEXATTRIB1FAPPLEPROC) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat* points);
typedef void (GLAPIENTRY * PFNGLMAPVERTEXATTRIB2DAPPLEPROC) (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble* points);
typedef void (GLAPIENTRY * PFNGLMAPVERTEXATTRIB2FAPPLEPROC) (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat* points);

#define glDisableVertexAttribAPPLE GLEW_GET_FUN(G_glewDisableVertexAttribAPPLE)
#define glEnableVertexAttribAPPLE GLEW_GET_FUN(G_glewEnableVertexAttribAPPLE)
#define glIsVertexAttribEnabledAPPLE GLEW_GET_FUN(G_glewIsVertexAttribEnabledAPPLE)
#define glMapVertexAttrib1dAPPLE GLEW_GET_FUN(G_glewMapVertexAttrib1dAPPLE)
#define glMapVertexAttrib1fAPPLE GLEW_GET_FUN(G_glewMapVertexAttrib1fAPPLE)
#define glMapVertexAttrib2dAPPLE GLEW_GET_FUN(G_glewMapVertexAttrib2dAPPLE)
#define glMapVertexAttrib2fAPPLE GLEW_GET_FUN(G_glewMapVertexAttrib2fAPPLE)

#define GLEW_APPLE_vertex_program_evaluators GLEW_GET_VAR(G_GLEW_APPLE_vertex_program_evaluators)

#endif /* GL_APPLE_vertex_program_evaluators */

/* --------------------------- GL_APPLE_ycbcr_422 -------------------------- */

#ifndef GL_APPLE_ycbcr_422
#define GL_APPLE_ycbcr_422 1

#define GL_YCBCR_422_APPLE 0x85B9

#define GLEW_APPLE_ycbcr_422 GLEW_GET_VAR(G_GLEW_APPLE_ycbcr_422)

#endif /* GL_APPLE_ycbcr_422 */

/* ------------------------ GL_ARB_ES2_compatibility ----------------------- */

#ifndef GL_ARB_ES2_compatibility
#define GL_ARB_ES2_compatibility 1

#define GL_FIXED 0x140C
#define GL_IMPLEMENTATION_COLOR_READ_TYPE 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT 0x8B9B
#define GL_RGB565 0x8D62
#define GL_LOW_FLOAT 0x8DF0
#define GL_MEDIUM_FLOAT 0x8DF1
#define GL_HIGH_FLOAT 0x8DF2
#define GL_LOW_INT 0x8DF3
#define GL_MEDIUM_INT 0x8DF4
#define GL_HIGH_INT 0x8DF5
#define GL_SHADER_BINARY_FORMATS 0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS 0x8DF9
#define GL_SHADER_COMPILER 0x8DFA
#define GL_MAX_VERTEX_UNIFORM_VECTORS 0x8DFB
#define GL_MAX_VARYING_VECTORS 0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS 0x8DFD

typedef void (GLAPIENTRY * PFNGLCLEARDEPTHFPROC) (GLclampf d);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEFPROC) (GLclampf n, GLclampf f);
typedef void (GLAPIENTRY * PFNGLGETSHADERPRECISIONFORMATPROC) (GLenum shadertype, GLenum precisiontype, GLint* range, GLint *precision);
typedef void (GLAPIENTRY * PFNGLRELEASESHADERCOMPILERPROC) (void);
typedef void (GLAPIENTRY * PFNGLSHADERBINARYPROC) (GLsizei count, const GLuint* shaders, GLenum binaryformat, const GLvoid*binary, GLsizei length);

#define glClearDepthf GLEW_GET_FUN(G_glewClearDepthf)
#define glDepthRangef GLEW_GET_FUN(G_glewDepthRangef)
#define glGetShaderPrecisionFormat GLEW_GET_FUN(G_glewGetShaderPrecisionFormat)
#define glReleaseShaderCompiler GLEW_GET_FUN(G_glewReleaseShaderCompiler)
#define glShaderBinary GLEW_GET_FUN(G_glewShaderBinary)

#define GLEW_ARB_ES2_compatibility GLEW_GET_VAR(G_GLEW_ARB_ES2_compatibility)

#endif /* GL_ARB_ES2_compatibility */

/* ------------------------ GL_ARB_ES3_compatibility ----------------------- */

#ifndef GL_ARB_ES3_compatibility
#define GL_ARB_ES3_compatibility 1

#define GL_PRIMITIVE_RESTART_FIXED_INDEX 0x8D69
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE 0x8D6A
#define GL_MAX_ELEMENT_INDEX 0x8D6B
#define GL_COMPRESSED_R11_EAC 0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC 0x9271
#define GL_COMPRESSED_RG11_EAC 0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC 0x9273
#define GL_COMPRESSED_RGB8_ETC2 0x9274
#define GL_COMPRESSED_SRGB8_ETC2 0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2 0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC 0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC 0x9279

#define GLEW_ARB_ES3_compatibility GLEW_GET_VAR(G_GLEW_ARB_ES3_compatibility)

#endif /* GL_ARB_ES3_compatibility */

/* ------------------------ GL_ARB_arrays_of_arrays ------------------------ */

#ifndef GL_ARB_arrays_of_arrays
#define GL_ARB_arrays_of_arrays 1

#define GLEW_ARB_arrays_of_arrays GLEW_GET_VAR(G_GLEW_ARB_arrays_of_arrays)

#endif /* GL_ARB_arrays_of_arrays */

/* -------------------------- GL_ARB_base_instance ------------------------- */

#ifndef GL_ARB_base_instance
#define GL_ARB_base_instance 1

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount, GLuint baseinstance);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLuint baseinstance);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex, GLuint baseinstance);

#define glDrawArraysInstancedBaseInstance GLEW_GET_FUN(G_glewDrawArraysInstancedBaseInstance)
#define glDrawElementsInstancedBaseInstance GLEW_GET_FUN(G_glewDrawElementsInstancedBaseInstance)
#define glDrawElementsInstancedBaseVertexBaseInstance GLEW_GET_FUN(G_glewDrawElementsInstancedBaseVertexBaseInstance)

#define GLEW_ARB_base_instance GLEW_GET_VAR(G_GLEW_ARB_base_instance)

#endif /* GL_ARB_base_instance */

/* ----------------------- GL_ARB_blend_func_extended ---------------------- */

#ifndef GL_ARB_blend_func_extended
#define GL_ARB_blend_func_extended 1

#define GL_SRC1_COLOR 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR 0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA 0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS 0x88FC

typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) (GLuint program, GLuint colorNumber, GLuint index, const GLchar * name);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATAINDEXPROC) (GLuint program, const GLchar * name);

#define glBindFragDataLocationIndexed GLEW_GET_FUN(G_glewBindFragDataLocationIndexed)
#define glGetFragDataIndex GLEW_GET_FUN(G_glewGetFragDataIndex)

#define GLEW_ARB_blend_func_extended GLEW_GET_VAR(G_GLEW_ARB_blend_func_extended)

#endif /* GL_ARB_blend_func_extended */

/* ---------------------------- GL_ARB_cl_event ---------------------------- */

#ifndef GL_ARB_cl_event
#define GL_ARB_cl_event 1

#define GL_SYNC_CL_EVENT_ARB 0x8240
#define GL_SYNC_CL_EVENT_COMPLETE_ARB 0x8241

typedef struct _cl_context *cl_context;
typedef struct _cl_event *cl_event;

typedef GLsync (GLAPIENTRY * PFNGLCREATESYNCFROMCLEVENTARBPROC) (cl_context context, cl_event event, GLbitfield flags);

#define glCreateSyncFromCLeventARB GLEW_GET_FUN(G_glewCreateSyncFromCLeventARB)

#define GLEW_ARB_cl_event GLEW_GET_VAR(G_GLEW_ARB_cl_event)

#endif /* GL_ARB_cl_event */

/* ----------------------- GL_ARB_clear_buffer_object ---------------------- */

#ifndef GL_ARB_clear_buffer_object
#define GL_ARB_clear_buffer_object 1

typedef void (GLAPIENTRY * PFNGLCLEARBUFFERDATAPROC) (GLenum target, GLenum internalformat, GLenum format, GLenum type, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLCLEARBUFFERSUBDATAPROC) (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const GLvoid* data);

#define glClearBufferData GLEW_GET_FUN(G_glewClearBufferData)
#define glClearBufferSubData GLEW_GET_FUN(G_glewClearBufferSubData)
#define glClearNamedBufferDataEXT GLEW_GET_FUN(G_glewClearNamedBufferDataEXT)
#define glClearNamedBufferSubDataEXT GLEW_GET_FUN(G_glewClearNamedBufferSubDataEXT)

#define GLEW_ARB_clear_buffer_object GLEW_GET_VAR(G_GLEW_ARB_clear_buffer_object)

#endif /* GL_ARB_clear_buffer_object */

/* ----------------------- GL_ARB_color_buffer_float ----------------------- */

#ifndef GL_ARB_color_buffer_float
#define GL_ARB_color_buffer_float 1

#define GL_RGBA_FLOAT_MODE_ARB 0x8820
#define GL_CLAMP_VERTEX_COLOR_ARB 0x891A
#define GL_CLAMP_FRAGMENT_COLOR_ARB 0x891B
#define GL_CLAMP_READ_COLOR_ARB 0x891C
#define GL_FIXED_ONLY_ARB 0x891D

typedef void (GLAPIENTRY * PFNGLCLAMPCOLORARBPROC) (GLenum target, GLenum clamp);

#define glClampColorARB GLEW_GET_FUN(G_glewClampColorARB)

#define GLEW_ARB_color_buffer_float GLEW_GET_VAR(G_GLEW_ARB_color_buffer_float)

#endif /* GL_ARB_color_buffer_float */

/* -------------------------- GL_ARB_compatibility ------------------------- */

#ifndef GL_ARB_compatibility
#define GL_ARB_compatibility 1

#define GLEW_ARB_compatibility GLEW_GET_VAR(G_GLEW_ARB_compatibility)

#endif /* GL_ARB_compatibility */

/* ---------------- GL_ARB_compressed_texture_pixel_storage ---------------- */

#ifndef GL_ARB_compressed_texture_pixel_storage
#define GL_ARB_compressed_texture_pixel_storage 1

#define GL_UNPACK_COMPRESSED_BLOCK_WIDTH 0x9127
#define GL_UNPACK_COMPRESSED_BLOCK_HEIGHT 0x9128
#define GL_UNPACK_COMPRESSED_BLOCK_DEPTH 0x9129
#define GL_UNPACK_COMPRESSED_BLOCK_SIZE 0x912A
#define GL_PACK_COMPRESSED_BLOCK_WIDTH 0x912B
#define GL_PACK_COMPRESSED_BLOCK_HEIGHT 0x912C
#define GL_PACK_COMPRESSED_BLOCK_DEPTH 0x912D
#define GL_PACK_COMPRESSED_BLOCK_SIZE 0x912E

#define GLEW_ARB_compressed_texture_pixel_storage GLEW_GET_VAR(G_GLEW_ARB_compressed_texture_pixel_storage)

#endif /* GL_ARB_compressed_texture_pixel_storage */

/* ------------------------- GL_ARB_compute_shader ------------------------- */

#ifndef GL_ARB_compute_shader
#define GL_ARB_compute_shader 1

#define GL_COMPUTE_SHADER_BIT 0x00000020
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE 0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS 0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS 0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS 0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS 0x8266
#define GL_COMPUTE_WORK_GROUP_SIZE 0x8267
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS 0x90EB
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER 0x90EC
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER 0x90ED
#define GL_DISPATCH_INDIRECT_BUFFER 0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING 0x90EF
#define GL_COMPUTE_SHADER 0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS 0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS 0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS 0x91BD
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT 0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE 0x91BF

typedef void (GLAPIENTRY * PFNGLDISPATCHCOMPUTEPROC) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void (GLAPIENTRY * PFNGLDISPATCHCOMPUTEINDIRECTPROC) (GLintptr indirect);

#define glDispatchCompute GLEW_GET_FUN(G_glewDispatchCompute)
#define glDispatchComputeIndirect GLEW_GET_FUN(G_glewDispatchComputeIndirect)

#define GLEW_ARB_compute_shader GLEW_GET_VAR(G_GLEW_ARB_compute_shader)

#endif /* GL_ARB_compute_shader */

/* ----------------------- GL_ARB_conservative_depth ----------------------- */

#ifndef GL_ARB_conservative_depth
#define GL_ARB_conservative_depth 1

#define GLEW_ARB_conservative_depth GLEW_GET_VAR(G_GLEW_ARB_conservative_depth)

#endif /* GL_ARB_conservative_depth */

/* --------------------------- GL_ARB_copy_buffer -------------------------- */

#ifndef GL_ARB_copy_buffer
#define GL_ARB_copy_buffer 1

#define GL_COPY_READ_BUFFER 0x8F36
#define GL_COPY_WRITE_BUFFER 0x8F37

typedef void (GLAPIENTRY * PFNGLCOPYBUFFERSUBDATAPROC) (GLenum readtarget, GLenum writetarget, GLintptr readoffset, GLintptr writeoffset, GLsizeiptr size);

#define glCopyBufferSubData GLEW_GET_FUN(G_glewCopyBufferSubData)

#define GLEW_ARB_copy_buffer GLEW_GET_VAR(G_GLEW_ARB_copy_buffer)

#endif /* GL_ARB_copy_buffer */

/* --------------------------- GL_ARB_copy_image --------------------------- */

#ifndef GL_ARB_copy_image
#define GL_ARB_copy_image 1

typedef void (GLAPIENTRY * PFNGLCOPYIMAGESUBDATAPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);

#define glCopyImageSubData GLEW_GET_FUN(G_glewCopyImageSubData)

#define GLEW_ARB_copy_image GLEW_GET_VAR(G_GLEW_ARB_copy_image)

#endif /* GL_ARB_copy_image */

/* -------------------------- GL_ARB_debug_output -------------------------- */

#ifndef GL_ARB_debug_output
#define GL_ARB_debug_output 1

#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION_ARB 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM_ARB 0x8245
#define GL_DEBUG_SOURCE_API_ARB 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY_ARB 0x8249
#define GL_DEBUG_SOURCE_APPLICATION_ARB 0x824A
#define GL_DEBUG_SOURCE_OTHER_ARB 0x824B
#define GL_DEBUG_TYPE_ERROR_ARB 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB 0x824E
#define GL_DEBUG_TYPE_PORTABILITY_ARB 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE_ARB 0x8250
#define GL_DEBUG_TYPE_OTHER_ARB 0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH_ARB 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_ARB 0x9144
#define GL_DEBUG_LOGGED_MESSAGES_ARB 0x9145
#define GL_DEBUG_SEVERITY_HIGH_ARB 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_ARB 0x9147
#define GL_DEBUG_SEVERITY_LOW_ARB 0x9148

typedef void (APIENTRY *GLDEBUGPROCARB)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECALLBACKARBPROC) (GLDEBUGPROCARB callback, void* userParam);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECONTROLARBPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEINSERTARBPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
typedef GLuint (GLAPIENTRY * PFNGLGETDEBUGMESSAGELOGARBPROC) (GLuint count, GLsizei bufsize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);

#define glDebugMessageCallbackARB GLEW_GET_FUN(G_glewDebugMessageCallbackARB)
#define glDebugMessageControlARB GLEW_GET_FUN(G_glewDebugMessageControlARB)
#define glDebugMessageInsertARB GLEW_GET_FUN(G_glewDebugMessageInsertARB)
#define glGetDebugMessageLogARB GLEW_GET_FUN(G_glewGetDebugMessageLogARB)

#define GLEW_ARB_debug_output GLEW_GET_VAR(G_GLEW_ARB_debug_output)

#endif /* GL_ARB_debug_output */

/* ----------------------- GL_ARB_depth_buffer_float ----------------------- */

#ifndef GL_ARB_depth_buffer_float
#define GL_ARB_depth_buffer_float 1

#define GL_DEPTH_COMPONENT32F 0x8CAC
#define GL_DEPTH32F_STENCIL8 0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD

#define GLEW_ARB_depth_buffer_float GLEW_GET_VAR(G_GLEW_ARB_depth_buffer_float)

#endif /* GL_ARB_depth_buffer_float */

/* --------------------------- GL_ARB_depth_clamp -------------------------- */

#ifndef GL_ARB_depth_clamp
#define GL_ARB_depth_clamp 1

#define GL_DEPTH_CLAMP 0x864F

#define GLEW_ARB_depth_clamp GLEW_GET_VAR(G_GLEW_ARB_depth_clamp)

#endif /* GL_ARB_depth_clamp */

/* -------------------------- GL_ARB_depth_texture ------------------------- */

#ifndef GL_ARB_depth_texture
#define GL_ARB_depth_texture 1

#define GL_DEPTH_COMPONENT16_ARB 0x81A5
#define GL_DEPTH_COMPONENT24_ARB 0x81A6
#define GL_DEPTH_COMPONENT32_ARB 0x81A7
#define GL_TEXTURE_DEPTH_SIZE_ARB 0x884A
#define GL_DEPTH_TEXTURE_MODE_ARB 0x884B

#define GLEW_ARB_depth_texture GLEW_GET_VAR(G_GLEW_ARB_depth_texture)

#endif /* GL_ARB_depth_texture */

/* -------------------------- GL_ARB_draw_buffers -------------------------- */

#ifndef GL_ARB_draw_buffers
#define GL_ARB_draw_buffers 1

#define GL_MAX_DRAW_BUFFERS_ARB 0x8824
#define GL_DRAW_BUFFER0_ARB 0x8825
#define GL_DRAW_BUFFER1_ARB 0x8826
#define GL_DRAW_BUFFER2_ARB 0x8827
#define GL_DRAW_BUFFER3_ARB 0x8828
#define GL_DRAW_BUFFER4_ARB 0x8829
#define GL_DRAW_BUFFER5_ARB 0x882A
#define GL_DRAW_BUFFER6_ARB 0x882B
#define GL_DRAW_BUFFER7_ARB 0x882C
#define GL_DRAW_BUFFER8_ARB 0x882D
#define GL_DRAW_BUFFER9_ARB 0x882E
#define GL_DRAW_BUFFER10_ARB 0x882F
#define GL_DRAW_BUFFER11_ARB 0x8830
#define GL_DRAW_BUFFER12_ARB 0x8831
#define GL_DRAW_BUFFER13_ARB 0x8832
#define GL_DRAW_BUFFER14_ARB 0x8833
#define GL_DRAW_BUFFER15_ARB 0x8834

typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSARBPROC) (GLsizei n, const GLenum* bufs);

#define glDrawBuffersARB GLEW_GET_FUN(G_glewDrawBuffersARB)

#define GLEW_ARB_draw_buffers GLEW_GET_VAR(G_GLEW_ARB_draw_buffers)

#endif /* GL_ARB_draw_buffers */

/* ----------------------- GL_ARB_draw_buffers_blend ----------------------- */

#ifndef GL_ARB_draw_buffers_blend
#define GL_ARB_draw_buffers_blend 1

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEIARBPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONIARBPROC) (GLuint buf, GLenum mode);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEIARBPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void (GLAPIENTRY * PFNGLBLENDFUNCIARBPROC) (GLuint buf, GLenum src, GLenum dst);

#define glBlendEquationSeparateiARB GLEW_GET_FUN(G_glewBlendEquationSeparateiARB)
#define glBlendEquationiARB GLEW_GET_FUN(G_glewBlendEquationiARB)
#define glBlendFuncSeparateiARB GLEW_GET_FUN(G_glewBlendFuncSeparateiARB)
#define glBlendFunciARB GLEW_GET_FUN(G_glewBlendFunciARB)

#define GLEW_ARB_draw_buffers_blend GLEW_GET_VAR(G_GLEW_ARB_draw_buffers_blend)

#endif /* GL_ARB_draw_buffers_blend */

/* -------------------- GL_ARB_draw_elements_base_vertex ------------------- */

#ifndef GL_ARB_draw_elements_base_vertex
#define GL_ARB_draw_elements_base_vertex 1

typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, void* indices, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, void* indices, GLint basevertex);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC) (GLenum mode, GLsizei* count, GLenum type, GLvoid**indices, GLsizei primcount, GLint *basevertex);

#define glDrawElementsBaseVertex GLEW_GET_FUN(G_glewDrawElementsBaseVertex)
#define glDrawElementsInstancedBaseVertex GLEW_GET_FUN(G_glewDrawElementsInstancedBaseVertex)
#define glDrawRangeElementsBaseVertex GLEW_GET_FUN(G_glewDrawRangeElementsBaseVertex)
#define glMultiDrawElementsBaseVertex GLEW_GET_FUN(G_glewMultiDrawElementsBaseVertex)

#define GLEW_ARB_draw_elements_base_vertex GLEW_GET_VAR(G_GLEW_ARB_draw_elements_base_vertex)

#endif /* GL_ARB_draw_elements_base_vertex */

/* -------------------------- GL_ARB_draw_indirect ------------------------- */

#ifndef GL_ARB_draw_indirect
#define GL_ARB_draw_indirect 1

#define GL_DRAW_INDIRECT_BUFFER 0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING 0x8F43

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINDIRECTPROC) (GLenum mode, const void* indirect);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void* indirect);

#define glDrawArraysIndirect GLEW_GET_FUN(G_glewDrawArraysIndirect)
#define glDrawElementsIndirect GLEW_GET_FUN(G_glewDrawElementsIndirect)

#define GLEW_ARB_draw_indirect GLEW_GET_VAR(G_GLEW_ARB_draw_indirect)

#endif /* GL_ARB_draw_indirect */

/* ------------------------- GL_ARB_draw_instanced ------------------------- */

#ifndef GL_ARB_draw_instanced
#define GL_ARB_draw_instanced 1

#define GLEW_ARB_draw_instanced GLEW_GET_VAR(G_GLEW_ARB_draw_instanced)

#endif /* GL_ARB_draw_instanced */

/* -------------------- GL_ARB_explicit_attrib_location -------------------- */

#ifndef GL_ARB_explicit_attrib_location
#define GL_ARB_explicit_attrib_location 1

#define GLEW_ARB_explicit_attrib_location GLEW_GET_VAR(G_GLEW_ARB_explicit_attrib_location)

#endif /* GL_ARB_explicit_attrib_location */

/* -------------------- GL_ARB_explicit_uniform_location ------------------- */

#ifndef GL_ARB_explicit_uniform_location
#define GL_ARB_explicit_uniform_location 1

#define GL_MAX_UNIFORM_LOCATIONS 0x826E

#define GLEW_ARB_explicit_uniform_location GLEW_GET_VAR(G_GLEW_ARB_explicit_uniform_location)

#endif /* GL_ARB_explicit_uniform_location */

/* ------------------- GL_ARB_fragment_coord_conventions ------------------- */

#ifndef GL_ARB_fragment_coord_conventions
#define GL_ARB_fragment_coord_conventions 1

#define GLEW_ARB_fragment_coord_conventions GLEW_GET_VAR(G_GLEW_ARB_fragment_coord_conventions)

#endif /* GL_ARB_fragment_coord_conventions */

/* --------------------- GL_ARB_fragment_layer_viewport -------------------- */

#ifndef GL_ARB_fragment_layer_viewport
#define GL_ARB_fragment_layer_viewport 1

#define GLEW_ARB_fragment_layer_viewport GLEW_GET_VAR(G_GLEW_ARB_fragment_layer_viewport)

#endif /* GL_ARB_fragment_layer_viewport */

/* ------------------------ GL_ARB_fragment_program ------------------------ */

#ifndef GL_ARB_fragment_program
#define GL_ARB_fragment_program 1

#define GL_FRAGMENT_PROGRAM_ARB 0x8804
#define GL_PROGRAM_ALU_INSTRUCTIONS_ARB 0x8805
#define GL_PROGRAM_TEX_INSTRUCTIONS_ARB 0x8806
#define GL_PROGRAM_TEX_INDIRECTIONS_ARB 0x8807
#define GL_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x8808
#define GL_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x8809
#define GL_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x880A
#define GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB 0x880B
#define GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB 0x880C
#define GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB 0x880D
#define GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB 0x880E
#define GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB 0x880F
#define GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB 0x8810
#define GL_MAX_TEXTURE_COORDS_ARB 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB 0x8872

#define GLEW_ARB_fragment_program GLEW_GET_VAR(G_GLEW_ARB_fragment_program)

#endif /* GL_ARB_fragment_program */

/* --------------------- GL_ARB_fragment_program_shadow -------------------- */

#ifndef GL_ARB_fragment_program_shadow
#define GL_ARB_fragment_program_shadow 1

#define GLEW_ARB_fragment_program_shadow GLEW_GET_VAR(G_GLEW_ARB_fragment_program_shadow)

#endif /* GL_ARB_fragment_program_shadow */

/* ------------------------- GL_ARB_fragment_shader ------------------------ */

#ifndef GL_ARB_fragment_shader
#define GL_ARB_fragment_shader 1

#define GL_FRAGMENT_SHADER_ARB 0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB 0x8B49
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB 0x8B8B

#define GLEW_ARB_fragment_shader GLEW_GET_VAR(G_GLEW_ARB_fragment_shader)

#endif /* GL_ARB_fragment_shader */

/* ------------------- GL_ARB_framebuffer_no_attachments ------------------- */

#ifndef GL_ARB_framebuffer_no_attachments
#define GL_ARB_framebuffer_no_attachments 1

#define GL_FRAMEBUFFER_DEFAULT_WIDTH 0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT 0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS 0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES 0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH 0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT 0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS 0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES 0x9318

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERPARAMETERIEXTPROC) (GLuint framebuffer, GLenum pname, GLint param);

#define glFramebufferParameteri GLEW_GET_FUN(G_glewFramebufferParameteri)
#define glGetFramebufferParameteriv GLEW_GET_FUN(G_glewGetFramebufferParameteriv)
#define glGetNamedFramebufferParameterivEXT GLEW_GET_FUN(G_glewGetNamedFramebufferParameterivEXT)
#define glNamedFramebufferParameteriEXT GLEW_GET_FUN(G_glewNamedFramebufferParameteriEXT)

#define GLEW_ARB_framebuffer_no_attachments GLEW_GET_VAR(G_GLEW_ARB_framebuffer_no_attachments)

#endif /* GL_ARB_framebuffer_no_attachments */

/* ----------------------- GL_ARB_framebuffer_object ----------------------- */

#ifndef GL_ARB_framebuffer_object
#define GL_ARB_framebuffer_object 1

#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT 0x8218
#define GL_FRAMEBUFFER_UNDEFINED 0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT 0x821A
#define GL_INDEX 0x8222
#define GL_MAX_RENDERBUFFER_SIZE 0x84E8
#define GL_DEPTH_STENCIL 0x84F9
#define GL_UNSIGNED_INT_24_8 0x84FA
#define GL_DEPTH24_STENCIL8 0x88F0
#define GL_TEXTURE_STENCIL_SIZE 0x88F1
#define GL_UNSIGNED_NORMALIZED 0x8C17
#define GL_SRGB 0x8C40
#define GL_DRAW_FRAMEBUFFER_BINDING 0x8CA6
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_RENDERBUFFER_BINDING 0x8CA7
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING 0x8CAA
#define GL_RENDERBUFFER_SAMPLES 0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS 0x8CDF
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_COLOR_ATTACHMENT4 0x8CE4
#define GL_COLOR_ATTACHMENT5 0x8CE5
#define GL_COLOR_ATTACHMENT6 0x8CE6
#define GL_COLOR_ATTACHMENT7 0x8CE7
#define GL_COLOR_ATTACHMENT8 0x8CE8
#define GL_COLOR_ATTACHMENT9 0x8CE9
#define GL_COLOR_ATTACHMENT10 0x8CEA
#define GL_COLOR_ATTACHMENT11 0x8CEB
#define GL_COLOR_ATTACHMENT12 0x8CEC
#define GL_COLOR_ATTACHMENT13 0x8CED
#define GL_COLOR_ATTACHMENT14 0x8CEE
#define GL_COLOR_ATTACHMENT15 0x8CEF
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_FRAMEBUFFER 0x8D40
#define GL_RENDERBUFFER 0x8D41
#define GL_RENDERBUFFER_WIDTH 0x8D42
#define GL_RENDERBUFFER_HEIGHT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT 0x8D44
#define GL_STENCIL_INDEX1 0x8D46
#define GL_STENCIL_INDEX4 0x8D47
#define GL_STENCIL_INDEX8 0x8D48
#define GL_STENCIL_INDEX16 0x8D49
#define GL_RENDERBUFFER_RED_SIZE 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE 0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES 0x8D57

typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFERPROC) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFERPROC) (GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFERPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFERPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE1DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE3DPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERPROC) (GLenum target,GLenum attachment, GLuint texture,GLint level,GLint layer);
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSPROC) (GLsizei n, GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISFRAMEBUFFERPROC) (GLuint framebuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISRENDERBUFFERPROC) (GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

#define glBindFramebuffer GLEW_GET_FUN(G_glewBindFramebuffer)
#define glBindRenderbuffer GLEW_GET_FUN(G_glewBindRenderbuffer)
#define glBlitFramebuffer GLEW_GET_FUN(G_glewBlitFramebuffer)
#define glCheckFramebufferStatus GLEW_GET_FUN(G_glewCheckFramebufferStatus)
#define glDeleteFramebuffers GLEW_GET_FUN(G_glewDeleteFramebuffers)
#define glDeleteRenderbuffers GLEW_GET_FUN(G_glewDeleteRenderbuffers)
#define glFramebufferRenderbuffer GLEW_GET_FUN(G_glewFramebufferRenderbuffer)
#define glFramebufferTexture1D GLEW_GET_FUN(G_glewFramebufferTexture1D)
#define glFramebufferTexture2D GLEW_GET_FUN(G_glewFramebufferTexture2D)
#define glFramebufferTexture3D GLEW_GET_FUN(G_glewFramebufferTexture3D)
#define glFramebufferTextureLayer GLEW_GET_FUN(G_glewFramebufferTextureLayer)
#define glGenFramebuffers GLEW_GET_FUN(G_glewGenFramebuffers)
#define glGenRenderbuffers GLEW_GET_FUN(G_glewGenRenderbuffers)
#define glGenerateMipmap GLEW_GET_FUN(G_glewGenerateMipmap)
#define glGetFramebufferAttachmentParameteriv GLEW_GET_FUN(G_glewGetFramebufferAttachmentParameteriv)
#define glGetRenderbufferParameteriv GLEW_GET_FUN(G_glewGetRenderbufferParameteriv)
#define glIsFramebuffer GLEW_GET_FUN(G_glewIsFramebuffer)
#define glIsRenderbuffer GLEW_GET_FUN(G_glewIsRenderbuffer)
#define glRenderbufferStorage GLEW_GET_FUN(G_glewRenderbufferStorage)
#define glRenderbufferStorageMultisample GLEW_GET_FUN(G_glewRenderbufferStorageMultisample)

#define GLEW_ARB_framebuffer_object GLEW_GET_VAR(G_GLEW_ARB_framebuffer_object)

#endif /* GL_ARB_framebuffer_object */

/* ------------------------ GL_ARB_framebuffer_sRGB ------------------------ */

#ifndef GL_ARB_framebuffer_sRGB
#define GL_ARB_framebuffer_sRGB 1

#define GL_FRAMEBUFFER_SRGB 0x8DB9

#define GLEW_ARB_framebuffer_sRGB GLEW_GET_VAR(G_GLEW_ARB_framebuffer_sRGB)

#endif /* GL_ARB_framebuffer_sRGB */

/* ------------------------ GL_ARB_geometry_shader4 ------------------------ */

#ifndef GL_ARB_geometry_shader4
#define GL_ARB_geometry_shader4 1

#define GL_LINES_ADJACENCY_ARB 0xA
#define GL_LINE_STRIP_ADJACENCY_ARB 0xB
#define GL_TRIANGLES_ADJACENCY_ARB 0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_ARB 0xD
#define GL_PROGRAM_POINT_SIZE_ARB 0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_ARB 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_ARB 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_ARB 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_ARB 0x8DA9
#define GL_GEOMETRY_SHADER_ARB 0x8DD9
#define GL_GEOMETRY_VERTICES_OUT_ARB 0x8DDA
#define GL_GEOMETRY_INPUT_TYPE_ARB 0x8DDB
#define GL_GEOMETRY_OUTPUT_TYPE_ARB 0x8DDC
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS_ARB 0x8DDD
#define GL_MAX_VERTEX_VARYING_COMPONENTS_ARB 0x8DDE
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_ARB 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_ARB 0x8DE1

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREFACEARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYERARBPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIARBPROC) (GLuint program, GLenum pname, GLint value);

#define glFramebufferTextureARB GLEW_GET_FUN(G_glewFramebufferTextureARB)
#define glFramebufferTextureFaceARB GLEW_GET_FUN(G_glewFramebufferTextureFaceARB)
#define glFramebufferTextureLayerARB GLEW_GET_FUN(G_glewFramebufferTextureLayerARB)
#define glProgramParameteriARB GLEW_GET_FUN(G_glewProgramParameteriARB)

#define GLEW_ARB_geometry_shader4 GLEW_GET_VAR(G_GLEW_ARB_geometry_shader4)

#endif /* GL_ARB_geometry_shader4 */

/* ----------------------- GL_ARB_get_program_binary ----------------------- */

#ifndef GL_ARB_get_program_binary
#define GL_ARB_get_program_binary 1

#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT 0x8257
#define GL_PROGRAM_BINARY_LENGTH 0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS 0x87FE
#define GL_PROGRAM_BINARY_FORMATS 0x87FF

typedef void (GLAPIENTRY * PFNGLGETPROGRAMBINARYPROC) (GLuint program, GLsizei bufSize, GLsizei* length, GLenum *binaryFormat, GLvoid*binary);
typedef void (GLAPIENTRY * PFNGLPROGRAMBINARYPROC) (GLuint program, GLenum binaryFormat, const void* binary, GLsizei length);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);

#define glGetProgramBinary GLEW_GET_FUN(G_glewGetProgramBinary)
#define glProgramBinary GLEW_GET_FUN(G_glewProgramBinary)
#define glProgramParameteri GLEW_GET_FUN(G_glewProgramParameteri)

#define GLEW_ARB_get_program_binary GLEW_GET_VAR(G_GLEW_ARB_get_program_binary)

#endif /* GL_ARB_get_program_binary */

/* --------------------------- GL_ARB_gpu_shader5 -------------------------- */

#ifndef GL_ARB_gpu_shader5
#define GL_ARB_gpu_shader5 1

#define GL_GEOMETRY_SHADER_INVOCATIONS 0x887F
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET 0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS 0x8E5D
#define GL_MAX_VERTEX_STREAMS 0x8E71

#define GLEW_ARB_gpu_shader5 GLEW_GET_VAR(G_GLEW_ARB_gpu_shader5)

#endif /* GL_ARB_gpu_shader5 */

/* ------------------------- GL_ARB_gpu_shader_fp64 ------------------------ */

#ifndef GL_ARB_gpu_shader_fp64
#define GL_ARB_gpu_shader_fp64 1

#define GL_DOUBLE_MAT2 0x8F46
#define GL_DOUBLE_MAT3 0x8F47
#define GL_DOUBLE_MAT4 0x8F48
#define GL_DOUBLE_MAT2x3 0x8F49
#define GL_DOUBLE_MAT2x4 0x8F4A
#define GL_DOUBLE_MAT3x2 0x8F4B
#define GL_DOUBLE_MAT3x4 0x8F4C
#define GL_DOUBLE_MAT4x2 0x8F4D
#define GL_DOUBLE_MAT4x3 0x8F4E
#define GL_DOUBLE_VEC2 0x8FFC
#define GL_DOUBLE_VEC3 0x8FFD
#define GL_DOUBLE_VEC4 0x8FFE

typedef void (GLAPIENTRY * PFNGLGETUNIFORMDVPROC) (GLuint program, GLint location, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLUNIFORM1DPROC) (GLint location, GLdouble x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2DPROC) (GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4DVPROC) (GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);

#define glGetUniformdv GLEW_GET_FUN(G_glewGetUniformdv)
#define glUniform1d GLEW_GET_FUN(G_glewUniform1d)
#define glUniform1dv GLEW_GET_FUN(G_glewUniform1dv)
#define glUniform2d GLEW_GET_FUN(G_glewUniform2d)
#define glUniform2dv GLEW_GET_FUN(G_glewUniform2dv)
#define glUniform3d GLEW_GET_FUN(G_glewUniform3d)
#define glUniform3dv GLEW_GET_FUN(G_glewUniform3dv)
#define glUniform4d GLEW_GET_FUN(G_glewUniform4d)
#define glUniform4dv GLEW_GET_FUN(G_glewUniform4dv)
#define glUniformMatrix2dv GLEW_GET_FUN(G_glewUniformMatrix2dv)
#define glUniformMatrix2x3dv GLEW_GET_FUN(G_glewUniformMatrix2x3dv)
#define glUniformMatrix2x4dv GLEW_GET_FUN(G_glewUniformMatrix2x4dv)
#define glUniformMatrix3dv GLEW_GET_FUN(G_glewUniformMatrix3dv)
#define glUniformMatrix3x2dv GLEW_GET_FUN(G_glewUniformMatrix3x2dv)
#define glUniformMatrix3x4dv GLEW_GET_FUN(G_glewUniformMatrix3x4dv)
#define glUniformMatrix4dv GLEW_GET_FUN(G_glewUniformMatrix4dv)
#define glUniformMatrix4x2dv GLEW_GET_FUN(G_glewUniformMatrix4x2dv)
#define glUniformMatrix4x3dv GLEW_GET_FUN(G_glewUniformMatrix4x3dv)

#define GLEW_ARB_gpu_shader_fp64 GLEW_GET_VAR(G_GLEW_ARB_gpu_shader_fp64)

#endif /* GL_ARB_gpu_shader_fp64 */

/* ------------------------ GL_ARB_half_float_pixel ------------------------ */

#ifndef GL_ARB_half_float_pixel
#define GL_ARB_half_float_pixel 1

#define GL_HALF_FLOAT_ARB 0x140B

#define GLEW_ARB_half_float_pixel GLEW_GET_VAR(G_GLEW_ARB_half_float_pixel)

#endif /* GL_ARB_half_float_pixel */

/* ------------------------ GL_ARB_half_float_vertex ----------------------- */

#ifndef GL_ARB_half_float_vertex
#define GL_ARB_half_float_vertex 1

#define GL_HALF_FLOAT 0x140B

#define GLEW_ARB_half_float_vertex GLEW_GET_VAR(G_GLEW_ARB_half_float_vertex)

#endif /* GL_ARB_half_float_vertex */

/* ----------------------------- GL_ARB_imaging ---------------------------- */

#ifndef GL_ARB_imaging
#define GL_ARB_imaging 1

#define GL_CONSTANT_COLOR 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_CONSTANT_ALPHA 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_BLEND_COLOR 0x8005
#define GL_FUNC_ADD 0x8006
#define GL_MIN 0x8007
#define GL_MAX 0x8008
#define GL_BLEND_EQUATION 0x8009
#define GL_FUNC_SUBTRACT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_CONVOLUTION_1D 0x8010
#define GL_CONVOLUTION_2D 0x8011
#define GL_SEPARABLE_2D 0x8012
#define GL_CONVOLUTION_BORDER_MODE 0x8013
#define GL_CONVOLUTION_FILTER_SCALE 0x8014
#define GL_CONVOLUTION_FILTER_BIAS 0x8015
#define GL_REDUCE 0x8016
#define GL_CONVOLUTION_FORMAT 0x8017
#define GL_CONVOLUTION_WIDTH 0x8018
#define GL_CONVOLUTION_HEIGHT 0x8019
#define GL_MAX_CONVOLUTION_WIDTH 0x801A
#define GL_MAX_CONVOLUTION_HEIGHT 0x801B
#define GL_POST_CONVOLUTION_RED_SCALE 0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE 0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS 0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS 0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS 0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS 0x8023
#define GL_HISTOGRAM 0x8024
#define GL_PROXY_HISTOGRAM 0x8025
#define GL_HISTOGRAM_WIDTH 0x8026
#define GL_HISTOGRAM_FORMAT 0x8027
#define GL_HISTOGRAM_RED_SIZE 0x8028
#define GL_HISTOGRAM_GREEN_SIZE 0x8029
#define GL_HISTOGRAM_BLUE_SIZE 0x802A
#define GL_HISTOGRAM_ALPHA_SIZE 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE 0x802C
#define GL_HISTOGRAM_SINK 0x802D
#define GL_MINMAX 0x802E
#define GL_MINMAX_FORMAT 0x802F
#define GL_MINMAX_SINK 0x8030
#define GL_TABLE_TOO_LARGE 0x8031
#define GL_COLOR_MATRIX 0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH 0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH 0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE 0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE 0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE 0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE 0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS 0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS 0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS 0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS 0x80BB
#define GL_COLOR_TABLE 0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE 0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE 0x80D2
#define GL_PROXY_COLOR_TABLE 0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#define GL_COLOR_TABLE_SCALE 0x80D6
#define GL_COLOR_TABLE_BIAS 0x80D7
#define GL_COLOR_TABLE_FORMAT 0x80D8
#define GL_COLOR_TABLE_WIDTH 0x80D9
#define GL_COLOR_TABLE_RED_SIZE 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE 0x80DF
#define GL_IGNORE_BORDER 0x8150
#define GL_CONSTANT_BORDER 0x8151
#define GL_WRAP_BORDER 0x8152
#define GL_REPLICATE_BORDER 0x8153
#define GL_CONVOLUTION_BORDER_COLOR 0x8154

typedef void (GLAPIENTRY * PFNGLCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const GLvoid *data);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *table);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const GLvoid *image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, const GLfloat *params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIPROC) (GLenum target, GLenum pname, GLint params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORSUBTABLEPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORTABLEPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER1DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER2DPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPROC) (GLenum target, GLenum format, GLenum type, GLvoid *table);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONFILTERPROC) (GLenum target, GLenum format, GLenum type, GLvoid *image);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLvoid *values);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPROC) (GLenum target, GLboolean reset, GLenum format, GLenum types, GLvoid *values);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERFVPROC) (GLenum target, GLenum pname, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETSEPARABLEFILTERPROC) (GLenum target, GLenum format, GLenum type, GLvoid *row, GLvoid *column, GLvoid *span);
typedef void (GLAPIENTRY * PFNGLHISTOGRAMPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLMINMAXPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLRESETHISTOGRAMPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLRESETMINMAXPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLSEPARABLEFILTER2DPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *row, const GLvoid *column);

#define glColorSubTable GLEW_GET_FUN(G_glewColorSubTable)
#define glColorTable GLEW_GET_FUN(G_glewColorTable)
#define glColorTableParameterfv GLEW_GET_FUN(G_glewColorTableParameterfv)
#define glColorTableParameteriv GLEW_GET_FUN(G_glewColorTableParameteriv)
#define glConvolutionFilter1D GLEW_GET_FUN(G_glewConvolutionFilter1D)
#define glConvolutionFilter2D GLEW_GET_FUN(G_glewConvolutionFilter2D)
#define glConvolutionParameterf GLEW_GET_FUN(G_glewConvolutionParameterf)
#define glConvolutionParameterfv GLEW_GET_FUN(G_glewConvolutionParameterfv)
#define glConvolutionParameteri GLEW_GET_FUN(G_glewConvolutionParameteri)
#define glConvolutionParameteriv GLEW_GET_FUN(G_glewConvolutionParameteriv)
#define glCopyColorSubTable GLEW_GET_FUN(G_glewCopyColorSubTable)
#define glCopyColorTable GLEW_GET_FUN(G_glewCopyColorTable)
#define glCopyConvolutionFilter1D GLEW_GET_FUN(G_glewCopyConvolutionFilter1D)
#define glCopyConvolutionFilter2D GLEW_GET_FUN(G_glewCopyConvolutionFilter2D)
#define glGetColorTable GLEW_GET_FUN(G_glewGetColorTable)
#define glGetColorTableParameterfv GLEW_GET_FUN(G_glewGetColorTableParameterfv)
#define glGetColorTableParameteriv GLEW_GET_FUN(G_glewGetColorTableParameteriv)
#define glGetConvolutionFilter GLEW_GET_FUN(G_glewGetConvolutionFilter)
#define glGetConvolutionParameterfv GLEW_GET_FUN(G_glewGetConvolutionParameterfv)
#define glGetConvolutionParameteriv GLEW_GET_FUN(G_glewGetConvolutionParameteriv)
#define glGetHistogram GLEW_GET_FUN(G_glewGetHistogram)
#define glGetHistogramParameterfv GLEW_GET_FUN(G_glewGetHistogramParameterfv)
#define glGetHistogramParameteriv GLEW_GET_FUN(G_glewGetHistogramParameteriv)
#define glGetMinmax GLEW_GET_FUN(G_glewGetMinmax)
#define glGetMinmaxParameterfv GLEW_GET_FUN(G_glewGetMinmaxParameterfv)
#define glGetMinmaxParameteriv GLEW_GET_FUN(G_glewGetMinmaxParameteriv)
#define glGetSeparableFilter GLEW_GET_FUN(G_glewGetSeparableFilter)
#define glHistogram GLEW_GET_FUN(G_glewHistogram)
#define glMinmax GLEW_GET_FUN(G_glewMinmax)
#define glResetHistogram GLEW_GET_FUN(G_glewResetHistogram)
#define glResetMinmax GLEW_GET_FUN(G_glewResetMinmax)
#define glSeparableFilter2D GLEW_GET_FUN(G_glewSeparableFilter2D)

#define GLEW_ARB_imaging GLEW_GET_VAR(G_GLEW_ARB_imaging)

#endif /* GL_ARB_imaging */

/* ------------------------ GL_ARB_instanced_arrays ------------------------ */

#ifndef GL_ARB_instanced_arrays
#define GL_ARB_instanced_arrays 1

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB 0x88FE

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDARBPROC) (GLenum mode, GLint first, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDARBPROC) (GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBDIVISORARBPROC) (GLuint index, GLuint divisor);

#define glDrawArraysInstancedARB GLEW_GET_FUN(G_glewDrawArraysInstancedARB)
#define glDrawElementsInstancedARB GLEW_GET_FUN(G_glewDrawElementsInstancedARB)
#define glVertexAttribDivisorARB GLEW_GET_FUN(G_glewVertexAttribDivisorARB)

#define GLEW_ARB_instanced_arrays GLEW_GET_VAR(G_GLEW_ARB_instanced_arrays)

#endif /* GL_ARB_instanced_arrays */

/* ---------------------- GL_ARB_internalformat_query ---------------------- */

#ifndef GL_ARB_internalformat_query
#define GL_ARB_internalformat_query 1

#define GL_NUM_SAMPLE_COUNTS 0x9380

typedef void (GLAPIENTRY * PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint* params);

#define glGetInternalformativ GLEW_GET_FUN(G_glewGetInternalformativ)

#define GLEW_ARB_internalformat_query GLEW_GET_VAR(G_GLEW_ARB_internalformat_query)

#endif /* GL_ARB_internalformat_query */

/* ---------------------- GL_ARB_internalformat_query2 --------------------- */

#ifndef GL_ARB_internalformat_query2
#define GL_ARB_internalformat_query2 1

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_TEXTURE_3D 0x806F
#define GL_SAMPLES 0x80A9
#define GL_INTERNALFORMAT_SUPPORTED 0x826F
#define GL_INTERNALFORMAT_PREFERRED 0x8270
#define GL_INTERNALFORMAT_RED_SIZE 0x8271
#define GL_INTERNALFORMAT_GREEN_SIZE 0x8272
#define GL_INTERNALFORMAT_BLUE_SIZE 0x8273
#define GL_INTERNALFORMAT_ALPHA_SIZE 0x8274
#define GL_INTERNALFORMAT_DEPTH_SIZE 0x8275
#define GL_INTERNALFORMAT_STENCIL_SIZE 0x8276
#define GL_INTERNALFORMAT_SHARED_SIZE 0x8277
#define GL_INTERNALFORMAT_RED_TYPE 0x8278
#define GL_INTERNALFORMAT_GREEN_TYPE 0x8279
#define GL_INTERNALFORMAT_BLUE_TYPE 0x827A
#define GL_INTERNALFORMAT_ALPHA_TYPE 0x827B
#define GL_INTERNALFORMAT_DEPTH_TYPE 0x827C
#define GL_INTERNALFORMAT_STENCIL_TYPE 0x827D
#define GL_MAX_WIDTH 0x827E
#define GL_MAX_HEIGHT 0x827F
#define GL_MAX_DEPTH 0x8280
#define GL_MAX_LAYERS 0x8281
#define GL_MAX_COMBINED_DIMENSIONS 0x8282
#define GL_COLOR_COMPONENTS 0x8283
#define GL_DEPTH_COMPONENTS 0x8284
#define GL_STENCIL_COMPONENTS 0x8285
#define GL_COLOR_RENDERABLE 0x8286
#define GL_DEPTH_RENDERABLE 0x8287
#define GL_STENCIL_RENDERABLE 0x8288
#define GL_FRAMEBUFFER_RENDERABLE 0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED 0x828A
#define GL_FRAMEBUFFER_BLEND 0x828B
#define GL_READ_PIXELS 0x828C
#define GL_READ_PIXELS_FORMAT 0x828D
#define GL_READ_PIXELS_TYPE 0x828E
#define GL_TEXTURE_IMAGE_FORMAT 0x828F
#define GL_TEXTURE_IMAGE_TYPE 0x8290
#define GL_GET_TEXTURE_IMAGE_FORMAT 0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE 0x8292
#define GL_MIPMAP 0x8293
#define GL_MANUAL_GENERATE_MIPMAP 0x8294
#define GL_AUTO_GENERATE_MIPMAP 0x8295
#define GL_COLOR_ENCODING 0x8296
#define GL_SRGB_READ 0x8297
#define GL_SRGB_WRITE 0x8298
#define GL_SRGB_DECODE_ARB 0x8299
#define GL_FILTER 0x829A
#define GL_VERTEX_TEXTURE 0x829B
#define GL_TESS_CONTROL_TEXTURE 0x829C
#define GL_TESS_EVALUATION_TEXTURE 0x829D
#define GL_GEOMETRY_TEXTURE 0x829E
#define GL_FRAGMENT_TEXTURE 0x829F
#define GL_COMPUTE_TEXTURE 0x82A0
#define GL_TEXTURE_SHADOW 0x82A1
#define GL_TEXTURE_GATHER 0x82A2
#define GL_TEXTURE_GATHER_SHADOW 0x82A3
#define GL_SHADER_IMAGE_LOAD 0x82A4
#define GL_SHADER_IMAGE_STORE 0x82A5
#define GL_SHADER_IMAGE_ATOMIC 0x82A6
#define GL_IMAGE_TEXEL_SIZE 0x82A7
#define GL_IMAGE_COMPATIBILITY_CLASS 0x82A8
#define GL_IMAGE_PIXEL_FORMAT 0x82A9
#define GL_IMAGE_PIXEL_TYPE 0x82AA
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST 0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST 0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE 0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE 0x82AF
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH 0x82B1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT 0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE 0x82B3
#define GL_CLEAR_BUFFER 0x82B4
#define GL_TEXTURE_VIEW 0x82B5
#define GL_VIEW_COMPATIBILITY_CLASS 0x82B6
#define GL_FULL_SUPPORT 0x82B7
#define GL_CAVEAT_SUPPORT 0x82B8
#define GL_IMAGE_CLASS_4_X_32 0x82B9
#define GL_IMAGE_CLASS_2_X_32 0x82BA
#define GL_IMAGE_CLASS_1_X_32 0x82BB
#define GL_IMAGE_CLASS_4_X_16 0x82BC
#define GL_IMAGE_CLASS_2_X_16 0x82BD
#define GL_IMAGE_CLASS_1_X_16 0x82BE
#define GL_IMAGE_CLASS_4_X_8 0x82BF
#define GL_IMAGE_CLASS_2_X_8 0x82C0
#define GL_IMAGE_CLASS_1_X_8 0x82C1
#define GL_IMAGE_CLASS_11_11_10 0x82C2
#define GL_IMAGE_CLASS_10_10_10_2 0x82C3
#define GL_VIEW_CLASS_128_BITS 0x82C4
#define GL_VIEW_CLASS_96_BITS 0x82C5
#define GL_VIEW_CLASS_64_BITS 0x82C6
#define GL_VIEW_CLASS_48_BITS 0x82C7
#define GL_VIEW_CLASS_32_BITS 0x82C8
#define GL_VIEW_CLASS_24_BITS 0x82C9
#define GL_VIEW_CLASS_16_BITS 0x82CA
#define GL_VIEW_CLASS_8_BITS 0x82CB
#define GL_VIEW_CLASS_S3TC_DXT1_RGB 0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA 0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA 0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA 0x82CF
#define GL_VIEW_CLASS_RGTC1_RED 0x82D0
#define GL_VIEW_CLASS_RGTC2_RG 0x82D1
#define GL_VIEW_CLASS_BPTC_UNORM 0x82D2
#define GL_VIEW_CLASS_BPTC_FLOAT 0x82D3
#define GL_TEXTURE_RECTANGLE 0x84F5
#define GL_TEXTURE_1D_ARRAY 0x8C18
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TEXTURE_BUFFER 0x8C2A
#define GL_RENDERBUFFER 0x8D41
#define GL_TEXTURE_CUBE_MAP_ARRAY 0x9009
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_NUM_SAMPLE_COUNTS 0x9380

typedef void (GLAPIENTRY * PFNGLGETINTERNALFORMATI64VPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei bufSize, GLint64* params);

#define glGetInternalformati64v GLEW_GET_FUN(G_glewGetInternalformati64v)

#define GLEW_ARB_internalformat_query2 GLEW_GET_VAR(G_GLEW_ARB_internalformat_query2)

#endif /* GL_ARB_internalformat_query2 */

/* ----------------------- GL_ARB_invalidate_subdata ----------------------- */

#ifndef GL_ARB_invalidate_subdata
#define GL_ARB_invalidate_subdata 1

typedef void (GLAPIENTRY * PFNGLINVALIDATEBUFFERDATAPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLINVALIDATEBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLINVALIDATEFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum* attachments);
typedef void (GLAPIENTRY * PFNGLINVALIDATESUBFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum* attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLINVALIDATETEXIMAGEPROC) (GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLINVALIDATETEXSUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);

#define glInvalidateBufferData GLEW_GET_FUN(G_glewInvalidateBufferData)
#define glInvalidateBufferSubData GLEW_GET_FUN(G_glewInvalidateBufferSubData)
#define glInvalidateFramebuffer GLEW_GET_FUN(G_glewInvalidateFramebuffer)
#define glInvalidateSubFramebuffer GLEW_GET_FUN(G_glewInvalidateSubFramebuffer)
#define glInvalidateTexImage GLEW_GET_FUN(G_glewInvalidateTexImage)
#define glInvalidateTexSubImage GLEW_GET_FUN(G_glewInvalidateTexSubImage)

#define GLEW_ARB_invalidate_subdata GLEW_GET_VAR(G_GLEW_ARB_invalidate_subdata)

#endif /* GL_ARB_invalidate_subdata */

/* ---------------------- GL_ARB_map_buffer_alignment ---------------------- */

#ifndef GL_ARB_map_buffer_alignment
#define GL_ARB_map_buffer_alignment 1

#define GL_MIN_MAP_BUFFER_ALIGNMENT 0x90BC

#define GLEW_ARB_map_buffer_alignment GLEW_GET_VAR(G_GLEW_ARB_map_buffer_alignment)

#endif /* GL_ARB_map_buffer_alignment */

/* ------------------------ GL_ARB_map_buffer_range ------------------------ */

#ifndef GL_ARB_map_buffer_range
#define GL_ARB_map_buffer_range 1

#define GL_MAP_READ_BIT 0x0001
#define GL_MAP_WRITE_BIT 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT 0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT 0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT 0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT 0x0020

typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPBUFFERRANGEPROC) (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);

#define glFlushMappedBufferRange GLEW_GET_FUN(G_glewFlushMappedBufferRange)
#define glMapBufferRange GLEW_GET_FUN(G_glewMapBufferRange)

#define GLEW_ARB_map_buffer_range GLEW_GET_VAR(G_GLEW_ARB_map_buffer_range)

#endif /* GL_ARB_map_buffer_range */

/* ------------------------- GL_ARB_matrix_palette ------------------------- */

#ifndef GL_ARB_matrix_palette
#define GL_ARB_matrix_palette 1

#define GL_MATRIX_PALETTE_ARB 0x8840
#define GL_MAX_MATRIX_PALETTE_STACK_DEPTH_ARB 0x8841
#define GL_MAX_PALETTE_MATRICES_ARB 0x8842
#define GL_CURRENT_PALETTE_MATRIX_ARB 0x8843
#define GL_MATRIX_INDEX_ARRAY_ARB 0x8844
#define GL_CURRENT_MATRIX_INDEX_ARB 0x8845
#define GL_MATRIX_INDEX_ARRAY_SIZE_ARB 0x8846
#define GL_MATRIX_INDEX_ARRAY_TYPE_ARB 0x8847
#define GL_MATRIX_INDEX_ARRAY_STRIDE_ARB 0x8848
#define GL_MATRIX_INDEX_ARRAY_POINTER_ARB 0x8849

typedef void (GLAPIENTRY * PFNGLCURRENTPALETTEMATRIXARBPROC) (GLint index);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXPOINTERARBPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUBVARBPROC) (GLint size, GLubyte *indices);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUIVARBPROC) (GLint size, GLuint *indices);
typedef void (GLAPIENTRY * PFNGLMATRIXINDEXUSVARBPROC) (GLint size, GLushort *indices);

#define glCurrentPaletteMatrixARB GLEW_GET_FUN(G_glewCurrentPaletteMatrixARB)
#define glMatrixIndexPointerARB GLEW_GET_FUN(G_glewMatrixIndexPointerARB)
#define glMatrixIndexubvARB GLEW_GET_FUN(G_glewMatrixIndexubvARB)
#define glMatrixIndexuivARB GLEW_GET_FUN(G_glewMatrixIndexuivARB)
#define glMatrixIndexusvARB GLEW_GET_FUN(G_glewMatrixIndexusvARB)

#define GLEW_ARB_matrix_palette GLEW_GET_VAR(G_GLEW_ARB_matrix_palette)

#endif /* GL_ARB_matrix_palette */

/* ----------------------- GL_ARB_multi_draw_indirect ---------------------- */

#ifndef GL_ARB_multi_draw_indirect
#define GL_ARB_multi_draw_indirect 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSINDIRECTPROC) (GLenum mode, const void* indirect, GLsizei primcount, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void* indirect, GLsizei primcount, GLsizei stride);

#define glMultiDrawArraysIndirect GLEW_GET_FUN(G_glewMultiDrawArraysIndirect)
#define glMultiDrawElementsIndirect GLEW_GET_FUN(G_glewMultiDrawElementsIndirect)

#define GLEW_ARB_multi_draw_indirect GLEW_GET_VAR(G_GLEW_ARB_multi_draw_indirect)

#endif /* GL_ARB_multi_draw_indirect */

/* --------------------------- GL_ARB_multisample -------------------------- */

#ifndef GL_ARB_multisample
#define GL_ARB_multisample 1

#define GL_MULTISAMPLE_ARB 0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE_ARB 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_ARB 0x809F
#define GL_SAMPLE_COVERAGE_ARB 0x80A0
#define GL_SAMPLE_BUFFERS_ARB 0x80A8
#define GL_SAMPLES_ARB 0x80A9
#define GL_SAMPLE_COVERAGE_VALUE_ARB 0x80AA
#define GL_SAMPLE_COVERAGE_INVERT_ARB 0x80AB
#define GL_MULTISAMPLE_BIT_ARB 0x20000000

typedef void (GLAPIENTRY * PFNGLSAMPLECOVERAGEARBPROC) (GLclampf value, GLboolean invert);

#define glSampleCoverageARB GLEW_GET_FUN(G_glewSampleCoverageARB)

#define GLEW_ARB_multisample GLEW_GET_VAR(G_GLEW_ARB_multisample)

#endif /* GL_ARB_multisample */

/* -------------------------- GL_ARB_multitexture -------------------------- */

#ifndef GL_ARB_multitexture
#define GL_ARB_multitexture 1

#define GL_TEXTURE0_ARB 0x84C0
#define GL_TEXTURE1_ARB 0x84C1
#define GL_TEXTURE2_ARB 0x84C2
#define GL_TEXTURE3_ARB 0x84C3
#define GL_TEXTURE4_ARB 0x84C4
#define GL_TEXTURE5_ARB 0x84C5
#define GL_TEXTURE6_ARB 0x84C6
#define GL_TEXTURE7_ARB 0x84C7
#define GL_TEXTURE8_ARB 0x84C8
#define GL_TEXTURE9_ARB 0x84C9
#define GL_TEXTURE10_ARB 0x84CA
#define GL_TEXTURE11_ARB 0x84CB
#define GL_TEXTURE12_ARB 0x84CC
#define GL_TEXTURE13_ARB 0x84CD
#define GL_TEXTURE14_ARB 0x84CE
#define GL_TEXTURE15_ARB 0x84CF
#define GL_TEXTURE16_ARB 0x84D0
#define GL_TEXTURE17_ARB 0x84D1
#define GL_TEXTURE18_ARB 0x84D2
#define GL_TEXTURE19_ARB 0x84D3
#define GL_TEXTURE20_ARB 0x84D4
#define GL_TEXTURE21_ARB 0x84D5
#define GL_TEXTURE22_ARB 0x84D6
#define GL_TEXTURE23_ARB 0x84D7
#define GL_TEXTURE24_ARB 0x84D8
#define GL_TEXTURE25_ARB 0x84D9
#define GL_TEXTURE26_ARB 0x84DA
#define GL_TEXTURE27_ARB 0x84DB
#define GL_TEXTURE28_ARB 0x84DC
#define GL_TEXTURE29_ARB 0x84DD
#define GL_TEXTURE30_ARB 0x84DE
#define GL_TEXTURE31_ARB 0x84DF
#define GL_ACTIVE_TEXTURE_ARB 0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB 0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB 0x84E2

typedef void (GLAPIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DARBPROC) (GLenum target, GLdouble s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FARBPROC) (GLenum target, GLfloat s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IARBPROC) (GLenum target, GLint s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SARBPROC) (GLenum target, GLshort s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DARBPROC) (GLenum target, GLdouble s, GLdouble t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IARBPROC) (GLenum target, GLint s, GLint t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SARBPROC) (GLenum target, GLshort s, GLshort t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IARBPROC) (GLenum target, GLint s, GLint t, GLint r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3SVARBPROC) (GLenum target, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DARBPROC) (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4DVARBPROC) (GLenum target, const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FARBPROC) (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4FVARBPROC) (GLenum target, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IARBPROC) (GLenum target, GLint s, GLint t, GLint r, GLint q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4IVARBPROC) (GLenum target, const GLint *v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SARBPROC) (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4SVARBPROC) (GLenum target, const GLshort *v);

#define glActiveTextureARB GLEW_GET_FUN(G_glewActiveTextureARB)
#define glClientActiveTextureARB GLEW_GET_FUN(G_glewClientActiveTextureARB)
#define glMultiTexCoord1dARB GLEW_GET_FUN(G_glewMultiTexCoord1dARB)
#define glMultiTexCoord1dvARB GLEW_GET_FUN(G_glewMultiTexCoord1dvARB)
#define glMultiTexCoord1fARB GLEW_GET_FUN(G_glewMultiTexCoord1fARB)
#define glMultiTexCoord1fvARB GLEW_GET_FUN(G_glewMultiTexCoord1fvARB)
#define glMultiTexCoord1iARB GLEW_GET_FUN(G_glewMultiTexCoord1iARB)
#define glMultiTexCoord1ivARB GLEW_GET_FUN(G_glewMultiTexCoord1ivARB)
#define glMultiTexCoord1sARB GLEW_GET_FUN(G_glewMultiTexCoord1sARB)
#define glMultiTexCoord1svARB GLEW_GET_FUN(G_glewMultiTexCoord1svARB)
#define glMultiTexCoord2dARB GLEW_GET_FUN(G_glewMultiTexCoord2dARB)
#define glMultiTexCoord2dvARB GLEW_GET_FUN(G_glewMultiTexCoord2dvARB)
#define glMultiTexCoord2fARB GLEW_GET_FUN(G_glewMultiTexCoord2fARB)
#define glMultiTexCoord2fvARB GLEW_GET_FUN(G_glewMultiTexCoord2fvARB)
#define glMultiTexCoord2iARB GLEW_GET_FUN(G_glewMultiTexCoord2iARB)
#define glMultiTexCoord2ivARB GLEW_GET_FUN(G_glewMultiTexCoord2ivARB)
#define glMultiTexCoord2sARB GLEW_GET_FUN(G_glewMultiTexCoord2sARB)
#define glMultiTexCoord2svARB GLEW_GET_FUN(G_glewMultiTexCoord2svARB)
#define glMultiTexCoord3dARB GLEW_GET_FUN(G_glewMultiTexCoord3dARB)
#define glMultiTexCoord3dvARB GLEW_GET_FUN(G_glewMultiTexCoord3dvARB)
#define glMultiTexCoord3fARB GLEW_GET_FUN(G_glewMultiTexCoord3fARB)
#define glMultiTexCoord3fvARB GLEW_GET_FUN(G_glewMultiTexCoord3fvARB)
#define glMultiTexCoord3iARB GLEW_GET_FUN(G_glewMultiTexCoord3iARB)
#define glMultiTexCoord3ivARB GLEW_GET_FUN(G_glewMultiTexCoord3ivARB)
#define glMultiTexCoord3sARB GLEW_GET_FUN(G_glewMultiTexCoord3sARB)
#define glMultiTexCoord3svARB GLEW_GET_FUN(G_glewMultiTexCoord3svARB)
#define glMultiTexCoord4dARB GLEW_GET_FUN(G_glewMultiTexCoord4dARB)
#define glMultiTexCoord4dvARB GLEW_GET_FUN(G_glewMultiTexCoord4dvARB)
#define glMultiTexCoord4fARB GLEW_GET_FUN(G_glewMultiTexCoord4fARB)
#define glMultiTexCoord4fvARB GLEW_GET_FUN(G_glewMultiTexCoord4fvARB)
#define glMultiTexCoord4iARB GLEW_GET_FUN(G_glewMultiTexCoord4iARB)
#define glMultiTexCoord4ivARB GLEW_GET_FUN(G_glewMultiTexCoord4ivARB)
#define glMultiTexCoord4sARB GLEW_GET_FUN(G_glewMultiTexCoord4sARB)
#define glMultiTexCoord4svARB GLEW_GET_FUN(G_glewMultiTexCoord4svARB)

#define GLEW_ARB_multitexture GLEW_GET_VAR(G_GLEW_ARB_multitexture)

#endif /* GL_ARB_multitexture */

/* ------------------------- GL_ARB_occlusion_query ------------------------ */

#ifndef GL_ARB_occlusion_query
#define GL_ARB_occlusion_query 1

#define GL_QUERY_COUNTER_BITS_ARB 0x8864
#define GL_CURRENT_QUERY_ARB 0x8865
#define GL_QUERY_RESULT_ARB 0x8866
#define GL_QUERY_RESULT_AVAILABLE_ARB 0x8867
#define GL_SAMPLES_PASSED_ARB 0x8914

typedef void (GLAPIENTRY * PFNGLBEGINQUERYARBPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEQUERIESARBPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLENDQUERYARBPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGENQUERIESARBPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTIVARBPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUIVARBPROC) (GLuint id, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISQUERYARBPROC) (GLuint id);

#define glBeginQueryARB GLEW_GET_FUN(G_glewBeginQueryARB)
#define glDeleteQueriesARB GLEW_GET_FUN(G_glewDeleteQueriesARB)
#define glEndQueryARB GLEW_GET_FUN(G_glewEndQueryARB)
#define glGenQueriesARB GLEW_GET_FUN(G_glewGenQueriesARB)
#define glGetQueryObjectivARB GLEW_GET_FUN(G_glewGetQueryObjectivARB)
#define glGetQueryObjectuivARB GLEW_GET_FUN(G_glewGetQueryObjectuivARB)
#define glGetQueryivARB GLEW_GET_FUN(G_glewGetQueryivARB)
#define glIsQueryARB GLEW_GET_FUN(G_glewIsQueryARB)

#define GLEW_ARB_occlusion_query GLEW_GET_VAR(G_GLEW_ARB_occlusion_query)

#endif /* GL_ARB_occlusion_query */

/* ------------------------ GL_ARB_occlusion_query2 ------------------------ */

#ifndef GL_ARB_occlusion_query2
#define GL_ARB_occlusion_query2 1

#define GL_ANY_SAMPLES_PASSED 0x8C2F

#define GLEW_ARB_occlusion_query2 GLEW_GET_VAR(G_GLEW_ARB_occlusion_query2)

#endif /* GL_ARB_occlusion_query2 */

/* ----------------------- GL_ARB_pixel_buffer_object ---------------------- */

#ifndef GL_ARB_pixel_buffer_object
#define GL_ARB_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_ARB 0x88EB
#define GL_PIXEL_UNPACK_BUFFER_ARB 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_ARB 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_ARB 0x88EF

#define GLEW_ARB_pixel_buffer_object GLEW_GET_VAR(G_GLEW_ARB_pixel_buffer_object)

#endif /* GL_ARB_pixel_buffer_object */

/* ------------------------ GL_ARB_point_parameters ------------------------ */

#ifndef GL_ARB_point_parameters
#define GL_ARB_point_parameters 1

#define GL_POINT_SIZE_MIN_ARB 0x8126
#define GL_POINT_SIZE_MAX_ARB 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_ARB 0x8128
#define GL_POINT_DISTANCE_ATTENUATION_ARB 0x8129

typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFARBPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVARBPROC) (GLenum pname, const GLfloat* params);

#define glPointParameterfARB GLEW_GET_FUN(G_glewPointParameterfARB)
#define glPointParameterfvARB GLEW_GET_FUN(G_glewPointParameterfvARB)

#define GLEW_ARB_point_parameters GLEW_GET_VAR(G_GLEW_ARB_point_parameters)

#endif /* GL_ARB_point_parameters */

/* -------------------------- GL_ARB_point_sprite -------------------------- */

#ifndef GL_ARB_point_sprite
#define GL_ARB_point_sprite 1

#define GL_POINT_SPRITE_ARB 0x8861
#define GL_COORD_REPLACE_ARB 0x8862

#define GLEW_ARB_point_sprite GLEW_GET_VAR(G_GLEW_ARB_point_sprite)

#endif /* GL_ARB_point_sprite */

/* --------------------- GL_ARB_program_interface_query -------------------- */

#ifndef GL_ARB_program_interface_query
#define GL_ARB_program_interface_query 1

#define GL_UNIFORM 0x92E1
#define GL_UNIFORM_BLOCK 0x92E2
#define GL_PROGRAM_INPUT 0x92E3
#define GL_PROGRAM_OUTPUT 0x92E4
#define GL_BUFFER_VARIABLE 0x92E5
#define GL_SHADER_STORAGE_BLOCK 0x92E6
#define GL_IS_PER_PATCH 0x92E7
#define GL_VERTEX_SUBROUTINE 0x92E8
#define GL_TESS_CONTROL_SUBROUTINE 0x92E9
#define GL_TESS_EVALUATION_SUBROUTINE 0x92EA
#define GL_GEOMETRY_SUBROUTINE 0x92EB
#define GL_FRAGMENT_SUBROUTINE 0x92EC
#define GL_COMPUTE_SUBROUTINE 0x92ED
#define GL_VERTEX_SUBROUTINE_UNIFORM 0x92EE
#define GL_TESS_CONTROL_SUBROUTINE_UNIFORM 0x92EF
#define GL_TESS_EVALUATION_SUBROUTINE_UNIFORM 0x92F0
#define GL_GEOMETRY_SUBROUTINE_UNIFORM 0x92F1
#define GL_FRAGMENT_SUBROUTINE_UNIFORM 0x92F2
#define GL_COMPUTE_SUBROUTINE_UNIFORM 0x92F3
#define GL_TRANSFORM_FEEDBACK_VARYING 0x92F4
#define GL_ACTIVE_RESOURCES 0x92F5
#define GL_MAX_NAME_LENGTH 0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES 0x92F7
#define GL_MAX_NUM_COMPATIBLE_SUBROUTINES 0x92F8
#define GL_NAME_LENGTH 0x92F9
#define GL_TYPE 0x92FA
#define GL_ARRAY_SIZE 0x92FB
#define GL_OFFSET 0x92FC
#define GL_BLOCK_INDEX 0x92FD
#define GL_ARRAY_STRIDE 0x92FE
#define GL_MATRIX_STRIDE 0x92FF
#define GL_IS_ROW_MAJOR 0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX 0x9301
#define GL_BUFFER_BINDING 0x9302
#define GL_BUFFER_DATA_SIZE 0x9303
#define GL_NUM_ACTIVE_VARIABLES 0x9304
#define GL_ACTIVE_VARIABLES 0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER 0x9306
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER 0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER 0x9308
#define GL_REFERENCED_BY_GEOMETRY_SHADER 0x9309
#define GL_REFERENCED_BY_FRAGMENT_SHADER 0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER 0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE 0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE 0x930D
#define GL_LOCATION 0x930E
#define GL_LOCATION_INDEX 0x930F

typedef void (GLAPIENTRY * PFNGLGETPROGRAMINTERFACEIVPROC) (GLuint program, GLenum programInterface, GLenum pname, GLint* params);
typedef GLuint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCEINDEXPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCELOCATIONPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC) (GLuint program, GLenum programInterface, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMRESOURCENAMEPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMRESOURCEIVPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum* props, GLsizei bufSize, GLsizei *length, GLint *params);

#define glGetProgramInterfaceiv GLEW_GET_FUN(G_glewGetProgramInterfaceiv)
#define glGetProgramResourceIndex GLEW_GET_FUN(G_glewGetProgramResourceIndex)
#define glGetProgramResourceLocation GLEW_GET_FUN(G_glewGetProgramResourceLocation)
#define glGetProgramResourceLocationIndex GLEW_GET_FUN(G_glewGetProgramResourceLocationIndex)
#define glGetProgramResourceName GLEW_GET_FUN(G_glewGetProgramResourceName)
#define glGetProgramResourceiv GLEW_GET_FUN(G_glewGetProgramResourceiv)

#define GLEW_ARB_program_interface_query GLEW_GET_VAR(G_GLEW_ARB_program_interface_query)

#endif /* GL_ARB_program_interface_query */

/* ------------------------ GL_ARB_provoking_vertex ------------------------ */

#ifndef GL_ARB_provoking_vertex
#define GL_ARB_provoking_vertex 1

#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_PROVOKING_VERTEX 0x8E4F

typedef void (GLAPIENTRY * PFNGLPROVOKINGVERTEXPROC) (GLenum mode);

#define glProvokingVertex GLEW_GET_FUN(G_glewProvokingVertex)

#define GLEW_ARB_provoking_vertex GLEW_GET_VAR(G_GLEW_ARB_provoking_vertex)

#endif /* GL_ARB_provoking_vertex */

/* ------------------ GL_ARB_robust_buffer_access_behavior ----------------- */

#ifndef GL_ARB_robust_buffer_access_behavior
#define GL_ARB_robust_buffer_access_behavior 1

#define GLEW_ARB_robust_buffer_access_behavior GLEW_GET_VAR(G_GLEW_ARB_robust_buffer_access_behavior)

#endif /* GL_ARB_robust_buffer_access_behavior */

/* --------------------------- GL_ARB_robustness --------------------------- */

#ifndef GL_ARB_robustness
#define GL_ARB_robustness 1

#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB 0x00000004
#define GL_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define GL_GUILTY_CONTEXT_RESET_ARB 0x8253
#define GL_INNOCENT_CONTEXT_RESET_ARB 0x8254
#define GL_UNKNOWN_CONTEXT_RESET_ARB 0x8255
#define GL_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define GL_NO_RESET_NOTIFICATION_ARB 0x8261

typedef GLenum (GLAPIENTRY * PFNGLGETGRAPHICSRESETSTATUSARBPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETNCOLORTABLEARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* table);
typedef void (GLAPIENTRY * PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, GLsizei bufSize, void* img);
typedef void (GLAPIENTRY * PFNGLGETNCONVOLUTIONFILTERARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void* image);
typedef void (GLAPIENTRY * PFNGLGETNHISTOGRAMARBPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
typedef void (GLAPIENTRY * PFNGLGETNMAPDVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLdouble* v);
typedef void (GLAPIENTRY * PFNGLGETNMAPFVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLfloat* v);
typedef void (GLAPIENTRY * PFNGLGETNMAPIVARBPROC) (GLenum target, GLenum query, GLsizei bufSize, GLint* v);
typedef void (GLAPIENTRY * PFNGLGETNMINMAXARBPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPFVARBPROC) (GLenum map, GLsizei bufSize, GLfloat* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPUIVARBPROC) (GLenum map, GLsizei bufSize, GLuint* values);
typedef void (GLAPIENTRY * PFNGLGETNPIXELMAPUSVARBPROC) (GLenum map, GLsizei bufSize, GLushort* values);
typedef void (GLAPIENTRY * PFNGLGETNPOLYGONSTIPPLEARBPROC) (GLsizei bufSize, GLubyte* pattern);
typedef void (GLAPIENTRY * PFNGLGETNSEPARABLEFILTERARBPROC) (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void* row, GLsizei columnBufSize, GLvoid*column, GLvoid*span);
typedef void (GLAPIENTRY * PFNGLGETNTEXIMAGEARBPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void* img);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMDVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMFVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNUNIFORMUIVARBPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint* params);
typedef void (GLAPIENTRY * PFNGLREADNPIXELSARBPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void* data);

#define glGetGraphicsResetStatusARB GLEW_GET_FUN(G_glewGetGraphicsResetStatusARB)
#define glGetnColorTableARB GLEW_GET_FUN(G_glewGetnColorTableARB)
#define glGetnCompressedTexImageARB GLEW_GET_FUN(G_glewGetnCompressedTexImageARB)
#define glGetnConvolutionFilterARB GLEW_GET_FUN(G_glewGetnConvolutionFilterARB)
#define glGetnHistogramARB GLEW_GET_FUN(G_glewGetnHistogramARB)
#define glGetnMapdvARB GLEW_GET_FUN(G_glewGetnMapdvARB)
#define glGetnMapfvARB GLEW_GET_FUN(G_glewGetnMapfvARB)
#define glGetnMapivARB GLEW_GET_FUN(G_glewGetnMapivARB)
#define glGetnMinmaxARB GLEW_GET_FUN(G_glewGetnMinmaxARB)
#define glGetnPixelMapfvARB GLEW_GET_FUN(G_glewGetnPixelMapfvARB)
#define glGetnPixelMapuivARB GLEW_GET_FUN(G_glewGetnPixelMapuivARB)
#define glGetnPixelMapusvARB GLEW_GET_FUN(G_glewGetnPixelMapusvARB)
#define glGetnPolygonStippleARB GLEW_GET_FUN(G_glewGetnPolygonStippleARB)
#define glGetnSeparableFilterARB GLEW_GET_FUN(G_glewGetnSeparableFilterARB)
#define glGetnTexImageARB GLEW_GET_FUN(G_glewGetnTexImageARB)
#define glGetnUniformdvARB GLEW_GET_FUN(G_glewGetnUniformdvARB)
#define glGetnUniformfvARB GLEW_GET_FUN(G_glewGetnUniformfvARB)
#define glGetnUniformivARB GLEW_GET_FUN(G_glewGetnUniformivARB)
#define glGetnUniformuivARB GLEW_GET_FUN(G_glewGetnUniformuivARB)
#define glReadnPixelsARB GLEW_GET_FUN(G_glewReadnPixelsARB)

#define GLEW_ARB_robustness GLEW_GET_VAR(G_GLEW_ARB_robustness)

#endif /* GL_ARB_robustness */

/* ---------------- GL_ARB_robustness_application_isolation ---------------- */

#ifndef GL_ARB_robustness_application_isolation
#define GL_ARB_robustness_application_isolation 1

#define GLEW_ARB_robustness_application_isolation GLEW_GET_VAR(G_GLEW_ARB_robustness_application_isolation)

#endif /* GL_ARB_robustness_application_isolation */

/* ---------------- GL_ARB_robustness_share_group_isolation ---------------- */

#ifndef GL_ARB_robustness_share_group_isolation
#define GL_ARB_robustness_share_group_isolation 1

#define GLEW_ARB_robustness_share_group_isolation GLEW_GET_VAR(G_GLEW_ARB_robustness_share_group_isolation)

#endif /* GL_ARB_robustness_share_group_isolation */

/* ------------------------- GL_ARB_sample_shading ------------------------- */

#ifndef GL_ARB_sample_shading
#define GL_ARB_sample_shading 1

#define GL_SAMPLE_SHADING_ARB 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE_ARB 0x8C37

typedef void (GLAPIENTRY * PFNGLMINSAMPLESHADINGARBPROC) (GLclampf value);

#define glMinSampleShadingARB GLEW_GET_FUN(G_glewMinSampleShadingARB)

#define GLEW_ARB_sample_shading GLEW_GET_VAR(G_GLEW_ARB_sample_shading)

#endif /* GL_ARB_sample_shading */

/* ------------------------- GL_ARB_sampler_objects ------------------------ */

#ifndef GL_ARB_sampler_objects
#define GL_ARB_sampler_objects 1

#define GL_SAMPLER_BINDING 0x8919

typedef void (GLAPIENTRY * PFNGLBINDSAMPLERPROC) (GLuint unit, GLuint sampler);
typedef void (GLAPIENTRY * PFNGLDELETESAMPLERSPROC) (GLsizei count, const GLuint * samplers);
typedef void (GLAPIENTRY * PFNGLGENSAMPLERSPROC) (GLsizei count, GLuint* samplers);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISSAMPLERPROC) (GLuint sampler);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIIVPROC) (GLuint sampler, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIUIVPROC) (GLuint sampler, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERFPROC) (GLuint sampler, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERFVPROC) (GLuint sampler, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIPROC) (GLuint sampler, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLSAMPLERPARAMETERIVPROC) (GLuint sampler, GLenum pname, const GLint* params);

#define glBindSampler GLEW_GET_FUN(G_glewBindSampler)
#define glDeleteSamplers GLEW_GET_FUN(G_glewDeleteSamplers)
#define glGenSamplers GLEW_GET_FUN(G_glewGenSamplers)
#define glGetSamplerParameterIiv GLEW_GET_FUN(G_glewGetSamplerParameterIiv)
#define glGetSamplerParameterIuiv GLEW_GET_FUN(G_glewGetSamplerParameterIuiv)
#define glGetSamplerParameterfv GLEW_GET_FUN(G_glewGetSamplerParameterfv)
#define glGetSamplerParameteriv GLEW_GET_FUN(G_glewGetSamplerParameteriv)
#define glIsSampler GLEW_GET_FUN(G_glewIsSampler)
#define glSamplerParameterIiv GLEW_GET_FUN(G_glewSamplerParameterIiv)
#define glSamplerParameterIuiv GLEW_GET_FUN(G_glewSamplerParameterIuiv)
#define glSamplerParameterf GLEW_GET_FUN(G_glewSamplerParameterf)
#define glSamplerParameterfv GLEW_GET_FUN(G_glewSamplerParameterfv)
#define glSamplerParameteri GLEW_GET_FUN(G_glewSamplerParameteri)
#define glSamplerParameteriv GLEW_GET_FUN(G_glewSamplerParameteriv)

#define GLEW_ARB_sampler_objects GLEW_GET_VAR(G_GLEW_ARB_sampler_objects)

#endif /* GL_ARB_sampler_objects */

/* ------------------------ GL_ARB_seamless_cube_map ----------------------- */

#ifndef GL_ARB_seamless_cube_map
#define GL_ARB_seamless_cube_map 1

#define GL_TEXTURE_CUBE_MAP_SEAMLESS 0x884F

#define GLEW_ARB_seamless_cube_map GLEW_GET_VAR(G_GLEW_ARB_seamless_cube_map)

#endif /* GL_ARB_seamless_cube_map */

/* --------------------- GL_ARB_separate_shader_objects -------------------- */

#ifndef GL_ARB_separate_shader_objects
#define GL_ARB_separate_shader_objects 1

#define GL_VERTEX_SHADER_BIT 0x00000001
#define GL_FRAGMENT_SHADER_BIT 0x00000002
#define GL_GEOMETRY_SHADER_BIT 0x00000004
#define GL_TESS_CONTROL_SHADER_BIT 0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT 0x00000010
#define GL_PROGRAM_SEPARABLE 0x8258
#define GL_ACTIVE_PROGRAM 0x8259
#define GL_PROGRAM_PIPELINE_BINDING 0x825A
#define GL_ALL_SHADER_BITS 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLACTIVESHADERPROGRAMPROC) (GLuint pipeline, GLuint program);
typedef void (GLAPIENTRY * PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROGRAMVPROC) (GLenum type, GLsizei count, const GLchar ** strings);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint* pipelines);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint* pipelines);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPIPELINEINFOLOGPROC) (GLuint pipeline, GLsizei bufSize, GLsizei* length, GLchar *infoLog);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DPROC) (GLuint program, GLint location, GLdouble x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FPROC) (GLuint program, GLint location, GLfloat x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIPROC) (GLuint program, GLint location, GLuint x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIPROC) (GLuint program, GLint location, GLuint x, GLuint y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FPROC) (GLuint program, GLint location, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IPROC) (GLuint program, GLint location, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIPROC) (GLuint program, GLint location, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);

#define glActiveShaderProgram GLEW_GET_FUN(G_glewActiveShaderProgram)
#define glBindProgramPipeline GLEW_GET_FUN(G_glewBindProgramPipeline)
#define glCreateShaderProgramv GLEW_GET_FUN(G_glewCreateShaderProgramv)
#define glDeleteProgramPipelines GLEW_GET_FUN(G_glewDeleteProgramPipelines)
#define glGenProgramPipelines GLEW_GET_FUN(G_glewGenProgramPipelines)
#define glGetProgramPipelineInfoLog GLEW_GET_FUN(G_glewGetProgramPipelineInfoLog)
#define glGetProgramPipelineiv GLEW_GET_FUN(G_glewGetProgramPipelineiv)
#define glIsProgramPipeline GLEW_GET_FUN(G_glewIsProgramPipeline)
#define glProgramUniform1d GLEW_GET_FUN(G_glewProgramUniform1d)
#define glProgramUniform1dv GLEW_GET_FUN(G_glewProgramUniform1dv)
#define glProgramUniform1f GLEW_GET_FUN(G_glewProgramUniform1f)
#define glProgramUniform1fv GLEW_GET_FUN(G_glewProgramUniform1fv)
#define glProgramUniform1i GLEW_GET_FUN(G_glewProgramUniform1i)
#define glProgramUniform1iv GLEW_GET_FUN(G_glewProgramUniform1iv)
#define glProgramUniform1ui GLEW_GET_FUN(G_glewProgramUniform1ui)
#define glProgramUniform1uiv GLEW_GET_FUN(G_glewProgramUniform1uiv)
#define glProgramUniform2d GLEW_GET_FUN(G_glewProgramUniform2d)
#define glProgramUniform2dv GLEW_GET_FUN(G_glewProgramUniform2dv)
#define glProgramUniform2f GLEW_GET_FUN(G_glewProgramUniform2f)
#define glProgramUniform2fv GLEW_GET_FUN(G_glewProgramUniform2fv)
#define glProgramUniform2i GLEW_GET_FUN(G_glewProgramUniform2i)
#define glProgramUniform2iv GLEW_GET_FUN(G_glewProgramUniform2iv)
#define glProgramUniform2ui GLEW_GET_FUN(G_glewProgramUniform2ui)
#define glProgramUniform2uiv GLEW_GET_FUN(G_glewProgramUniform2uiv)
#define glProgramUniform3d GLEW_GET_FUN(G_glewProgramUniform3d)
#define glProgramUniform3dv GLEW_GET_FUN(G_glewProgramUniform3dv)
#define glProgramUniform3f GLEW_GET_FUN(G_glewProgramUniform3f)
#define glProgramUniform3fv GLEW_GET_FUN(G_glewProgramUniform3fv)
#define glProgramUniform3i GLEW_GET_FUN(G_glewProgramUniform3i)
#define glProgramUniform3iv GLEW_GET_FUN(G_glewProgramUniform3iv)
#define glProgramUniform3ui GLEW_GET_FUN(G_glewProgramUniform3ui)
#define glProgramUniform3uiv GLEW_GET_FUN(G_glewProgramUniform3uiv)
#define glProgramUniform4d GLEW_GET_FUN(G_glewProgramUniform4d)
#define glProgramUniform4dv GLEW_GET_FUN(G_glewProgramUniform4dv)
#define glProgramUniform4f GLEW_GET_FUN(G_glewProgramUniform4f)
#define glProgramUniform4fv GLEW_GET_FUN(G_glewProgramUniform4fv)
#define glProgramUniform4i GLEW_GET_FUN(G_glewProgramUniform4i)
#define glProgramUniform4iv GLEW_GET_FUN(G_glewProgramUniform4iv)
#define glProgramUniform4ui GLEW_GET_FUN(G_glewProgramUniform4ui)
#define glProgramUniform4uiv GLEW_GET_FUN(G_glewProgramUniform4uiv)
#define glProgramUniformMatrix2dv GLEW_GET_FUN(G_glewProgramUniformMatrix2dv)
#define glProgramUniformMatrix2fv GLEW_GET_FUN(G_glewProgramUniformMatrix2fv)
#define glProgramUniformMatrix2x3dv GLEW_GET_FUN(G_glewProgramUniformMatrix2x3dv)
#define glProgramUniformMatrix2x3fv GLEW_GET_FUN(G_glewProgramUniformMatrix2x3fv)
#define glProgramUniformMatrix2x4dv GLEW_GET_FUN(G_glewProgramUniformMatrix2x4dv)
#define glProgramUniformMatrix2x4fv GLEW_GET_FUN(G_glewProgramUniformMatrix2x4fv)
#define glProgramUniformMatrix3dv GLEW_GET_FUN(G_glewProgramUniformMatrix3dv)
#define glProgramUniformMatrix3fv GLEW_GET_FUN(G_glewProgramUniformMatrix3fv)
#define glProgramUniformMatrix3x2dv GLEW_GET_FUN(G_glewProgramUniformMatrix3x2dv)
#define glProgramUniformMatrix3x2fv GLEW_GET_FUN(G_glewProgramUniformMatrix3x2fv)
#define glProgramUniformMatrix3x4dv GLEW_GET_FUN(G_glewProgramUniformMatrix3x4dv)
#define glProgramUniformMatrix3x4fv GLEW_GET_FUN(G_glewProgramUniformMatrix3x4fv)
#define glProgramUniformMatrix4dv GLEW_GET_FUN(G_glewProgramUniformMatrix4dv)
#define glProgramUniformMatrix4fv GLEW_GET_FUN(G_glewProgramUniformMatrix4fv)
#define glProgramUniformMatrix4x2dv GLEW_GET_FUN(G_glewProgramUniformMatrix4x2dv)
#define glProgramUniformMatrix4x2fv GLEW_GET_FUN(G_glewProgramUniformMatrix4x2fv)
#define glProgramUniformMatrix4x3dv GLEW_GET_FUN(G_glewProgramUniformMatrix4x3dv)
#define glProgramUniformMatrix4x3fv GLEW_GET_FUN(G_glewProgramUniformMatrix4x3fv)
#define glUseProgramStages GLEW_GET_FUN(G_glewUseProgramStages)
#define glValidateProgramPipeline GLEW_GET_FUN(G_glewValidateProgramPipeline)

#define GLEW_ARB_separate_shader_objects GLEW_GET_VAR(G_GLEW_ARB_separate_shader_objects)

#endif /* GL_ARB_separate_shader_objects */

/* --------------------- GL_ARB_shader_atomic_counters --------------------- */

#ifndef GL_ARB_shader_atomic_counters
#define GL_ARB_shader_atomic_counters 1

#define GL_ATOMIC_COUNTER_BUFFER 0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_BINDING 0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START 0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE 0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE 0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS 0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES 0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER 0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER 0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER 0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER 0x92CB
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS 0x92CC
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS 0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS 0x92CE
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS 0x92CF
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS 0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS 0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS 0x92D2
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS 0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS 0x92D4
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS 0x92D5
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS 0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS 0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE 0x92D8
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS 0x92D9
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX 0x92DA
#define GL_UNSIGNED_INT_ATOMIC_COUNTER 0x92DB
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS 0x92DC

typedef void (GLAPIENTRY * PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC) (GLuint program, GLuint bufferIndex, GLenum pname, GLint* params);

#define glGetActiveAtomicCounterBufferiv GLEW_GET_FUN(G_glewGetActiveAtomicCounterBufferiv)

#define GLEW_ARB_shader_atomic_counters GLEW_GET_VAR(G_GLEW_ARB_shader_atomic_counters)

#endif /* GL_ARB_shader_atomic_counters */

/* ----------------------- GL_ARB_shader_bit_encoding ---------------------- */

#ifndef GL_ARB_shader_bit_encoding
#define GL_ARB_shader_bit_encoding 1

#define GLEW_ARB_shader_bit_encoding GLEW_GET_VAR(G_GLEW_ARB_shader_bit_encoding)

#endif /* GL_ARB_shader_bit_encoding */

/* --------------------- GL_ARB_shader_image_load_store -------------------- */

#ifndef GL_ARB_shader_image_load_store
#define GL_ARB_shader_image_load_store 1

#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT 0x00000002
#define GL_UNIFORM_BARRIER_BIT 0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT 0x00000020
#define GL_COMMAND_BARRIER_BIT 0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT 0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT 0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT 0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT 0x00001000
#define GL_MAX_IMAGE_UNITS 0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS 0x8F39
#define GL_IMAGE_BINDING_NAME 0x8F3A
#define GL_IMAGE_BINDING_LEVEL 0x8F3B
#define GL_IMAGE_BINDING_LAYERED 0x8F3C
#define GL_IMAGE_BINDING_LAYER 0x8F3D
#define GL_IMAGE_BINDING_ACCESS 0x8F3E
#define GL_IMAGE_1D 0x904C
#define GL_IMAGE_2D 0x904D
#define GL_IMAGE_3D 0x904E
#define GL_IMAGE_2D_RECT 0x904F
#define GL_IMAGE_CUBE 0x9050
#define GL_IMAGE_BUFFER 0x9051
#define GL_IMAGE_1D_ARRAY 0x9052
#define GL_IMAGE_2D_ARRAY 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY 0x9054
#define GL_IMAGE_2D_MULTISAMPLE 0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY 0x9056
#define GL_INT_IMAGE_1D 0x9057
#define GL_INT_IMAGE_2D 0x9058
#define GL_INT_IMAGE_3D 0x9059
#define GL_INT_IMAGE_2D_RECT 0x905A
#define GL_INT_IMAGE_CUBE 0x905B
#define GL_INT_IMAGE_BUFFER 0x905C
#define GL_INT_IMAGE_1D_ARRAY 0x905D
#define GL_INT_IMAGE_2D_ARRAY 0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY 0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE 0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D 0x9062
#define GL_UNSIGNED_INT_IMAGE_2D 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D 0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE 0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER 0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY 0x906C
#define GL_MAX_IMAGE_SAMPLES 0x906D
#define GL_IMAGE_BINDING_FORMAT 0x906E
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE 0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE 0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS 0x90C9
#define GL_MAX_VERTEX_IMAGE_UNIFORMS 0x90CA
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS 0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS 0x90CC
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS 0x90CD
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS 0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS 0x90CF
#define GL_ALL_BARRIER_BITS 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (GLAPIENTRY * PFNGLMEMORYBARRIERPROC) (GLbitfield barriers);

#define glBindImageTexture GLEW_GET_FUN(G_glewBindImageTexture)
#define glMemoryBarrier GLEW_GET_FUN(G_glewMemoryBarrier)

#define GLEW_ARB_shader_image_load_store GLEW_GET_VAR(G_GLEW_ARB_shader_image_load_store)

#endif /* GL_ARB_shader_image_load_store */

/* ------------------------ GL_ARB_shader_image_size ----------------------- */

#ifndef GL_ARB_shader_image_size
#define GL_ARB_shader_image_size 1

#define GLEW_ARB_shader_image_size GLEW_GET_VAR(G_GLEW_ARB_shader_image_size)

#endif /* GL_ARB_shader_image_size */

/* ------------------------- GL_ARB_shader_objects ------------------------- */

#ifndef GL_ARB_shader_objects
#define GL_ARB_shader_objects 1

#define GL_PROGRAM_OBJECT_ARB 0x8B40
#define GL_SHADER_OBJECT_ARB 0x8B48
#define GL_OBJECT_TYPE_ARB 0x8B4E
#define GL_OBJECT_SUBTYPE_ARB 0x8B4F
#define GL_FLOAT_VEC2_ARB 0x8B50
#define GL_FLOAT_VEC3_ARB 0x8B51
#define GL_FLOAT_VEC4_ARB 0x8B52
#define GL_INT_VEC2_ARB 0x8B53
#define GL_INT_VEC3_ARB 0x8B54
#define GL_INT_VEC4_ARB 0x8B55
#define GL_BOOL_ARB 0x8B56
#define GL_BOOL_VEC2_ARB 0x8B57
#define GL_BOOL_VEC3_ARB 0x8B58
#define GL_BOOL_VEC4_ARB 0x8B59
#define GL_FLOAT_MAT2_ARB 0x8B5A
#define GL_FLOAT_MAT3_ARB 0x8B5B
#define GL_FLOAT_MAT4_ARB 0x8B5C
#define GL_SAMPLER_1D_ARB 0x8B5D
#define GL_SAMPLER_2D_ARB 0x8B5E
#define GL_SAMPLER_3D_ARB 0x8B5F
#define GL_SAMPLER_CUBE_ARB 0x8B60
#define GL_SAMPLER_1D_SHADOW_ARB 0x8B61
#define GL_SAMPLER_2D_SHADOW_ARB 0x8B62
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64
#define GL_OBJECT_DELETE_STATUS_ARB 0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB 0x8B81
#define GL_OBJECT_LINK_STATUS_ARB 0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB 0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB 0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB 0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB 0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB 0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB 0x8B88

typedef char GLcharARB;
typedef unsigned int GLhandleARB;

typedef void (GLAPIENTRY * PFNGLATTACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB obj);
typedef void (GLAPIENTRY * PFNGLCOMPILESHADERARBPROC) (GLhandleARB shaderObj);
typedef GLhandleARB (GLAPIENTRY * PFNGLCREATEPROGRAMOBJECTARBPROC) (void);
typedef GLhandleARB (GLAPIENTRY * PFNGLCREATESHADEROBJECTARBPROC) (GLenum shaderType);
typedef void (GLAPIENTRY * PFNGLDELETEOBJECTARBPROC) (GLhandleARB obj);
typedef void (GLAPIENTRY * PFNGLDETACHOBJECTARBPROC) (GLhandleARB containerObj, GLhandleARB attachedObj);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
typedef void (GLAPIENTRY * PFNGLGETATTACHEDOBJECTSARBPROC) (GLhandleARB containerObj, GLsizei maxCount, GLsizei* count, GLhandleARB *obj);
typedef GLhandleARB (GLAPIENTRY * PFNGLGETHANDLEARBPROC) (GLenum pname);
typedef void (GLAPIENTRY * PFNGLGETINFOLOGARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *infoLog);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERFVARBPROC) (GLhandleARB obj, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPARAMETERIVARBPROC) (GLhandleARB obj, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSHADERSOURCEARBPROC) (GLhandleARB obj, GLsizei maxLength, GLsizei* length, GLcharARB *source);
typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMFVARBPROC) (GLhandleARB programObj, GLint location, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMIVARBPROC) (GLhandleARB programObj, GLint location, GLint* params);
typedef void (GLAPIENTRY * PFNGLLINKPROGRAMARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLSHADERSOURCEARBPROC) (GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint *length);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FARBPROC) (GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IARBPROC) (GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FARBPROC) (GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IARBPROC) (GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FARBPROC) (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4FVARBPROC) (GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IARBPROC) (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4IVARBPROC) (GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX2FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX3FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMMATRIX4FVARBPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLUSEPROGRAMOBJECTARBPROC) (GLhandleARB programObj);
typedef void (GLAPIENTRY * PFNGLVALIDATEPROGRAMARBPROC) (GLhandleARB programObj);

#define glAttachObjectARB GLEW_GET_FUN(G_glewAttachObjectARB)
#define glCompileShaderARB GLEW_GET_FUN(G_glewCompileShaderARB)
#define glCreateProgramObjectARB GLEW_GET_FUN(G_glewCreateProgramObjectARB)
#define glCreateShaderObjectARB GLEW_GET_FUN(G_glewCreateShaderObjectARB)
#define glDeleteObjectARB GLEW_GET_FUN(G_glewDeleteObjectARB)
#define glDetachObjectARB GLEW_GET_FUN(G_glewDetachObjectARB)
#define glGetActiveUniformARB GLEW_GET_FUN(G_glewGetActiveUniformARB)
#define glGetAttachedObjectsARB GLEW_GET_FUN(G_glewGetAttachedObjectsARB)
#define glGetHandleARB GLEW_GET_FUN(G_glewGetHandleARB)
#define glGetInfoLogARB GLEW_GET_FUN(G_glewGetInfoLogARB)
#define glGetObjectParameterfvARB GLEW_GET_FUN(G_glewGetObjectParameterfvARB)
#define glGetObjectParameterivARB GLEW_GET_FUN(G_glewGetObjectParameterivARB)
#define glGetShaderSourceARB GLEW_GET_FUN(G_glewGetShaderSourceARB)
#define glGetUniformLocationARB GLEW_GET_FUN(G_glewGetUniformLocationARB)
#define glGetUniformfvARB GLEW_GET_FUN(G_glewGetUniformfvARB)
#define glGetUniformivARB GLEW_GET_FUN(G_glewGetUniformivARB)
#define glLinkProgramARB GLEW_GET_FUN(G_glewLinkProgramARB)
#define glShaderSourceARB GLEW_GET_FUN(G_glewShaderSourceARB)
#define glUniform1fARB GLEW_GET_FUN(G_glewUniform1fARB)
#define glUniform1fvARB GLEW_GET_FUN(G_glewUniform1fvARB)
#define glUniform1iARB GLEW_GET_FUN(G_glewUniform1iARB)
#define glUniform1ivARB GLEW_GET_FUN(G_glewUniform1ivARB)
#define glUniform2fARB GLEW_GET_FUN(G_glewUniform2fARB)
#define glUniform2fvARB GLEW_GET_FUN(G_glewUniform2fvARB)
#define glUniform2iARB GLEW_GET_FUN(G_glewUniform2iARB)
#define glUniform2ivARB GLEW_GET_FUN(G_glewUniform2ivARB)
#define glUniform3fARB GLEW_GET_FUN(G_glewUniform3fARB)
#define glUniform3fvARB GLEW_GET_FUN(G_glewUniform3fvARB)
#define glUniform3iARB GLEW_GET_FUN(G_glewUniform3iARB)
#define glUniform3ivARB GLEW_GET_FUN(G_glewUniform3ivARB)
#define glUniform4fARB GLEW_GET_FUN(G_glewUniform4fARB)
#define glUniform4fvARB GLEW_GET_FUN(G_glewUniform4fvARB)
#define glUniform4iARB GLEW_GET_FUN(G_glewUniform4iARB)
#define glUniform4ivARB GLEW_GET_FUN(G_glewUniform4ivARB)
#define glUniformMatrix2fvARB GLEW_GET_FUN(G_glewUniformMatrix2fvARB)
#define glUniformMatrix3fvARB GLEW_GET_FUN(G_glewUniformMatrix3fvARB)
#define glUniformMatrix4fvARB GLEW_GET_FUN(G_glewUniformMatrix4fvARB)
#define glUseProgramObjectARB GLEW_GET_FUN(G_glewUseProgramObjectARB)
#define glValidateProgramARB GLEW_GET_FUN(G_glewValidateProgramARB)

#define GLEW_ARB_shader_objects GLEW_GET_VAR(G_GLEW_ARB_shader_objects)

#endif /* GL_ARB_shader_objects */

/* ------------------------ GL_ARB_shader_precision ------------------------ */

#ifndef GL_ARB_shader_precision
#define GL_ARB_shader_precision 1

#define GLEW_ARB_shader_precision GLEW_GET_VAR(G_GLEW_ARB_shader_precision)

#endif /* GL_ARB_shader_precision */

/* ---------------------- GL_ARB_shader_stencil_export --------------------- */

#ifndef GL_ARB_shader_stencil_export
#define GL_ARB_shader_stencil_export 1

#define GLEW_ARB_shader_stencil_export GLEW_GET_VAR(G_GLEW_ARB_shader_stencil_export)

#endif /* GL_ARB_shader_stencil_export */

/* ------------------ GL_ARB_shader_storage_buffer_object ------------------ */

#ifndef GL_ARB_shader_storage_buffer_object
#define GL_ARB_shader_storage_buffer_object 1

#define GL_SHADER_STORAGE_BARRIER_BIT 0x2000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES 0x8F39
#define GL_SHADER_STORAGE_BUFFER 0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING 0x90D3
#define GL_SHADER_STORAGE_BUFFER_START 0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS 0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS 0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS 0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS 0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS 0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS 0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS 0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS 0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE 0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT 0x90DF

typedef void (GLAPIENTRY * PFNGLSHADERSTORAGEBLOCKBINDINGPROC) (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);

#define glShaderStorageBlockBinding GLEW_GET_FUN(G_glewShaderStorageBlockBinding)

#define GLEW_ARB_shader_storage_buffer_object GLEW_GET_VAR(G_GLEW_ARB_shader_storage_buffer_object)

#endif /* GL_ARB_shader_storage_buffer_object */

/* ------------------------ GL_ARB_shader_subroutine ----------------------- */

#ifndef GL_ARB_shader_subroutine
#define GL_ARB_shader_subroutine 1

#define GL_ACTIVE_SUBROUTINES 0x8DE5
#define GL_ACTIVE_SUBROUTINE_UNIFORMS 0x8DE6
#define GL_MAX_SUBROUTINES 0x8DE7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS 0x8DE8
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS 0x8E47
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH 0x8E48
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH 0x8E49
#define GL_NUM_COMPATIBLE_SUBROUTINES 0x8E4A
#define GL_COMPATIBLE_SUBROUTINES 0x8E4B

typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINENAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufsize, GLsizei* length, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint* values);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTAGEIVPROC) (GLuint program, GLenum shadertype, GLenum pname, GLint* values);
typedef GLuint (GLAPIENTRY * PFNGLGETSUBROUTINEINDEXPROC) (GLuint program, GLenum shadertype, const GLchar* name);
typedef GLint (GLAPIENTRY * PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) (GLuint program, GLenum shadertype, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMSUBROUTINEUIVPROC) (GLenum shadertype, GLint location, GLuint* params);
typedef void (GLAPIENTRY * PFNGLUNIFORMSUBROUTINESUIVPROC) (GLenum shadertype, GLsizei count, const GLuint* indices);

#define glGetActiveSubroutineName GLEW_GET_FUN(G_glewGetActiveSubroutineName)
#define glGetActiveSubroutineUniformName GLEW_GET_FUN(G_glewGetActiveSubroutineUniformName)
#define glGetActiveSubroutineUniformiv GLEW_GET_FUN(G_glewGetActiveSubroutineUniformiv)
#define glGetProgramStageiv GLEW_GET_FUN(G_glewGetProgramStageiv)
#define glGetSubroutineIndex GLEW_GET_FUN(G_glewGetSubroutineIndex)
#define glGetSubroutineUniformLocation GLEW_GET_FUN(G_glewGetSubroutineUniformLocation)
#define glGetUniformSubroutineuiv GLEW_GET_FUN(G_glewGetUniformSubroutineuiv)
#define glUniformSubroutinesuiv GLEW_GET_FUN(G_glewUniformSubroutinesuiv)

#define GLEW_ARB_shader_subroutine GLEW_GET_VAR(G_GLEW_ARB_shader_subroutine)

#endif /* GL_ARB_shader_subroutine */

/* ----------------------- GL_ARB_shader_texture_lod ----------------------- */

#ifndef GL_ARB_shader_texture_lod
#define GL_ARB_shader_texture_lod 1

#define GLEW_ARB_shader_texture_lod GLEW_GET_VAR(G_GLEW_ARB_shader_texture_lod)

#endif /* GL_ARB_shader_texture_lod */

/* ---------------------- GL_ARB_shading_language_100 ---------------------- */

#ifndef GL_ARB_shading_language_100
#define GL_ARB_shading_language_100 1

#define GL_SHADING_LANGUAGE_VERSION_ARB 0x8B8C

#define GLEW_ARB_shading_language_100 GLEW_GET_VAR(G_GLEW_ARB_shading_language_100)

#endif /* GL_ARB_shading_language_100 */

/* -------------------- GL_ARB_shading_language_420pack -------------------- */

#ifndef GL_ARB_shading_language_420pack
#define GL_ARB_shading_language_420pack 1

#define GLEW_ARB_shading_language_420pack GLEW_GET_VAR(G_GLEW_ARB_shading_language_420pack)

#endif /* GL_ARB_shading_language_420pack */

/* -------------------- GL_ARB_shading_language_include -------------------- */

#ifndef GL_ARB_shading_language_include
#define GL_ARB_shading_language_include 1

#define GL_SHADER_INCLUDE_ARB 0x8DAE
#define GL_NAMED_STRING_LENGTH_ARB 0x8DE9
#define GL_NAMED_STRING_TYPE_ARB 0x8DEA

typedef void (GLAPIENTRY * PFNGLCOMPILESHADERINCLUDEARBPROC) (GLuint shader, GLsizei count, const GLchar ** path, const GLint *length);
typedef void (GLAPIENTRY * PFNGLDELETENAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLGETNAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name, GLsizei bufSize, GLint *stringlen, GLchar *string);
typedef void (GLAPIENTRY * PFNGLGETNAMEDSTRINGIVARBPROC) (GLint namelen, const GLchar* name, GLenum pname, GLint *params);
typedef GLboolean (GLAPIENTRY * PFNGLISNAMEDSTRINGARBPROC) (GLint namelen, const GLchar* name);
typedef void (GLAPIENTRY * PFNGLNAMEDSTRINGARBPROC) (GLenum type, GLint namelen, const GLchar* name, GLint stringlen, const GLchar *string);

#define glCompileShaderIncludeARB GLEW_GET_FUN(G_glewCompileShaderIncludeARB)
#define glDeleteNamedStringARB GLEW_GET_FUN(G_glewDeleteNamedStringARB)
#define glGetNamedStringARB GLEW_GET_FUN(G_glewGetNamedStringARB)
#define glGetNamedStringivARB GLEW_GET_FUN(G_glewGetNamedStringivARB)
#define glIsNamedStringARB GLEW_GET_FUN(G_glewIsNamedStringARB)
#define glNamedStringARB GLEW_GET_FUN(G_glewNamedStringARB)

#define GLEW_ARB_shading_language_include GLEW_GET_VAR(G_GLEW_ARB_shading_language_include)

#endif /* GL_ARB_shading_language_include */

/* -------------------- GL_ARB_shading_language_packing -------------------- */

#ifndef GL_ARB_shading_language_packing
#define GL_ARB_shading_language_packing 1

#define GLEW_ARB_shading_language_packing GLEW_GET_VAR(G_GLEW_ARB_shading_language_packing)

#endif /* GL_ARB_shading_language_packing */

/* ----------------------------- GL_ARB_shadow ----------------------------- */

#ifndef GL_ARB_shadow
#define GL_ARB_shadow 1

#define GL_TEXTURE_COMPARE_MODE_ARB 0x884C
#define GL_TEXTURE_COMPARE_FUNC_ARB 0x884D
#define GL_COMPARE_R_TO_TEXTURE_ARB 0x884E

#define GLEW_ARB_shadow GLEW_GET_VAR(G_GLEW_ARB_shadow)

#endif /* GL_ARB_shadow */

/* ------------------------- GL_ARB_shadow_ambient ------------------------- */

#ifndef GL_ARB_shadow_ambient
#define GL_ARB_shadow_ambient 1

#define GL_TEXTURE_COMPARE_FAIL_VALUE_ARB 0x80BF

#define GLEW_ARB_shadow_ambient GLEW_GET_VAR(G_GLEW_ARB_shadow_ambient)

#endif /* GL_ARB_shadow_ambient */

/* ------------------------ GL_ARB_stencil_texturing ----------------------- */

#ifndef GL_ARB_stencil_texturing
#define GL_ARB_stencil_texturing 1

#define GL_DEPTH_STENCIL_TEXTURE_MODE 0x90EA

#define GLEW_ARB_stencil_texturing GLEW_GET_VAR(G_GLEW_ARB_stencil_texturing)

#endif /* GL_ARB_stencil_texturing */

/* ------------------------------ GL_ARB_sync ------------------------------ */

#ifndef GL_ARB_sync
#define GL_ARB_sync 1

#define GL_SYNC_FLUSH_COMMANDS_BIT 0x00000001
#define GL_MAX_SERVER_WAIT_TIMEOUT 0x9111
#define GL_OBJECT_TYPE 0x9112
#define GL_SYNC_CONDITION 0x9113
#define GL_SYNC_STATUS 0x9114
#define GL_SYNC_FLAGS 0x9115
#define GL_SYNC_FENCE 0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE 0x9117
#define GL_UNSIGNALED 0x9118
#define GL_SIGNALED 0x9119
#define GL_ALREADY_SIGNALED 0x911A
#define GL_TIMEOUT_EXPIRED 0x911B
#define GL_CONDITION_SATISFIED 0x911C
#define GL_WAIT_FAILED 0x911D
#define GL_TIMEOUT_IGNORED 0xFFFFFFFFFFFFFFFF

typedef GLenum (GLAPIENTRY * PFNGLCLIENTWAITSYNCPROC) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);
typedef void (GLAPIENTRY * PFNGLDELETESYNCPROC) (GLsync GLsync);
typedef GLsync (GLAPIENTRY * PFNGLFENCESYNCPROC) (GLenum condition,GLbitfield flags);
typedef void (GLAPIENTRY * PFNGLGETINTEGER64VPROC) (GLenum pname, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETSYNCIVPROC) (GLsync GLsync,GLenum pname,GLsizei bufSize,GLsizei* length, GLint *values);
typedef GLboolean (GLAPIENTRY * PFNGLISSYNCPROC) (GLsync GLsync);
typedef void (GLAPIENTRY * PFNGLWAITSYNCPROC) (GLsync GLsync,GLbitfield flags,GLuint64 timeout);

#define glClientWaitSync GLEW_GET_FUN(G_glewClientWaitSync)
#define glDeleteSync GLEW_GET_FUN(G_glewDeleteSync)
#define glFenceSync GLEW_GET_FUN(G_glewFenceSync)
#define glGetInteger64v GLEW_GET_FUN(G_glewGetInteger64v)
#define glGetSynciv GLEW_GET_FUN(G_glewGetSynciv)
#define glIsSync GLEW_GET_FUN(G_glewIsSync)
#define glWaitSync GLEW_GET_FUN(G_glewWaitSync)

#define GLEW_ARB_sync GLEW_GET_VAR(G_GLEW_ARB_sync)

#endif /* GL_ARB_sync */

/* ----------------------- GL_ARB_tessellation_shader ---------------------- */

#ifndef GL_ARB_tessellation_shader
#define GL_ARB_tessellation_shader 1

#define GL_PATCHES 0xE
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER 0x84F0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER 0x84F1
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS 0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS 0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E1F
#define GL_PATCH_VERTICES 0x8E72
#define GL_PATCH_DEFAULT_INNER_LEVEL 0x8E73
#define GL_PATCH_DEFAULT_OUTER_LEVEL 0x8E74
#define GL_TESS_CONTROL_OUTPUT_VERTICES 0x8E75
#define GL_TESS_GEN_MODE 0x8E76
#define GL_TESS_GEN_SPACING 0x8E77
#define GL_TESS_GEN_VERTEX_ORDER 0x8E78
#define GL_TESS_GEN_POINT_MODE 0x8E79
#define GL_ISOLINES 0x8E7A
#define GL_FRACTIONAL_ODD 0x8E7B
#define GL_FRACTIONAL_EVEN 0x8E7C
#define GL_MAX_PATCH_VERTICES 0x8E7D
#define GL_MAX_TESS_GEN_LEVEL 0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS 0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS 0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS 0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS 0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS 0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS 0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS 0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS 0x8E86
#define GL_TESS_EVALUATION_SHADER 0x8E87
#define GL_TESS_CONTROL_SHADER 0x8E88
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS 0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS 0x8E8A

typedef void (GLAPIENTRY * PFNGLPATCHPARAMETERFVPROC) (GLenum pname, const GLfloat* values);
typedef void (GLAPIENTRY * PFNGLPATCHPARAMETERIPROC) (GLenum pname, GLint value);

#define glPatchParameterfv GLEW_GET_FUN(G_glewPatchParameterfv)
#define glPatchParameteri GLEW_GET_FUN(G_glewPatchParameteri)

#define GLEW_ARB_tessellation_shader GLEW_GET_VAR(G_GLEW_ARB_tessellation_shader)

#endif /* GL_ARB_tessellation_shader */

/* ---------------------- GL_ARB_texture_border_clamp ---------------------- */

#ifndef GL_ARB_texture_border_clamp
#define GL_ARB_texture_border_clamp 1

#define GL_CLAMP_TO_BORDER_ARB 0x812D

#define GLEW_ARB_texture_border_clamp GLEW_GET_VAR(G_GLEW_ARB_texture_border_clamp)

#endif /* GL_ARB_texture_border_clamp */

/* ---------------------- GL_ARB_texture_buffer_object --------------------- */

#ifndef GL_ARB_texture_buffer_object
#define GL_ARB_texture_buffer_object 1

#define GL_TEXTURE_BUFFER_ARB 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE_ARB 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER_ARB 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_ARB 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT_ARB 0x8C2E

typedef void (GLAPIENTRY * PFNGLTEXBUFFERARBPROC) (GLenum target, GLenum internalformat, GLuint buffer);

#define glTexBufferARB GLEW_GET_FUN(G_glewTexBufferARB)

#define GLEW_ARB_texture_buffer_object GLEW_GET_VAR(G_GLEW_ARB_texture_buffer_object)

#endif /* GL_ARB_texture_buffer_object */

/* ------------------- GL_ARB_texture_buffer_object_rgb32 ------------------ */

#ifndef GL_ARB_texture_buffer_object_rgb32
#define GL_ARB_texture_buffer_object_rgb32 1

#define GLEW_ARB_texture_buffer_object_rgb32 GLEW_GET_VAR(G_GLEW_ARB_texture_buffer_object_rgb32)

#endif /* GL_ARB_texture_buffer_object_rgb32 */

/* ---------------------- GL_ARB_texture_buffer_range ---------------------- */

#ifndef GL_ARB_texture_buffer_range
#define GL_ARB_texture_buffer_range 1

#define GL_TEXTURE_BUFFER_OFFSET 0x919D
#define GL_TEXTURE_BUFFER_SIZE 0x919E
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT 0x919F

typedef void (GLAPIENTRY * PFNGLTEXBUFFERRANGEPROC) (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFERRANGEEXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);

#define glTexBufferRange GLEW_GET_FUN(G_glewTexBufferRange)
#define glTextureBufferRangeEXT GLEW_GET_FUN(G_glewTextureBufferRangeEXT)

#define GLEW_ARB_texture_buffer_range GLEW_GET_VAR(G_GLEW_ARB_texture_buffer_range)

#endif /* GL_ARB_texture_buffer_range */

/* ----------------------- GL_ARB_texture_compression ---------------------- */

#ifndef GL_ARB_texture_compression
#define GL_ARB_texture_compression 1

#define GL_COMPRESSED_ALPHA_ARB 0x84E9
#define GL_COMPRESSED_LUMINANCE_ARB 0x84EA
#define GL_COMPRESSED_LUMINANCE_ALPHA_ARB 0x84EB
#define GL_COMPRESSED_INTENSITY_ARB 0x84EC
#define GL_COMPRESSED_RGB_ARB 0x84ED
#define GL_COMPRESSED_RGBA_ARB 0x84EE
#define GL_TEXTURE_COMPRESSION_HINT_ARB 0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE_ARB 0x86A0
#define GL_TEXTURE_COMPRESSED_ARB 0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS_ARB 0x86A3

typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE1DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE2DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXIMAGE3DARBPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXIMAGEARBPROC) (GLenum target, GLint lod, void* img);

#define glCompressedTexImage1DARB GLEW_GET_FUN(G_glewCompressedTexImage1DARB)
#define glCompressedTexImage2DARB GLEW_GET_FUN(G_glewCompressedTexImage2DARB)
#define glCompressedTexImage3DARB GLEW_GET_FUN(G_glewCompressedTexImage3DARB)
#define glCompressedTexSubImage1DARB GLEW_GET_FUN(G_glewCompressedTexSubImage1DARB)
#define glCompressedTexSubImage2DARB GLEW_GET_FUN(G_glewCompressedTexSubImage2DARB)
#define glCompressedTexSubImage3DARB GLEW_GET_FUN(G_glewCompressedTexSubImage3DARB)
#define glGetCompressedTexImageARB GLEW_GET_FUN(G_glewGetCompressedTexImageARB)

#define GLEW_ARB_texture_compression GLEW_GET_VAR(G_GLEW_ARB_texture_compression)

#endif /* GL_ARB_texture_compression */

/* -------------------- GL_ARB_texture_compression_bptc -------------------- */

#ifndef GL_ARB_texture_compression_bptc
#define GL_ARB_texture_compression_bptc 1

#define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB 0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB 0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB 0x8E8F

#define GLEW_ARB_texture_compression_bptc GLEW_GET_VAR(G_GLEW_ARB_texture_compression_bptc)

#endif /* GL_ARB_texture_compression_bptc */

/* -------------------- GL_ARB_texture_compression_rgtc -------------------- */

#ifndef GL_ARB_texture_compression_rgtc
#define GL_ARB_texture_compression_rgtc 1

#define GL_COMPRESSED_RED_RGTC1 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1 0x8DBC
#define GL_COMPRESSED_RG_RGTC2 0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2 0x8DBE

#define GLEW_ARB_texture_compression_rgtc GLEW_GET_VAR(G_GLEW_ARB_texture_compression_rgtc)

#endif /* GL_ARB_texture_compression_rgtc */

/* ------------------------ GL_ARB_texture_cube_map ------------------------ */

#ifndef GL_ARB_texture_cube_map
#define GL_ARB_texture_cube_map 1

#define GL_NORMAL_MAP_ARB 0x8511
#define GL_REFLECTION_MAP_ARB 0x8512
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB 0x851C

#define GLEW_ARB_texture_cube_map GLEW_GET_VAR(G_GLEW_ARB_texture_cube_map)

#endif /* GL_ARB_texture_cube_map */

/* --------------------- GL_ARB_texture_cube_map_array --------------------- */

#ifndef GL_ARB_texture_cube_map_array
#define GL_ARB_texture_cube_map_array 1

#define GL_TEXTURE_CUBE_MAP_ARRAY_ARB 0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB 0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB 0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB 0x900F

#define GLEW_ARB_texture_cube_map_array GLEW_GET_VAR(G_GLEW_ARB_texture_cube_map_array)

#endif /* GL_ARB_texture_cube_map_array */

/* ------------------------- GL_ARB_texture_env_add ------------------------ */

#ifndef GL_ARB_texture_env_add
#define GL_ARB_texture_env_add 1

#define GLEW_ARB_texture_env_add GLEW_GET_VAR(G_GLEW_ARB_texture_env_add)

#endif /* GL_ARB_texture_env_add */

/* ----------------------- GL_ARB_texture_env_combine ---------------------- */

#ifndef GL_ARB_texture_env_combine
#define GL_ARB_texture_env_combine 1

#define GL_SUBTRACT_ARB 0x84E7
#define GL_COMBINE_ARB 0x8570
#define GL_COMBINE_RGB_ARB 0x8571
#define GL_COMBINE_ALPHA_ARB 0x8572
#define GL_RGB_SCALE_ARB 0x8573
#define GL_ADD_SIGNED_ARB 0x8574
#define GL_INTERPOLATE_ARB 0x8575
#define GL_CONSTANT_ARB 0x8576
#define GL_PRIMARY_COLOR_ARB 0x8577
#define GL_PREVIOUS_ARB 0x8578
#define GL_SOURCE0_RGB_ARB 0x8580
#define GL_SOURCE1_RGB_ARB 0x8581
#define GL_SOURCE2_RGB_ARB 0x8582
#define GL_SOURCE0_ALPHA_ARB 0x8588
#define GL_SOURCE1_ALPHA_ARB 0x8589
#define GL_SOURCE2_ALPHA_ARB 0x858A
#define GL_OPERAND0_RGB_ARB 0x8590
#define GL_OPERAND1_RGB_ARB 0x8591
#define GL_OPERAND2_RGB_ARB 0x8592
#define GL_OPERAND0_ALPHA_ARB 0x8598
#define GL_OPERAND1_ALPHA_ARB 0x8599
#define GL_OPERAND2_ALPHA_ARB 0x859A

#define GLEW_ARB_texture_env_combine GLEW_GET_VAR(G_GLEW_ARB_texture_env_combine)

#endif /* GL_ARB_texture_env_combine */

/* ---------------------- GL_ARB_texture_env_crossbar ---------------------- */

#ifndef GL_ARB_texture_env_crossbar
#define GL_ARB_texture_env_crossbar 1

#define GLEW_ARB_texture_env_crossbar GLEW_GET_VAR(G_GLEW_ARB_texture_env_crossbar)

#endif /* GL_ARB_texture_env_crossbar */

/* ------------------------ GL_ARB_texture_env_dot3 ------------------------ */

#ifndef GL_ARB_texture_env_dot3
#define GL_ARB_texture_env_dot3 1

#define GL_DOT3_RGB_ARB 0x86AE
#define GL_DOT3_RGBA_ARB 0x86AF

#define GLEW_ARB_texture_env_dot3 GLEW_GET_VAR(G_GLEW_ARB_texture_env_dot3)

#endif /* GL_ARB_texture_env_dot3 */

/* -------------------------- GL_ARB_texture_float ------------------------- */

#ifndef GL_ARB_texture_float
#define GL_ARB_texture_float 1

#define GL_RGBA32F_ARB 0x8814
#define GL_RGB32F_ARB 0x8815
#define GL_ALPHA32F_ARB 0x8816
#define GL_INTENSITY32F_ARB 0x8817
#define GL_LUMINANCE32F_ARB 0x8818
#define GL_LUMINANCE_ALPHA32F_ARB 0x8819
#define GL_RGBA16F_ARB 0x881A
#define GL_RGB16F_ARB 0x881B
#define GL_ALPHA16F_ARB 0x881C
#define GL_INTENSITY16F_ARB 0x881D
#define GL_LUMINANCE16F_ARB 0x881E
#define GL_LUMINANCE_ALPHA16F_ARB 0x881F
#define GL_TEXTURE_RED_TYPE_ARB 0x8C10
#define GL_TEXTURE_GREEN_TYPE_ARB 0x8C11
#define GL_TEXTURE_BLUE_TYPE_ARB 0x8C12
#define GL_TEXTURE_ALPHA_TYPE_ARB 0x8C13
#define GL_TEXTURE_LUMINANCE_TYPE_ARB 0x8C14
#define GL_TEXTURE_INTENSITY_TYPE_ARB 0x8C15
#define GL_TEXTURE_DEPTH_TYPE_ARB 0x8C16
#define GL_UNSIGNED_NORMALIZED_ARB 0x8C17

#define GLEW_ARB_texture_float GLEW_GET_VAR(G_GLEW_ARB_texture_float)

#endif /* GL_ARB_texture_float */

/* ------------------------- GL_ARB_texture_gather ------------------------- */

#ifndef GL_ARB_texture_gather
#define GL_ARB_texture_gather 1

#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB 0x8F9F

#define GLEW_ARB_texture_gather GLEW_GET_VAR(G_GLEW_ARB_texture_gather)

#endif /* GL_ARB_texture_gather */

/* --------------------- GL_ARB_texture_mirrored_repeat -------------------- */

#ifndef GL_ARB_texture_mirrored_repeat
#define GL_ARB_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_ARB 0x8370

#define GLEW_ARB_texture_mirrored_repeat GLEW_GET_VAR(G_GLEW_ARB_texture_mirrored_repeat)

#endif /* GL_ARB_texture_mirrored_repeat */

/* ----------------------- GL_ARB_texture_multisample ---------------------- */

#ifndef GL_ARB_texture_multisample
#define GL_ARB_texture_multisample 1

#define GL_SAMPLE_POSITION 0x8E50
#define GL_SAMPLE_MASK 0x8E51
#define GL_SAMPLE_MASK_VALUE 0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS 0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE 0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE 0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_SAMPLES 0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_SAMPLER_2D_MULTISAMPLE 0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES 0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES 0x910F
#define GL_MAX_INTEGER_SAMPLES 0x9110

typedef void (GLAPIENTRY * PFNGLGETMULTISAMPLEFVPROC) (GLenum pname, GLuint index, GLfloat* val);
typedef void (GLAPIENTRY * PFNGLSAMPLEMASKIPROC) (GLuint index, GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

#define glGetMultisamplefv GLEW_GET_FUN(G_glewGetMultisamplefv)
#define glSampleMaski GLEW_GET_FUN(G_glewSampleMaski)
#define glTexImage2DMultisample GLEW_GET_FUN(G_glewTexImage2DMultisample)
#define glTexImage3DMultisample GLEW_GET_FUN(G_glewTexImage3DMultisample)

#define GLEW_ARB_texture_multisample GLEW_GET_VAR(G_GLEW_ARB_texture_multisample)

#endif /* GL_ARB_texture_multisample */

/* -------------------- GL_ARB_texture_non_power_of_two -------------------- */

#ifndef GL_ARB_texture_non_power_of_two
#define GL_ARB_texture_non_power_of_two 1

#define GLEW_ARB_texture_non_power_of_two GLEW_GET_VAR(G_GLEW_ARB_texture_non_power_of_two)

#endif /* GL_ARB_texture_non_power_of_two */

/* ---------------------- GL_ARB_texture_query_levels ---------------------- */

#ifndef GL_ARB_texture_query_levels
#define GL_ARB_texture_query_levels 1

#define GLEW_ARB_texture_query_levels GLEW_GET_VAR(G_GLEW_ARB_texture_query_levels)

#endif /* GL_ARB_texture_query_levels */

/* ------------------------ GL_ARB_texture_query_lod ----------------------- */

#ifndef GL_ARB_texture_query_lod
#define GL_ARB_texture_query_lod 1

#define GLEW_ARB_texture_query_lod GLEW_GET_VAR(G_GLEW_ARB_texture_query_lod)

#endif /* GL_ARB_texture_query_lod */

/* ------------------------ GL_ARB_texture_rectangle ----------------------- */

#ifndef GL_ARB_texture_rectangle
#define GL_ARB_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_ARB 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_ARB 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_ARB 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_ARB 0x84F8
#define GL_SAMPLER_2D_RECT_ARB 0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB 0x8B64

#define GLEW_ARB_texture_rectangle GLEW_GET_VAR(G_GLEW_ARB_texture_rectangle)

#endif /* GL_ARB_texture_rectangle */

/* --------------------------- GL_ARB_texture_rg --------------------------- */

#ifndef GL_ARB_texture_rg
#define GL_ARB_texture_rg 1

#define GL_COMPRESSED_RED 0x8225
#define GL_COMPRESSED_RG 0x8226
#define GL_RG 0x8227
#define GL_RG_INTEGER 0x8228
#define GL_R8 0x8229
#define GL_R16 0x822A
#define GL_RG8 0x822B
#define GL_RG16 0x822C
#define GL_R16F 0x822D
#define GL_R32F 0x822E
#define GL_RG16F 0x822F
#define GL_RG32F 0x8230
#define GL_R8I 0x8231
#define GL_R8UI 0x8232
#define GL_R16I 0x8233
#define GL_R16UI 0x8234
#define GL_R32I 0x8235
#define GL_R32UI 0x8236
#define GL_RG8I 0x8237
#define GL_RG8UI 0x8238
#define GL_RG16I 0x8239
#define GL_RG16UI 0x823A
#define GL_RG32I 0x823B
#define GL_RG32UI 0x823C

#define GLEW_ARB_texture_rg GLEW_GET_VAR(G_GLEW_ARB_texture_rg)

#endif /* GL_ARB_texture_rg */

/* ----------------------- GL_ARB_texture_rgb10_a2ui ----------------------- */

#ifndef GL_ARB_texture_rgb10_a2ui
#define GL_ARB_texture_rgb10_a2ui 1

#define GL_RGB10_A2UI 0x906F

#define GLEW_ARB_texture_rgb10_a2ui GLEW_GET_VAR(G_GLEW_ARB_texture_rgb10_a2ui)

#endif /* GL_ARB_texture_rgb10_a2ui */

/* ------------------------- GL_ARB_texture_storage ------------------------ */

#ifndef GL_ARB_texture_storage
#define GL_ARB_texture_storage 1

#define GL_TEXTURE_IMMUTABLE_FORMAT 0x912F

typedef void (GLAPIENTRY * PFNGLTEXSTORAGE1DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE3DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE1DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DEXTPROC) (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

#define glTexStorage1D GLEW_GET_FUN(G_glewTexStorage1D)
#define glTexStorage2D GLEW_GET_FUN(G_glewTexStorage2D)
#define glTexStorage3D GLEW_GET_FUN(G_glewTexStorage3D)
#define glTextureStorage1DEXT GLEW_GET_FUN(G_glewTextureStorage1DEXT)
#define glTextureStorage2DEXT GLEW_GET_FUN(G_glewTextureStorage2DEXT)
#define glTextureStorage3DEXT GLEW_GET_FUN(G_glewTextureStorage3DEXT)

#define GLEW_ARB_texture_storage GLEW_GET_VAR(G_GLEW_ARB_texture_storage)

#endif /* GL_ARB_texture_storage */

/* ------------------- GL_ARB_texture_storage_multisample ------------------ */

#ifndef GL_ARB_texture_storage_multisample
#define GL_ARB_texture_storage_multisample 1

typedef void (GLAPIENTRY * PFNGLTEXSTORAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXSTORAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC) (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void (GLAPIENTRY * PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC) (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);

#define glTexStorage2DMultisample GLEW_GET_FUN(G_glewTexStorage2DMultisample)
#define glTexStorage3DMultisample GLEW_GET_FUN(G_glewTexStorage3DMultisample)
#define glTextureStorage2DMultisampleEXT GLEW_GET_FUN(G_glewTextureStorage2DMultisampleEXT)
#define glTextureStorage3DMultisampleEXT GLEW_GET_FUN(G_glewTextureStorage3DMultisampleEXT)

#define GLEW_ARB_texture_storage_multisample GLEW_GET_VAR(G_GLEW_ARB_texture_storage_multisample)

#endif /* GL_ARB_texture_storage_multisample */

/* ------------------------- GL_ARB_texture_swizzle ------------------------ */

#ifndef GL_ARB_texture_swizzle
#define GL_ARB_texture_swizzle 1

#define GL_TEXTURE_SWIZZLE_R 0x8E42
#define GL_TEXTURE_SWIZZLE_G 0x8E43
#define GL_TEXTURE_SWIZZLE_B 0x8E44
#define GL_TEXTURE_SWIZZLE_A 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA 0x8E46

#define GLEW_ARB_texture_swizzle GLEW_GET_VAR(G_GLEW_ARB_texture_swizzle)

#endif /* GL_ARB_texture_swizzle */

/* -------------------------- GL_ARB_texture_view -------------------------- */

#ifndef GL_ARB_texture_view
#define GL_ARB_texture_view 1

#define GL_TEXTURE_VIEW_MIN_LEVEL 0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS 0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER 0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS 0x82DE
#define GL_TEXTURE_IMMUTABLE_LEVELS 0x82DF

typedef void (GLAPIENTRY * PFNGLTEXTUREVIEWPROC) (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);

#define glTextureView GLEW_GET_FUN(G_glewTextureView)

#define GLEW_ARB_texture_view GLEW_GET_VAR(G_GLEW_ARB_texture_view)

#endif /* GL_ARB_texture_view */

/* --------------------------- GL_ARB_timer_query -------------------------- */

#ifndef GL_ARB_timer_query
#define GL_ARB_timer_query 1

#define GL_TIME_ELAPSED 0x88BF
#define GL_TIMESTAMP 0x8E28

typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTI64VPROC) (GLuint id, GLenum pname, GLint64* params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUI64VPROC) (GLuint id, GLenum pname, GLuint64* params);
typedef void (GLAPIENTRY * PFNGLQUERYCOUNTERPROC) (GLuint id, GLenum target);

#define glGetQueryObjecti64v GLEW_GET_FUN(G_glewGetQueryObjecti64v)
#define glGetQueryObjectui64v GLEW_GET_FUN(G_glewGetQueryObjectui64v)
#define glQueryCounter GLEW_GET_FUN(G_glewQueryCounter)

#define GLEW_ARB_timer_query GLEW_GET_VAR(G_GLEW_ARB_timer_query)

#endif /* GL_ARB_timer_query */

/* ----------------------- GL_ARB_transform_feedback2 ---------------------- */

#ifndef GL_ARB_transform_feedback2
#define GL_ARB_transform_feedback2 1

#define GL_TRANSFORM_FEEDBACK 0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING 0x8E25

typedef void (GLAPIENTRY * PFNGLBINDTRANSFORMFEEDBACKPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETETRANSFORMFEEDBACKSPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKPROC) (GLenum mode, GLuint id);
typedef void (GLAPIENTRY * PFNGLGENTRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint* ids);
typedef GLboolean (GLAPIENTRY * PFNGLISTRANSFORMFEEDBACKPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLPAUSETRANSFORMFEEDBACKPROC) (void);
typedef void (GLAPIENTRY * PFNGLRESUMETRANSFORMFEEDBACKPROC) (void);

#define glBindTransformFeedback GLEW_GET_FUN(G_glewBindTransformFeedback)
#define glDeleteTransformFeedbacks GLEW_GET_FUN(G_glewDeleteTransformFeedbacks)
#define glDrawTransformFeedback GLEW_GET_FUN(G_glewDrawTransformFeedback)
#define glGenTransformFeedbacks GLEW_GET_FUN(G_glewGenTransformFeedbacks)
#define glIsTransformFeedback GLEW_GET_FUN(G_glewIsTransformFeedback)
#define glPauseTransformFeedback GLEW_GET_FUN(G_glewPauseTransformFeedback)
#define glResumeTransformFeedback GLEW_GET_FUN(G_glewResumeTransformFeedback)

#define GLEW_ARB_transform_feedback2 GLEW_GET_VAR(G_GLEW_ARB_transform_feedback2)

#endif /* GL_ARB_transform_feedback2 */

/* ----------------------- GL_ARB_transform_feedback3 ---------------------- */

#ifndef GL_ARB_transform_feedback3
#define GL_ARB_transform_feedback3 1

#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS 0x8E70
#define GL_MAX_VERTEX_STREAMS 0x8E71

typedef void (GLAPIENTRY * PFNGLBEGINQUERYINDEXEDPROC) (GLenum target, GLuint index, GLuint id);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) (GLenum mode, GLuint id, GLuint stream);
typedef void (GLAPIENTRY * PFNGLENDQUERYINDEXEDPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETQUERYINDEXEDIVPROC) (GLenum target, GLuint index, GLenum pname, GLint* params);

#define glBeginQueryIndexed GLEW_GET_FUN(G_glewBeginQueryIndexed)
#define glDrawTransformFeedbackStream GLEW_GET_FUN(G_glewDrawTransformFeedbackStream)
#define glEndQueryIndexed GLEW_GET_FUN(G_glewEndQueryIndexed)
#define glGetQueryIndexediv GLEW_GET_FUN(G_glewGetQueryIndexediv)

#define GLEW_ARB_transform_feedback3 GLEW_GET_VAR(G_GLEW_ARB_transform_feedback3)

#endif /* GL_ARB_transform_feedback3 */

/* ------------------ GL_ARB_transform_feedback_instanced ------------------ */

#ifndef GL_ARB_transform_feedback_instanced
#define GL_ARB_transform_feedback_instanced 1

typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC) (GLenum mode, GLuint id, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC) (GLenum mode, GLuint id, GLuint stream, GLsizei primcount);

#define glDrawTransformFeedbackInstanced GLEW_GET_FUN(G_glewDrawTransformFeedbackInstanced)
#define glDrawTransformFeedbackStreamInstanced GLEW_GET_FUN(G_glewDrawTransformFeedbackStreamInstanced)

#define GLEW_ARB_transform_feedback_instanced GLEW_GET_VAR(G_GLEW_ARB_transform_feedback_instanced)

#endif /* GL_ARB_transform_feedback_instanced */

/* ------------------------ GL_ARB_transpose_matrix ------------------------ */

#ifndef GL_ARB_transpose_matrix
#define GL_ARB_transpose_matrix 1

#define GL_TRANSPOSE_MODELVIEW_MATRIX_ARB 0x84E3
#define GL_TRANSPOSE_PROJECTION_MATRIX_ARB 0x84E4
#define GL_TRANSPOSE_TEXTURE_MATRIX_ARB 0x84E5
#define GL_TRANSPOSE_COLOR_MATRIX_ARB 0x84E6

typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXDARBPROC) (GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLLOADTRANSPOSEMATRIXFARBPROC) (GLfloat m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXDARBPROC) (GLdouble m[16]);
typedef void (GLAPIENTRY * PFNGLMULTTRANSPOSEMATRIXFARBPROC) (GLfloat m[16]);

#define glLoadTransposeMatrixdARB GLEW_GET_FUN(G_glewLoadTransposeMatrixdARB)
#define glLoadTransposeMatrixfARB GLEW_GET_FUN(G_glewLoadTransposeMatrixfARB)
#define glMultTransposeMatrixdARB GLEW_GET_FUN(G_glewMultTransposeMatrixdARB)
#define glMultTransposeMatrixfARB GLEW_GET_FUN(G_glewMultTransposeMatrixfARB)

#define GLEW_ARB_transpose_matrix GLEW_GET_VAR(G_GLEW_ARB_transpose_matrix)

#endif /* GL_ARB_transpose_matrix */

/* ---------------------- GL_ARB_uniform_buffer_object --------------------- */

#ifndef GL_ARB_uniform_buffer_object
#define GL_ARB_uniform_buffer_object 1

#define GL_UNIFORM_BUFFER 0x8A11
#define GL_UNIFORM_BUFFER_BINDING 0x8A28
#define GL_UNIFORM_BUFFER_START 0x8A29
#define GL_UNIFORM_BUFFER_SIZE 0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS 0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS 0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS 0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS 0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS 0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE 0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS 0x8A36
#define GL_UNIFORM_TYPE 0x8A37
#define GL_UNIFORM_SIZE 0x8A38
#define GL_UNIFORM_NAME_LENGTH 0x8A39
#define GL_UNIFORM_BLOCK_INDEX 0x8A3A
#define GL_UNIFORM_OFFSET 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE 0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE 0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR 0x8A3E
#define GL_UNIFORM_BLOCK_BINDING 0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE 0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH 0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASEPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGEPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformBlockName);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMBLOCKIVPROC) (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMNAMEPROC) (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei* length, GLchar* uniformName);
typedef void (GLAPIENTRY * PFNGLGETACTIVEUNIFORMSIVPROC) (GLuint program, GLsizei uniformCount, const GLuint* uniformIndices, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETINTEGERI_VPROC) (GLenum target, GLuint index, GLint* data);
typedef GLuint (GLAPIENTRY * PFNGLGETUNIFORMBLOCKINDEXPROC) (GLuint program, const GLchar* uniformBlockName);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMINDICESPROC) (GLuint program, GLsizei uniformCount, const GLchar** uniformNames, GLuint* uniformIndices);
typedef void (GLAPIENTRY * PFNGLUNIFORMBLOCKBINDINGPROC) (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);

#define glBindBufferBase GLEW_GET_FUN(G_glewBindBufferBase)
#define glBindBufferRange GLEW_GET_FUN(G_glewBindBufferRange)
#define glGetActiveUniformBlockName GLEW_GET_FUN(G_glewGetActiveUniformBlockName)
#define glGetActiveUniformBlockiv GLEW_GET_FUN(G_glewGetActiveUniformBlockiv)
#define glGetActiveUniformName GLEW_GET_FUN(G_glewGetActiveUniformName)
#define glGetActiveUniformsiv GLEW_GET_FUN(G_glewGetActiveUniformsiv)
#define glGetIntegeri_v GLEW_GET_FUN(G_glewGetIntegeri_v)
#define glGetUniformBlockIndex GLEW_GET_FUN(G_glewGetUniformBlockIndex)
#define glGetUniformIndices GLEW_GET_FUN(G_glewGetUniformIndices)
#define glUniformBlockBinding GLEW_GET_FUN(G_glewUniformBlockBinding)

#define GLEW_ARB_uniform_buffer_object GLEW_GET_VAR(G_GLEW_ARB_uniform_buffer_object)

#endif /* GL_ARB_uniform_buffer_object */

/* ------------------------ GL_ARB_vertex_array_bgra ----------------------- */

#ifndef GL_ARB_vertex_array_bgra
#define GL_ARB_vertex_array_bgra 1

#define GL_BGRA 0x80E1

#define GLEW_ARB_vertex_array_bgra GLEW_GET_VAR(G_GLEW_ARB_vertex_array_bgra)

#endif /* GL_ARB_vertex_array_bgra */

/* ----------------------- GL_ARB_vertex_array_object ---------------------- */

#ifndef GL_ARB_vertex_array_object
#define GL_ARB_vertex_array_object 1

#define GL_VERTEX_ARRAY_BINDING 0x85B5

typedef void (GLAPIENTRY * PFNGLBINDVERTEXARRAYPROC) (GLuint array);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXARRAYSPROC) (GLsizei n, const GLuint* arrays);
typedef void (GLAPIENTRY * PFNGLGENVERTEXARRAYSPROC) (GLsizei n, GLuint* arrays);
typedef GLboolean (GLAPIENTRY * PFNGLISVERTEXARRAYPROC) (GLuint array);

#define glBindVertexArray GLEW_GET_FUN(G_glewBindVertexArray)
#define glDeleteVertexArrays GLEW_GET_FUN(G_glewDeleteVertexArrays)
#define glGenVertexArrays GLEW_GET_FUN(G_glewGenVertexArrays)
#define glIsVertexArray GLEW_GET_FUN(G_glewIsVertexArray)

#define GLEW_ARB_vertex_array_object GLEW_GET_VAR(G_GLEW_ARB_vertex_array_object)

#endif /* GL_ARB_vertex_array_object */

/* ----------------------- GL_ARB_vertex_attrib_64bit ---------------------- */

#ifndef GL_ARB_vertex_attrib_64bit
#define GL_ARB_vertex_attrib_64bit 1

#define GL_DOUBLE_MAT2 0x8F46
#define GL_DOUBLE_MAT3 0x8F47
#define GL_DOUBLE_MAT4 0x8F48
#define GL_DOUBLE_VEC2 0x8FFC
#define GL_DOUBLE_VEC3 0x8FFD
#define GL_DOUBLE_VEC4 0x8FFE

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLDVPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

#define glGetVertexAttribLdv GLEW_GET_FUN(G_glewGetVertexAttribLdv)
#define glVertexAttribL1d GLEW_GET_FUN(G_glewVertexAttribL1d)
#define glVertexAttribL1dv GLEW_GET_FUN(G_glewVertexAttribL1dv)
#define glVertexAttribL2d GLEW_GET_FUN(G_glewVertexAttribL2d)
#define glVertexAttribL2dv GLEW_GET_FUN(G_glewVertexAttribL2dv)
#define glVertexAttribL3d GLEW_GET_FUN(G_glewVertexAttribL3d)
#define glVertexAttribL3dv GLEW_GET_FUN(G_glewVertexAttribL3dv)
#define glVertexAttribL4d GLEW_GET_FUN(G_glewVertexAttribL4d)
#define glVertexAttribL4dv GLEW_GET_FUN(G_glewVertexAttribL4dv)
#define glVertexAttribLPointer GLEW_GET_FUN(G_glewVertexAttribLPointer)

#define GLEW_ARB_vertex_attrib_64bit GLEW_GET_VAR(G_GLEW_ARB_vertex_attrib_64bit)

#endif /* GL_ARB_vertex_attrib_64bit */

/* ---------------------- GL_ARB_vertex_attrib_binding --------------------- */

#ifndef GL_ARB_vertex_attrib_binding
#define GL_ARB_vertex_attrib_binding 1

#define GL_VERTEX_ATTRIB_BINDING 0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D5
#define GL_VERTEX_BINDING_DIVISOR 0x82D6
#define GL_VERTEX_BINDING_OFFSET 0x82D7
#define GL_VERTEX_BINDING_STRIDE 0x82D8
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET 0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS 0x82DA

typedef void (GLAPIENTRY * PFNGLBINDVERTEXBUFFERPROC) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBBINDINGPROC) (GLuint attribindex, GLuint bindingindex);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void (GLAPIENTRY * PFNGLVERTEXBINDINGDIVISORPROC) (GLuint bindingindex, GLuint divisor);

#define glBindVertexBuffer GLEW_GET_FUN(G_glewBindVertexBuffer)
#define glVertexAttribBinding GLEW_GET_FUN(G_glewVertexAttribBinding)
#define glVertexAttribFormat GLEW_GET_FUN(G_glewVertexAttribFormat)
#define glVertexAttribIFormat GLEW_GET_FUN(G_glewVertexAttribIFormat)
#define glVertexAttribLFormat GLEW_GET_FUN(G_glewVertexAttribLFormat)
#define glVertexBindingDivisor GLEW_GET_FUN(G_glewVertexBindingDivisor)

#define GLEW_ARB_vertex_attrib_binding GLEW_GET_VAR(G_GLEW_ARB_vertex_attrib_binding)

#endif /* GL_ARB_vertex_attrib_binding */

/* -------------------------- GL_ARB_vertex_blend -------------------------- */

#ifndef GL_ARB_vertex_blend
#define GL_ARB_vertex_blend 1

#define GL_MODELVIEW0_ARB 0x1700
#define GL_MODELVIEW1_ARB 0x850A
#define GL_MAX_VERTEX_UNITS_ARB 0x86A4
#define GL_ACTIVE_VERTEX_UNITS_ARB 0x86A5
#define GL_WEIGHT_SUM_UNITY_ARB 0x86A6
#define GL_VERTEX_BLEND_ARB 0x86A7
#define GL_CURRENT_WEIGHT_ARB 0x86A8
#define GL_WEIGHT_ARRAY_TYPE_ARB 0x86A9
#define GL_WEIGHT_ARRAY_STRIDE_ARB 0x86AA
#define GL_WEIGHT_ARRAY_SIZE_ARB 0x86AB
#define GL_WEIGHT_ARRAY_POINTER_ARB 0x86AC
#define GL_WEIGHT_ARRAY_ARB 0x86AD
#define GL_MODELVIEW2_ARB 0x8722
#define GL_MODELVIEW3_ARB 0x8723
#define GL_MODELVIEW4_ARB 0x8724
#define GL_MODELVIEW5_ARB 0x8725
#define GL_MODELVIEW6_ARB 0x8726
#define GL_MODELVIEW7_ARB 0x8727
#define GL_MODELVIEW8_ARB 0x8728
#define GL_MODELVIEW9_ARB 0x8729
#define GL_MODELVIEW10_ARB 0x872A
#define GL_MODELVIEW11_ARB 0x872B
#define GL_MODELVIEW12_ARB 0x872C
#define GL_MODELVIEW13_ARB 0x872D
#define GL_MODELVIEW14_ARB 0x872E
#define GL_MODELVIEW15_ARB 0x872F
#define GL_MODELVIEW16_ARB 0x8730
#define GL_MODELVIEW17_ARB 0x8731
#define GL_MODELVIEW18_ARB 0x8732
#define GL_MODELVIEW19_ARB 0x8733
#define GL_MODELVIEW20_ARB 0x8734
#define GL_MODELVIEW21_ARB 0x8735
#define GL_MODELVIEW22_ARB 0x8736
#define GL_MODELVIEW23_ARB 0x8737
#define GL_MODELVIEW24_ARB 0x8738
#define GL_MODELVIEW25_ARB 0x8739
#define GL_MODELVIEW26_ARB 0x873A
#define GL_MODELVIEW27_ARB 0x873B
#define GL_MODELVIEW28_ARB 0x873C
#define GL_MODELVIEW29_ARB 0x873D
#define GL_MODELVIEW30_ARB 0x873E
#define GL_MODELVIEW31_ARB 0x873F

typedef void (GLAPIENTRY * PFNGLVERTEXBLENDARBPROC) (GLint count);
typedef void (GLAPIENTRY * PFNGLWEIGHTPOINTERARBPROC) (GLint size, GLenum type, GLsizei stride, GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLWEIGHTBVARBPROC) (GLint size, GLbyte *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTDVARBPROC) (GLint size, GLdouble *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTFVARBPROC) (GLint size, GLfloat *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTIVARBPROC) (GLint size, GLint *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTSVARBPROC) (GLint size, GLshort *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUBVARBPROC) (GLint size, GLubyte *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUIVARBPROC) (GLint size, GLuint *weights);
typedef void (GLAPIENTRY * PFNGLWEIGHTUSVARBPROC) (GLint size, GLushort *weights);

#define glVertexBlendARB GLEW_GET_FUN(G_glewVertexBlendARB)
#define glWeightPointerARB GLEW_GET_FUN(G_glewWeightPointerARB)
#define glWeightbvARB GLEW_GET_FUN(G_glewWeightbvARB)
#define glWeightdvARB GLEW_GET_FUN(G_glewWeightdvARB)
#define glWeightfvARB GLEW_GET_FUN(G_glewWeightfvARB)
#define glWeightivARB GLEW_GET_FUN(G_glewWeightivARB)
#define glWeightsvARB GLEW_GET_FUN(G_glewWeightsvARB)
#define glWeightubvARB GLEW_GET_FUN(G_glewWeightubvARB)
#define glWeightuivARB GLEW_GET_FUN(G_glewWeightuivARB)
#define glWeightusvARB GLEW_GET_FUN(G_glewWeightusvARB)

#define GLEW_ARB_vertex_blend GLEW_GET_VAR(G_GLEW_ARB_vertex_blend)

#endif /* GL_ARB_vertex_blend */

/* ---------------------- GL_ARB_vertex_buffer_object ---------------------- */

#ifndef GL_ARB_vertex_buffer_object
#define GL_ARB_vertex_buffer_object 1

#define GL_BUFFER_SIZE_ARB 0x8764
#define GL_BUFFER_USAGE_ARB 0x8765
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB 0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB 0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_READ_ONLY_ARB 0x88B8
#define GL_WRITE_ONLY_ARB 0x88B9
#define GL_READ_WRITE_ARB 0x88BA
#define GL_BUFFER_ACCESS_ARB 0x88BB
#define GL_BUFFER_MAPPED_ARB 0x88BC
#define GL_BUFFER_MAP_POINTER_ARB 0x88BD
#define GL_STREAM_DRAW_ARB 0x88E0
#define GL_STREAM_READ_ARB 0x88E1
#define GL_STREAM_COPY_ARB 0x88E2
#define GL_STATIC_DRAW_ARB 0x88E4
#define GL_STATIC_READ_ARB 0x88E5
#define GL_STATIC_COPY_ARB 0x88E6
#define GL_DYNAMIC_DRAW_ARB 0x88E8
#define GL_DYNAMIC_READ_ARB 0x88E9
#define GL_DYNAMIC_COPY_ARB 0x88EA

typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;

typedef void (GLAPIENTRY * PFNGLBINDBUFFERARBPROC) (GLenum target, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBUFFERDATAARBPROC) (GLenum target, GLsizeiptrARB size, const GLvoid* data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid* data);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERSARBPROC) (GLsizei n, const GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGENBUFFERSARBPROC) (GLsizei n, GLuint* buffers);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERPOINTERVARBPROC) (GLenum target, GLenum pname, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETBUFFERSUBDATAARBPROC) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, GLvoid* data);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERARBPROC) (GLuint buffer);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPBUFFERARBPROC) (GLenum target, GLenum access);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPBUFFERARBPROC) (GLenum target);

#define glBindBufferARB GLEW_GET_FUN(G_glewBindBufferARB)
#define glBufferDataARB GLEW_GET_FUN(G_glewBufferDataARB)
#define glBufferSubDataARB GLEW_GET_FUN(G_glewBufferSubDataARB)
#define glDeleteBuffersARB GLEW_GET_FUN(G_glewDeleteBuffersARB)
#define glGenBuffersARB GLEW_GET_FUN(G_glewGenBuffersARB)
#define glGetBufferParameterivARB GLEW_GET_FUN(G_glewGetBufferParameterivARB)
#define glGetBufferPointervARB GLEW_GET_FUN(G_glewGetBufferPointervARB)
#define glGetBufferSubDataARB GLEW_GET_FUN(G_glewGetBufferSubDataARB)
#define glIsBufferARB GLEW_GET_FUN(G_glewIsBufferARB)
#define glMapBufferARB GLEW_GET_FUN(G_glewMapBufferARB)
#define glUnmapBufferARB GLEW_GET_FUN(G_glewUnmapBufferARB)

#define GLEW_ARB_vertex_buffer_object GLEW_GET_VAR(G_GLEW_ARB_vertex_buffer_object)

#endif /* GL_ARB_vertex_buffer_object */

/* ------------------------- GL_ARB_vertex_program ------------------------- */

#ifndef GL_ARB_vertex_program
#define GL_ARB_vertex_program 1

#define GL_COLOR_SUM_ARB 0x8458
#define GL_VERTEX_PROGRAM_ARB 0x8620
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB 0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB 0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB 0x8625
#define GL_CURRENT_VERTEX_ATTRIB_ARB 0x8626
#define GL_PROGRAM_LENGTH_ARB 0x8627
#define GL_PROGRAM_STRING_ARB 0x8628
#define GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB 0x862E
#define GL_MAX_PROGRAM_MATRICES_ARB 0x862F
#define GL_CURRENT_MATRIX_STACK_DEPTH_ARB 0x8640
#define GL_CURRENT_MATRIX_ARB 0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB 0x8643
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB 0x8645
#define GL_PROGRAM_ERROR_POSITION_ARB 0x864B
#define GL_PROGRAM_BINDING_ARB 0x8677
#define GL_MAX_VERTEX_ATTRIBS_ARB 0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB 0x886A
#define GL_PROGRAM_ERROR_STRING_ARB 0x8874
#define GL_PROGRAM_FORMAT_ASCII_ARB 0x8875
#define GL_PROGRAM_FORMAT_ARB 0x8876
#define GL_PROGRAM_INSTRUCTIONS_ARB 0x88A0
#define GL_MAX_PROGRAM_INSTRUCTIONS_ARB 0x88A1
#define GL_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A2
#define GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB 0x88A3
#define GL_PROGRAM_TEMPORARIES_ARB 0x88A4
#define GL_MAX_PROGRAM_TEMPORARIES_ARB 0x88A5
#define GL_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A6
#define GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB 0x88A7
#define GL_PROGRAM_PARAMETERS_ARB 0x88A8
#define GL_MAX_PROGRAM_PARAMETERS_ARB 0x88A9
#define GL_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AA
#define GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB 0x88AB
#define GL_PROGRAM_ATTRIBS_ARB 0x88AC
#define GL_MAX_PROGRAM_ATTRIBS_ARB 0x88AD
#define GL_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AE
#define GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB 0x88AF
#define GL_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B0
#define GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB 0x88B1
#define GL_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B2
#define GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB 0x88B3
#define GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB 0x88B4
#define GL_MAX_PROGRAM_ENV_PARAMETERS_ARB 0x88B5
#define GL_PROGRAM_UNDER_NATIVE_LIMITS_ARB 0x88B6
#define GL_TRANSPOSE_CURRENT_MATRIX_ARB 0x88B7
#define GL_MATRIX0_ARB 0x88C0
#define GL_MATRIX1_ARB 0x88C1
#define GL_MATRIX2_ARB 0x88C2
#define GL_MATRIX3_ARB 0x88C3
#define GL_MATRIX4_ARB 0x88C4
#define GL_MATRIX5_ARB 0x88C5
#define GL_MATRIX6_ARB 0x88C6
#define GL_MATRIX7_ARB 0x88C7
#define GL_MATRIX8_ARB 0x88C8
#define GL_MATRIX9_ARB 0x88C9
#define GL_MATRIX10_ARB 0x88CA
#define GL_MATRIX11_ARB 0x88CB
#define GL_MATRIX12_ARB 0x88CC
#define GL_MATRIX13_ARB 0x88CD
#define GL_MATRIX14_ARB 0x88CE
#define GL_MATRIX15_ARB 0x88CF
#define GL_MATRIX16_ARB 0x88D0
#define GL_MATRIX17_ARB 0x88D1
#define GL_MATRIX18_ARB 0x88D2
#define GL_MATRIX19_ARB 0x88D3
#define GL_MATRIX20_ARB 0x88D4
#define GL_MATRIX21_ARB 0x88D5
#define GL_MATRIX22_ARB 0x88D6
#define GL_MATRIX23_ARB 0x88D7
#define GL_MATRIX24_ARB 0x88D8
#define GL_MATRIX25_ARB 0x88D9
#define GL_MATRIX26_ARB 0x88DA
#define GL_MATRIX27_ARB 0x88DB
#define GL_MATRIX28_ARB 0x88DC
#define GL_MATRIX29_ARB 0x88DD
#define GL_MATRIX30_ARB 0x88DE
#define GL_MATRIX31_ARB 0x88DF

typedef void (GLAPIENTRY * PFNGLBINDPROGRAMARBPROC) (GLenum target, GLuint program);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMSARBPROC) (GLsizei n, const GLuint* programs);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXATTRIBARRAYARBPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMSARBPROC) (GLsizei n, GLuint* programs);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMENVPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTRINGARBPROC) (GLenum target, GLenum pname, void* string);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVARBPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVARBPROC) (GLuint index, GLenum pname, GLvoid** pointer);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVARBPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVARBPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVARBPROC) (GLuint index, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMARBPROC) (GLuint program);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4DVARBPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4FARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETER4FVARBPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4DARBPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4DVARBPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4FARBPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETER4FVARBPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMSTRINGARBPROC) (GLenum target, GLenum format, GLsizei len, const void* string);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DARBPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FARBPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SARBPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DARBPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FARBPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SARBPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NBVARBPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NIVARBPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NSVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBARBPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUBVARBPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUIVARBPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4NUSVARBPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4BVARBPROC) (GLuint index, const GLbyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DARBPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVARBPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FARBPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVARBPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4IVARBPROC) (GLuint index, const GLint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SARBPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVARBPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVARBPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UIVARBPROC) (GLuint index, const GLuint* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4USVARBPROC) (GLuint index, const GLushort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERARBPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

#define glBindProgramARB GLEW_GET_FUN(G_glewBindProgramARB)
#define glDeleteProgramsARB GLEW_GET_FUN(G_glewDeleteProgramsARB)
#define glDisableVertexAttribArrayARB GLEW_GET_FUN(G_glewDisableVertexAttribArrayARB)
#define glEnableVertexAttribArrayARB GLEW_GET_FUN(G_glewEnableVertexAttribArrayARB)
#define glGenProgramsARB GLEW_GET_FUN(G_glewGenProgramsARB)
#define glGetProgramEnvParameterdvARB GLEW_GET_FUN(G_glewGetProgramEnvParameterdvARB)
#define glGetProgramEnvParameterfvARB GLEW_GET_FUN(G_glewGetProgramEnvParameterfvARB)
#define glGetProgramLocalParameterdvARB GLEW_GET_FUN(G_glewGetProgramLocalParameterdvARB)
#define glGetProgramLocalParameterfvARB GLEW_GET_FUN(G_glewGetProgramLocalParameterfvARB)
#define glGetProgramStringARB GLEW_GET_FUN(G_glewGetProgramStringARB)
#define glGetProgramivARB GLEW_GET_FUN(G_glewGetProgramivARB)
#define glGetVertexAttribPointervARB GLEW_GET_FUN(G_glewGetVertexAttribPointervARB)
#define glGetVertexAttribdvARB GLEW_GET_FUN(G_glewGetVertexAttribdvARB)
#define glGetVertexAttribfvARB GLEW_GET_FUN(G_glewGetVertexAttribfvARB)
#define glGetVertexAttribivARB GLEW_GET_FUN(G_glewGetVertexAttribivARB)
#define glIsProgramARB GLEW_GET_FUN(G_glewIsProgramARB)
#define glProgramEnvParameter4dARB GLEW_GET_FUN(G_glewProgramEnvParameter4dARB)
#define glProgramEnvParameter4dvARB GLEW_GET_FUN(G_glewProgramEnvParameter4dvARB)
#define glProgramEnvParameter4fARB GLEW_GET_FUN(G_glewProgramEnvParameter4fARB)
#define glProgramEnvParameter4fvARB GLEW_GET_FUN(G_glewProgramEnvParameter4fvARB)
#define glProgramLocalParameter4dARB GLEW_GET_FUN(G_glewProgramLocalParameter4dARB)
#define glProgramLocalParameter4dvARB GLEW_GET_FUN(G_glewProgramLocalParameter4dvARB)
#define glProgramLocalParameter4fARB GLEW_GET_FUN(G_glewProgramLocalParameter4fARB)
#define glProgramLocalParameter4fvARB GLEW_GET_FUN(G_glewProgramLocalParameter4fvARB)
#define glProgramStringARB GLEW_GET_FUN(G_glewProgramStringARB)
#define glVertexAttrib1dARB GLEW_GET_FUN(G_glewVertexAttrib1dARB)
#define glVertexAttrib1dvARB GLEW_GET_FUN(G_glewVertexAttrib1dvARB)
#define glVertexAttrib1fARB GLEW_GET_FUN(G_glewVertexAttrib1fARB)
#define glVertexAttrib1fvARB GLEW_GET_FUN(G_glewVertexAttrib1fvARB)
#define glVertexAttrib1sARB GLEW_GET_FUN(G_glewVertexAttrib1sARB)
#define glVertexAttrib1svARB GLEW_GET_FUN(G_glewVertexAttrib1svARB)
#define glVertexAttrib2dARB GLEW_GET_FUN(G_glewVertexAttrib2dARB)
#define glVertexAttrib2dvARB GLEW_GET_FUN(G_glewVertexAttrib2dvARB)
#define glVertexAttrib2fARB GLEW_GET_FUN(G_glewVertexAttrib2fARB)
#define glVertexAttrib2fvARB GLEW_GET_FUN(G_glewVertexAttrib2fvARB)
#define glVertexAttrib2sARB GLEW_GET_FUN(G_glewVertexAttrib2sARB)
#define glVertexAttrib2svARB GLEW_GET_FUN(G_glewVertexAttrib2svARB)
#define glVertexAttrib3dARB GLEW_GET_FUN(G_glewVertexAttrib3dARB)
#define glVertexAttrib3dvARB GLEW_GET_FUN(G_glewVertexAttrib3dvARB)
#define glVertexAttrib3fARB GLEW_GET_FUN(G_glewVertexAttrib3fARB)
#define glVertexAttrib3fvARB GLEW_GET_FUN(G_glewVertexAttrib3fvARB)
#define glVertexAttrib3sARB GLEW_GET_FUN(G_glewVertexAttrib3sARB)
#define glVertexAttrib3svARB GLEW_GET_FUN(G_glewVertexAttrib3svARB)
#define glVertexAttrib4NbvARB GLEW_GET_FUN(G_glewVertexAttrib4NbvARB)
#define glVertexAttrib4NivARB GLEW_GET_FUN(G_glewVertexAttrib4NivARB)
#define glVertexAttrib4NsvARB GLEW_GET_FUN(G_glewVertexAttrib4NsvARB)
#define glVertexAttrib4NubARB GLEW_GET_FUN(G_glewVertexAttrib4NubARB)
#define glVertexAttrib4NubvARB GLEW_GET_FUN(G_glewVertexAttrib4NubvARB)
#define glVertexAttrib4NuivARB GLEW_GET_FUN(G_glewVertexAttrib4NuivARB)
#define glVertexAttrib4NusvARB GLEW_GET_FUN(G_glewVertexAttrib4NusvARB)
#define glVertexAttrib4bvARB GLEW_GET_FUN(G_glewVertexAttrib4bvARB)
#define glVertexAttrib4dARB GLEW_GET_FUN(G_glewVertexAttrib4dARB)
#define glVertexAttrib4dvARB GLEW_GET_FUN(G_glewVertexAttrib4dvARB)
#define glVertexAttrib4fARB GLEW_GET_FUN(G_glewVertexAttrib4fARB)
#define glVertexAttrib4fvARB GLEW_GET_FUN(G_glewVertexAttrib4fvARB)
#define glVertexAttrib4ivARB GLEW_GET_FUN(G_glewVertexAttrib4ivARB)
#define glVertexAttrib4sARB GLEW_GET_FUN(G_glewVertexAttrib4sARB)
#define glVertexAttrib4svARB GLEW_GET_FUN(G_glewVertexAttrib4svARB)
#define glVertexAttrib4ubvARB GLEW_GET_FUN(G_glewVertexAttrib4ubvARB)
#define glVertexAttrib4uivARB GLEW_GET_FUN(G_glewVertexAttrib4uivARB)
#define glVertexAttrib4usvARB GLEW_GET_FUN(G_glewVertexAttrib4usvARB)
#define glVertexAttribPointerARB GLEW_GET_FUN(G_glewVertexAttribPointerARB)

#define GLEW_ARB_vertex_program GLEW_GET_VAR(G_GLEW_ARB_vertex_program)

#endif /* GL_ARB_vertex_program */

/* -------------------------- GL_ARB_vertex_shader ------------------------- */

#ifndef GL_ARB_vertex_shader
#define GL_ARB_vertex_shader 1

#define GL_VERTEX_SHADER_ARB 0x8B31
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB 0x8B4A
#define GL_MAX_VARYING_FLOATS_ARB 0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB 0x8B4D
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB 0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB 0x8B8A

typedef void (GLAPIENTRY * PFNGLBINDATTRIBLOCATIONARBPROC) (GLhandleARB programObj, GLuint index, const GLcharARB* name);
typedef void (GLAPIENTRY * PFNGLGETACTIVEATTRIBARBPROC) (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei* length, GLint *size, GLenum *type, GLcharARB *name);
typedef GLint (GLAPIENTRY * PFNGLGETATTRIBLOCATIONARBPROC) (GLhandleARB programObj, const GLcharARB* name);

#define glBindAttribLocationARB GLEW_GET_FUN(G_glewBindAttribLocationARB)
#define glGetActiveAttribARB GLEW_GET_FUN(G_glewGetActiveAttribARB)
#define glGetAttribLocationARB GLEW_GET_FUN(G_glewGetAttribLocationARB)

#define GLEW_ARB_vertex_shader GLEW_GET_VAR(G_GLEW_ARB_vertex_shader)

#endif /* GL_ARB_vertex_shader */

/* ------------------- GL_ARB_vertex_type_2_10_10_10_rev ------------------- */

#ifndef GL_ARB_vertex_type_2_10_10_10_rev
#define GL_ARB_vertex_type_2_10_10_10_rev 1

#define GL_UNSIGNED_INT_2_10_10_10_REV 0x8368
#define GL_INT_2_10_10_10_REV 0x8D9F

typedef void (GLAPIENTRY * PFNGLCOLORP3UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLCOLORP3UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLCOLORP4UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLCOLORP4UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP1UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP1UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP2UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP2UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP3UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP3UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP4UIPROC) (GLenum texture, GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDP4UIVPROC) (GLenum texture, GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLNORMALP3UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLNORMALP3UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORP3UIPROC) (GLenum type, GLuint color);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORP3UIVPROC) (GLenum type, const GLuint* color);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP1UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP1UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP2UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP2UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP3UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP3UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP4UIPROC) (GLenum type, GLuint coords);
typedef void (GLAPIENTRY * PFNGLTEXCOORDP4UIVPROC) (GLenum type, const GLuint* coords);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP1UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP1UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP2UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP2UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP3UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP3UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP4UIPROC) (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBP4UIVPROC) (GLuint index, GLenum type, GLboolean normalized, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP2UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP2UIVPROC) (GLenum type, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP3UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP3UIVPROC) (GLenum type, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLVERTEXP4UIPROC) (GLenum type, GLuint value);
typedef void (GLAPIENTRY * PFNGLVERTEXP4UIVPROC) (GLenum type, const GLuint* value);

#define glColorP3ui GLEW_GET_FUN(G_glewColorP3ui)
#define glColorP3uiv GLEW_GET_FUN(G_glewColorP3uiv)
#define glColorP4ui GLEW_GET_FUN(G_glewColorP4ui)
#define glColorP4uiv GLEW_GET_FUN(G_glewColorP4uiv)
#define glMultiTexCoordP1ui GLEW_GET_FUN(G_glewMultiTexCoordP1ui)
#define glMultiTexCoordP1uiv GLEW_GET_FUN(G_glewMultiTexCoordP1uiv)
#define glMultiTexCoordP2ui GLEW_GET_FUN(G_glewMultiTexCoordP2ui)
#define glMultiTexCoordP2uiv GLEW_GET_FUN(G_glewMultiTexCoordP2uiv)
#define glMultiTexCoordP3ui GLEW_GET_FUN(G_glewMultiTexCoordP3ui)
#define glMultiTexCoordP3uiv GLEW_GET_FUN(G_glewMultiTexCoordP3uiv)
#define glMultiTexCoordP4ui GLEW_GET_FUN(G_glewMultiTexCoordP4ui)
#define glMultiTexCoordP4uiv GLEW_GET_FUN(G_glewMultiTexCoordP4uiv)
#define glNormalP3ui GLEW_GET_FUN(G_glewNormalP3ui)
#define glNormalP3uiv GLEW_GET_FUN(G_glewNormalP3uiv)
#define glSecondaryColorP3ui GLEW_GET_FUN(G_glewSecondaryColorP3ui)
#define glSecondaryColorP3uiv GLEW_GET_FUN(G_glewSecondaryColorP3uiv)
#define glTexCoordP1ui GLEW_GET_FUN(G_glewTexCoordP1ui)
#define glTexCoordP1uiv GLEW_GET_FUN(G_glewTexCoordP1uiv)
#define glTexCoordP2ui GLEW_GET_FUN(G_glewTexCoordP2ui)
#define glTexCoordP2uiv GLEW_GET_FUN(G_glewTexCoordP2uiv)
#define glTexCoordP3ui GLEW_GET_FUN(G_glewTexCoordP3ui)
#define glTexCoordP3uiv GLEW_GET_FUN(G_glewTexCoordP3uiv)
#define glTexCoordP4ui GLEW_GET_FUN(G_glewTexCoordP4ui)
#define glTexCoordP4uiv GLEW_GET_FUN(G_glewTexCoordP4uiv)
#define glVertexAttribP1ui GLEW_GET_FUN(G_glewVertexAttribP1ui)
#define glVertexAttribP1uiv GLEW_GET_FUN(G_glewVertexAttribP1uiv)
#define glVertexAttribP2ui GLEW_GET_FUN(G_glewVertexAttribP2ui)
#define glVertexAttribP2uiv GLEW_GET_FUN(G_glewVertexAttribP2uiv)
#define glVertexAttribP3ui GLEW_GET_FUN(G_glewVertexAttribP3ui)
#define glVertexAttribP3uiv GLEW_GET_FUN(G_glewVertexAttribP3uiv)
#define glVertexAttribP4ui GLEW_GET_FUN(G_glewVertexAttribP4ui)
#define glVertexAttribP4uiv GLEW_GET_FUN(G_glewVertexAttribP4uiv)
#define glVertexP2ui GLEW_GET_FUN(G_glewVertexP2ui)
#define glVertexP2uiv GLEW_GET_FUN(G_glewVertexP2uiv)
#define glVertexP3ui GLEW_GET_FUN(G_glewVertexP3ui)
#define glVertexP3uiv GLEW_GET_FUN(G_glewVertexP3uiv)
#define glVertexP4ui GLEW_GET_FUN(G_glewVertexP4ui)
#define glVertexP4uiv GLEW_GET_FUN(G_glewVertexP4uiv)

#define GLEW_ARB_vertex_type_2_10_10_10_rev GLEW_GET_VAR(G_GLEW_ARB_vertex_type_2_10_10_10_rev)

#endif /* GL_ARB_vertex_type_2_10_10_10_rev */

/* ------------------------- GL_ARB_viewport_array ------------------------- */

#ifndef GL_ARB_viewport_array
#define GL_ARB_viewport_array 1

#define GL_DEPTH_RANGE 0x0B70
#define GL_VIEWPORT 0x0BA2
#define GL_SCISSOR_BOX 0x0C10
#define GL_SCISSOR_TEST 0x0C11
#define GL_MAX_VIEWPORTS 0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS 0x825C
#define GL_VIEWPORT_BOUNDS_RANGE 0x825D
#define GL_LAYER_PROVOKING_VERTEX 0x825E
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX 0x825F
#define GL_UNDEFINED_VERTEX 0x8260
#define GL_FIRST_VERTEX_CONVENTION 0x8E4D
#define GL_LAST_VERTEX_CONVENTION 0x8E4E
#define GL_PROVOKING_VERTEX 0x8E4F

typedef void (GLAPIENTRY * PFNGLDEPTHRANGEARRAYVPROC) (GLuint first, GLsizei count, const GLclampd * v);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEINDEXEDPROC) (GLuint index, GLclampd n, GLclampd f);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEI_VPROC) (GLenum target, GLuint index, GLdouble* data);
typedef void (GLAPIENTRY * PFNGLGETFLOATI_VPROC) (GLenum target, GLuint index, GLfloat* data);
typedef void (GLAPIENTRY * PFNGLSCISSORARRAYVPROC) (GLuint first, GLsizei count, const GLint * v);
typedef void (GLAPIENTRY * PFNGLSCISSORINDEXEDPROC) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLSCISSORINDEXEDVPROC) (GLuint index, const GLint * v);
typedef void (GLAPIENTRY * PFNGLVIEWPORTARRAYVPROC) (GLuint first, GLsizei count, const GLfloat * v);
typedef void (GLAPIENTRY * PFNGLVIEWPORTINDEXEDFPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void (GLAPIENTRY * PFNGLVIEWPORTINDEXEDFVPROC) (GLuint index, const GLfloat * v);

#define glDepthRangeArrayv GLEW_GET_FUN(G_glewDepthRangeArrayv)
#define glDepthRangeIndexed GLEW_GET_FUN(G_glewDepthRangeIndexed)
#define glGetDoublei_v GLEW_GET_FUN(G_glewGetDoublei_v)
#define glGetFloati_v GLEW_GET_FUN(G_glewGetFloati_v)
#define glScissorArrayv GLEW_GET_FUN(G_glewScissorArrayv)
#define glScissorIndexed GLEW_GET_FUN(G_glewScissorIndexed)
#define glScissorIndexedv GLEW_GET_FUN(G_glewScissorIndexedv)
#define glViewportArrayv GLEW_GET_FUN(G_glewViewportArrayv)
#define glViewportIndexedf GLEW_GET_FUN(G_glewViewportIndexedf)
#define glViewportIndexedfv GLEW_GET_FUN(G_glewViewportIndexedfv)

#define GLEW_ARB_viewport_array GLEW_GET_VAR(G_GLEW_ARB_viewport_array)

#endif /* GL_ARB_viewport_array */

/* --------------------------- GL_ARB_window_pos --------------------------- */

#ifndef GL_ARB_window_pos
#define GL_ARB_window_pos 1

typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DARBPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVARBPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FARBPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVARBPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IARBPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVARBPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SARBPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVARBPROC) (const GLshort* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DARBPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVARBPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FARBPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVARBPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IARBPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVARBPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SARBPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVARBPROC) (const GLshort* p);

#define glWindowPos2dARB GLEW_GET_FUN(G_glewWindowPos2dARB)
#define glWindowPos2dvARB GLEW_GET_FUN(G_glewWindowPos2dvARB)
#define glWindowPos2fARB GLEW_GET_FUN(G_glewWindowPos2fARB)
#define glWindowPos2fvARB GLEW_GET_FUN(G_glewWindowPos2fvARB)
#define glWindowPos2iARB GLEW_GET_FUN(G_glewWindowPos2iARB)
#define glWindowPos2ivARB GLEW_GET_FUN(G_glewWindowPos2ivARB)
#define glWindowPos2sARB GLEW_GET_FUN(G_glewWindowPos2sARB)
#define glWindowPos2svARB GLEW_GET_FUN(G_glewWindowPos2svARB)
#define glWindowPos3dARB GLEW_GET_FUN(G_glewWindowPos3dARB)
#define glWindowPos3dvARB GLEW_GET_FUN(G_glewWindowPos3dvARB)
#define glWindowPos3fARB GLEW_GET_FUN(G_glewWindowPos3fARB)
#define glWindowPos3fvARB GLEW_GET_FUN(G_glewWindowPos3fvARB)
#define glWindowPos3iARB GLEW_GET_FUN(G_glewWindowPos3iARB)
#define glWindowPos3ivARB GLEW_GET_FUN(G_glewWindowPos3ivARB)
#define glWindowPos3sARB GLEW_GET_FUN(G_glewWindowPos3sARB)
#define glWindowPos3svARB GLEW_GET_FUN(G_glewWindowPos3svARB)

#define GLEW_ARB_window_pos GLEW_GET_VAR(G_GLEW_ARB_window_pos)

#endif /* GL_ARB_window_pos */

/* ------------------------- GL_ATIX_point_sprites ------------------------- */

#ifndef GL_ATIX_point_sprites
#define GL_ATIX_point_sprites 1

#define GL_TEXTURE_POINT_MODE_ATIX 0x60B0
#define GL_TEXTURE_POINT_ONE_COORD_ATIX 0x60B1
#define GL_TEXTURE_POINT_SPRITE_ATIX 0x60B2
#define GL_POINT_SPRITE_CULL_MODE_ATIX 0x60B3
#define GL_POINT_SPRITE_CULL_CENTER_ATIX 0x60B4
#define GL_POINT_SPRITE_CULL_CLIP_ATIX 0x60B5

#define GLEW_ATIX_point_sprites GLEW_GET_VAR(G_GLEW_ATIX_point_sprites)

#endif /* GL_ATIX_point_sprites */

/* ---------------------- GL_ATIX_texture_env_combine3 --------------------- */

#ifndef GL_ATIX_texture_env_combine3
#define GL_ATIX_texture_env_combine3 1

#define GL_MODULATE_ADD_ATIX 0x8744
#define GL_MODULATE_SIGNED_ADD_ATIX 0x8745
#define GL_MODULATE_SUBTRACT_ATIX 0x8746

#define GLEW_ATIX_texture_env_combine3 GLEW_GET_VAR(G_GLEW_ATIX_texture_env_combine3)

#endif /* GL_ATIX_texture_env_combine3 */

/* ----------------------- GL_ATIX_texture_env_route ----------------------- */

#ifndef GL_ATIX_texture_env_route
#define GL_ATIX_texture_env_route 1

#define GL_SECONDARY_COLOR_ATIX 0x8747
#define GL_TEXTURE_OUTPUT_RGB_ATIX 0x8748
#define GL_TEXTURE_OUTPUT_ALPHA_ATIX 0x8749

#define GLEW_ATIX_texture_env_route GLEW_GET_VAR(G_GLEW_ATIX_texture_env_route)

#endif /* GL_ATIX_texture_env_route */

/* ---------------- GL_ATIX_vertex_shader_output_point_size ---------------- */

#ifndef GL_ATIX_vertex_shader_output_point_size
#define GL_ATIX_vertex_shader_output_point_size 1

#define GL_OUTPUT_POINT_SIZE_ATIX 0x610E

#define GLEW_ATIX_vertex_shader_output_point_size GLEW_GET_VAR(G_GLEW_ATIX_vertex_shader_output_point_size)

#endif /* GL_ATIX_vertex_shader_output_point_size */

/* -------------------------- GL_ATI_draw_buffers -------------------------- */

#ifndef GL_ATI_draw_buffers
#define GL_ATI_draw_buffers 1

#define GL_MAX_DRAW_BUFFERS_ATI 0x8824
#define GL_DRAW_BUFFER0_ATI 0x8825
#define GL_DRAW_BUFFER1_ATI 0x8826
#define GL_DRAW_BUFFER2_ATI 0x8827
#define GL_DRAW_BUFFER3_ATI 0x8828
#define GL_DRAW_BUFFER4_ATI 0x8829
#define GL_DRAW_BUFFER5_ATI 0x882A
#define GL_DRAW_BUFFER6_ATI 0x882B
#define GL_DRAW_BUFFER7_ATI 0x882C
#define GL_DRAW_BUFFER8_ATI 0x882D
#define GL_DRAW_BUFFER9_ATI 0x882E
#define GL_DRAW_BUFFER10_ATI 0x882F
#define GL_DRAW_BUFFER11_ATI 0x8830
#define GL_DRAW_BUFFER12_ATI 0x8831
#define GL_DRAW_BUFFER13_ATI 0x8832
#define GL_DRAW_BUFFER14_ATI 0x8833
#define GL_DRAW_BUFFER15_ATI 0x8834

typedef void (GLAPIENTRY * PFNGLDRAWBUFFERSATIPROC) (GLsizei n, const GLenum* bufs);

#define glDrawBuffersATI GLEW_GET_FUN(G_glewDrawBuffersATI)

#define GLEW_ATI_draw_buffers GLEW_GET_VAR(G_GLEW_ATI_draw_buffers)

#endif /* GL_ATI_draw_buffers */

/* -------------------------- GL_ATI_element_array ------------------------- */

#ifndef GL_ATI_element_array
#define GL_ATI_element_array 1

#define GL_ELEMENT_ARRAY_ATI 0x8768
#define GL_ELEMENT_ARRAY_TYPE_ATI 0x8769
#define GL_ELEMENT_ARRAY_POINTER_ATI 0x876A

typedef void (GLAPIENTRY * PFNGLDRAWELEMENTARRAYATIPROC) (GLenum mode, GLsizei count);
typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTARRAYATIPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count);
typedef void (GLAPIENTRY * PFNGLELEMENTPOINTERATIPROC) (GLenum type, const void* pointer);

#define glDrawElementArrayATI GLEW_GET_FUN(G_glewDrawElementArrayATI)
#define glDrawRangeElementArrayATI GLEW_GET_FUN(G_glewDrawRangeElementArrayATI)
#define glElementPointerATI GLEW_GET_FUN(G_glewElementPointerATI)

#define GLEW_ATI_element_array GLEW_GET_VAR(G_GLEW_ATI_element_array)

#endif /* GL_ATI_element_array */

/* ------------------------- GL_ATI_envmap_bumpmap ------------------------- */

#ifndef GL_ATI_envmap_bumpmap
#define GL_ATI_envmap_bumpmap 1

#define GL_BUMP_ROT_MATRIX_ATI 0x8775
#define GL_BUMP_ROT_MATRIX_SIZE_ATI 0x8776
#define GL_BUMP_NUM_TEX_UNITS_ATI 0x8777
#define GL_BUMP_TEX_UNITS_ATI 0x8778
#define GL_DUDV_ATI 0x8779
#define GL_DU8DV8_ATI 0x877A
#define GL_BUMP_ENVMAP_ATI 0x877B
#define GL_BUMP_TARGET_ATI 0x877C

typedef void (GLAPIENTRY * PFNGLGETTEXBUMPPARAMETERFVATIPROC) (GLenum pname, GLfloat *param);
typedef void (GLAPIENTRY * PFNGLGETTEXBUMPPARAMETERIVATIPROC) (GLenum pname, GLint *param);
typedef void (GLAPIENTRY * PFNGLTEXBUMPPARAMETERFVATIPROC) (GLenum pname, GLfloat *param);
typedef void (GLAPIENTRY * PFNGLTEXBUMPPARAMETERIVATIPROC) (GLenum pname, GLint *param);

#define glGetTexBumpParameterfvATI GLEW_GET_FUN(G_glewGetTexBumpParameterfvATI)
#define glGetTexBumpParameterivATI GLEW_GET_FUN(G_glewGetTexBumpParameterivATI)
#define glTexBumpParameterfvATI GLEW_GET_FUN(G_glewTexBumpParameterfvATI)
#define glTexBumpParameterivATI GLEW_GET_FUN(G_glewTexBumpParameterivATI)

#define GLEW_ATI_envmap_bumpmap GLEW_GET_VAR(G_GLEW_ATI_envmap_bumpmap)

#endif /* GL_ATI_envmap_bumpmap */

/* ------------------------- GL_ATI_fragment_shader ------------------------ */

#ifndef GL_ATI_fragment_shader
#define GL_ATI_fragment_shader 1

#define GL_RED_BIT_ATI 0x00000001
#define GL_2X_BIT_ATI 0x00000001
#define GL_4X_BIT_ATI 0x00000002
#define GL_GREEN_BIT_ATI 0x00000002
#define GL_COMP_BIT_ATI 0x00000002
#define GL_BLUE_BIT_ATI 0x00000004
#define GL_8X_BIT_ATI 0x00000004
#define GL_NEGATE_BIT_ATI 0x00000004
#define GL_BIAS_BIT_ATI 0x00000008
#define GL_HALF_BIT_ATI 0x00000008
#define GL_QUARTER_BIT_ATI 0x00000010
#define GL_EIGHTH_BIT_ATI 0x00000020
#define GL_SATURATE_BIT_ATI 0x00000040
#define GL_FRAGMENT_SHADER_ATI 0x8920
#define GL_REG_0_ATI 0x8921
#define GL_REG_1_ATI 0x8922
#define GL_REG_2_ATI 0x8923
#define GL_REG_3_ATI 0x8924
#define GL_REG_4_ATI 0x8925
#define GL_REG_5_ATI 0x8926
#define GL_CON_0_ATI 0x8941
#define GL_CON_1_ATI 0x8942
#define GL_CON_2_ATI 0x8943
#define GL_CON_3_ATI 0x8944
#define GL_CON_4_ATI 0x8945
#define GL_CON_5_ATI 0x8946
#define GL_CON_6_ATI 0x8947
#define GL_CON_7_ATI 0x8948
#define GL_MOV_ATI 0x8961
#define GL_ADD_ATI 0x8963
#define GL_MUL_ATI 0x8964
#define GL_SUB_ATI 0x8965
#define GL_DOT3_ATI 0x8966
#define GL_DOT4_ATI 0x8967
#define GL_MAD_ATI 0x8968
#define GL_LERP_ATI 0x8969
#define GL_CND_ATI 0x896A
#define GL_CND0_ATI 0x896B
#define GL_DOT2_ADD_ATI 0x896C
#define GL_SECONDARY_INTERPOLATOR_ATI 0x896D
#define GL_NUM_FRAGMENT_REGISTERS_ATI 0x896E
#define GL_NUM_FRAGMENT_CONSTANTS_ATI 0x896F
#define GL_NUM_PASSES_ATI 0x8970
#define GL_NUM_INSTRUCTIONS_PER_PASS_ATI 0x8971
#define GL_NUM_INSTRUCTIONS_TOTAL_ATI 0x8972
#define GL_NUM_INPUT_INTERPOLATOR_COMPONENTS_ATI 0x8973
#define GL_NUM_LOOPBACK_COMPONENTS_ATI 0x8974
#define GL_COLOR_ALPHA_PAIRING_ATI 0x8975
#define GL_SWIZZLE_STR_ATI 0x8976
#define GL_SWIZZLE_STQ_ATI 0x8977
#define GL_SWIZZLE_STR_DR_ATI 0x8978
#define GL_SWIZZLE_STQ_DQ_ATI 0x8979
#define GL_SWIZZLE_STRQ_ATI 0x897A
#define GL_SWIZZLE_STRQ_DQ_ATI 0x897B

typedef void (GLAPIENTRY * PFNGLALPHAFRAGMENTOP1ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void (GLAPIENTRY * PFNGLALPHAFRAGMENTOP2ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void (GLAPIENTRY * PFNGLALPHAFRAGMENTOP3ATIPROC) (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void (GLAPIENTRY * PFNGLBEGINFRAGMENTSHADERATIPROC) (void);
typedef void (GLAPIENTRY * PFNGLBINDFRAGMENTSHADERATIPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLCOLORFRAGMENTOP1ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod);
typedef void (GLAPIENTRY * PFNGLCOLORFRAGMENTOP2ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod);
typedef void (GLAPIENTRY * PFNGLCOLORFRAGMENTOP3ATIPROC) (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod);
typedef void (GLAPIENTRY * PFNGLDELETEFRAGMENTSHADERATIPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLENDFRAGMENTSHADERATIPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLGENFRAGMENTSHADERSATIPROC) (GLuint range);
typedef void (GLAPIENTRY * PFNGLPASSTEXCOORDATIPROC) (GLuint dst, GLuint coord, GLenum swizzle);
typedef void (GLAPIENTRY * PFNGLSAMPLEMAPATIPROC) (GLuint dst, GLuint interp, GLenum swizzle);
typedef void (GLAPIENTRY * PFNGLSETFRAGMENTSHADERCONSTANTATIPROC) (GLuint dst, const GLfloat* value);

#define glAlphaFragmentOp1ATI GLEW_GET_FUN(G_glewAlphaFragmentOp1ATI)
#define glAlphaFragmentOp2ATI GLEW_GET_FUN(G_glewAlphaFragmentOp2ATI)
#define glAlphaFragmentOp3ATI GLEW_GET_FUN(G_glewAlphaFragmentOp3ATI)
#define glBeginFragmentShaderATI GLEW_GET_FUN(G_glewBeginFragmentShaderATI)
#define glBindFragmentShaderATI GLEW_GET_FUN(G_glewBindFragmentShaderATI)
#define glColorFragmentOp1ATI GLEW_GET_FUN(G_glewColorFragmentOp1ATI)
#define glColorFragmentOp2ATI GLEW_GET_FUN(G_glewColorFragmentOp2ATI)
#define glColorFragmentOp3ATI GLEW_GET_FUN(G_glewColorFragmentOp3ATI)
#define glDeleteFragmentShaderATI GLEW_GET_FUN(G_glewDeleteFragmentShaderATI)
#define glEndFragmentShaderATI GLEW_GET_FUN(G_glewEndFragmentShaderATI)
#define glGenFragmentShadersATI GLEW_GET_FUN(G_glewGenFragmentShadersATI)
#define glPassTexCoordATI GLEW_GET_FUN(G_glewPassTexCoordATI)
#define glSampleMapATI GLEW_GET_FUN(G_glewSampleMapATI)
#define glSetFragmentShaderConstantATI GLEW_GET_FUN(G_glewSetFragmentShaderConstantATI)

#define GLEW_ATI_fragment_shader GLEW_GET_VAR(G_GLEW_ATI_fragment_shader)

#endif /* GL_ATI_fragment_shader */

/* ------------------------ GL_ATI_map_object_buffer ----------------------- */

#ifndef GL_ATI_map_object_buffer
#define GL_ATI_map_object_buffer 1

typedef GLvoid * (GLAPIENTRY * PFNGLMAPOBJECTBUFFERATIPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLUNMAPOBJECTBUFFERATIPROC) (GLuint buffer);

#define glMapObjectBufferATI GLEW_GET_FUN(G_glewMapObjectBufferATI)
#define glUnmapObjectBufferATI GLEW_GET_FUN(G_glewUnmapObjectBufferATI)

#define GLEW_ATI_map_object_buffer GLEW_GET_VAR(G_GLEW_ATI_map_object_buffer)

#endif /* GL_ATI_map_object_buffer */

/* ----------------------------- GL_ATI_meminfo ---------------------------- */

#ifndef GL_ATI_meminfo
#define GL_ATI_meminfo 1

#define GL_VBO_FREE_MEMORY_ATI 0x87FB
#define GL_TEXTURE_FREE_MEMORY_ATI 0x87FC
#define GL_RENDERBUFFER_FREE_MEMORY_ATI 0x87FD

#define GLEW_ATI_meminfo GLEW_GET_VAR(G_GLEW_ATI_meminfo)

#endif /* GL_ATI_meminfo */

/* -------------------------- GL_ATI_pn_triangles -------------------------- */

#ifndef GL_ATI_pn_triangles
#define GL_ATI_pn_triangles 1

#define GL_PN_TRIANGLES_ATI 0x87F0
#define GL_MAX_PN_TRIANGLES_TESSELATION_LEVEL_ATI 0x87F1
#define GL_PN_TRIANGLES_POINT_MODE_ATI 0x87F2
#define GL_PN_TRIANGLES_NORMAL_MODE_ATI 0x87F3
#define GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI 0x87F4
#define GL_PN_TRIANGLES_POINT_MODE_LINEAR_ATI 0x87F5
#define GL_PN_TRIANGLES_POINT_MODE_CUBIC_ATI 0x87F6
#define GL_PN_TRIANGLES_NORMAL_MODE_LINEAR_ATI 0x87F7
#define GL_PN_TRIANGLES_NORMAL_MODE_QUADRATIC_ATI 0x87F8

typedef void (GLAPIENTRY * PFNGLPNTRIANGLESFATIPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPNTRIANGLESIATIPROC) (GLenum pname, GLint param);

#define glPNTrianglesfATI GLEW_GET_FUN(G_glewPNTrianglesfATI)
#define glPNTrianglesiATI GLEW_GET_FUN(G_glewPNTrianglesiATI)

#define GLEW_ATI_pn_triangles GLEW_GET_VAR(G_GLEW_ATI_pn_triangles)

#endif /* GL_ATI_pn_triangles */

/* ------------------------ GL_ATI_separate_stencil ------------------------ */

#ifndef GL_ATI_separate_stencil
#define GL_ATI_separate_stencil 1

#define GL_STENCIL_BACK_FUNC_ATI 0x8800
#define GL_STENCIL_BACK_FAIL_ATI 0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI 0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI 0x8803

typedef void (GLAPIENTRY * PFNGLSTENCILFUNCSEPARATEATIPROC) (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILOPSEPARATEATIPROC) (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);

#define glStencilFuncSeparateATI GLEW_GET_FUN(G_glewStencilFuncSeparateATI)
#define glStencilOpSeparateATI GLEW_GET_FUN(G_glewStencilOpSeparateATI)

#define GLEW_ATI_separate_stencil GLEW_GET_VAR(G_GLEW_ATI_separate_stencil)

#endif /* GL_ATI_separate_stencil */

/* ----------------------- GL_ATI_shader_texture_lod ----------------------- */

#ifndef GL_ATI_shader_texture_lod
#define GL_ATI_shader_texture_lod 1

#define GLEW_ATI_shader_texture_lod GLEW_GET_VAR(G_GLEW_ATI_shader_texture_lod)

#endif /* GL_ATI_shader_texture_lod */

/* ---------------------- GL_ATI_text_fragment_shader ---------------------- */

#ifndef GL_ATI_text_fragment_shader
#define GL_ATI_text_fragment_shader 1

#define GL_TEXT_FRAGMENT_SHADER_ATI 0x8200

#define GLEW_ATI_text_fragment_shader GLEW_GET_VAR(G_GLEW_ATI_text_fragment_shader)

#endif /* GL_ATI_text_fragment_shader */

/* --------------------- GL_ATI_texture_compression_3dc -------------------- */

#ifndef GL_ATI_texture_compression_3dc
#define GL_ATI_texture_compression_3dc 1

#define GL_COMPRESSED_LUMINANCE_ALPHA_3DC_ATI 0x8837

#define GLEW_ATI_texture_compression_3dc GLEW_GET_VAR(G_GLEW_ATI_texture_compression_3dc)

#endif /* GL_ATI_texture_compression_3dc */

/* ---------------------- GL_ATI_texture_env_combine3 ---------------------- */

#ifndef GL_ATI_texture_env_combine3
#define GL_ATI_texture_env_combine3 1

#define GL_MODULATE_ADD_ATI 0x8744
#define GL_MODULATE_SIGNED_ADD_ATI 0x8745
#define GL_MODULATE_SUBTRACT_ATI 0x8746

#define GLEW_ATI_texture_env_combine3 GLEW_GET_VAR(G_GLEW_ATI_texture_env_combine3)

#endif /* GL_ATI_texture_env_combine3 */

/* -------------------------- GL_ATI_texture_float ------------------------- */

#ifndef GL_ATI_texture_float
#define GL_ATI_texture_float 1

#define GL_RGBA_FLOAT32_ATI 0x8814
#define GL_RGB_FLOAT32_ATI 0x8815
#define GL_ALPHA_FLOAT32_ATI 0x8816
#define GL_INTENSITY_FLOAT32_ATI 0x8817
#define GL_LUMINANCE_FLOAT32_ATI 0x8818
#define GL_LUMINANCE_ALPHA_FLOAT32_ATI 0x8819
#define GL_RGBA_FLOAT16_ATI 0x881A
#define GL_RGB_FLOAT16_ATI 0x881B
#define GL_ALPHA_FLOAT16_ATI 0x881C
#define GL_INTENSITY_FLOAT16_ATI 0x881D
#define GL_LUMINANCE_FLOAT16_ATI 0x881E
#define GL_LUMINANCE_ALPHA_FLOAT16_ATI 0x881F

#define GLEW_ATI_texture_float GLEW_GET_VAR(G_GLEW_ATI_texture_float)

#endif /* GL_ATI_texture_float */

/* ----------------------- GL_ATI_texture_mirror_once ---------------------- */

#ifndef GL_ATI_texture_mirror_once
#define GL_ATI_texture_mirror_once 1

#define GL_MIRROR_CLAMP_ATI 0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_ATI 0x8743

#define GLEW_ATI_texture_mirror_once GLEW_GET_VAR(G_GLEW_ATI_texture_mirror_once)

#endif /* GL_ATI_texture_mirror_once */

/* ----------------------- GL_ATI_vertex_array_object ---------------------- */

#ifndef GL_ATI_vertex_array_object
#define GL_ATI_vertex_array_object 1

#define GL_STATIC_ATI 0x8760
#define GL_DYNAMIC_ATI 0x8761
#define GL_PRESERVE_ATI 0x8762
#define GL_DISCARD_ATI 0x8763
#define GL_OBJECT_BUFFER_SIZE_ATI 0x8764
#define GL_OBJECT_BUFFER_USAGE_ATI 0x8765
#define GL_ARRAY_OBJECT_BUFFER_ATI 0x8766
#define GL_ARRAY_OBJECT_OFFSET_ATI 0x8767

typedef void (GLAPIENTRY * PFNGLARRAYOBJECTATIPROC) (GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);
typedef void (GLAPIENTRY * PFNGLFREEOBJECTBUFFERATIPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLGETARRAYOBJECTFVATIPROC) (GLenum array, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETARRAYOBJECTIVATIPROC) (GLenum array, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTBUFFERFVATIPROC) (GLuint buffer, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETOBJECTBUFFERIVATIPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVARIANTARRAYOBJECTFVATIPROC) (GLuint id, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVARIANTARRAYOBJECTIVATIPROC) (GLuint id, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISOBJECTBUFFERATIPROC) (GLuint buffer);
typedef GLuint (GLAPIENTRY * PFNGLNEWOBJECTBUFFERATIPROC) (GLsizei size, const void* pointer, GLenum usage);
typedef void (GLAPIENTRY * PFNGLUPDATEOBJECTBUFFERATIPROC) (GLuint buffer, GLuint offset, GLsizei size, const void* pointer, GLenum preserve);
typedef void (GLAPIENTRY * PFNGLVARIANTARRAYOBJECTATIPROC) (GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset);

#define glArrayObjectATI GLEW_GET_FUN(G_glewArrayObjectATI)
#define glFreeObjectBufferATI GLEW_GET_FUN(G_glewFreeObjectBufferATI)
#define glGetArrayObjectfvATI GLEW_GET_FUN(G_glewGetArrayObjectfvATI)
#define glGetArrayObjectivATI GLEW_GET_FUN(G_glewGetArrayObjectivATI)
#define glGetObjectBufferfvATI GLEW_GET_FUN(G_glewGetObjectBufferfvATI)
#define glGetObjectBufferivATI GLEW_GET_FUN(G_glewGetObjectBufferivATI)
#define glGetVariantArrayObjectfvATI GLEW_GET_FUN(G_glewGetVariantArrayObjectfvATI)
#define glGetVariantArrayObjectivATI GLEW_GET_FUN(G_glewGetVariantArrayObjectivATI)
#define glIsObjectBufferATI GLEW_GET_FUN(G_glewIsObjectBufferATI)
#define glNewObjectBufferATI GLEW_GET_FUN(G_glewNewObjectBufferATI)
#define glUpdateObjectBufferATI GLEW_GET_FUN(G_glewUpdateObjectBufferATI)
#define glVariantArrayObjectATI GLEW_GET_FUN(G_glewVariantArrayObjectATI)

#define GLEW_ATI_vertex_array_object GLEW_GET_VAR(G_GLEW_ATI_vertex_array_object)

#endif /* GL_ATI_vertex_array_object */

/* ------------------- GL_ATI_vertex_attrib_array_object ------------------- */

#ifndef GL_ATI_vertex_attrib_array_object
#define GL_ATI_vertex_attrib_array_object 1

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC) (GLuint index, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBARRAYOBJECTATIPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset);

#define glGetVertexAttribArrayObjectfvATI GLEW_GET_FUN(G_glewGetVertexAttribArrayObjectfvATI)
#define glGetVertexAttribArrayObjectivATI GLEW_GET_FUN(G_glewGetVertexAttribArrayObjectivATI)
#define glVertexAttribArrayObjectATI GLEW_GET_FUN(G_glewVertexAttribArrayObjectATI)

#define GLEW_ATI_vertex_attrib_array_object GLEW_GET_VAR(G_GLEW_ATI_vertex_attrib_array_object)

#endif /* GL_ATI_vertex_attrib_array_object */

/* ------------------------- GL_ATI_vertex_streams ------------------------- */

#ifndef GL_ATI_vertex_streams
#define GL_ATI_vertex_streams 1

#define GL_MAX_VERTEX_STREAMS_ATI 0x876B
#define GL_VERTEX_SOURCE_ATI 0x876C
#define GL_VERTEX_STREAM0_ATI 0x876D
#define GL_VERTEX_STREAM1_ATI 0x876E
#define GL_VERTEX_STREAM2_ATI 0x876F
#define GL_VERTEX_STREAM3_ATI 0x8770
#define GL_VERTEX_STREAM4_ATI 0x8771
#define GL_VERTEX_STREAM5_ATI 0x8772
#define GL_VERTEX_STREAM6_ATI 0x8773
#define GL_VERTEX_STREAM7_ATI 0x8774

typedef void (GLAPIENTRY * PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC) (GLenum stream);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3BATIPROC) (GLenum stream, GLbyte x, GLbyte y, GLbyte z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3BVATIPROC) (GLenum stream, const GLbyte *coords);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3IATIPROC) (GLenum stream, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLNORMALSTREAM3SVATIPROC) (GLenum stream, const GLshort *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXBLENDENVFATIPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLVERTEXBLENDENVIATIPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1DATIPROC) (GLenum stream, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1FATIPROC) (GLenum stream, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1IATIPROC) (GLenum stream, GLint x);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1SATIPROC) (GLenum stream, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM1SVATIPROC) (GLenum stream, const GLshort *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2DATIPROC) (GLenum stream, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2FATIPROC) (GLenum stream, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2IATIPROC) (GLenum stream, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2SATIPROC) (GLenum stream, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM2SVATIPROC) (GLenum stream, const GLshort *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3IATIPROC) (GLenum stream, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM3SVATIPROC) (GLenum stream, const GLshort *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4DATIPROC) (GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4DVATIPROC) (GLenum stream, const GLdouble *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4FATIPROC) (GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4FVATIPROC) (GLenum stream, const GLfloat *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4IATIPROC) (GLenum stream, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4IVATIPROC) (GLenum stream, const GLint *coords);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4SATIPROC) (GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXSTREAM4SVATIPROC) (GLenum stream, const GLshort *coords);

#define glClientActiveVertexStreamATI GLEW_GET_FUN(G_glewClientActiveVertexStreamATI)
#define glNormalStream3bATI GLEW_GET_FUN(G_glewNormalStream3bATI)
#define glNormalStream3bvATI GLEW_GET_FUN(G_glewNormalStream3bvATI)
#define glNormalStream3dATI GLEW_GET_FUN(G_glewNormalStream3dATI)
#define glNormalStream3dvATI GLEW_GET_FUN(G_glewNormalStream3dvATI)
#define glNormalStream3fATI GLEW_GET_FUN(G_glewNormalStream3fATI)
#define glNormalStream3fvATI GLEW_GET_FUN(G_glewNormalStream3fvATI)
#define glNormalStream3iATI GLEW_GET_FUN(G_glewNormalStream3iATI)
#define glNormalStream3ivATI GLEW_GET_FUN(G_glewNormalStream3ivATI)
#define glNormalStream3sATI GLEW_GET_FUN(G_glewNormalStream3sATI)
#define glNormalStream3svATI GLEW_GET_FUN(G_glewNormalStream3svATI)
#define glVertexBlendEnvfATI GLEW_GET_FUN(G_glewVertexBlendEnvfATI)
#define glVertexBlendEnviATI GLEW_GET_FUN(G_glewVertexBlendEnviATI)
#define glVertexStream1dATI GLEW_GET_FUN(G_glewVertexStream1dATI)
#define glVertexStream1dvATI GLEW_GET_FUN(G_glewVertexStream1dvATI)
#define glVertexStream1fATI GLEW_GET_FUN(G_glewVertexStream1fATI)
#define glVertexStream1fvATI GLEW_GET_FUN(G_glewVertexStream1fvATI)
#define glVertexStream1iATI GLEW_GET_FUN(G_glewVertexStream1iATI)
#define glVertexStream1ivATI GLEW_GET_FUN(G_glewVertexStream1ivATI)
#define glVertexStream1sATI GLEW_GET_FUN(G_glewVertexStream1sATI)
#define glVertexStream1svATI GLEW_GET_FUN(G_glewVertexStream1svATI)
#define glVertexStream2dATI GLEW_GET_FUN(G_glewVertexStream2dATI)
#define glVertexStream2dvATI GLEW_GET_FUN(G_glewVertexStream2dvATI)
#define glVertexStream2fATI GLEW_GET_FUN(G_glewVertexStream2fATI)
#define glVertexStream2fvATI GLEW_GET_FUN(G_glewVertexStream2fvATI)
#define glVertexStream2iATI GLEW_GET_FUN(G_glewVertexStream2iATI)
#define glVertexStream2ivATI GLEW_GET_FUN(G_glewVertexStream2ivATI)
#define glVertexStream2sATI GLEW_GET_FUN(G_glewVertexStream2sATI)
#define glVertexStream2svATI GLEW_GET_FUN(G_glewVertexStream2svATI)
#define glVertexStream3dATI GLEW_GET_FUN(G_glewVertexStream3dATI)
#define glVertexStream3dvATI GLEW_GET_FUN(G_glewVertexStream3dvATI)
#define glVertexStream3fATI GLEW_GET_FUN(G_glewVertexStream3fATI)
#define glVertexStream3fvATI GLEW_GET_FUN(G_glewVertexStream3fvATI)
#define glVertexStream3iATI GLEW_GET_FUN(G_glewVertexStream3iATI)
#define glVertexStream3ivATI GLEW_GET_FUN(G_glewVertexStream3ivATI)
#define glVertexStream3sATI GLEW_GET_FUN(G_glewVertexStream3sATI)
#define glVertexStream3svATI GLEW_GET_FUN(G_glewVertexStream3svATI)
#define glVertexStream4dATI GLEW_GET_FUN(G_glewVertexStream4dATI)
#define glVertexStream4dvATI GLEW_GET_FUN(G_glewVertexStream4dvATI)
#define glVertexStream4fATI GLEW_GET_FUN(G_glewVertexStream4fATI)
#define glVertexStream4fvATI GLEW_GET_FUN(G_glewVertexStream4fvATI)
#define glVertexStream4iATI GLEW_GET_FUN(G_glewVertexStream4iATI)
#define glVertexStream4ivATI GLEW_GET_FUN(G_glewVertexStream4ivATI)
#define glVertexStream4sATI GLEW_GET_FUN(G_glewVertexStream4sATI)
#define glVertexStream4svATI GLEW_GET_FUN(G_glewVertexStream4svATI)

#define GLEW_ATI_vertex_streams GLEW_GET_VAR(G_GLEW_ATI_vertex_streams)

#endif /* GL_ATI_vertex_streams */

/* --------------------------- GL_EXT_422_pixels --------------------------- */

#ifndef GL_EXT_422_pixels
#define GL_EXT_422_pixels 1

#define GL_422_EXT 0x80CC
#define GL_422_REV_EXT 0x80CD
#define GL_422_AVERAGE_EXT 0x80CE
#define GL_422_REV_AVERAGE_EXT 0x80CF

#define GLEW_EXT_422_pixels GLEW_GET_VAR(G_GLEW_EXT_422_pixels)

#endif /* GL_EXT_422_pixels */

/* ---------------------------- GL_EXT_Cg_shader --------------------------- */

#ifndef GL_EXT_Cg_shader
#define GL_EXT_Cg_shader 1

#define GL_CG_VERTEX_SHADER_EXT 0x890E
#define GL_CG_FRAGMENT_SHADER_EXT 0x890F

#define GLEW_EXT_Cg_shader GLEW_GET_VAR(G_GLEW_EXT_Cg_shader)

#endif /* GL_EXT_Cg_shader */

/* ------------------------------ GL_EXT_abgr ------------------------------ */

#ifndef GL_EXT_abgr
#define GL_EXT_abgr 1

#define GL_ABGR_EXT 0x8000

#define GLEW_EXT_abgr GLEW_GET_VAR(G_GLEW_EXT_abgr)

#endif /* GL_EXT_abgr */

/* ------------------------------ GL_EXT_bgra ------------------------------ */

#ifndef GL_EXT_bgra
#define GL_EXT_bgra 1

#define GL_BGR_EXT 0x80E0
#define GL_BGRA_EXT 0x80E1

#define GLEW_EXT_bgra GLEW_GET_VAR(G_GLEW_EXT_bgra)

#endif /* GL_EXT_bgra */

/* ------------------------ GL_EXT_bindable_uniform ------------------------ */

#ifndef GL_EXT_bindable_uniform
#define GL_EXT_bindable_uniform 1

#define GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT 0x8DE2
#define GL_MAX_FRAGMENT_BINDABLE_UNIFORMS_EXT 0x8DE3
#define GL_MAX_GEOMETRY_BINDABLE_UNIFORMS_EXT 0x8DE4
#define GL_MAX_BINDABLE_UNIFORM_SIZE_EXT 0x8DED
#define GL_UNIFORM_BUFFER_EXT 0x8DEE
#define GL_UNIFORM_BUFFER_BINDING_EXT 0x8DEF

typedef GLint (GLAPIENTRY * PFNGLGETUNIFORMBUFFERSIZEEXTPROC) (GLuint program, GLint location);
typedef GLintptr (GLAPIENTRY * PFNGLGETUNIFORMOFFSETEXTPROC) (GLuint program, GLint location);
typedef void (GLAPIENTRY * PFNGLUNIFORMBUFFEREXTPROC) (GLuint program, GLint location, GLuint buffer);

#define glGetUniformBufferSizeEXT GLEW_GET_FUN(G_glewGetUniformBufferSizeEXT)
#define glGetUniformOffsetEXT GLEW_GET_FUN(G_glewGetUniformOffsetEXT)
#define glUniformBufferEXT GLEW_GET_FUN(G_glewUniformBufferEXT)

#define GLEW_EXT_bindable_uniform GLEW_GET_VAR(G_GLEW_EXT_bindable_uniform)

#endif /* GL_EXT_bindable_uniform */

/* --------------------------- GL_EXT_blend_color -------------------------- */

#ifndef GL_EXT_blend_color
#define GL_EXT_blend_color 1

#define GL_CONSTANT_COLOR_EXT 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR_EXT 0x8002
#define GL_CONSTANT_ALPHA_EXT 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA_EXT 0x8004
#define GL_BLEND_COLOR_EXT 0x8005

typedef void (GLAPIENTRY * PFNGLBLENDCOLOREXTPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

#define glBlendColorEXT GLEW_GET_FUN(G_glewBlendColorEXT)

#define GLEW_EXT_blend_color GLEW_GET_VAR(G_GLEW_EXT_blend_color)

#endif /* GL_EXT_blend_color */

/* --------------------- GL_EXT_blend_equation_separate -------------------- */

#ifndef GL_EXT_blend_equation_separate
#define GL_EXT_blend_equation_separate 1

#define GL_BLEND_EQUATION_RGB_EXT 0x8009
#define GL_BLEND_EQUATION_ALPHA_EXT 0x883D

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONSEPARATEEXTPROC) (GLenum modeRGB, GLenum modeAlpha);

#define glBlendEquationSeparateEXT GLEW_GET_FUN(G_glewBlendEquationSeparateEXT)

#define GLEW_EXT_blend_equation_separate GLEW_GET_VAR(G_GLEW_EXT_blend_equation_separate)

#endif /* GL_EXT_blend_equation_separate */

/* ----------------------- GL_EXT_blend_func_separate ---------------------- */

#ifndef GL_EXT_blend_func_separate
#define GL_EXT_blend_func_separate 1

#define GL_BLEND_DST_RGB_EXT 0x80C8
#define GL_BLEND_SRC_RGB_EXT 0x80C9
#define GL_BLEND_DST_ALPHA_EXT 0x80CA
#define GL_BLEND_SRC_ALPHA_EXT 0x80CB

typedef void (GLAPIENTRY * PFNGLBLENDFUNCSEPARATEEXTPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);

#define glBlendFuncSeparateEXT GLEW_GET_FUN(G_glewBlendFuncSeparateEXT)

#define GLEW_EXT_blend_func_separate GLEW_GET_VAR(G_GLEW_EXT_blend_func_separate)

#endif /* GL_EXT_blend_func_separate */

/* ------------------------- GL_EXT_blend_logic_op ------------------------- */

#ifndef GL_EXT_blend_logic_op
#define GL_EXT_blend_logic_op 1

#define GLEW_EXT_blend_logic_op GLEW_GET_VAR(G_GLEW_EXT_blend_logic_op)

#endif /* GL_EXT_blend_logic_op */

/* -------------------------- GL_EXT_blend_minmax -------------------------- */

#ifndef GL_EXT_blend_minmax
#define GL_EXT_blend_minmax 1

#define GL_FUNC_ADD_EXT 0x8006
#define GL_MIN_EXT 0x8007
#define GL_MAX_EXT 0x8008
#define GL_BLEND_EQUATION_EXT 0x8009

typedef void (GLAPIENTRY * PFNGLBLENDEQUATIONEXTPROC) (GLenum mode);

#define glBlendEquationEXT GLEW_GET_FUN(G_glewBlendEquationEXT)

#define GLEW_EXT_blend_minmax GLEW_GET_VAR(G_GLEW_EXT_blend_minmax)

#endif /* GL_EXT_blend_minmax */

/* ------------------------- GL_EXT_blend_subtract ------------------------- */

#ifndef GL_EXT_blend_subtract
#define GL_EXT_blend_subtract 1

#define GL_FUNC_SUBTRACT_EXT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT_EXT 0x800B

#define GLEW_EXT_blend_subtract GLEW_GET_VAR(G_GLEW_EXT_blend_subtract)

#endif /* GL_EXT_blend_subtract */

/* ------------------------ GL_EXT_clip_volume_hint ------------------------ */

#ifndef GL_EXT_clip_volume_hint
#define GL_EXT_clip_volume_hint 1

#define GL_CLIP_VOLUME_CLIPPING_HINT_EXT 0x80F0

#define GLEW_EXT_clip_volume_hint GLEW_GET_VAR(G_GLEW_EXT_clip_volume_hint)

#endif /* GL_EXT_clip_volume_hint */

/* ------------------------------ GL_EXT_cmyka ----------------------------- */

#ifndef GL_EXT_cmyka
#define GL_EXT_cmyka 1

#define GL_CMYK_EXT 0x800C
#define GL_CMYKA_EXT 0x800D
#define GL_PACK_CMYK_HINT_EXT 0x800E
#define GL_UNPACK_CMYK_HINT_EXT 0x800F

#define GLEW_EXT_cmyka GLEW_GET_VAR(G_GLEW_EXT_cmyka)

#endif /* GL_EXT_cmyka */

/* ------------------------- GL_EXT_color_subtable ------------------------- */

#ifndef GL_EXT_color_subtable
#define GL_EXT_color_subtable 1

typedef void (GLAPIENTRY * PFNGLCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void* data);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORSUBTABLEEXTPROC) (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width);

#define glColorSubTableEXT GLEW_GET_FUN(G_glewColorSubTableEXT)
#define glCopyColorSubTableEXT GLEW_GET_FUN(G_glewCopyColorSubTableEXT)

#define GLEW_EXT_color_subtable GLEW_GET_VAR(G_GLEW_EXT_color_subtable)

#endif /* GL_EXT_color_subtable */

/* ---------------------- GL_EXT_compiled_vertex_array --------------------- */

#ifndef GL_EXT_compiled_vertex_array
#define GL_EXT_compiled_vertex_array 1

#define GL_ARRAY_ELEMENT_LOCK_FIRST_EXT 0x81A8
#define GL_ARRAY_ELEMENT_LOCK_COUNT_EXT 0x81A9

typedef void (GLAPIENTRY * PFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLUNLOCKARRAYSEXTPROC) (void);

#define glLockArraysEXT GLEW_GET_FUN(G_glewLockArraysEXT)
#define glUnlockArraysEXT GLEW_GET_FUN(G_glewUnlockArraysEXT)

#define GLEW_EXT_compiled_vertex_array GLEW_GET_VAR(G_GLEW_EXT_compiled_vertex_array)

#endif /* GL_EXT_compiled_vertex_array */

/* --------------------------- GL_EXT_convolution -------------------------- */

#ifndef GL_EXT_convolution
#define GL_EXT_convolution 1

#define GL_CONVOLUTION_1D_EXT 0x8010
#define GL_CONVOLUTION_2D_EXT 0x8011
#define GL_SEPARABLE_2D_EXT 0x8012
#define GL_CONVOLUTION_BORDER_MODE_EXT 0x8013
#define GL_CONVOLUTION_FILTER_SCALE_EXT 0x8014
#define GL_CONVOLUTION_FILTER_BIAS_EXT 0x8015
#define GL_REDUCE_EXT 0x8016
#define GL_CONVOLUTION_FORMAT_EXT 0x8017
#define GL_CONVOLUTION_WIDTH_EXT 0x8018
#define GL_CONVOLUTION_HEIGHT_EXT 0x8019
#define GL_MAX_CONVOLUTION_WIDTH_EXT 0x801A
#define GL_MAX_CONVOLUTION_HEIGHT_EXT 0x801B
#define GL_POST_CONVOLUTION_RED_SCALE_EXT 0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE_EXT 0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE_EXT 0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE_EXT 0x801F
#define GL_POST_CONVOLUTION_RED_BIAS_EXT 0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS_EXT 0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS_EXT 0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS_EXT 0x8023

typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER1DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* image);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFEXTPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIEXTPROC) (GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONFILTEREXTPROC) (GLenum target, GLenum format, GLenum type, void* image);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETSEPARABLEFILTEREXTPROC) (GLenum target, GLenum format, GLenum type, void* row, void* column, void* span);
typedef void (GLAPIENTRY * PFNGLSEPARABLEFILTER2DEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* row, const void* column);

#define glConvolutionFilter1DEXT GLEW_GET_FUN(G_glewConvolutionFilter1DEXT)
#define glConvolutionFilter2DEXT GLEW_GET_FUN(G_glewConvolutionFilter2DEXT)
#define glConvolutionParameterfEXT GLEW_GET_FUN(G_glewConvolutionParameterfEXT)
#define glConvolutionParameterfvEXT GLEW_GET_FUN(G_glewConvolutionParameterfvEXT)
#define glConvolutionParameteriEXT GLEW_GET_FUN(G_glewConvolutionParameteriEXT)
#define glConvolutionParameterivEXT GLEW_GET_FUN(G_glewConvolutionParameterivEXT)
#define glCopyConvolutionFilter1DEXT GLEW_GET_FUN(G_glewCopyConvolutionFilter1DEXT)
#define glCopyConvolutionFilter2DEXT GLEW_GET_FUN(G_glewCopyConvolutionFilter2DEXT)
#define glGetConvolutionFilterEXT GLEW_GET_FUN(G_glewGetConvolutionFilterEXT)
#define glGetConvolutionParameterfvEXT GLEW_GET_FUN(G_glewGetConvolutionParameterfvEXT)
#define glGetConvolutionParameterivEXT GLEW_GET_FUN(G_glewGetConvolutionParameterivEXT)
#define glGetSeparableFilterEXT GLEW_GET_FUN(G_glewGetSeparableFilterEXT)
#define glSeparableFilter2DEXT GLEW_GET_FUN(G_glewSeparableFilter2DEXT)

#define GLEW_EXT_convolution GLEW_GET_VAR(G_GLEW_EXT_convolution)

#endif /* GL_EXT_convolution */

/* ------------------------ GL_EXT_coordinate_frame ------------------------ */

#ifndef GL_EXT_coordinate_frame
#define GL_EXT_coordinate_frame 1

#define GL_TANGENT_ARRAY_EXT 0x8439
#define GL_BINORMAL_ARRAY_EXT 0x843A
#define GL_CURRENT_TANGENT_EXT 0x843B
#define GL_CURRENT_BINORMAL_EXT 0x843C
#define GL_TANGENT_ARRAY_TYPE_EXT 0x843E
#define GL_TANGENT_ARRAY_STRIDE_EXT 0x843F
#define GL_BINORMAL_ARRAY_TYPE_EXT 0x8440
#define GL_BINORMAL_ARRAY_STRIDE_EXT 0x8441
#define GL_TANGENT_ARRAY_POINTER_EXT 0x8442
#define GL_BINORMAL_ARRAY_POINTER_EXT 0x8443
#define GL_MAP1_TANGENT_EXT 0x8444
#define GL_MAP2_TANGENT_EXT 0x8445
#define GL_MAP1_BINORMAL_EXT 0x8446
#define GL_MAP2_BINORMAL_EXT 0x8447

typedef void (GLAPIENTRY * PFNGLBINORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, void* pointer);
typedef void (GLAPIENTRY * PFNGLTANGENTPOINTEREXTPROC) (GLenum type, GLsizei stride, void* pointer);

#define glBinormalPointerEXT GLEW_GET_FUN(G_glewBinormalPointerEXT)
#define glTangentPointerEXT GLEW_GET_FUN(G_glewTangentPointerEXT)

#define GLEW_EXT_coordinate_frame GLEW_GET_VAR(G_GLEW_EXT_coordinate_frame)

#endif /* GL_EXT_coordinate_frame */

/* -------------------------- GL_EXT_copy_texture -------------------------- */

#ifndef GL_EXT_copy_texture
#define GL_EXT_copy_texture 1

typedef void (GLAPIENTRY * PFNGLCOPYTEXIMAGE1DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXIMAGE2DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);

#define glCopyTexImage1DEXT GLEW_GET_FUN(G_glewCopyTexImage1DEXT)
#define glCopyTexImage2DEXT GLEW_GET_FUN(G_glewCopyTexImage2DEXT)
#define glCopyTexSubImage1DEXT GLEW_GET_FUN(G_glewCopyTexSubImage1DEXT)
#define glCopyTexSubImage2DEXT GLEW_GET_FUN(G_glewCopyTexSubImage2DEXT)
#define glCopyTexSubImage3DEXT GLEW_GET_FUN(G_glewCopyTexSubImage3DEXT)

#define GLEW_EXT_copy_texture GLEW_GET_VAR(G_GLEW_EXT_copy_texture)

#endif /* GL_EXT_copy_texture */

/* --------------------------- GL_EXT_cull_vertex -------------------------- */

#ifndef GL_EXT_cull_vertex
#define GL_EXT_cull_vertex 1

#define GL_CULL_VERTEX_EXT 0x81AA
#define GL_CULL_VERTEX_EYE_POSITION_EXT 0x81AB
#define GL_CULL_VERTEX_OBJECT_POSITION_EXT 0x81AC

typedef void (GLAPIENTRY * PFNGLCULLPARAMETERDVEXTPROC) (GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLCULLPARAMETERFVEXTPROC) (GLenum pname, GLfloat* params);

#define glCullParameterdvEXT GLEW_GET_FUN(G_glewCullParameterdvEXT)
#define glCullParameterfvEXT GLEW_GET_FUN(G_glewCullParameterfvEXT)

#define GLEW_EXT_cull_vertex GLEW_GET_VAR(G_GLEW_EXT_cull_vertex)

#endif /* GL_EXT_cull_vertex */

/* -------------------------- GL_EXT_debug_marker -------------------------- */

#ifndef GL_EXT_debug_marker
#define GL_EXT_debug_marker 1

typedef void (GLAPIENTRY * PFNGLINSERTEVENTMARKEREXTPROC) (GLsizei length, const GLchar* marker);
typedef void (GLAPIENTRY * PFNGLPOPGROUPMARKEREXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLPUSHGROUPMARKEREXTPROC) (GLsizei length, const GLchar* marker);

#define glInsertEventMarkerEXT GLEW_GET_FUN(G_glewInsertEventMarkerEXT)
#define glPopGroupMarkerEXT GLEW_GET_FUN(G_glewPopGroupMarkerEXT)
#define glPushGroupMarkerEXT GLEW_GET_FUN(G_glewPushGroupMarkerEXT)

#define GLEW_EXT_debug_marker GLEW_GET_VAR(G_GLEW_EXT_debug_marker)

#endif /* GL_EXT_debug_marker */

/* ------------------------ GL_EXT_depth_bounds_test ----------------------- */

#ifndef GL_EXT_depth_bounds_test
#define GL_EXT_depth_bounds_test 1

#define GL_DEPTH_BOUNDS_TEST_EXT 0x8890
#define GL_DEPTH_BOUNDS_EXT 0x8891

typedef void (GLAPIENTRY * PFNGLDEPTHBOUNDSEXTPROC) (GLclampd zmin, GLclampd zmax);

#define glDepthBoundsEXT GLEW_GET_FUN(G_glewDepthBoundsEXT)

#define GLEW_EXT_depth_bounds_test GLEW_GET_VAR(G_GLEW_EXT_depth_bounds_test)

#endif /* GL_EXT_depth_bounds_test */

/* ----------------------- GL_EXT_direct_state_access ---------------------- */

#ifndef GL_EXT_direct_state_access
#define GL_EXT_direct_state_access 1

#define GL_PROGRAM_MATRIX_EXT 0x8E2D
#define GL_TRANSPOSE_PROGRAM_MATRIX_EXT 0x8E2E
#define GL_PROGRAM_MATRIX_STACK_DEPTH_EXT 0x8E2F

typedef void (GLAPIENTRY * PFNGLBINDMULTITEXTUREEXTPROC) (GLenum texunit, GLenum target, GLuint texture);
typedef GLenum (GLAPIENTRY * PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC) (GLuint framebuffer, GLenum target);
typedef void (GLAPIENTRY * PFNGLCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void* data);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLDISABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEINDEXEDEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLECLIENTSTATEIEXTPROC) (GLenum array, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYATTRIBEXTPROC) (GLuint vaobj, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEVERTEXARRAYEXTPROC) (GLuint vaobj, GLenum array);
typedef void (GLAPIENTRY * PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC) (GLuint framebuffer, GLsizei n, const GLenum* bufs);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERREADBUFFEREXTPROC) (GLuint framebuffer, GLenum mode);
typedef void (GLAPIENTRY * PFNGLGENERATEMULTITEXMIPMAPEXTPROC) (GLenum texunit, GLenum target);
typedef void (GLAPIENTRY * PFNGLGENERATETEXTUREMIPMAPEXTPROC) (GLuint texture, GLenum target);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, void* img);
typedef void (GLAPIENTRY * PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, void* img);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEINDEXEDVEXTPROC) (GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETDOUBLEI_VEXTPROC) (GLenum pname, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETFLOATINDEXEDVEXTPROC) (GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFLOATI_VEXTPROC) (GLenum pname, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXIMAGEEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC) (GLuint buffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPOINTERVEXTPROC) (GLuint buffer, GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void* data);
typedef void (GLAPIENTRY * PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum pname, void* string);
typedef void (GLAPIENTRY * PFNGLGETNAMEDPROGRAMIVEXTPROC) (GLuint program, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC) (GLuint renderbuffer, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERINDEXEDVEXTPROC) (GLenum target, GLuint index, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETPOINTERI_VEXTPROC) (GLenum pname, GLuint index, GLvoid** params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREIMAGEEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void* pixels);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLint level, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYINTEGERVEXTPROC) (GLuint vaobj, GLenum pname, GLint* param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC) (GLuint vaobj, GLuint index, GLenum pname, GLvoid** param);
typedef void (GLAPIENTRY * PFNGLGETVERTEXARRAYPOINTERVEXTPROC) (GLuint vaobj, GLenum pname, GLvoid** param);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPNAMEDBUFFEREXTPROC) (GLuint buffer, GLenum access);
typedef GLvoid * (GLAPIENTRY * PFNGLMAPNAMEDBUFFERRANGEEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void (GLAPIENTRY * PFNGLMATRIXFRUSTUMEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADIDENTITYEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXLOADFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTTRANSPOSEFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTDEXTPROC) (GLenum matrixMode, const GLdouble* m);
typedef void (GLAPIENTRY * PFNGLMATRIXMULTFEXTPROC) (GLenum matrixMode, const GLfloat* m);
typedef void (GLAPIENTRY * PFNGLMATRIXORTHOEXTPROC) (GLenum matrixMode, GLdouble l, GLdouble r, GLdouble b, GLdouble t, GLdouble n, GLdouble f);
typedef void (GLAPIENTRY * PFNGLMATRIXPOPEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXPUSHEXTPROC) (GLenum matrixMode);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEDEXTPROC) (GLenum matrixMode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXROTATEFEXTPROC) (GLenum matrixMode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXSCALEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEDEXTPROC) (GLenum matrixMode, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLMATRIXTRANSLATEFEXTPROC) (GLenum matrixMode, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLMULTITEXBUFFEREXTPROC) (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORDPOINTEREXTPROC) (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXENVIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLdouble param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENDVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENFVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXGENIVEXTPROC) (GLenum texunit, GLenum coord, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIUIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERFVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIEXTPROC) (GLenum texunit, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLMULTITEXPARAMETERIVEXTPROC) (GLenum texunit, GLenum target, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLMULTITEXRENDERBUFFEREXTPROC) (GLenum texunit, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE1DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE2DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLMULTITEXSUBIMAGE3DEXTPROC) (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERDATAEXTPROC) (GLuint buffer, GLsizeiptr size, const void* data, GLenum usage);
typedef void (GLAPIENTRY * PFNGLNAMEDBUFFERSUBDATAEXTPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void* data);
typedef void (GLAPIENTRY * PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC) (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC) (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC) (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC) (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC) (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC) (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLNAMEDPROGRAMSTRINGEXTPROC) (GLuint program, GLenum target, GLenum format, GLsizei len, const void* string);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC) (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DEXTPROC) (GLuint program, GLint location, GLdouble x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FEXTPROC) (GLuint program, GLint location, GLfloat v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IEXTPROC) (GLuint program, GLint location, GLint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIEXTPROC) (GLuint program, GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DEXTPROC) (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4DVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FEXTPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4FVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IEXTPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4IVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIEXTPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UIVEXTPROC) (GLuint program, GLint location, GLsizei count, const GLuint* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC) (GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXTUREBUFFEREXTPROC) (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIUIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLuint* params);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERFVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLfloat* param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIEXTPROC) (GLuint texture, GLenum target, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLTEXTUREPARAMETERIVEXTPROC) (GLuint texture, GLenum target, GLenum pname, const GLint* param);
typedef void (GLAPIENTRY * PFNGLTEXTURERENDERBUFFEREXTPROC) (GLuint texture, GLenum target, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE1DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE2DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXTURESUBIMAGE3DEXTPROC) (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
typedef GLboolean (GLAPIENTRY * PFNGLUNMAPNAMEDBUFFEREXTPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYINDEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYNORMALOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset);

#define glBindMultiTextureEXT GLEW_GET_FUN(G_glewBindMultiTextureEXT)
#define glCheckNamedFramebufferStatusEXT GLEW_GET_FUN(G_glewCheckNamedFramebufferStatusEXT)
#define glClientAttribDefaultEXT GLEW_GET_FUN(G_glewClientAttribDefaultEXT)
#define glCompressedMultiTexImage1DEXT GLEW_GET_FUN(G_glewCompressedMultiTexImage1DEXT)
#define glCompressedMultiTexImage2DEXT GLEW_GET_FUN(G_glewCompressedMultiTexImage2DEXT)
#define glCompressedMultiTexImage3DEXT GLEW_GET_FUN(G_glewCompressedMultiTexImage3DEXT)
#define glCompressedMultiTexSubImage1DEXT GLEW_GET_FUN(G_glewCompressedMultiTexSubImage1DEXT)
#define glCompressedMultiTexSubImage2DEXT GLEW_GET_FUN(G_glewCompressedMultiTexSubImage2DEXT)
#define glCompressedMultiTexSubImage3DEXT GLEW_GET_FUN(G_glewCompressedMultiTexSubImage3DEXT)
#define glCompressedTextureImage1DEXT GLEW_GET_FUN(G_glewCompressedTextureImage1DEXT)
#define glCompressedTextureImage2DEXT GLEW_GET_FUN(G_glewCompressedTextureImage2DEXT)
#define glCompressedTextureImage3DEXT GLEW_GET_FUN(G_glewCompressedTextureImage3DEXT)
#define glCompressedTextureSubImage1DEXT GLEW_GET_FUN(G_glewCompressedTextureSubImage1DEXT)
#define glCompressedTextureSubImage2DEXT GLEW_GET_FUN(G_glewCompressedTextureSubImage2DEXT)
#define glCompressedTextureSubImage3DEXT GLEW_GET_FUN(G_glewCompressedTextureSubImage3DEXT)
#define glCopyMultiTexImage1DEXT GLEW_GET_FUN(G_glewCopyMultiTexImage1DEXT)
#define glCopyMultiTexImage2DEXT GLEW_GET_FUN(G_glewCopyMultiTexImage2DEXT)
#define glCopyMultiTexSubImage1DEXT GLEW_GET_FUN(G_glewCopyMultiTexSubImage1DEXT)
#define glCopyMultiTexSubImage2DEXT GLEW_GET_FUN(G_glewCopyMultiTexSubImage2DEXT)
#define glCopyMultiTexSubImage3DEXT GLEW_GET_FUN(G_glewCopyMultiTexSubImage3DEXT)
#define glCopyTextureImage1DEXT GLEW_GET_FUN(G_glewCopyTextureImage1DEXT)
#define glCopyTextureImage2DEXT GLEW_GET_FUN(G_glewCopyTextureImage2DEXT)
#define glCopyTextureSubImage1DEXT GLEW_GET_FUN(G_glewCopyTextureSubImage1DEXT)
#define glCopyTextureSubImage2DEXT GLEW_GET_FUN(G_glewCopyTextureSubImage2DEXT)
#define glCopyTextureSubImage3DEXT GLEW_GET_FUN(G_glewCopyTextureSubImage3DEXT)
#define glDisableClientStateIndexedEXT GLEW_GET_FUN(G_glewDisableClientStateIndexedEXT)
#define glDisableClientStateiEXT GLEW_GET_FUN(G_glewDisableClientStateiEXT)
#define glDisableVertexArrayAttribEXT GLEW_GET_FUN(G_glewDisableVertexArrayAttribEXT)
#define glDisableVertexArrayEXT GLEW_GET_FUN(G_glewDisableVertexArrayEXT)
#define glEnableClientStateIndexedEXT GLEW_GET_FUN(G_glewEnableClientStateIndexedEXT)
#define glEnableClientStateiEXT GLEW_GET_FUN(G_glewEnableClientStateiEXT)
#define glEnableVertexArrayAttribEXT GLEW_GET_FUN(G_glewEnableVertexArrayAttribEXT)
#define glEnableVertexArrayEXT GLEW_GET_FUN(G_glewEnableVertexArrayEXT)
#define glFlushMappedNamedBufferRangeEXT GLEW_GET_FUN(G_glewFlushMappedNamedBufferRangeEXT)
#define glFramebufferDrawBufferEXT GLEW_GET_FUN(G_glewFramebufferDrawBufferEXT)
#define glFramebufferDrawBuffersEXT GLEW_GET_FUN(G_glewFramebufferDrawBuffersEXT)
#define glFramebufferReadBufferEXT GLEW_GET_FUN(G_glewFramebufferReadBufferEXT)
#define glGenerateMultiTexMipmapEXT GLEW_GET_FUN(G_glewGenerateMultiTexMipmapEXT)
#define glGenerateTextureMipmapEXT GLEW_GET_FUN(G_glewGenerateTextureMipmapEXT)
#define glGetCompressedMultiTexImageEXT GLEW_GET_FUN(G_glewGetCompressedMultiTexImageEXT)
#define glGetCompressedTextureImageEXT GLEW_GET_FUN(G_glewGetCompressedTextureImageEXT)
#define glGetDoubleIndexedvEXT GLEW_GET_FUN(G_glewGetDoubleIndexedvEXT)
#define glGetDoublei_vEXT GLEW_GET_FUN(G_glewGetDoublei_vEXT)
#define glGetFloatIndexedvEXT GLEW_GET_FUN(G_glewGetFloatIndexedvEXT)
#define glGetFloati_vEXT GLEW_GET_FUN(G_glewGetFloati_vEXT)
#define glGetFramebufferParameterivEXT GLEW_GET_FUN(G_glewGetFramebufferParameterivEXT)
#define glGetMultiTexEnvfvEXT GLEW_GET_FUN(G_glewGetMultiTexEnvfvEXT)
#define glGetMultiTexEnvivEXT GLEW_GET_FUN(G_glewGetMultiTexEnvivEXT)
#define glGetMultiTexGendvEXT GLEW_GET_FUN(G_glewGetMultiTexGendvEXT)
#define glGetMultiTexGenfvEXT GLEW_GET_FUN(G_glewGetMultiTexGenfvEXT)
#define glGetMultiTexGenivEXT GLEW_GET_FUN(G_glewGetMultiTexGenivEXT)
#define glGetMultiTexImageEXT GLEW_GET_FUN(G_glewGetMultiTexImageEXT)
#define glGetMultiTexLevelParameterfvEXT GLEW_GET_FUN(G_glewGetMultiTexLevelParameterfvEXT)
#define glGetMultiTexLevelParameterivEXT GLEW_GET_FUN(G_glewGetMultiTexLevelParameterivEXT)
#define glGetMultiTexParameterIivEXT GLEW_GET_FUN(G_glewGetMultiTexParameterIivEXT)
#define glGetMultiTexParameterIuivEXT GLEW_GET_FUN(G_glewGetMultiTexParameterIuivEXT)
#define glGetMultiTexParameterfvEXT GLEW_GET_FUN(G_glewGetMultiTexParameterfvEXT)
#define glGetMultiTexParameterivEXT GLEW_GET_FUN(G_glewGetMultiTexParameterivEXT)
#define glGetNamedBufferParameterivEXT GLEW_GET_FUN(G_glewGetNamedBufferParameterivEXT)
#define glGetNamedBufferPointervEXT GLEW_GET_FUN(G_glewGetNamedBufferPointervEXT)
#define glGetNamedBufferSubDataEXT GLEW_GET_FUN(G_glewGetNamedBufferSubDataEXT)
#define glGetNamedFramebufferAttachmentParameterivEXT GLEW_GET_FUN(G_glewGetNamedFramebufferAttachmentParameterivEXT)
#define glGetNamedProgramLocalParameterIivEXT GLEW_GET_FUN(G_glewGetNamedProgramLocalParameterIivEXT)
#define glGetNamedProgramLocalParameterIuivEXT GLEW_GET_FUN(G_glewGetNamedProgramLocalParameterIuivEXT)
#define glGetNamedProgramLocalParameterdvEXT GLEW_GET_FUN(G_glewGetNamedProgramLocalParameterdvEXT)
#define glGetNamedProgramLocalParameterfvEXT GLEW_GET_FUN(G_glewGetNamedProgramLocalParameterfvEXT)
#define glGetNamedProgramStringEXT GLEW_GET_FUN(G_glewGetNamedProgramStringEXT)
#define glGetNamedProgramivEXT GLEW_GET_FUN(G_glewGetNamedProgramivEXT)
#define glGetNamedRenderbufferParameterivEXT GLEW_GET_FUN(G_glewGetNamedRenderbufferParameterivEXT)
#define glGetPointerIndexedvEXT GLEW_GET_FUN(G_glewGetPointerIndexedvEXT)
#define glGetPointeri_vEXT GLEW_GET_FUN(G_glewGetPointeri_vEXT)
#define glGetTextureImageEXT GLEW_GET_FUN(G_glewGetTextureImageEXT)
#define glGetTextureLevelParameterfvEXT GLEW_GET_FUN(G_glewGetTextureLevelParameterfvEXT)
#define glGetTextureLevelParameterivEXT GLEW_GET_FUN(G_glewGetTextureLevelParameterivEXT)
#define glGetTextureParameterIivEXT GLEW_GET_FUN(G_glewGetTextureParameterIivEXT)
#define glGetTextureParameterIuivEXT GLEW_GET_FUN(G_glewGetTextureParameterIuivEXT)
#define glGetTextureParameterfvEXT GLEW_GET_FUN(G_glewGetTextureParameterfvEXT)
#define glGetTextureParameterivEXT GLEW_GET_FUN(G_glewGetTextureParameterivEXT)
#define glGetVertexArrayIntegeri_vEXT GLEW_GET_FUN(G_glewGetVertexArrayIntegeri_vEXT)
#define glGetVertexArrayIntegervEXT GLEW_GET_FUN(G_glewGetVertexArrayIntegervEXT)
#define glGetVertexArrayPointeri_vEXT GLEW_GET_FUN(G_glewGetVertexArrayPointeri_vEXT)
#define glGetVertexArrayPointervEXT GLEW_GET_FUN(G_glewGetVertexArrayPointervEXT)
#define glMapNamedBufferEXT GLEW_GET_FUN(G_glewMapNamedBufferEXT)
#define glMapNamedBufferRangeEXT GLEW_GET_FUN(G_glewMapNamedBufferRangeEXT)
#define glMatrixFrustumEXT GLEW_GET_FUN(G_glewMatrixFrustumEXT)
#define glMatrixLoadIdentityEXT GLEW_GET_FUN(G_glewMatrixLoadIdentityEXT)
#define glMatrixLoadTransposedEXT GLEW_GET_FUN(G_glewMatrixLoadTransposedEXT)
#define glMatrixLoadTransposefEXT GLEW_GET_FUN(G_glewMatrixLoadTransposefEXT)
#define glMatrixLoaddEXT GLEW_GET_FUN(G_glewMatrixLoaddEXT)
#define glMatrixLoadfEXT GLEW_GET_FUN(G_glewMatrixLoadfEXT)
#define glMatrixMultTransposedEXT GLEW_GET_FUN(G_glewMatrixMultTransposedEXT)
#define glMatrixMultTransposefEXT GLEW_GET_FUN(G_glewMatrixMultTransposefEXT)
#define glMatrixMultdEXT GLEW_GET_FUN(G_glewMatrixMultdEXT)
#define glMatrixMultfEXT GLEW_GET_FUN(G_glewMatrixMultfEXT)
#define glMatrixOrthoEXT GLEW_GET_FUN(G_glewMatrixOrthoEXT)
#define glMatrixPopEXT GLEW_GET_FUN(G_glewMatrixPopEXT)
#define glMatrixPushEXT GLEW_GET_FUN(G_glewMatrixPushEXT)
#define glMatrixRotatedEXT GLEW_GET_FUN(G_glewMatrixRotatedEXT)
#define glMatrixRotatefEXT GLEW_GET_FUN(G_glewMatrixRotatefEXT)
#define glMatrixScaledEXT GLEW_GET_FUN(G_glewMatrixScaledEXT)
#define glMatrixScalefEXT GLEW_GET_FUN(G_glewMatrixScalefEXT)
#define glMatrixTranslatedEXT GLEW_GET_FUN(G_glewMatrixTranslatedEXT)
#define glMatrixTranslatefEXT GLEW_GET_FUN(G_glewMatrixTranslatefEXT)
#define glMultiTexBufferEXT GLEW_GET_FUN(G_glewMultiTexBufferEXT)
#define glMultiTexCoordPointerEXT GLEW_GET_FUN(G_glewMultiTexCoordPointerEXT)
#define glMultiTexEnvfEXT GLEW_GET_FUN(G_glewMultiTexEnvfEXT)
#define glMultiTexEnvfvEXT GLEW_GET_FUN(G_glewMultiTexEnvfvEXT)
#define glMultiTexEnviEXT GLEW_GET_FUN(G_glewMultiTexEnviEXT)
#define glMultiTexEnvivEXT GLEW_GET_FUN(G_glewMultiTexEnvivEXT)
#define glMultiTexGendEXT GLEW_GET_FUN(G_glewMultiTexGendEXT)
#define glMultiTexGendvEXT GLEW_GET_FUN(G_glewMultiTexGendvEXT)
#define glMultiTexGenfEXT GLEW_GET_FUN(G_glewMultiTexGenfEXT)
#define glMultiTexGenfvEXT GLEW_GET_FUN(G_glewMultiTexGenfvEXT)
#define glMultiTexGeniEXT GLEW_GET_FUN(G_glewMultiTexGeniEXT)
#define glMultiTexGenivEXT GLEW_GET_FUN(G_glewMultiTexGenivEXT)
#define glMultiTexImage1DEXT GLEW_GET_FUN(G_glewMultiTexImage1DEXT)
#define glMultiTexImage2DEXT GLEW_GET_FUN(G_glewMultiTexImage2DEXT)
#define glMultiTexImage3DEXT GLEW_GET_FUN(G_glewMultiTexImage3DEXT)
#define glMultiTexParameterIivEXT GLEW_GET_FUN(G_glewMultiTexParameterIivEXT)
#define glMultiTexParameterIuivEXT GLEW_GET_FUN(G_glewMultiTexParameterIuivEXT)
#define glMultiTexParameterfEXT GLEW_GET_FUN(G_glewMultiTexParameterfEXT)
#define glMultiTexParameterfvEXT GLEW_GET_FUN(G_glewMultiTexParameterfvEXT)
#define glMultiTexParameteriEXT GLEW_GET_FUN(G_glewMultiTexParameteriEXT)
#define glMultiTexParameterivEXT GLEW_GET_FUN(G_glewMultiTexParameterivEXT)
#define glMultiTexRenderbufferEXT GLEW_GET_FUN(G_glewMultiTexRenderbufferEXT)
#define glMultiTexSubImage1DEXT GLEW_GET_FUN(G_glewMultiTexSubImage1DEXT)
#define glMultiTexSubImage2DEXT GLEW_GET_FUN(G_glewMultiTexSubImage2DEXT)
#define glMultiTexSubImage3DEXT GLEW_GET_FUN(G_glewMultiTexSubImage3DEXT)
#define glNamedBufferDataEXT GLEW_GET_FUN(G_glewNamedBufferDataEXT)
#define glNamedBufferSubDataEXT GLEW_GET_FUN(G_glewNamedBufferSubDataEXT)
#define glNamedCopyBufferSubDataEXT GLEW_GET_FUN(G_glewNamedCopyBufferSubDataEXT)
#define glNamedFramebufferRenderbufferEXT GLEW_GET_FUN(G_glewNamedFramebufferRenderbufferEXT)
#define glNamedFramebufferTexture1DEXT GLEW_GET_FUN(G_glewNamedFramebufferTexture1DEXT)
#define glNamedFramebufferTexture2DEXT GLEW_GET_FUN(G_glewNamedFramebufferTexture2DEXT)
#define glNamedFramebufferTexture3DEXT GLEW_GET_FUN(G_glewNamedFramebufferTexture3DEXT)
#define glNamedFramebufferTextureEXT GLEW_GET_FUN(G_glewNamedFramebufferTextureEXT)
#define glNamedFramebufferTextureFaceEXT GLEW_GET_FUN(G_glewNamedFramebufferTextureFaceEXT)
#define glNamedFramebufferTextureLayerEXT GLEW_GET_FUN(G_glewNamedFramebufferTextureLayerEXT)
#define glNamedProgramLocalParameter4dEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameter4dEXT)
#define glNamedProgramLocalParameter4dvEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameter4dvEXT)
#define glNamedProgramLocalParameter4fEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameter4fEXT)
#define glNamedProgramLocalParameter4fvEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameter4fvEXT)
#define glNamedProgramLocalParameterI4iEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameterI4iEXT)
#define glNamedProgramLocalParameterI4ivEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameterI4ivEXT)
#define glNamedProgramLocalParameterI4uiEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameterI4uiEXT)
#define glNamedProgramLocalParameterI4uivEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameterI4uivEXT)
#define glNamedProgramLocalParameters4fvEXT GLEW_GET_FUN(G_glewNamedProgramLocalParameters4fvEXT)
#define glNamedProgramLocalParametersI4ivEXT GLEW_GET_FUN(G_glewNamedProgramLocalParametersI4ivEXT)
#define glNamedProgramLocalParametersI4uivEXT GLEW_GET_FUN(G_glewNamedProgramLocalParametersI4uivEXT)
#define glNamedProgramStringEXT GLEW_GET_FUN(G_glewNamedProgramStringEXT)
#define glNamedRenderbufferStorageEXT GLEW_GET_FUN(G_glewNamedRenderbufferStorageEXT)
#define glNamedRenderbufferStorageMultisampleCoverageEXT GLEW_GET_FUN(G_glewNamedRenderbufferStorageMultisampleCoverageEXT)
#define glNamedRenderbufferStorageMultisampleEXT GLEW_GET_FUN(G_glewNamedRenderbufferStorageMultisampleEXT)
#define glProgramUniform1dEXT GLEW_GET_FUN(G_glewProgramUniform1dEXT)
#define glProgramUniform1dvEXT GLEW_GET_FUN(G_glewProgramUniform1dvEXT)
#define glProgramUniform1fEXT GLEW_GET_FUN(G_glewProgramUniform1fEXT)
#define glProgramUniform1fvEXT GLEW_GET_FUN(G_glewProgramUniform1fvEXT)
#define glProgramUniform1iEXT GLEW_GET_FUN(G_glewProgramUniform1iEXT)
#define glProgramUniform1ivEXT GLEW_GET_FUN(G_glewProgramUniform1ivEXT)
#define glProgramUniform1uiEXT GLEW_GET_FUN(G_glewProgramUniform1uiEXT)
#define glProgramUniform1uivEXT GLEW_GET_FUN(G_glewProgramUniform1uivEXT)
#define glProgramUniform2dEXT GLEW_GET_FUN(G_glewProgramUniform2dEXT)
#define glProgramUniform2dvEXT GLEW_GET_FUN(G_glewProgramUniform2dvEXT)
#define glProgramUniform2fEXT GLEW_GET_FUN(G_glewProgramUniform2fEXT)
#define glProgramUniform2fvEXT GLEW_GET_FUN(G_glewProgramUniform2fvEXT)
#define glProgramUniform2iEXT GLEW_GET_FUN(G_glewProgramUniform2iEXT)
#define glProgramUniform2ivEXT GLEW_GET_FUN(G_glewProgramUniform2ivEXT)
#define glProgramUniform2uiEXT GLEW_GET_FUN(G_glewProgramUniform2uiEXT)
#define glProgramUniform2uivEXT GLEW_GET_FUN(G_glewProgramUniform2uivEXT)
#define glProgramUniform3dEXT GLEW_GET_FUN(G_glewProgramUniform3dEXT)
#define glProgramUniform3dvEXT GLEW_GET_FUN(G_glewProgramUniform3dvEXT)
#define glProgramUniform3fEXT GLEW_GET_FUN(G_glewProgramUniform3fEXT)
#define glProgramUniform3fvEXT GLEW_GET_FUN(G_glewProgramUniform3fvEXT)
#define glProgramUniform3iEXT GLEW_GET_FUN(G_glewProgramUniform3iEXT)
#define glProgramUniform3ivEXT GLEW_GET_FUN(G_glewProgramUniform3ivEXT)
#define glProgramUniform3uiEXT GLEW_GET_FUN(G_glewProgramUniform3uiEXT)
#define glProgramUniform3uivEXT GLEW_GET_FUN(G_glewProgramUniform3uivEXT)
#define glProgramUniform4dEXT GLEW_GET_FUN(G_glewProgramUniform4dEXT)
#define glProgramUniform4dvEXT GLEW_GET_FUN(G_glewProgramUniform4dvEXT)
#define glProgramUniform4fEXT GLEW_GET_FUN(G_glewProgramUniform4fEXT)
#define glProgramUniform4fvEXT GLEW_GET_FUN(G_glewProgramUniform4fvEXT)
#define glProgramUniform4iEXT GLEW_GET_FUN(G_glewProgramUniform4iEXT)
#define glProgramUniform4ivEXT GLEW_GET_FUN(G_glewProgramUniform4ivEXT)
#define glProgramUniform4uiEXT GLEW_GET_FUN(G_glewProgramUniform4uiEXT)
#define glProgramUniform4uivEXT GLEW_GET_FUN(G_glewProgramUniform4uivEXT)
#define glProgramUniformMatrix2dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix2dvEXT)
#define glProgramUniformMatrix2fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix2fvEXT)
#define glProgramUniformMatrix2x3dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix2x3dvEXT)
#define glProgramUniformMatrix2x3fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix2x3fvEXT)
#define glProgramUniformMatrix2x4dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix2x4dvEXT)
#define glProgramUniformMatrix2x4fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix2x4fvEXT)
#define glProgramUniformMatrix3dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix3dvEXT)
#define glProgramUniformMatrix3fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix3fvEXT)
#define glProgramUniformMatrix3x2dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix3x2dvEXT)
#define glProgramUniformMatrix3x2fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix3x2fvEXT)
#define glProgramUniformMatrix3x4dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix3x4dvEXT)
#define glProgramUniformMatrix3x4fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix3x4fvEXT)
#define glProgramUniformMatrix4dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix4dvEXT)
#define glProgramUniformMatrix4fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix4fvEXT)
#define glProgramUniformMatrix4x2dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix4x2dvEXT)
#define glProgramUniformMatrix4x2fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix4x2fvEXT)
#define glProgramUniformMatrix4x3dvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix4x3dvEXT)
#define glProgramUniformMatrix4x3fvEXT GLEW_GET_FUN(G_glewProgramUniformMatrix4x3fvEXT)
#define glPushClientAttribDefaultEXT GLEW_GET_FUN(G_glewPushClientAttribDefaultEXT)
#define glTextureBufferEXT GLEW_GET_FUN(G_glewTextureBufferEXT)
#define glTextureImage1DEXT GLEW_GET_FUN(G_glewTextureImage1DEXT)
#define glTextureImage2DEXT GLEW_GET_FUN(G_glewTextureImage2DEXT)
#define glTextureImage3DEXT GLEW_GET_FUN(G_glewTextureImage3DEXT)
#define glTextureParameterIivEXT GLEW_GET_FUN(G_glewTextureParameterIivEXT)
#define glTextureParameterIuivEXT GLEW_GET_FUN(G_glewTextureParameterIuivEXT)
#define glTextureParameterfEXT GLEW_GET_FUN(G_glewTextureParameterfEXT)
#define glTextureParameterfvEXT GLEW_GET_FUN(G_glewTextureParameterfvEXT)
#define glTextureParameteriEXT GLEW_GET_FUN(G_glewTextureParameteriEXT)
#define glTextureParameterivEXT GLEW_GET_FUN(G_glewTextureParameterivEXT)
#define glTextureRenderbufferEXT GLEW_GET_FUN(G_glewTextureRenderbufferEXT)
#define glTextureSubImage1DEXT GLEW_GET_FUN(G_glewTextureSubImage1DEXT)
#define glTextureSubImage2DEXT GLEW_GET_FUN(G_glewTextureSubImage2DEXT)
#define glTextureSubImage3DEXT GLEW_GET_FUN(G_glewTextureSubImage3DEXT)
#define glUnmapNamedBufferEXT GLEW_GET_FUN(G_glewUnmapNamedBufferEXT)
#define glVertexArrayColorOffsetEXT GLEW_GET_FUN(G_glewVertexArrayColorOffsetEXT)
#define glVertexArrayEdgeFlagOffsetEXT GLEW_GET_FUN(G_glewVertexArrayEdgeFlagOffsetEXT)
#define glVertexArrayFogCoordOffsetEXT GLEW_GET_FUN(G_glewVertexArrayFogCoordOffsetEXT)
#define glVertexArrayIndexOffsetEXT GLEW_GET_FUN(G_glewVertexArrayIndexOffsetEXT)
#define glVertexArrayMultiTexCoordOffsetEXT GLEW_GET_FUN(G_glewVertexArrayMultiTexCoordOffsetEXT)
#define glVertexArrayNormalOffsetEXT GLEW_GET_FUN(G_glewVertexArrayNormalOffsetEXT)
#define glVertexArraySecondaryColorOffsetEXT GLEW_GET_FUN(G_glewVertexArraySecondaryColorOffsetEXT)
#define glVertexArrayTexCoordOffsetEXT GLEW_GET_FUN(G_glewVertexArrayTexCoordOffsetEXT)
#define glVertexArrayVertexAttribIOffsetEXT GLEW_GET_FUN(G_glewVertexArrayVertexAttribIOffsetEXT)
#define glVertexArrayVertexAttribOffsetEXT GLEW_GET_FUN(G_glewVertexArrayVertexAttribOffsetEXT)
#define glVertexArrayVertexOffsetEXT GLEW_GET_FUN(G_glewVertexArrayVertexOffsetEXT)

#define GLEW_EXT_direct_state_access GLEW_GET_VAR(G_GLEW_EXT_direct_state_access)

#endif /* GL_EXT_direct_state_access */

/* -------------------------- GL_EXT_draw_buffers2 ------------------------- */

#ifndef GL_EXT_draw_buffers2
#define GL_EXT_draw_buffers2 1

typedef void (GLAPIENTRY * PFNGLCOLORMASKINDEXEDEXTPROC) (GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void (GLAPIENTRY * PFNGLDISABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLENABLEINDEXEDEXTPROC) (GLenum target, GLuint index);
typedef void (GLAPIENTRY * PFNGLGETBOOLEANINDEXEDVEXTPROC) (GLenum value, GLuint index, GLboolean* data);
typedef void (GLAPIENTRY * PFNGLGETINTEGERINDEXEDVEXTPROC) (GLenum value, GLuint index, GLint* data);
typedef GLboolean (GLAPIENTRY * PFNGLISENABLEDINDEXEDEXTPROC) (GLenum target, GLuint index);

#define glColorMaskIndexedEXT GLEW_GET_FUN(G_glewColorMaskIndexedEXT)
#define glDisableIndexedEXT GLEW_GET_FUN(G_glewDisableIndexedEXT)
#define glEnableIndexedEXT GLEW_GET_FUN(G_glewEnableIndexedEXT)
#define glGetBooleanIndexedvEXT GLEW_GET_FUN(G_glewGetBooleanIndexedvEXT)
#define glGetIntegerIndexedvEXT GLEW_GET_FUN(G_glewGetIntegerIndexedvEXT)
#define glIsEnabledIndexedEXT GLEW_GET_FUN(G_glewIsEnabledIndexedEXT)

#define GLEW_EXT_draw_buffers2 GLEW_GET_VAR(G_GLEW_EXT_draw_buffers2)

#endif /* GL_EXT_draw_buffers2 */

/* ------------------------- GL_EXT_draw_instanced ------------------------- */

#ifndef GL_EXT_draw_instanced
#define GL_EXT_draw_instanced 1

typedef void (GLAPIENTRY * PFNGLDRAWARRAYSINSTANCEDEXTPROC) (GLenum mode, GLint start, GLsizei count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLDRAWELEMENTSINSTANCEDEXTPROC) (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices, GLsizei primcount);

#define glDrawArraysInstancedEXT GLEW_GET_FUN(G_glewDrawArraysInstancedEXT)
#define glDrawElementsInstancedEXT GLEW_GET_FUN(G_glewDrawElementsInstancedEXT)

#define GLEW_EXT_draw_instanced GLEW_GET_VAR(G_GLEW_EXT_draw_instanced)

#endif /* GL_EXT_draw_instanced */

/* ----------------------- GL_EXT_draw_range_elements ---------------------- */

#ifndef GL_EXT_draw_range_elements
#define GL_EXT_draw_range_elements 1

#define GL_MAX_ELEMENTS_VERTICES_EXT 0x80E8
#define GL_MAX_ELEMENTS_INDICES_EXT 0x80E9

typedef void (GLAPIENTRY * PFNGLDRAWRANGEELEMENTSEXTPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

#define glDrawRangeElementsEXT GLEW_GET_FUN(G_glewDrawRangeElementsEXT)

#define GLEW_EXT_draw_range_elements GLEW_GET_VAR(G_GLEW_EXT_draw_range_elements)

#endif /* GL_EXT_draw_range_elements */

/* ---------------------------- GL_EXT_fog_coord --------------------------- */

#ifndef GL_EXT_fog_coord
#define GL_EXT_fog_coord 1

#define GL_FOG_COORDINATE_SOURCE_EXT 0x8450
#define GL_FOG_COORDINATE_EXT 0x8451
#define GL_FRAGMENT_DEPTH_EXT 0x8452
#define GL_CURRENT_FOG_COORDINATE_EXT 0x8453
#define GL_FOG_COORDINATE_ARRAY_TYPE_EXT 0x8454
#define GL_FOG_COORDINATE_ARRAY_STRIDE_EXT 0x8455
#define GL_FOG_COORDINATE_ARRAY_POINTER_EXT 0x8456
#define GL_FOG_COORDINATE_ARRAY_EXT 0x8457

typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTEREXTPROC) (GLenum type, GLsizei stride, const GLvoid *pointer);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDEXTPROC) (GLdouble coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDDVEXTPROC) (const GLdouble *coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFVEXTPROC) (const GLfloat *coord);

#define glFogCoordPointerEXT GLEW_GET_FUN(G_glewFogCoordPointerEXT)
#define glFogCoorddEXT GLEW_GET_FUN(G_glewFogCoorddEXT)
#define glFogCoorddvEXT GLEW_GET_FUN(G_glewFogCoorddvEXT)
#define glFogCoordfEXT GLEW_GET_FUN(G_glewFogCoordfEXT)
#define glFogCoordfvEXT GLEW_GET_FUN(G_glewFogCoordfvEXT)

#define GLEW_EXT_fog_coord GLEW_GET_VAR(G_GLEW_EXT_fog_coord)

#endif /* GL_EXT_fog_coord */

/* ------------------------ GL_EXT_fragment_lighting ----------------------- */

#ifndef GL_EXT_fragment_lighting
#define GL_EXT_fragment_lighting 1

#define GL_FRAGMENT_LIGHTING_EXT 0x8400
#define GL_FRAGMENT_COLOR_MATERIAL_EXT 0x8401
#define GL_FRAGMENT_COLOR_MATERIAL_FACE_EXT 0x8402
#define GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_EXT 0x8403
#define GL_MAX_FRAGMENT_LIGHTS_EXT 0x8404
#define GL_MAX_ACTIVE_LIGHTS_EXT 0x8405
#define GL_CURRENT_RASTER_NORMAL_EXT 0x8406
#define GL_LIGHT_ENV_MODE_EXT 0x8407
#define GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_EXT 0x8408
#define GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_EXT 0x8409
#define GL_FRAGMENT_LIGHT_MODEL_AMBIENT_EXT 0x840A
#define GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_EXT 0x840B
#define GL_FRAGMENT_LIGHT0_EXT 0x840C
#define GL_FRAGMENT_LIGHT7_EXT 0x8413

typedef void (GLAPIENTRY * PFNGLFRAGMENTCOLORMATERIALEXTPROC) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFEXTPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFVEXTPROC) (GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELIEXTPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELIVEXTPROC) (GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFEXTPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFVEXTPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTIEXTPROC) (GLenum light, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTIVEXTPROC) (GLenum light, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFEXTPROC) (GLenum face, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFVEXTPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALIEXTPROC) (GLenum face, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALIVEXTPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTFVEXTPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTIVEXTPROC) (GLenum light, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALFVEXTPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALIVEXTPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLLIGHTENVIEXTPROC) (GLenum pname, GLint param);

#define glFragmentColorMaterialEXT GLEW_GET_FUN(G_glewFragmentColorMaterialEXT)
#define glFragmentLightModelfEXT GLEW_GET_FUN(G_glewFragmentLightModelfEXT)
#define glFragmentLightModelfvEXT GLEW_GET_FUN(G_glewFragmentLightModelfvEXT)
#define glFragmentLightModeliEXT GLEW_GET_FUN(G_glewFragmentLightModeliEXT)
#define glFragmentLightModelivEXT GLEW_GET_FUN(G_glewFragmentLightModelivEXT)
#define glFragmentLightfEXT GLEW_GET_FUN(G_glewFragmentLightfEXT)
#define glFragmentLightfvEXT GLEW_GET_FUN(G_glewFragmentLightfvEXT)
#define glFragmentLightiEXT GLEW_GET_FUN(G_glewFragmentLightiEXT)
#define glFragmentLightivEXT GLEW_GET_FUN(G_glewFragmentLightivEXT)
#define glFragmentMaterialfEXT GLEW_GET_FUN(G_glewFragmentMaterialfEXT)
#define glFragmentMaterialfvEXT GLEW_GET_FUN(G_glewFragmentMaterialfvEXT)
#define glFragmentMaterialiEXT GLEW_GET_FUN(G_glewFragmentMaterialiEXT)
#define glFragmentMaterialivEXT GLEW_GET_FUN(G_glewFragmentMaterialivEXT)
#define glGetFragmentLightfvEXT GLEW_GET_FUN(G_glewGetFragmentLightfvEXT)
#define glGetFragmentLightivEXT GLEW_GET_FUN(G_glewGetFragmentLightivEXT)
#define glGetFragmentMaterialfvEXT GLEW_GET_FUN(G_glewGetFragmentMaterialfvEXT)
#define glGetFragmentMaterialivEXT GLEW_GET_FUN(G_glewGetFragmentMaterialivEXT)
#define glLightEnviEXT GLEW_GET_FUN(G_glewLightEnviEXT)

#define GLEW_EXT_fragment_lighting GLEW_GET_VAR(G_GLEW_EXT_fragment_lighting)

#endif /* GL_EXT_fragment_lighting */

/* ------------------------ GL_EXT_framebuffer_blit ------------------------ */

#ifndef GL_EXT_framebuffer_blit
#define GL_EXT_framebuffer_blit 1

#define GL_DRAW_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_READ_FRAMEBUFFER_EXT 0x8CA8
#define GL_DRAW_FRAMEBUFFER_EXT 0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING_EXT 0x8CAA

typedef void (GLAPIENTRY * PFNGLBLITFRAMEBUFFEREXTPROC) (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);

#define glBlitFramebufferEXT GLEW_GET_FUN(G_glewBlitFramebufferEXT)

#define GLEW_EXT_framebuffer_blit GLEW_GET_VAR(G_GLEW_EXT_framebuffer_blit)

#endif /* GL_EXT_framebuffer_blit */

/* --------------------- GL_EXT_framebuffer_multisample -------------------- */

#ifndef GL_EXT_framebuffer_multisample
#define GL_EXT_framebuffer_multisample 1

#define GL_RENDERBUFFER_SAMPLES_EXT 0x8CAB
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE_EXT 0x8D56
#define GL_MAX_SAMPLES_EXT 0x8D57

typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);

#define glRenderbufferStorageMultisampleEXT GLEW_GET_FUN(G_glewRenderbufferStorageMultisampleEXT)

#define GLEW_EXT_framebuffer_multisample GLEW_GET_VAR(G_GLEW_EXT_framebuffer_multisample)

#endif /* GL_EXT_framebuffer_multisample */

/* --------------- GL_EXT_framebuffer_multisample_blit_scaled -------------- */

#ifndef GL_EXT_framebuffer_multisample_blit_scaled
#define GL_EXT_framebuffer_multisample_blit_scaled 1

#define GL_SCALED_RESOLVE_FASTEST_EXT 0x90BA
#define GL_SCALED_RESOLVE_NICEST_EXT 0x90BB

#define GLEW_EXT_framebuffer_multisample_blit_scaled GLEW_GET_VAR(G_GLEW_EXT_framebuffer_multisample_blit_scaled)

#endif /* GL_EXT_framebuffer_multisample_blit_scaled */

/* ----------------------- GL_EXT_framebuffer_object ----------------------- */

#ifndef GL_EXT_framebuffer_object
#define GL_EXT_framebuffer_object 1

#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT 0x0506
#define GL_MAX_RENDERBUFFER_SIZE_EXT 0x84E8
#define GL_FRAMEBUFFER_BINDING_EXT 0x8CA6
#define GL_RENDERBUFFER_BINDING_EXT 0x8CA7
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE_EXT 0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT 0x8CD9
#define GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT 0x8CDA
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED_EXT 0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS_EXT 0x8CDF
#define GL_COLOR_ATTACHMENT0_EXT 0x8CE0
#define GL_COLOR_ATTACHMENT1_EXT 0x8CE1
#define GL_COLOR_ATTACHMENT2_EXT 0x8CE2
#define GL_COLOR_ATTACHMENT3_EXT 0x8CE3
#define GL_COLOR_ATTACHMENT4_EXT 0x8CE4
#define GL_COLOR_ATTACHMENT5_EXT 0x8CE5
#define GL_COLOR_ATTACHMENT6_EXT 0x8CE6
#define GL_COLOR_ATTACHMENT7_EXT 0x8CE7
#define GL_COLOR_ATTACHMENT8_EXT 0x8CE8
#define GL_COLOR_ATTACHMENT9_EXT 0x8CE9
#define GL_COLOR_ATTACHMENT10_EXT 0x8CEA
#define GL_COLOR_ATTACHMENT11_EXT 0x8CEB
#define GL_COLOR_ATTACHMENT12_EXT 0x8CEC
#define GL_COLOR_ATTACHMENT13_EXT 0x8CED
#define GL_COLOR_ATTACHMENT14_EXT 0x8CEE
#define GL_COLOR_ATTACHMENT15_EXT 0x8CEF
#define GL_DEPTH_ATTACHMENT_EXT 0x8D00
#define GL_STENCIL_ATTACHMENT_EXT 0x8D20
#define GL_FRAMEBUFFER_EXT 0x8D40
#define GL_RENDERBUFFER_EXT 0x8D41
#define GL_RENDERBUFFER_WIDTH_EXT 0x8D42
#define GL_RENDERBUFFER_HEIGHT_EXT 0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT_EXT 0x8D44
#define GL_STENCIL_INDEX1_EXT 0x8D46
#define GL_STENCIL_INDEX4_EXT 0x8D47
#define GL_STENCIL_INDEX8_EXT 0x8D48
#define GL_STENCIL_INDEX16_EXT 0x8D49
#define GL_RENDERBUFFER_RED_SIZE_EXT 0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE_EXT 0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE_EXT 0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE_EXT 0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE_EXT 0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE_EXT 0x8D55

typedef void (GLAPIENTRY * PFNGLBINDFRAMEBUFFEREXTPROC) (GLenum target, GLuint framebuffer);
typedef void (GLAPIENTRY * PFNGLBINDRENDERBUFFEREXTPROC) (GLenum target, GLuint renderbuffer);
typedef GLenum (GLAPIENTRY * PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLDELETEFRAMEBUFFERSEXTPROC) (GLsizei n, const GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLDELETERENDERBUFFERSEXTPROC) (GLsizei n, const GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE1DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE2DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURE3DEXTPROC) (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void (GLAPIENTRY * PFNGLGENFRAMEBUFFERSEXTPROC) (GLsizei n, GLuint* framebuffers);
typedef void (GLAPIENTRY * PFNGLGENRENDERBUFFERSEXTPROC) (GLsizei n, GLuint* renderbuffers);
typedef void (GLAPIENTRY * PFNGLGENERATEMIPMAPEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) (GLenum target, GLenum attachment, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISFRAMEBUFFEREXTPROC) (GLuint framebuffer);
typedef GLboolean (GLAPIENTRY * PFNGLISRENDERBUFFEREXTPROC) (GLuint renderbuffer);
typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEEXTPROC) (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);

#define glBindFramebufferEXT GLEW_GET_FUN(G_glewBindFramebufferEXT)
#define glBindRenderbufferEXT GLEW_GET_FUN(G_glewBindRenderbufferEXT)
#define glCheckFramebufferStatusEXT GLEW_GET_FUN(G_glewCheckFramebufferStatusEXT)
#define glDeleteFramebuffersEXT GLEW_GET_FUN(G_glewDeleteFramebuffersEXT)
#define glDeleteRenderbuffersEXT GLEW_GET_FUN(G_glewDeleteRenderbuffersEXT)
#define glFramebufferRenderbufferEXT GLEW_GET_FUN(G_glewFramebufferRenderbufferEXT)
#define glFramebufferTexture1DEXT GLEW_GET_FUN(G_glewFramebufferTexture1DEXT)
#define glFramebufferTexture2DEXT GLEW_GET_FUN(G_glewFramebufferTexture2DEXT)
#define glFramebufferTexture3DEXT GLEW_GET_FUN(G_glewFramebufferTexture3DEXT)
#define glGenFramebuffersEXT GLEW_GET_FUN(G_glewGenFramebuffersEXT)
#define glGenRenderbuffersEXT GLEW_GET_FUN(G_glewGenRenderbuffersEXT)
#define glGenerateMipmapEXT GLEW_GET_FUN(G_glewGenerateMipmapEXT)
#define glGetFramebufferAttachmentParameterivEXT GLEW_GET_FUN(G_glewGetFramebufferAttachmentParameterivEXT)
#define glGetRenderbufferParameterivEXT GLEW_GET_FUN(G_glewGetRenderbufferParameterivEXT)
#define glIsFramebufferEXT GLEW_GET_FUN(G_glewIsFramebufferEXT)
#define glIsRenderbufferEXT GLEW_GET_FUN(G_glewIsRenderbufferEXT)
#define glRenderbufferStorageEXT GLEW_GET_FUN(G_glewRenderbufferStorageEXT)

#define GLEW_EXT_framebuffer_object GLEW_GET_VAR(G_GLEW_EXT_framebuffer_object)

#endif /* GL_EXT_framebuffer_object */

/* ------------------------ GL_EXT_framebuffer_sRGB ------------------------ */

#ifndef GL_EXT_framebuffer_sRGB
#define GL_EXT_framebuffer_sRGB 1

#define GL_FRAMEBUFFER_SRGB_EXT 0x8DB9
#define GL_FRAMEBUFFER_SRGB_CAPABLE_EXT 0x8DBA

#define GLEW_EXT_framebuffer_sRGB GLEW_GET_VAR(G_GLEW_EXT_framebuffer_sRGB)

#endif /* GL_EXT_framebuffer_sRGB */

/* ------------------------ GL_EXT_geometry_shader4 ------------------------ */

#ifndef GL_EXT_geometry_shader4
#define GL_EXT_geometry_shader4 1

#define GL_LINES_ADJACENCY_EXT 0xA
#define GL_LINE_STRIP_ADJACENCY_EXT 0xB
#define GL_TRIANGLES_ADJACENCY_EXT 0xC
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT 0xD
#define GL_PROGRAM_POINT_SIZE_EXT 0x8642
#define GL_MAX_VARYING_COMPONENTS_EXT 0x8B4B
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS_EXT 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT 0x8CD4
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS_EXT 0x8DA8
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_COUNT_EXT 0x8DA9
#define GL_GEOMETRY_SHADER_EXT 0x8DD9
#define GL_GEOMETRY_VERTICES_OUT_EXT 0x8DDA
#define GL_GEOMETRY_INPUT_TYPE_EXT 0x8DDB
#define GL_GEOMETRY_OUTPUT_TYPE_EXT 0x8DDC
#define GL_MAX_GEOMETRY_VARYING_COMPONENTS_EXT 0x8DDD
#define GL_MAX_VERTEX_VARYING_COMPONENTS_EXT 0x8DDE
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS_EXT 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT 0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS_EXT 0x8DE1

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERIEXTPROC) (GLuint program, GLenum pname, GLint value);

#define glFramebufferTextureEXT GLEW_GET_FUN(G_glewFramebufferTextureEXT)
#define glFramebufferTextureFaceEXT GLEW_GET_FUN(G_glewFramebufferTextureFaceEXT)
#define glProgramParameteriEXT GLEW_GET_FUN(G_glewProgramParameteriEXT)

#define GLEW_EXT_geometry_shader4 GLEW_GET_VAR(G_GLEW_EXT_geometry_shader4)

#endif /* GL_EXT_geometry_shader4 */

/* --------------------- GL_EXT_gpu_program_parameters --------------------- */

#ifndef GL_EXT_gpu_program_parameters
#define GL_EXT_gpu_program_parameters 1

typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERS4FVEXTPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC) (GLenum target, GLuint index, GLsizei count, const GLfloat* params);

#define glProgramEnvParameters4fvEXT GLEW_GET_FUN(G_glewProgramEnvParameters4fvEXT)
#define glProgramLocalParameters4fvEXT GLEW_GET_FUN(G_glewProgramLocalParameters4fvEXT)

#define GLEW_EXT_gpu_program_parameters GLEW_GET_VAR(G_GLEW_EXT_gpu_program_parameters)

#endif /* GL_EXT_gpu_program_parameters */

/* --------------------------- GL_EXT_gpu_shader4 -------------------------- */

#ifndef GL_EXT_gpu_shader4
#define GL_EXT_gpu_shader4 1

#define GL_VERTEX_ATTRIB_ARRAY_INTEGER_EXT 0x88FD
#define GL_SAMPLER_1D_ARRAY_EXT 0x8DC0
#define GL_SAMPLER_2D_ARRAY_EXT 0x8DC1
#define GL_SAMPLER_BUFFER_EXT 0x8DC2
#define GL_SAMPLER_1D_ARRAY_SHADOW_EXT 0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW_EXT 0x8DC4
#define GL_SAMPLER_CUBE_SHADOW_EXT 0x8DC5
#define GL_UNSIGNED_INT_VEC2_EXT 0x8DC6
#define GL_UNSIGNED_INT_VEC3_EXT 0x8DC7
#define GL_UNSIGNED_INT_VEC4_EXT 0x8DC8
#define GL_INT_SAMPLER_1D_EXT 0x8DC9
#define GL_INT_SAMPLER_2D_EXT 0x8DCA
#define GL_INT_SAMPLER_3D_EXT 0x8DCB
#define GL_INT_SAMPLER_CUBE_EXT 0x8DCC
#define GL_INT_SAMPLER_2D_RECT_EXT 0x8DCD
#define GL_INT_SAMPLER_1D_ARRAY_EXT 0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY_EXT 0x8DCF
#define GL_INT_SAMPLER_BUFFER_EXT 0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_1D_EXT 0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D_EXT 0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D_EXT 0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE_EXT 0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT_EXT 0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY_EXT 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY_EXT 0x8DD7
#define GL_UNSIGNED_INT_SAMPLER_BUFFER_EXT 0x8DD8

typedef void (GLAPIENTRY * PFNGLBINDFRAGDATALOCATIONEXTPROC) (GLuint program, GLuint color, const GLchar *name);
typedef GLint (GLAPIENTRY * PFNGLGETFRAGDATALOCATIONEXTPROC) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUIVEXTPROC) (GLuint program, GLint location, GLuint *params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIIVEXTPROC) (GLuint index, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIUIVEXTPROC) (GLuint index, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIEXTPROC) (GLint location, GLuint v0);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIEXTPROC) (GLint location, GLuint v0, GLuint v1);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIEXTPROC) (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UIVEXTPROC) (GLint location, GLsizei count, const GLuint *value);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IEXTPROC) (GLuint index, GLint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIEXTPROC) (GLuint index, GLuint x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI1UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IEXTPROC) (GLuint index, GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIEXTPROC) (GLuint index, GLuint x, GLuint y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI2UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IEXTPROC) (GLuint index, GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI3UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4BVEXTPROC) (GLuint index, const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IEXTPROC) (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4IVEXTPROC) (GLuint index, const GLint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4SVEXTPROC) (GLuint index, const GLshort *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UBVEXTPROC) (GLuint index, const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIEXTPROC) (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4UIVEXTPROC) (GLuint index, const GLuint *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBI4USVEXTPROC) (GLuint index, const GLushort *v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

#define glBindFragDataLocationEXT GLEW_GET_FUN(G_glewBindFragDataLocationEXT)
#define glGetFragDataLocationEXT GLEW_GET_FUN(G_glewGetFragDataLocationEXT)
#define glGetUniformuivEXT GLEW_GET_FUN(G_glewGetUniformuivEXT)
#define glGetVertexAttribIivEXT GLEW_GET_FUN(G_glewGetVertexAttribIivEXT)
#define glGetVertexAttribIuivEXT GLEW_GET_FUN(G_glewGetVertexAttribIuivEXT)
#define glUniform1uiEXT GLEW_GET_FUN(G_glewUniform1uiEXT)
#define glUniform1uivEXT GLEW_GET_FUN(G_glewUniform1uivEXT)
#define glUniform2uiEXT GLEW_GET_FUN(G_glewUniform2uiEXT)
#define glUniform2uivEXT GLEW_GET_FUN(G_glewUniform2uivEXT)
#define glUniform3uiEXT GLEW_GET_FUN(G_glewUniform3uiEXT)
#define glUniform3uivEXT GLEW_GET_FUN(G_glewUniform3uivEXT)
#define glUniform4uiEXT GLEW_GET_FUN(G_glewUniform4uiEXT)
#define glUniform4uivEXT GLEW_GET_FUN(G_glewUniform4uivEXT)
#define glVertexAttribI1iEXT GLEW_GET_FUN(G_glewVertexAttribI1iEXT)
#define glVertexAttribI1ivEXT GLEW_GET_FUN(G_glewVertexAttribI1ivEXT)
#define glVertexAttribI1uiEXT GLEW_GET_FUN(G_glewVertexAttribI1uiEXT)
#define glVertexAttribI1uivEXT GLEW_GET_FUN(G_glewVertexAttribI1uivEXT)
#define glVertexAttribI2iEXT GLEW_GET_FUN(G_glewVertexAttribI2iEXT)
#define glVertexAttribI2ivEXT GLEW_GET_FUN(G_glewVertexAttribI2ivEXT)
#define glVertexAttribI2uiEXT GLEW_GET_FUN(G_glewVertexAttribI2uiEXT)
#define glVertexAttribI2uivEXT GLEW_GET_FUN(G_glewVertexAttribI2uivEXT)
#define glVertexAttribI3iEXT GLEW_GET_FUN(G_glewVertexAttribI3iEXT)
#define glVertexAttribI3ivEXT GLEW_GET_FUN(G_glewVertexAttribI3ivEXT)
#define glVertexAttribI3uiEXT GLEW_GET_FUN(G_glewVertexAttribI3uiEXT)
#define glVertexAttribI3uivEXT GLEW_GET_FUN(G_glewVertexAttribI3uivEXT)
#define glVertexAttribI4bvEXT GLEW_GET_FUN(G_glewVertexAttribI4bvEXT)
#define glVertexAttribI4iEXT GLEW_GET_FUN(G_glewVertexAttribI4iEXT)
#define glVertexAttribI4ivEXT GLEW_GET_FUN(G_glewVertexAttribI4ivEXT)
#define glVertexAttribI4svEXT GLEW_GET_FUN(G_glewVertexAttribI4svEXT)
#define glVertexAttribI4ubvEXT GLEW_GET_FUN(G_glewVertexAttribI4ubvEXT)
#define glVertexAttribI4uiEXT GLEW_GET_FUN(G_glewVertexAttribI4uiEXT)
#define glVertexAttribI4uivEXT GLEW_GET_FUN(G_glewVertexAttribI4uivEXT)
#define glVertexAttribI4usvEXT GLEW_GET_FUN(G_glewVertexAttribI4usvEXT)
#define glVertexAttribIPointerEXT GLEW_GET_FUN(G_glewVertexAttribIPointerEXT)

#define GLEW_EXT_gpu_shader4 GLEW_GET_VAR(G_GLEW_EXT_gpu_shader4)

#endif /* GL_EXT_gpu_shader4 */

/* ---------------------------- GL_EXT_histogram --------------------------- */

#ifndef GL_EXT_histogram
#define GL_EXT_histogram 1

#define GL_HISTOGRAM_EXT 0x8024
#define GL_PROXY_HISTOGRAM_EXT 0x8025
#define GL_HISTOGRAM_WIDTH_EXT 0x8026
#define GL_HISTOGRAM_FORMAT_EXT 0x8027
#define GL_HISTOGRAM_RED_SIZE_EXT 0x8028
#define GL_HISTOGRAM_GREEN_SIZE_EXT 0x8029
#define GL_HISTOGRAM_BLUE_SIZE_EXT 0x802A
#define GL_HISTOGRAM_ALPHA_SIZE_EXT 0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE_EXT 0x802C
#define GL_HISTOGRAM_SINK_EXT 0x802D
#define GL_MINMAX_EXT 0x802E
#define GL_MINMAX_FORMAT_EXT 0x802F
#define GL_MINMAX_SINK_EXT 0x8030

typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETHISTOGRAMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXEXTPROC) (GLenum target, GLboolean reset, GLenum format, GLenum type, void* values);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMINMAXPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLHISTOGRAMEXTPROC) (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLMINMAXEXTPROC) (GLenum target, GLenum internalformat, GLboolean sink);
typedef void (GLAPIENTRY * PFNGLRESETHISTOGRAMEXTPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLRESETMINMAXEXTPROC) (GLenum target);

#define glGetHistogramEXT GLEW_GET_FUN(G_glewGetHistogramEXT)
#define glGetHistogramParameterfvEXT GLEW_GET_FUN(G_glewGetHistogramParameterfvEXT)
#define glGetHistogramParameterivEXT GLEW_GET_FUN(G_glewGetHistogramParameterivEXT)
#define glGetMinmaxEXT GLEW_GET_FUN(G_glewGetMinmaxEXT)
#define glGetMinmaxParameterfvEXT GLEW_GET_FUN(G_glewGetMinmaxParameterfvEXT)
#define glGetMinmaxParameterivEXT GLEW_GET_FUN(G_glewGetMinmaxParameterivEXT)
#define glHistogramEXT GLEW_GET_FUN(G_glewHistogramEXT)
#define glMinmaxEXT GLEW_GET_FUN(G_glewMinmaxEXT)
#define glResetHistogramEXT GLEW_GET_FUN(G_glewResetHistogramEXT)
#define glResetMinmaxEXT GLEW_GET_FUN(G_glewResetMinmaxEXT)

#define GLEW_EXT_histogram GLEW_GET_VAR(G_GLEW_EXT_histogram)

#endif /* GL_EXT_histogram */

/* ----------------------- GL_EXT_index_array_formats ---------------------- */

#ifndef GL_EXT_index_array_formats
#define GL_EXT_index_array_formats 1

#define GLEW_EXT_index_array_formats GLEW_GET_VAR(G_GLEW_EXT_index_array_formats)

#endif /* GL_EXT_index_array_formats */

/* --------------------------- GL_EXT_index_func --------------------------- */

#ifndef GL_EXT_index_func
#define GL_EXT_index_func 1

typedef void (GLAPIENTRY * PFNGLINDEXFUNCEXTPROC) (GLenum func, GLfloat ref);

#define glIndexFuncEXT GLEW_GET_FUN(G_glewIndexFuncEXT)

#define GLEW_EXT_index_func GLEW_GET_VAR(G_GLEW_EXT_index_func)

#endif /* GL_EXT_index_func */

/* ------------------------- GL_EXT_index_material ------------------------- */

#ifndef GL_EXT_index_material
#define GL_EXT_index_material 1

typedef void (GLAPIENTRY * PFNGLINDEXMATERIALEXTPROC) (GLenum face, GLenum mode);

#define glIndexMaterialEXT GLEW_GET_FUN(G_glewIndexMaterialEXT)

#define GLEW_EXT_index_material GLEW_GET_VAR(G_GLEW_EXT_index_material)

#endif /* GL_EXT_index_material */

/* -------------------------- GL_EXT_index_texture ------------------------- */

#ifndef GL_EXT_index_texture
#define GL_EXT_index_texture 1

#define GLEW_EXT_index_texture GLEW_GET_VAR(G_GLEW_EXT_index_texture)

#endif /* GL_EXT_index_texture */

/* -------------------------- GL_EXT_light_texture ------------------------- */

#ifndef GL_EXT_light_texture
#define GL_EXT_light_texture 1

#define GL_FRAGMENT_MATERIAL_EXT 0x8349
#define GL_FRAGMENT_NORMAL_EXT 0x834A
#define GL_FRAGMENT_COLOR_EXT 0x834C
#define GL_ATTENUATION_EXT 0x834D
#define GL_SHADOW_ATTENUATION_EXT 0x834E
#define GL_TEXTURE_APPLICATION_MODE_EXT 0x834F
#define GL_TEXTURE_LIGHT_EXT 0x8350
#define GL_TEXTURE_MATERIAL_FACE_EXT 0x8351
#define GL_TEXTURE_MATERIAL_PARAMETER_EXT 0x8352

typedef void (GLAPIENTRY * PFNGLAPPLYTEXTUREEXTPROC) (GLenum mode);
typedef void (GLAPIENTRY * PFNGLTEXTURELIGHTEXTPROC) (GLenum pname);
typedef void (GLAPIENTRY * PFNGLTEXTUREMATERIALEXTPROC) (GLenum face, GLenum mode);

#define glApplyTextureEXT GLEW_GET_FUN(G_glewApplyTextureEXT)
#define glTextureLightEXT GLEW_GET_FUN(G_glewTextureLightEXT)
#define glTextureMaterialEXT GLEW_GET_FUN(G_glewTextureMaterialEXT)

#define GLEW_EXT_light_texture GLEW_GET_VAR(G_GLEW_EXT_light_texture)

#endif /* GL_EXT_light_texture */

/* ------------------------- GL_EXT_misc_attribute ------------------------- */

#ifndef GL_EXT_misc_attribute
#define GL_EXT_misc_attribute 1

#define GLEW_EXT_misc_attribute GLEW_GET_VAR(G_GLEW_EXT_misc_attribute)

#endif /* GL_EXT_misc_attribute */

/* ------------------------ GL_EXT_multi_draw_arrays ----------------------- */

#ifndef GL_EXT_multi_draw_arrays
#define GL_EXT_multi_draw_arrays 1

typedef void (GLAPIENTRY * PFNGLMULTIDRAWARRAYSEXTPROC) (GLenum mode, const GLint* first, const GLsizei *count, GLsizei primcount);
typedef void (GLAPIENTRY * PFNGLMULTIDRAWELEMENTSEXTPROC) (GLenum mode, GLsizei* count, GLenum type, const GLvoid **indices, GLsizei primcount);

#define glMultiDrawArraysEXT GLEW_GET_FUN(G_glewMultiDrawArraysEXT)
#define glMultiDrawElementsEXT GLEW_GET_FUN(G_glewMultiDrawElementsEXT)

#define GLEW_EXT_multi_draw_arrays GLEW_GET_VAR(G_GLEW_EXT_multi_draw_arrays)

#endif /* GL_EXT_multi_draw_arrays */

/* --------------------------- GL_EXT_multisample -------------------------- */

#ifndef GL_EXT_multisample
#define GL_EXT_multisample 1

#define GL_MULTISAMPLE_EXT 0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_EXT 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_EXT 0x809F
#define GL_SAMPLE_MASK_EXT 0x80A0
#define GL_1PASS_EXT 0x80A1
#define GL_2PASS_0_EXT 0x80A2
#define GL_2PASS_1_EXT 0x80A3
#define GL_4PASS_0_EXT 0x80A4
#define GL_4PASS_1_EXT 0x80A5
#define GL_4PASS_2_EXT 0x80A6
#define GL_4PASS_3_EXT 0x80A7
#define GL_SAMPLE_BUFFERS_EXT 0x80A8
#define GL_SAMPLES_EXT 0x80A9
#define GL_SAMPLE_MASK_VALUE_EXT 0x80AA
#define GL_SAMPLE_MASK_INVERT_EXT 0x80AB
#define GL_SAMPLE_PATTERN_EXT 0x80AC
#define GL_MULTISAMPLE_BIT_EXT 0x20000000

typedef void (GLAPIENTRY * PFNGLSAMPLEMASKEXTPROC) (GLclampf value, GLboolean invert);
typedef void (GLAPIENTRY * PFNGLSAMPLEPATTERNEXTPROC) (GLenum pattern);

#define glSampleMaskEXT GLEW_GET_FUN(G_glewSampleMaskEXT)
#define glSamplePatternEXT GLEW_GET_FUN(G_glewSamplePatternEXT)

#define GLEW_EXT_multisample GLEW_GET_VAR(G_GLEW_EXT_multisample)

#endif /* GL_EXT_multisample */

/* ---------------------- GL_EXT_packed_depth_stencil ---------------------- */

#ifndef GL_EXT_packed_depth_stencil
#define GL_EXT_packed_depth_stencil 1

#define GL_DEPTH_STENCIL_EXT 0x84F9
#define GL_UNSIGNED_INT_24_8_EXT 0x84FA
#define GL_DEPTH24_STENCIL8_EXT 0x88F0
#define GL_TEXTURE_STENCIL_SIZE_EXT 0x88F1

#define GLEW_EXT_packed_depth_stencil GLEW_GET_VAR(G_GLEW_EXT_packed_depth_stencil)

#endif /* GL_EXT_packed_depth_stencil */

/* -------------------------- GL_EXT_packed_float -------------------------- */

#ifndef GL_EXT_packed_float
#define GL_EXT_packed_float 1

#define GL_R11F_G11F_B10F_EXT 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV_EXT 0x8C3B
#define GL_RGBA_SIGNED_COMPONENTS_EXT 0x8C3C

#define GLEW_EXT_packed_float GLEW_GET_VAR(G_GLEW_EXT_packed_float)

#endif /* GL_EXT_packed_float */

/* -------------------------- GL_EXT_packed_pixels ------------------------- */

#ifndef GL_EXT_packed_pixels
#define GL_EXT_packed_pixels 1

#define GL_UNSIGNED_BYTE_3_3_2_EXT 0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4_EXT 0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1_EXT 0x8034
#define GL_UNSIGNED_INT_8_8_8_8_EXT 0x8035
#define GL_UNSIGNED_INT_10_10_10_2_EXT 0x8036

#define GLEW_EXT_packed_pixels GLEW_GET_VAR(G_GLEW_EXT_packed_pixels)

#endif /* GL_EXT_packed_pixels */

/* ------------------------ GL_EXT_paletted_texture ------------------------ */

#ifndef GL_EXT_paletted_texture
#define GL_EXT_paletted_texture 1

#define GL_TEXTURE_1D 0x0DE0
#define GL_TEXTURE_2D 0x0DE1
#define GL_PROXY_TEXTURE_1D 0x8063
#define GL_PROXY_TEXTURE_2D 0x8064
#define GL_COLOR_TABLE_FORMAT_EXT 0x80D8
#define GL_COLOR_TABLE_WIDTH_EXT 0x80D9
#define GL_COLOR_TABLE_RED_SIZE_EXT 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_EXT 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_EXT 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_EXT 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_EXT 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_EXT 0x80DF
#define GL_COLOR_INDEX1_EXT 0x80E2
#define GL_COLOR_INDEX2_EXT 0x80E3
#define GL_COLOR_INDEX4_EXT 0x80E4
#define GL_COLOR_INDEX8_EXT 0x80E5
#define GL_COLOR_INDEX12_EXT 0x80E6
#define GL_COLOR_INDEX16_EXT 0x80E7
#define GL_TEXTURE_INDEX_SIZE_EXT 0x80ED
#define GL_TEXTURE_CUBE_MAP_ARB 0x8513
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB 0x851B

typedef void (GLAPIENTRY * PFNGLCOLORTABLEEXTPROC) (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void* data);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEEXTPROC) (GLenum target, GLenum format, GLenum type, void* data);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVEXTPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVEXTPROC) (GLenum target, GLenum pname, GLint* params);

#define glColorTableEXT GLEW_GET_FUN(G_glewColorTableEXT)
#define glGetColorTableEXT GLEW_GET_FUN(G_glewGetColorTableEXT)
#define glGetColorTableParameterfvEXT GLEW_GET_FUN(G_glewGetColorTableParameterfvEXT)
#define glGetColorTableParameterivEXT GLEW_GET_FUN(G_glewGetColorTableParameterivEXT)

#define GLEW_EXT_paletted_texture GLEW_GET_VAR(G_GLEW_EXT_paletted_texture)

#endif /* GL_EXT_paletted_texture */

/* ----------------------- GL_EXT_pixel_buffer_object ---------------------- */

#ifndef GL_EXT_pixel_buffer_object
#define GL_EXT_pixel_buffer_object 1

#define GL_PIXEL_PACK_BUFFER_EXT 0x88EB
#define GL_PIXEL_UNPACK_BUFFER_EXT 0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING_EXT 0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING_EXT 0x88EF

#define GLEW_EXT_pixel_buffer_object GLEW_GET_VAR(G_GLEW_EXT_pixel_buffer_object)

#endif /* GL_EXT_pixel_buffer_object */

/* ------------------------- GL_EXT_pixel_transform ------------------------ */

#ifndef GL_EXT_pixel_transform
#define GL_EXT_pixel_transform 1

#define GL_PIXEL_TRANSFORM_2D_EXT 0x8330
#define GL_PIXEL_MAG_FILTER_EXT 0x8331
#define GL_PIXEL_MIN_FILTER_EXT 0x8332
#define GL_PIXEL_CUBIC_WEIGHT_EXT 0x8333
#define GL_CUBIC_EXT 0x8334
#define GL_AVERAGE_EXT 0x8335
#define GL_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8336
#define GL_MAX_PIXEL_TRANSFORM_2D_STACK_DEPTH_EXT 0x8337
#define GL_PIXEL_TRANSFORM_2D_MATRIX_EXT 0x8338

typedef void (GLAPIENTRY * PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERFEXTPROC) (GLenum target, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERIEXTPROC) (GLenum target, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC) (GLenum target, GLenum pname, const GLint* params);

#define glGetPixelTransformParameterfvEXT GLEW_GET_FUN(G_glewGetPixelTransformParameterfvEXT)
#define glGetPixelTransformParameterivEXT GLEW_GET_FUN(G_glewGetPixelTransformParameterivEXT)
#define glPixelTransformParameterfEXT GLEW_GET_FUN(G_glewPixelTransformParameterfEXT)
#define glPixelTransformParameterfvEXT GLEW_GET_FUN(G_glewPixelTransformParameterfvEXT)
#define glPixelTransformParameteriEXT GLEW_GET_FUN(G_glewPixelTransformParameteriEXT)
#define glPixelTransformParameterivEXT GLEW_GET_FUN(G_glewPixelTransformParameterivEXT)

#define GLEW_EXT_pixel_transform GLEW_GET_VAR(G_GLEW_EXT_pixel_transform)

#endif /* GL_EXT_pixel_transform */

/* ------------------- GL_EXT_pixel_transform_color_table ------------------ */

#ifndef GL_EXT_pixel_transform_color_table
#define GL_EXT_pixel_transform_color_table 1

#define GLEW_EXT_pixel_transform_color_table GLEW_GET_VAR(G_GLEW_EXT_pixel_transform_color_table)

#endif /* GL_EXT_pixel_transform_color_table */

/* ------------------------ GL_EXT_point_parameters ------------------------ */

#ifndef GL_EXT_point_parameters
#define GL_EXT_point_parameters 1

#define GL_POINT_SIZE_MIN_EXT 0x8126
#define GL_POINT_SIZE_MAX_EXT 0x8127
#define GL_POINT_FADE_THRESHOLD_SIZE_EXT 0x8128
#define GL_DISTANCE_ATTENUATION_EXT 0x8129

typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFEXTPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERFVEXTPROC) (GLenum pname, const GLfloat* params);

#define glPointParameterfEXT GLEW_GET_FUN(G_glewPointParameterfEXT)
#define glPointParameterfvEXT GLEW_GET_FUN(G_glewPointParameterfvEXT)

#define GLEW_EXT_point_parameters GLEW_GET_VAR(G_GLEW_EXT_point_parameters)

#endif /* GL_EXT_point_parameters */

/* ------------------------- GL_EXT_polygon_offset ------------------------- */

#ifndef GL_EXT_polygon_offset
#define GL_EXT_polygon_offset 1

#define GL_POLYGON_OFFSET_EXT 0x8037
#define GL_POLYGON_OFFSET_FACTOR_EXT 0x8038
#define GL_POLYGON_OFFSET_BIAS_EXT 0x8039

typedef void (GLAPIENTRY * PFNGLPOLYGONOFFSETEXTPROC) (GLfloat factor, GLfloat bias);

#define glPolygonOffsetEXT GLEW_GET_FUN(G_glewPolygonOffsetEXT)

#define GLEW_EXT_polygon_offset GLEW_GET_VAR(G_GLEW_EXT_polygon_offset)

#endif /* GL_EXT_polygon_offset */

/* ------------------------ GL_EXT_provoking_vertex ------------------------ */

#ifndef GL_EXT_provoking_vertex
#define GL_EXT_provoking_vertex 1

#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION_EXT 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION_EXT 0x8E4D
#define GL_LAST_VERTEX_CONVENTION_EXT 0x8E4E
#define GL_PROVOKING_VERTEX_EXT 0x8E4F

typedef void (GLAPIENTRY * PFNGLPROVOKINGVERTEXEXTPROC) (GLenum mode);

#define glProvokingVertexEXT GLEW_GET_FUN(G_glewProvokingVertexEXT)

#define GLEW_EXT_provoking_vertex GLEW_GET_VAR(G_GLEW_EXT_provoking_vertex)

#endif /* GL_EXT_provoking_vertex */

/* ------------------------- GL_EXT_rescale_normal ------------------------- */

#ifndef GL_EXT_rescale_normal
#define GL_EXT_rescale_normal 1

#define GL_RESCALE_NORMAL_EXT 0x803A

#define GLEW_EXT_rescale_normal GLEW_GET_VAR(G_GLEW_EXT_rescale_normal)

#endif /* GL_EXT_rescale_normal */

/* -------------------------- GL_EXT_scene_marker -------------------------- */

#ifndef GL_EXT_scene_marker
#define GL_EXT_scene_marker 1

typedef void (GLAPIENTRY * PFNGLBEGINSCENEEXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLENDSCENEEXTPROC) (void);

#define glBeginSceneEXT GLEW_GET_FUN(G_glewBeginSceneEXT)
#define glEndSceneEXT GLEW_GET_FUN(G_glewEndSceneEXT)

#define GLEW_EXT_scene_marker GLEW_GET_VAR(G_GLEW_EXT_scene_marker)

#endif /* GL_EXT_scene_marker */

/* ------------------------- GL_EXT_secondary_color ------------------------ */

#ifndef GL_EXT_secondary_color
#define GL_EXT_secondary_color 1

#define GL_COLOR_SUM_EXT 0x8458
#define GL_CURRENT_SECONDARY_COLOR_EXT 0x8459
#define GL_SECONDARY_COLOR_ARRAY_SIZE_EXT 0x845A
#define GL_SECONDARY_COLOR_ARRAY_TYPE_EXT 0x845B
#define GL_SECONDARY_COLOR_ARRAY_STRIDE_EXT 0x845C
#define GL_SECONDARY_COLOR_ARRAY_POINTER_EXT 0x845D
#define GL_SECONDARY_COLOR_ARRAY_EXT 0x845E

typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BEXTPROC) (GLbyte red, GLbyte green, GLbyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3BVEXTPROC) (const GLbyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DEXTPROC) (GLdouble red, GLdouble green, GLdouble blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3DVEXTPROC) (const GLdouble *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FEXTPROC) (GLfloat red, GLfloat green, GLfloat blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3FVEXTPROC) (const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IEXTPROC) (GLint red, GLint green, GLint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3IVEXTPROC) (const GLint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SEXTPROC) (GLshort red, GLshort green, GLshort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3SVEXTPROC) (const GLshort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBEXTPROC) (GLubyte red, GLubyte green, GLubyte blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UBVEXTPROC) (const GLubyte *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIEXTPROC) (GLuint red, GLuint green, GLuint blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3UIVEXTPROC) (const GLuint *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USEXTPROC) (GLushort red, GLushort green, GLushort blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3USVEXTPROC) (const GLushort *v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);

#define glSecondaryColor3bEXT GLEW_GET_FUN(G_glewSecondaryColor3bEXT)
#define glSecondaryColor3bvEXT GLEW_GET_FUN(G_glewSecondaryColor3bvEXT)
#define glSecondaryColor3dEXT GLEW_GET_FUN(G_glewSecondaryColor3dEXT)
#define glSecondaryColor3dvEXT GLEW_GET_FUN(G_glewSecondaryColor3dvEXT)
#define glSecondaryColor3fEXT GLEW_GET_FUN(G_glewSecondaryColor3fEXT)
#define glSecondaryColor3fvEXT GLEW_GET_FUN(G_glewSecondaryColor3fvEXT)
#define glSecondaryColor3iEXT GLEW_GET_FUN(G_glewSecondaryColor3iEXT)
#define glSecondaryColor3ivEXT GLEW_GET_FUN(G_glewSecondaryColor3ivEXT)
#define glSecondaryColor3sEXT GLEW_GET_FUN(G_glewSecondaryColor3sEXT)
#define glSecondaryColor3svEXT GLEW_GET_FUN(G_glewSecondaryColor3svEXT)
#define glSecondaryColor3ubEXT GLEW_GET_FUN(G_glewSecondaryColor3ubEXT)
#define glSecondaryColor3ubvEXT GLEW_GET_FUN(G_glewSecondaryColor3ubvEXT)
#define glSecondaryColor3uiEXT GLEW_GET_FUN(G_glewSecondaryColor3uiEXT)
#define glSecondaryColor3uivEXT GLEW_GET_FUN(G_glewSecondaryColor3uivEXT)
#define glSecondaryColor3usEXT GLEW_GET_FUN(G_glewSecondaryColor3usEXT)
#define glSecondaryColor3usvEXT GLEW_GET_FUN(G_glewSecondaryColor3usvEXT)
#define glSecondaryColorPointerEXT GLEW_GET_FUN(G_glewSecondaryColorPointerEXT)

#define GLEW_EXT_secondary_color GLEW_GET_VAR(G_GLEW_EXT_secondary_color)

#endif /* GL_EXT_secondary_color */

/* --------------------- GL_EXT_separate_shader_objects -------------------- */

#ifndef GL_EXT_separate_shader_objects
#define GL_EXT_separate_shader_objects 1

#define GL_ACTIVE_PROGRAM_EXT 0x8B8D

typedef void (GLAPIENTRY * PFNGLACTIVEPROGRAMEXTPROC) (GLuint program);
typedef GLuint (GLAPIENTRY * PFNGLCREATESHADERPROGRAMEXTPROC) (GLenum type, const GLchar* string);
typedef void (GLAPIENTRY * PFNGLUSESHADERPROGRAMEXTPROC) (GLenum type, GLuint program);

#define glActiveProgramEXT GLEW_GET_FUN(G_glewActiveProgramEXT)
#define glCreateShaderProgramEXT GLEW_GET_FUN(G_glewCreateShaderProgramEXT)
#define glUseShaderProgramEXT GLEW_GET_FUN(G_glewUseShaderProgramEXT)

#define GLEW_EXT_separate_shader_objects GLEW_GET_VAR(G_GLEW_EXT_separate_shader_objects)

#endif /* GL_EXT_separate_shader_objects */

/* --------------------- GL_EXT_separate_specular_color -------------------- */

#ifndef GL_EXT_separate_specular_color
#define GL_EXT_separate_specular_color 1

#define GL_LIGHT_MODEL_COLOR_CONTROL_EXT 0x81F8
#define GL_SINGLE_COLOR_EXT 0x81F9
#define GL_SEPARATE_SPECULAR_COLOR_EXT 0x81FA

#define GLEW_EXT_separate_specular_color GLEW_GET_VAR(G_GLEW_EXT_separate_specular_color)

#endif /* GL_EXT_separate_specular_color */

/* --------------------- GL_EXT_shader_image_load_store -------------------- */

#ifndef GL_EXT_shader_image_load_store
#define GL_EXT_shader_image_load_store 1

#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT_EXT 0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT_EXT 0x00000002
#define GL_UNIFORM_BARRIER_BIT_EXT 0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT_EXT 0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_EXT 0x00000020
#define GL_COMMAND_BARRIER_BIT_EXT 0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT_EXT 0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT_EXT 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT_EXT 0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT_EXT 0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT_EXT 0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT_EXT 0x00001000
#define GL_MAX_IMAGE_UNITS_EXT 0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS_EXT 0x8F39
#define GL_IMAGE_BINDING_NAME_EXT 0x8F3A
#define GL_IMAGE_BINDING_LEVEL_EXT 0x8F3B
#define GL_IMAGE_BINDING_LAYERED_EXT 0x8F3C
#define GL_IMAGE_BINDING_LAYER_EXT 0x8F3D
#define GL_IMAGE_BINDING_ACCESS_EXT 0x8F3E
#define GL_IMAGE_1D_EXT 0x904C
#define GL_IMAGE_2D_EXT 0x904D
#define GL_IMAGE_3D_EXT 0x904E
#define GL_IMAGE_2D_RECT_EXT 0x904F
#define GL_IMAGE_CUBE_EXT 0x9050
#define GL_IMAGE_BUFFER_EXT 0x9051
#define GL_IMAGE_1D_ARRAY_EXT 0x9052
#define GL_IMAGE_2D_ARRAY_EXT 0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY_EXT 0x9054
#define GL_IMAGE_2D_MULTISAMPLE_EXT 0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x9056
#define GL_INT_IMAGE_1D_EXT 0x9057
#define GL_INT_IMAGE_2D_EXT 0x9058
#define GL_INT_IMAGE_3D_EXT 0x9059
#define GL_INT_IMAGE_2D_RECT_EXT 0x905A
#define GL_INT_IMAGE_CUBE_EXT 0x905B
#define GL_INT_IMAGE_BUFFER_EXT 0x905C
#define GL_INT_IMAGE_1D_ARRAY_EXT 0x905D
#define GL_INT_IMAGE_2D_ARRAY_EXT 0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE_EXT 0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x9061
#define GL_UNSIGNED_INT_IMAGE_1D_EXT 0x9062
#define GL_UNSIGNED_INT_IMAGE_2D_EXT 0x9063
#define GL_UNSIGNED_INT_IMAGE_3D_EXT 0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT_EXT 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE_EXT 0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER_EXT 0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY_EXT 0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY_EXT 0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY_EXT 0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_EXT 0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY_EXT 0x906C
#define GL_MAX_IMAGE_SAMPLES_EXT 0x906D
#define GL_IMAGE_BINDING_FORMAT_EXT 0x906E
#define GL_ALL_BARRIER_BITS_EXT 0xFFFFFFFF

typedef void (GLAPIENTRY * PFNGLBINDIMAGETEXTUREEXTPROC) (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format);
typedef void (GLAPIENTRY * PFNGLMEMORYBARRIEREXTPROC) (GLbitfield barriers);

#define glBindImageTextureEXT GLEW_GET_FUN(G_glewBindImageTextureEXT)
#define glMemoryBarrierEXT GLEW_GET_FUN(G_glewMemoryBarrierEXT)

#define GLEW_EXT_shader_image_load_store GLEW_GET_VAR(G_GLEW_EXT_shader_image_load_store)

#endif /* GL_EXT_shader_image_load_store */

/* -------------------------- GL_EXT_shadow_funcs -------------------------- */

#ifndef GL_EXT_shadow_funcs
#define GL_EXT_shadow_funcs 1

#define GLEW_EXT_shadow_funcs GLEW_GET_VAR(G_GLEW_EXT_shadow_funcs)

#endif /* GL_EXT_shadow_funcs */

/* --------------------- GL_EXT_shared_texture_palette --------------------- */

#ifndef GL_EXT_shared_texture_palette
#define GL_EXT_shared_texture_palette 1

#define GL_SHARED_TEXTURE_PALETTE_EXT 0x81FB

#define GLEW_EXT_shared_texture_palette GLEW_GET_VAR(G_GLEW_EXT_shared_texture_palette)

#endif /* GL_EXT_shared_texture_palette */

/* ------------------------ GL_EXT_stencil_clear_tag ----------------------- */

#ifndef GL_EXT_stencil_clear_tag
#define GL_EXT_stencil_clear_tag 1

#define GL_STENCIL_TAG_BITS_EXT 0x88F2
#define GL_STENCIL_CLEAR_TAG_VALUE_EXT 0x88F3

#define GLEW_EXT_stencil_clear_tag GLEW_GET_VAR(G_GLEW_EXT_stencil_clear_tag)

#endif /* GL_EXT_stencil_clear_tag */

/* ------------------------ GL_EXT_stencil_two_side ------------------------ */

#ifndef GL_EXT_stencil_two_side
#define GL_EXT_stencil_two_side 1

#define GL_STENCIL_TEST_TWO_SIDE_EXT 0x8910
#define GL_ACTIVE_STENCIL_FACE_EXT 0x8911

typedef void (GLAPIENTRY * PFNGLACTIVESTENCILFACEEXTPROC) (GLenum face);

#define glActiveStencilFaceEXT GLEW_GET_FUN(G_glewActiveStencilFaceEXT)

#define GLEW_EXT_stencil_two_side GLEW_GET_VAR(G_GLEW_EXT_stencil_two_side)

#endif /* GL_EXT_stencil_two_side */

/* -------------------------- GL_EXT_stencil_wrap -------------------------- */

#ifndef GL_EXT_stencil_wrap
#define GL_EXT_stencil_wrap 1

#define GL_INCR_WRAP_EXT 0x8507
#define GL_DECR_WRAP_EXT 0x8508

#define GLEW_EXT_stencil_wrap GLEW_GET_VAR(G_GLEW_EXT_stencil_wrap)

#endif /* GL_EXT_stencil_wrap */

/* --------------------------- GL_EXT_subtexture --------------------------- */

#ifndef GL_EXT_subtexture
#define GL_EXT_subtexture 1

typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE1DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE2DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE3DEXTPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);

#define glTexSubImage1DEXT GLEW_GET_FUN(G_glewTexSubImage1DEXT)
#define glTexSubImage2DEXT GLEW_GET_FUN(G_glewTexSubImage2DEXT)
#define glTexSubImage3DEXT GLEW_GET_FUN(G_glewTexSubImage3DEXT)

#define GLEW_EXT_subtexture GLEW_GET_VAR(G_GLEW_EXT_subtexture)

#endif /* GL_EXT_subtexture */

/* ----------------------------- GL_EXT_texture ---------------------------- */

#ifndef GL_EXT_texture
#define GL_EXT_texture 1

#define GL_ALPHA4_EXT 0x803B
#define GL_ALPHA8_EXT 0x803C
#define GL_ALPHA12_EXT 0x803D
#define GL_ALPHA16_EXT 0x803E
#define GL_LUMINANCE4_EXT 0x803F
#define GL_LUMINANCE8_EXT 0x8040
#define GL_LUMINANCE12_EXT 0x8041
#define GL_LUMINANCE16_EXT 0x8042
#define GL_LUMINANCE4_ALPHA4_EXT 0x8043
#define GL_LUMINANCE6_ALPHA2_EXT 0x8044
#define GL_LUMINANCE8_ALPHA8_EXT 0x8045
#define GL_LUMINANCE12_ALPHA4_EXT 0x8046
#define GL_LUMINANCE12_ALPHA12_EXT 0x8047
#define GL_LUMINANCE16_ALPHA16_EXT 0x8048
#define GL_INTENSITY_EXT 0x8049
#define GL_INTENSITY4_EXT 0x804A
#define GL_INTENSITY8_EXT 0x804B
#define GL_INTENSITY12_EXT 0x804C
#define GL_INTENSITY16_EXT 0x804D
#define GL_RGB2_EXT 0x804E
#define GL_RGB4_EXT 0x804F
#define GL_RGB5_EXT 0x8050
#define GL_RGB8_EXT 0x8051
#define GL_RGB10_EXT 0x8052
#define GL_RGB12_EXT 0x8053
#define GL_RGB16_EXT 0x8054
#define GL_RGBA2_EXT 0x8055
#define GL_RGBA4_EXT 0x8056
#define GL_RGB5_A1_EXT 0x8057
#define GL_RGBA8_EXT 0x8058
#define GL_RGB10_A2_EXT 0x8059
#define GL_RGBA12_EXT 0x805A
#define GL_RGBA16_EXT 0x805B
#define GL_TEXTURE_RED_SIZE_EXT 0x805C
#define GL_TEXTURE_GREEN_SIZE_EXT 0x805D
#define GL_TEXTURE_BLUE_SIZE_EXT 0x805E
#define GL_TEXTURE_ALPHA_SIZE_EXT 0x805F
#define GL_TEXTURE_LUMINANCE_SIZE_EXT 0x8060
#define GL_TEXTURE_INTENSITY_SIZE_EXT 0x8061
#define GL_REPLACE_EXT 0x8062
#define GL_PROXY_TEXTURE_1D_EXT 0x8063
#define GL_PROXY_TEXTURE_2D_EXT 0x8064

#define GLEW_EXT_texture GLEW_GET_VAR(G_GLEW_EXT_texture)

#endif /* GL_EXT_texture */

/* ---------------------------- GL_EXT_texture3D --------------------------- */

#ifndef GL_EXT_texture3D
#define GL_EXT_texture3D 1

#define GL_PACK_SKIP_IMAGES_EXT 0x806B
#define GL_PACK_IMAGE_HEIGHT_EXT 0x806C
#define GL_UNPACK_SKIP_IMAGES_EXT 0x806D
#define GL_UNPACK_IMAGE_HEIGHT_EXT 0x806E
#define GL_TEXTURE_3D_EXT 0x806F
#define GL_PROXY_TEXTURE_3D_EXT 0x8070
#define GL_TEXTURE_DEPTH_EXT 0x8071
#define GL_TEXTURE_WRAP_R_EXT 0x8072
#define GL_MAX_3D_TEXTURE_SIZE_EXT 0x8073

typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DEXTPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);

#define glTexImage3DEXT GLEW_GET_FUN(G_glewTexImage3DEXT)

#define GLEW_EXT_texture3D GLEW_GET_VAR(G_GLEW_EXT_texture3D)

#endif /* GL_EXT_texture3D */

/* -------------------------- GL_EXT_texture_array ------------------------- */

#ifndef GL_EXT_texture_array
#define GL_EXT_texture_array 1

#define GL_COMPARE_REF_DEPTH_TO_TEXTURE_EXT 0x884E
#define GL_MAX_ARRAY_TEXTURE_LAYERS_EXT 0x88FF
#define GL_TEXTURE_1D_ARRAY_EXT 0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY_EXT 0x8C19
#define GL_TEXTURE_2D_ARRAY_EXT 0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY_EXT 0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY_EXT 0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY_EXT 0x8C1D

typedef void (GLAPIENTRY * PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC) (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);

#define glFramebufferTextureLayerEXT GLEW_GET_FUN(G_glewFramebufferTextureLayerEXT)

#define GLEW_EXT_texture_array GLEW_GET_VAR(G_GLEW_EXT_texture_array)

#endif /* GL_EXT_texture_array */

/* ---------------------- GL_EXT_texture_buffer_object --------------------- */

#ifndef GL_EXT_texture_buffer_object
#define GL_EXT_texture_buffer_object 1

#define GL_TEXTURE_BUFFER_EXT 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE_EXT 0x8C2B
#define GL_TEXTURE_BINDING_BUFFER_EXT 0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_EXT 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT_EXT 0x8C2E

typedef void (GLAPIENTRY * PFNGLTEXBUFFEREXTPROC) (GLenum target, GLenum internalformat, GLuint buffer);

#define glTexBufferEXT GLEW_GET_FUN(G_glewTexBufferEXT)

#define GLEW_EXT_texture_buffer_object GLEW_GET_VAR(G_GLEW_EXT_texture_buffer_object)

#endif /* GL_EXT_texture_buffer_object */

/* -------------------- GL_EXT_texture_compression_dxt1 -------------------- */

#ifndef GL_EXT_texture_compression_dxt1
#define GL_EXT_texture_compression_dxt1 1

#define GLEW_EXT_texture_compression_dxt1 GLEW_GET_VAR(G_GLEW_EXT_texture_compression_dxt1)

#endif /* GL_EXT_texture_compression_dxt1 */

/* -------------------- GL_EXT_texture_compression_latc -------------------- */

#ifndef GL_EXT_texture_compression_latc
#define GL_EXT_texture_compression_latc 1

#define GL_COMPRESSED_LUMINANCE_LATC1_EXT 0x8C70
#define GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT 0x8C71
#define GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT 0x8C72
#define GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT 0x8C73

#define GLEW_EXT_texture_compression_latc GLEW_GET_VAR(G_GLEW_EXT_texture_compression_latc)

#endif /* GL_EXT_texture_compression_latc */

/* -------------------- GL_EXT_texture_compression_rgtc -------------------- */

#ifndef GL_EXT_texture_compression_rgtc
#define GL_EXT_texture_compression_rgtc 1

#define GL_COMPRESSED_RED_RGTC1_EXT 0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1_EXT 0x8DBC
#define GL_COMPRESSED_RED_GREEN_RGTC2_EXT 0x8DBD
#define GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT 0x8DBE

#define GLEW_EXT_texture_compression_rgtc GLEW_GET_VAR(G_GLEW_EXT_texture_compression_rgtc)

#endif /* GL_EXT_texture_compression_rgtc */

/* -------------------- GL_EXT_texture_compression_s3tc -------------------- */

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT 0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

#define GLEW_EXT_texture_compression_s3tc GLEW_GET_VAR(G_GLEW_EXT_texture_compression_s3tc)

#endif /* GL_EXT_texture_compression_s3tc */

/* ------------------------ GL_EXT_texture_cube_map ------------------------ */

#ifndef GL_EXT_texture_cube_map
#define GL_EXT_texture_cube_map 1

#define GL_NORMAL_MAP_EXT 0x8511
#define GL_REFLECTION_MAP_EXT 0x8512
#define GL_TEXTURE_CUBE_MAP_EXT 0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_EXT 0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT 0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_EXT 0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT 0x851C

#define GLEW_EXT_texture_cube_map GLEW_GET_VAR(G_GLEW_EXT_texture_cube_map)

#endif /* GL_EXT_texture_cube_map */

/* ----------------------- GL_EXT_texture_edge_clamp ----------------------- */

#ifndef GL_EXT_texture_edge_clamp
#define GL_EXT_texture_edge_clamp 1

#define GL_CLAMP_TO_EDGE_EXT 0x812F

#define GLEW_EXT_texture_edge_clamp GLEW_GET_VAR(G_GLEW_EXT_texture_edge_clamp)

#endif /* GL_EXT_texture_edge_clamp */

/* --------------------------- GL_EXT_texture_env -------------------------- */

#ifndef GL_EXT_texture_env
#define GL_EXT_texture_env 1

#define GL_TEXTURE_ENV0_EXT 0
#define GL_ENV_BLEND_EXT 0
#define GL_TEXTURE_ENV_SHIFT_EXT 0
#define GL_ENV_REPLACE_EXT 0
#define GL_ENV_ADD_EXT 0
#define GL_ENV_SUBTRACT_EXT 0
#define GL_TEXTURE_ENV_MODE_ALPHA_EXT 0
#define GL_ENV_REVERSE_SUBTRACT_EXT 0
#define GL_ENV_REVERSE_BLEND_EXT 0
#define GL_ENV_COPY_EXT 0
#define GL_ENV_MODULATE_EXT 0

#define GLEW_EXT_texture_env GLEW_GET_VAR(G_GLEW_EXT_texture_env)

#endif /* GL_EXT_texture_env */

/* ------------------------- GL_EXT_texture_env_add ------------------------ */

#ifndef GL_EXT_texture_env_add
#define GL_EXT_texture_env_add 1

#define GLEW_EXT_texture_env_add GLEW_GET_VAR(G_GLEW_EXT_texture_env_add)

#endif /* GL_EXT_texture_env_add */

/* ----------------------- GL_EXT_texture_env_combine ---------------------- */

#ifndef GL_EXT_texture_env_combine
#define GL_EXT_texture_env_combine 1

#define GL_COMBINE_EXT 0x8570
#define GL_COMBINE_RGB_EXT 0x8571
#define GL_COMBINE_ALPHA_EXT 0x8572
#define GL_RGB_SCALE_EXT 0x8573
#define GL_ADD_SIGNED_EXT 0x8574
#define GL_INTERPOLATE_EXT 0x8575
#define GL_CONSTANT_EXT 0x8576
#define GL_PRIMARY_COLOR_EXT 0x8577
#define GL_PREVIOUS_EXT 0x8578
#define GL_SOURCE0_RGB_EXT 0x8580
#define GL_SOURCE1_RGB_EXT 0x8581
#define GL_SOURCE2_RGB_EXT 0x8582
#define GL_SOURCE0_ALPHA_EXT 0x8588
#define GL_SOURCE1_ALPHA_EXT 0x8589
#define GL_SOURCE2_ALPHA_EXT 0x858A
#define GL_OPERAND0_RGB_EXT 0x8590
#define GL_OPERAND1_RGB_EXT 0x8591
#define GL_OPERAND2_RGB_EXT 0x8592
#define GL_OPERAND0_ALPHA_EXT 0x8598
#define GL_OPERAND1_ALPHA_EXT 0x8599
#define GL_OPERAND2_ALPHA_EXT 0x859A

#define GLEW_EXT_texture_env_combine GLEW_GET_VAR(G_GLEW_EXT_texture_env_combine)

#endif /* GL_EXT_texture_env_combine */

/* ------------------------ GL_EXT_texture_env_dot3 ------------------------ */

#ifndef GL_EXT_texture_env_dot3
#define GL_EXT_texture_env_dot3 1

#define GL_DOT3_RGB_EXT 0x8740
#define GL_DOT3_RGBA_EXT 0x8741

#define GLEW_EXT_texture_env_dot3 GLEW_GET_VAR(G_GLEW_EXT_texture_env_dot3)

#endif /* GL_EXT_texture_env_dot3 */

/* ------------------- GL_EXT_texture_filter_anisotropic ------------------- */

#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1

#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF

#define GLEW_EXT_texture_filter_anisotropic GLEW_GET_VAR(G_GLEW_EXT_texture_filter_anisotropic)

#endif /* GL_EXT_texture_filter_anisotropic */

/* ------------------------- GL_EXT_texture_integer ------------------------ */

#ifndef GL_EXT_texture_integer
#define GL_EXT_texture_integer 1

#define GL_RGBA32UI_EXT 0x8D70
#define GL_RGB32UI_EXT 0x8D71
#define GL_ALPHA32UI_EXT 0x8D72
#define GL_INTENSITY32UI_EXT 0x8D73
#define GL_LUMINANCE32UI_EXT 0x8D74
#define GL_LUMINANCE_ALPHA32UI_EXT 0x8D75
#define GL_RGBA16UI_EXT 0x8D76
#define GL_RGB16UI_EXT 0x8D77
#define GL_ALPHA16UI_EXT 0x8D78
#define GL_INTENSITY16UI_EXT 0x8D79
#define GL_LUMINANCE16UI_EXT 0x8D7A
#define GL_LUMINANCE_ALPHA16UI_EXT 0x8D7B
#define GL_RGBA8UI_EXT 0x8D7C
#define GL_RGB8UI_EXT 0x8D7D
#define GL_ALPHA8UI_EXT 0x8D7E
#define GL_INTENSITY8UI_EXT 0x8D7F
#define GL_LUMINANCE8UI_EXT 0x8D80
#define GL_LUMINANCE_ALPHA8UI_EXT 0x8D81
#define GL_RGBA32I_EXT 0x8D82
#define GL_RGB32I_EXT 0x8D83
#define GL_ALPHA32I_EXT 0x8D84
#define GL_INTENSITY32I_EXT 0x8D85
#define GL_LUMINANCE32I_EXT 0x8D86
#define GL_LUMINANCE_ALPHA32I_EXT 0x8D87
#define GL_RGBA16I_EXT 0x8D88
#define GL_RGB16I_EXT 0x8D89
#define GL_ALPHA16I_EXT 0x8D8A
#define GL_INTENSITY16I_EXT 0x8D8B
#define GL_LUMINANCE16I_EXT 0x8D8C
#define GL_LUMINANCE_ALPHA16I_EXT 0x8D8D
#define GL_RGBA8I_EXT 0x8D8E
#define GL_RGB8I_EXT 0x8D8F
#define GL_ALPHA8I_EXT 0x8D90
#define GL_INTENSITY8I_EXT 0x8D91
#define GL_LUMINANCE8I_EXT 0x8D92
#define GL_LUMINANCE_ALPHA8I_EXT 0x8D93
#define GL_RED_INTEGER_EXT 0x8D94
#define GL_GREEN_INTEGER_EXT 0x8D95
#define GL_BLUE_INTEGER_EXT 0x8D96
#define GL_ALPHA_INTEGER_EXT 0x8D97
#define GL_RGB_INTEGER_EXT 0x8D98
#define GL_RGBA_INTEGER_EXT 0x8D99
#define GL_BGR_INTEGER_EXT 0x8D9A
#define GL_BGRA_INTEGER_EXT 0x8D9B
#define GL_LUMINANCE_INTEGER_EXT 0x8D9C
#define GL_LUMINANCE_ALPHA_INTEGER_EXT 0x8D9D
#define GL_RGBA_INTEGER_MODE_EXT 0x8D9E

typedef void (GLAPIENTRY * PFNGLCLEARCOLORIIEXTPROC) (GLint red, GLint green, GLint blue, GLint alpha);
typedef void (GLAPIENTRY * PFNGLCLEARCOLORIUIEXTPROC) (GLuint red, GLuint green, GLuint blue, GLuint alpha);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, GLint *params);
typedef void (GLAPIENTRY * PFNGLGETTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, GLuint *params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIIVEXTPROC) (GLenum target, GLenum pname, const GLint *params);
typedef void (GLAPIENTRY * PFNGLTEXPARAMETERIUIVEXTPROC) (GLenum target, GLenum pname, const GLuint *params);

#define glClearColorIiEXT GLEW_GET_FUN(G_glewClearColorIiEXT)
#define glClearColorIuiEXT GLEW_GET_FUN(G_glewClearColorIuiEXT)
#define glGetTexParameterIivEXT GLEW_GET_FUN(G_glewGetTexParameterIivEXT)
#define glGetTexParameterIuivEXT GLEW_GET_FUN(G_glewGetTexParameterIuivEXT)
#define glTexParameterIivEXT GLEW_GET_FUN(G_glewTexParameterIivEXT)
#define glTexParameterIuivEXT GLEW_GET_FUN(G_glewTexParameterIuivEXT)

#define GLEW_EXT_texture_integer GLEW_GET_VAR(G_GLEW_EXT_texture_integer)

#endif /* GL_EXT_texture_integer */

/* ------------------------ GL_EXT_texture_lod_bias ------------------------ */

#ifndef GL_EXT_texture_lod_bias
#define GL_EXT_texture_lod_bias 1

#define GL_MAX_TEXTURE_LOD_BIAS_EXT 0x84FD
#define GL_TEXTURE_FILTER_CONTROL_EXT 0x8500
#define GL_TEXTURE_LOD_BIAS_EXT 0x8501

#define GLEW_EXT_texture_lod_bias GLEW_GET_VAR(G_GLEW_EXT_texture_lod_bias)

#endif /* GL_EXT_texture_lod_bias */

/* ---------------------- GL_EXT_texture_mirror_clamp ---------------------- */

#ifndef GL_EXT_texture_mirror_clamp
#define GL_EXT_texture_mirror_clamp 1

#define GL_MIRROR_CLAMP_EXT 0x8742
#define GL_MIRROR_CLAMP_TO_EDGE_EXT 0x8743
#define GL_MIRROR_CLAMP_TO_BORDER_EXT 0x8912

#define GLEW_EXT_texture_mirror_clamp GLEW_GET_VAR(G_GLEW_EXT_texture_mirror_clamp)

#endif /* GL_EXT_texture_mirror_clamp */

/* ------------------------- GL_EXT_texture_object ------------------------- */

#ifndef GL_EXT_texture_object
#define GL_EXT_texture_object 1

#define GL_TEXTURE_PRIORITY_EXT 0x8066
#define GL_TEXTURE_RESIDENT_EXT 0x8067
#define GL_TEXTURE_1D_BINDING_EXT 0x8068
#define GL_TEXTURE_2D_BINDING_EXT 0x8069
#define GL_TEXTURE_3D_BINDING_EXT 0x806A

typedef GLboolean (GLAPIENTRY * PFNGLARETEXTURESRESIDENTEXTPROC) (GLsizei n, const GLuint* textures, GLboolean* residences);
typedef void (GLAPIENTRY * PFNGLBINDTEXTUREEXTPROC) (GLenum target, GLuint texture);
typedef void (GLAPIENTRY * PFNGLDELETETEXTURESEXTPROC) (GLsizei n, const GLuint* textures);
typedef void (GLAPIENTRY * PFNGLGENTEXTURESEXTPROC) (GLsizei n, GLuint* textures);
typedef GLboolean (GLAPIENTRY * PFNGLISTEXTUREEXTPROC) (GLuint texture);
typedef void (GLAPIENTRY * PFNGLPRIORITIZETEXTURESEXTPROC) (GLsizei n, const GLuint* textures, const GLclampf* priorities);

#define glAreTexturesResidentEXT GLEW_GET_FUN(G_glewAreTexturesResidentEXT)
#define glBindTextureEXT GLEW_GET_FUN(G_glewBindTextureEXT)
#define glDeleteTexturesEXT GLEW_GET_FUN(G_glewDeleteTexturesEXT)
#define glGenTexturesEXT GLEW_GET_FUN(G_glewGenTexturesEXT)
#define glIsTextureEXT GLEW_GET_FUN(G_glewIsTextureEXT)
#define glPrioritizeTexturesEXT GLEW_GET_FUN(G_glewPrioritizeTexturesEXT)

#define GLEW_EXT_texture_object GLEW_GET_VAR(G_GLEW_EXT_texture_object)

#endif /* GL_EXT_texture_object */

/* --------------------- GL_EXT_texture_perturb_normal --------------------- */

#ifndef GL_EXT_texture_perturb_normal
#define GL_EXT_texture_perturb_normal 1

#define GL_PERTURB_EXT 0x85AE
#define GL_TEXTURE_NORMAL_EXT 0x85AF

typedef void (GLAPIENTRY * PFNGLTEXTURENORMALEXTPROC) (GLenum mode);

#define glTextureNormalEXT GLEW_GET_FUN(G_glewTextureNormalEXT)

#define GLEW_EXT_texture_perturb_normal GLEW_GET_VAR(G_GLEW_EXT_texture_perturb_normal)

#endif /* GL_EXT_texture_perturb_normal */

/* ------------------------ GL_EXT_texture_rectangle ----------------------- */

#ifndef GL_EXT_texture_rectangle
#define GL_EXT_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_EXT 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_EXT 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_EXT 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_EXT 0x84F8

#define GLEW_EXT_texture_rectangle GLEW_GET_VAR(G_GLEW_EXT_texture_rectangle)

#endif /* GL_EXT_texture_rectangle */

/* -------------------------- GL_EXT_texture_sRGB -------------------------- */

#ifndef GL_EXT_texture_sRGB
#define GL_EXT_texture_sRGB 1

#define GL_SRGB_EXT 0x8C40
#define GL_SRGB8_EXT 0x8C41
#define GL_SRGB_ALPHA_EXT 0x8C42
#define GL_SRGB8_ALPHA8_EXT 0x8C43
#define GL_SLUMINANCE_ALPHA_EXT 0x8C44
#define GL_SLUMINANCE8_ALPHA8_EXT 0x8C45
#define GL_SLUMINANCE_EXT 0x8C46
#define GL_SLUMINANCE8_EXT 0x8C47
#define GL_COMPRESSED_SRGB_EXT 0x8C48
#define GL_COMPRESSED_SRGB_ALPHA_EXT 0x8C49
#define GL_COMPRESSED_SLUMINANCE_EXT 0x8C4A
#define GL_COMPRESSED_SLUMINANCE_ALPHA_EXT 0x8C4B
#define GL_COMPRESSED_SRGB_S3TC_DXT1_EXT 0x8C4C
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT 0x8C4D
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT 0x8C4E
#define GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT 0x8C4F

#define GLEW_EXT_texture_sRGB GLEW_GET_VAR(G_GLEW_EXT_texture_sRGB)

#endif /* GL_EXT_texture_sRGB */

/* ----------------------- GL_EXT_texture_sRGB_decode ---------------------- */

#ifndef GL_EXT_texture_sRGB_decode
#define GL_EXT_texture_sRGB_decode 1

#define GL_TEXTURE_SRGB_DECODE_EXT 0x8A48
#define GL_DECODE_EXT 0x8A49
#define GL_SKIP_DECODE_EXT 0x8A4A

#define GLEW_EXT_texture_sRGB_decode GLEW_GET_VAR(G_GLEW_EXT_texture_sRGB_decode)

#endif /* GL_EXT_texture_sRGB_decode */

/* --------------------- GL_EXT_texture_shared_exponent -------------------- */

#ifndef GL_EXT_texture_shared_exponent
#define GL_EXT_texture_shared_exponent 1

#define GL_RGB9_E5_EXT 0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV_EXT 0x8C3E
#define GL_TEXTURE_SHARED_SIZE_EXT 0x8C3F

#define GLEW_EXT_texture_shared_exponent GLEW_GET_VAR(G_GLEW_EXT_texture_shared_exponent)

#endif /* GL_EXT_texture_shared_exponent */

/* -------------------------- GL_EXT_texture_snorm ------------------------- */

#ifndef GL_EXT_texture_snorm
#define GL_EXT_texture_snorm 1

#define GL_RED_SNORM 0x8F90
#define GL_RG_SNORM 0x8F91
#define GL_RGB_SNORM 0x8F92
#define GL_RGBA_SNORM 0x8F93
#define GL_R8_SNORM 0x8F94
#define GL_RG8_SNORM 0x8F95
#define GL_RGB8_SNORM 0x8F96
#define GL_RGBA8_SNORM 0x8F97
#define GL_R16_SNORM 0x8F98
#define GL_RG16_SNORM 0x8F99
#define GL_RGB16_SNORM 0x8F9A
#define GL_RGBA16_SNORM 0x8F9B
#define GL_SIGNED_NORMALIZED 0x8F9C
#define GL_ALPHA_SNORM 0x9010
#define GL_LUMINANCE_SNORM 0x9011
#define GL_LUMINANCE_ALPHA_SNORM 0x9012
#define GL_INTENSITY_SNORM 0x9013
#define GL_ALPHA8_SNORM 0x9014
#define GL_LUMINANCE8_SNORM 0x9015
#define GL_LUMINANCE8_ALPHA8_SNORM 0x9016
#define GL_INTENSITY8_SNORM 0x9017
#define GL_ALPHA16_SNORM 0x9018
#define GL_LUMINANCE16_SNORM 0x9019
#define GL_LUMINANCE16_ALPHA16_SNORM 0x901A
#define GL_INTENSITY16_SNORM 0x901B

#define GLEW_EXT_texture_snorm GLEW_GET_VAR(G_GLEW_EXT_texture_snorm)

#endif /* GL_EXT_texture_snorm */

/* ------------------------- GL_EXT_texture_swizzle ------------------------ */

#ifndef GL_EXT_texture_swizzle
#define GL_EXT_texture_swizzle 1

#define GL_TEXTURE_SWIZZLE_R_EXT 0x8E42
#define GL_TEXTURE_SWIZZLE_G_EXT 0x8E43
#define GL_TEXTURE_SWIZZLE_B_EXT 0x8E44
#define GL_TEXTURE_SWIZZLE_A_EXT 0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA_EXT 0x8E46

#define GLEW_EXT_texture_swizzle GLEW_GET_VAR(G_GLEW_EXT_texture_swizzle)

#endif /* GL_EXT_texture_swizzle */

/* --------------------------- GL_EXT_timer_query -------------------------- */

#ifndef GL_EXT_timer_query
#define GL_EXT_timer_query 1

#define GL_TIME_ELAPSED_EXT 0x88BF

typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTI64VEXTPROC) (GLuint id, GLenum pname, GLint64EXT *params);
typedef void (GLAPIENTRY * PFNGLGETQUERYOBJECTUI64VEXTPROC) (GLuint id, GLenum pname, GLuint64EXT *params);

#define glGetQueryObjecti64vEXT GLEW_GET_FUN(G_glewGetQueryObjecti64vEXT)
#define glGetQueryObjectui64vEXT GLEW_GET_FUN(G_glewGetQueryObjectui64vEXT)

#define GLEW_EXT_timer_query GLEW_GET_VAR(G_GLEW_EXT_timer_query)

#endif /* GL_EXT_timer_query */

/* ----------------------- GL_EXT_transform_feedback ----------------------- */

#ifndef GL_EXT_transform_feedback
#define GL_EXT_transform_feedback 1

#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH_EXT 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE_EXT 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_EXT 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS_EXT 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START_EXT 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_EXT 0x8C85
#define GL_PRIMITIVES_GENERATED_EXT 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_EXT 0x8C88
#define GL_RASTERIZER_DISCARD_EXT 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_EXT 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_EXT 0x8C8B
#define GL_INTERLEAVED_ATTRIBS_EXT 0x8C8C
#define GL_SEPARATE_ATTRIBS_EXT 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER_EXT 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_EXT 0x8C8F

typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKEXTPROC) (GLenum primitiveMode);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASEEXTPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFEROFFSETEXTPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGEEXTPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKEXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei* length, GLsizei *size, GLenum *type, GLchar *name);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC) (GLuint program, GLsizei count, const GLchar ** varyings, GLenum bufferMode);

#define glBeginTransformFeedbackEXT GLEW_GET_FUN(G_glewBeginTransformFeedbackEXT)
#define glBindBufferBaseEXT GLEW_GET_FUN(G_glewBindBufferBaseEXT)
#define glBindBufferOffsetEXT GLEW_GET_FUN(G_glewBindBufferOffsetEXT)
#define glBindBufferRangeEXT GLEW_GET_FUN(G_glewBindBufferRangeEXT)
#define glEndTransformFeedbackEXT GLEW_GET_FUN(G_glewEndTransformFeedbackEXT)
#define glGetTransformFeedbackVaryingEXT GLEW_GET_FUN(G_glewGetTransformFeedbackVaryingEXT)
#define glTransformFeedbackVaryingsEXT GLEW_GET_FUN(G_glewTransformFeedbackVaryingsEXT)

#define GLEW_EXT_transform_feedback GLEW_GET_VAR(G_GLEW_EXT_transform_feedback)

#endif /* GL_EXT_transform_feedback */

/* -------------------------- GL_EXT_vertex_array -------------------------- */

#ifndef GL_EXT_vertex_array
#define GL_EXT_vertex_array 1

#define GL_DOUBLE_EXT 0x140A
#define GL_VERTEX_ARRAY_EXT 0x8074
#define GL_NORMAL_ARRAY_EXT 0x8075
#define GL_COLOR_ARRAY_EXT 0x8076
#define GL_INDEX_ARRAY_EXT 0x8077
#define GL_TEXTURE_COORD_ARRAY_EXT 0x8078
#define GL_EDGE_FLAG_ARRAY_EXT 0x8079
#define GL_VERTEX_ARRAY_SIZE_EXT 0x807A
#define GL_VERTEX_ARRAY_TYPE_EXT 0x807B
#define GL_VERTEX_ARRAY_STRIDE_EXT 0x807C
#define GL_VERTEX_ARRAY_COUNT_EXT 0x807D
#define GL_NORMAL_ARRAY_TYPE_EXT 0x807E
#define GL_NORMAL_ARRAY_STRIDE_EXT 0x807F
#define GL_NORMAL_ARRAY_COUNT_EXT 0x8080
#define GL_COLOR_ARRAY_SIZE_EXT 0x8081
#define GL_COLOR_ARRAY_TYPE_EXT 0x8082
#define GL_COLOR_ARRAY_STRIDE_EXT 0x8083
#define GL_COLOR_ARRAY_COUNT_EXT 0x8084
#define GL_INDEX_ARRAY_TYPE_EXT 0x8085
#define GL_INDEX_ARRAY_STRIDE_EXT 0x8086
#define GL_INDEX_ARRAY_COUNT_EXT 0x8087
#define GL_TEXTURE_COORD_ARRAY_SIZE_EXT 0x8088
#define GL_TEXTURE_COORD_ARRAY_TYPE_EXT 0x8089
#define GL_TEXTURE_COORD_ARRAY_STRIDE_EXT 0x808A
#define GL_TEXTURE_COORD_ARRAY_COUNT_EXT 0x808B
#define GL_EDGE_FLAG_ARRAY_STRIDE_EXT 0x808C
#define GL_EDGE_FLAG_ARRAY_COUNT_EXT 0x808D
#define GL_VERTEX_ARRAY_POINTER_EXT 0x808E
#define GL_NORMAL_ARRAY_POINTER_EXT 0x808F
#define GL_COLOR_ARRAY_POINTER_EXT 0x8090
#define GL_INDEX_ARRAY_POINTER_EXT 0x8091
#define GL_TEXTURE_COORD_ARRAY_POINTER_EXT 0x8092
#define GL_EDGE_FLAG_ARRAY_POINTER_EXT 0x8093

typedef void (GLAPIENTRY * PFNGLARRAYELEMENTEXTPROC) (GLint i);
typedef void (GLAPIENTRY * PFNGLCOLORPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void (GLAPIENTRY * PFNGLDRAWARRAYSEXTPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGPOINTEREXTPROC) (GLsizei stride, GLsizei count, const GLboolean* pointer);
typedef void (GLAPIENTRY * PFNGLINDEXPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTEREXTPROC) (GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, GLsizei count, const void* pointer);

#define glArrayElementEXT GLEW_GET_FUN(G_glewArrayElementEXT)
#define glColorPointerEXT GLEW_GET_FUN(G_glewColorPointerEXT)
#define glDrawArraysEXT GLEW_GET_FUN(G_glewDrawArraysEXT)
#define glEdgeFlagPointerEXT GLEW_GET_FUN(G_glewEdgeFlagPointerEXT)
#define glIndexPointerEXT GLEW_GET_FUN(G_glewIndexPointerEXT)
#define glNormalPointerEXT GLEW_GET_FUN(G_glewNormalPointerEXT)
#define glTexCoordPointerEXT GLEW_GET_FUN(G_glewTexCoordPointerEXT)
#define glVertexPointerEXT GLEW_GET_FUN(G_glewVertexPointerEXT)

#define GLEW_EXT_vertex_array GLEW_GET_VAR(G_GLEW_EXT_vertex_array)

#endif /* GL_EXT_vertex_array */

/* ------------------------ GL_EXT_vertex_array_bgra ----------------------- */

#ifndef GL_EXT_vertex_array_bgra
#define GL_EXT_vertex_array_bgra 1

#define GL_BGRA 0x80E1

#define GLEW_EXT_vertex_array_bgra GLEW_GET_VAR(G_GLEW_EXT_vertex_array_bgra)

#endif /* GL_EXT_vertex_array_bgra */

/* ----------------------- GL_EXT_vertex_attrib_64bit ---------------------- */

#ifndef GL_EXT_vertex_attrib_64bit
#define GL_EXT_vertex_attrib_64bit 1

#define GL_DOUBLE_MAT2_EXT 0x8F46
#define GL_DOUBLE_MAT3_EXT 0x8F47
#define GL_DOUBLE_MAT4_EXT 0x8F48
#define GL_DOUBLE_MAT2x3_EXT 0x8F49
#define GL_DOUBLE_MAT2x4_EXT 0x8F4A
#define GL_DOUBLE_MAT3x2_EXT 0x8F4B
#define GL_DOUBLE_MAT3x4_EXT 0x8F4C
#define GL_DOUBLE_MAT4x2_EXT 0x8F4D
#define GL_DOUBLE_MAT4x3_EXT 0x8F4E
#define GL_DOUBLE_VEC2_EXT 0x8FFC
#define GL_DOUBLE_VEC3_EXT 0x8FFD
#define GL_DOUBLE_VEC4_EXT 0x8FFE

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLDVEXTPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC) (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DEXTPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DEXTPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DEXTPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DEXTPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4DVEXTPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLPOINTEREXTPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);

#define glGetVertexAttribLdvEXT GLEW_GET_FUN(G_glewGetVertexAttribLdvEXT)
#define glVertexArrayVertexAttribLOffsetEXT GLEW_GET_FUN(G_glewVertexArrayVertexAttribLOffsetEXT)
#define glVertexAttribL1dEXT GLEW_GET_FUN(G_glewVertexAttribL1dEXT)
#define glVertexAttribL1dvEXT GLEW_GET_FUN(G_glewVertexAttribL1dvEXT)
#define glVertexAttribL2dEXT GLEW_GET_FUN(G_glewVertexAttribL2dEXT)
#define glVertexAttribL2dvEXT GLEW_GET_FUN(G_glewVertexAttribL2dvEXT)
#define glVertexAttribL3dEXT GLEW_GET_FUN(G_glewVertexAttribL3dEXT)
#define glVertexAttribL3dvEXT GLEW_GET_FUN(G_glewVertexAttribL3dvEXT)
#define glVertexAttribL4dEXT GLEW_GET_FUN(G_glewVertexAttribL4dEXT)
#define glVertexAttribL4dvEXT GLEW_GET_FUN(G_glewVertexAttribL4dvEXT)
#define glVertexAttribLPointerEXT GLEW_GET_FUN(G_glewVertexAttribLPointerEXT)

#define GLEW_EXT_vertex_attrib_64bit GLEW_GET_VAR(G_GLEW_EXT_vertex_attrib_64bit)

#endif /* GL_EXT_vertex_attrib_64bit */

/* -------------------------- GL_EXT_vertex_shader ------------------------- */

#ifndef GL_EXT_vertex_shader
#define GL_EXT_vertex_shader 1

#define GL_VERTEX_SHADER_EXT 0x8780
#define GL_VERTEX_SHADER_BINDING_EXT 0x8781
#define GL_OP_INDEX_EXT 0x8782
#define GL_OP_NEGATE_EXT 0x8783
#define GL_OP_DOT3_EXT 0x8784
#define GL_OP_DOT4_EXT 0x8785
#define GL_OP_MUL_EXT 0x8786
#define GL_OP_ADD_EXT 0x8787
#define GL_OP_MADD_EXT 0x8788
#define GL_OP_FRAC_EXT 0x8789
#define GL_OP_MAX_EXT 0x878A
#define GL_OP_MIN_EXT 0x878B
#define GL_OP_SET_GE_EXT 0x878C
#define GL_OP_SET_LT_EXT 0x878D
#define GL_OP_CLAMP_EXT 0x878E
#define GL_OP_FLOOR_EXT 0x878F
#define GL_OP_ROUND_EXT 0x8790
#define GL_OP_EXP_BASE_2_EXT 0x8791
#define GL_OP_LOG_BASE_2_EXT 0x8792
#define GL_OP_POWER_EXT 0x8793
#define GL_OP_RECIP_EXT 0x8794
#define GL_OP_RECIP_SQRT_EXT 0x8795
#define GL_OP_SUB_EXT 0x8796
#define GL_OP_CROSS_PRODUCT_EXT 0x8797
#define GL_OP_MULTIPLY_MATRIX_EXT 0x8798
#define GL_OP_MOV_EXT 0x8799
#define GL_OUTPUT_VERTEX_EXT 0x879A
#define GL_OUTPUT_COLOR0_EXT 0x879B
#define GL_OUTPUT_COLOR1_EXT 0x879C
#define GL_OUTPUT_TEXTURE_COORD0_EXT 0x879D
#define GL_OUTPUT_TEXTURE_COORD1_EXT 0x879E
#define GL_OUTPUT_TEXTURE_COORD2_EXT 0x879F
#define GL_OUTPUT_TEXTURE_COORD3_EXT 0x87A0
#define GL_OUTPUT_TEXTURE_COORD4_EXT 0x87A1
#define GL_OUTPUT_TEXTURE_COORD5_EXT 0x87A2
#define GL_OUTPUT_TEXTURE_COORD6_EXT 0x87A3
#define GL_OUTPUT_TEXTURE_COORD7_EXT 0x87A4
#define GL_OUTPUT_TEXTURE_COORD8_EXT 0x87A5
#define GL_OUTPUT_TEXTURE_COORD9_EXT 0x87A6
#define GL_OUTPUT_TEXTURE_COORD10_EXT 0x87A7
#define GL_OUTPUT_TEXTURE_COORD11_EXT 0x87A8
#define GL_OUTPUT_TEXTURE_COORD12_EXT 0x87A9
#define GL_OUTPUT_TEXTURE_COORD13_EXT 0x87AA
#define GL_OUTPUT_TEXTURE_COORD14_EXT 0x87AB
#define GL_OUTPUT_TEXTURE_COORD15_EXT 0x87AC
#define GL_OUTPUT_TEXTURE_COORD16_EXT 0x87AD
#define GL_OUTPUT_TEXTURE_COORD17_EXT 0x87AE
#define GL_OUTPUT_TEXTURE_COORD18_EXT 0x87AF
#define GL_OUTPUT_TEXTURE_COORD19_EXT 0x87B0
#define GL_OUTPUT_TEXTURE_COORD20_EXT 0x87B1
#define GL_OUTPUT_TEXTURE_COORD21_EXT 0x87B2
#define GL_OUTPUT_TEXTURE_COORD22_EXT 0x87B3
#define GL_OUTPUT_TEXTURE_COORD23_EXT 0x87B4
#define GL_OUTPUT_TEXTURE_COORD24_EXT 0x87B5
#define GL_OUTPUT_TEXTURE_COORD25_EXT 0x87B6
#define GL_OUTPUT_TEXTURE_COORD26_EXT 0x87B7
#define GL_OUTPUT_TEXTURE_COORD27_EXT 0x87B8
#define GL_OUTPUT_TEXTURE_COORD28_EXT 0x87B9
#define GL_OUTPUT_TEXTURE_COORD29_EXT 0x87BA
#define GL_OUTPUT_TEXTURE_COORD30_EXT 0x87BB
#define GL_OUTPUT_TEXTURE_COORD31_EXT 0x87BC
#define GL_OUTPUT_FOG_EXT 0x87BD
#define GL_SCALAR_EXT 0x87BE
#define GL_VECTOR_EXT 0x87BF
#define GL_MATRIX_EXT 0x87C0
#define GL_VARIANT_EXT 0x87C1
#define GL_INVARIANT_EXT 0x87C2
#define GL_LOCAL_CONSTANT_EXT 0x87C3
#define GL_LOCAL_EXT 0x87C4
#define GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87C5
#define GL_MAX_VERTEX_SHADER_VARIANTS_EXT 0x87C6
#define GL_MAX_VERTEX_SHADER_INVARIANTS_EXT 0x87C7
#define GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87C8
#define GL_MAX_VERTEX_SHADER_LOCALS_EXT 0x87C9
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87CA
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT 0x87CB
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT 0x87CC
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87CD
#define GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT 0x87CE
#define GL_VERTEX_SHADER_INSTRUCTIONS_EXT 0x87CF
#define GL_VERTEX_SHADER_VARIANTS_EXT 0x87D0
#define GL_VERTEX_SHADER_INVARIANTS_EXT 0x87D1
#define GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT 0x87D2
#define GL_VERTEX_SHADER_LOCALS_EXT 0x87D3
#define GL_VERTEX_SHADER_OPTIMIZED_EXT 0x87D4
#define GL_X_EXT 0x87D5
#define GL_Y_EXT 0x87D6
#define GL_Z_EXT 0x87D7
#define GL_W_EXT 0x87D8
#define GL_NEGATIVE_X_EXT 0x87D9
#define GL_NEGATIVE_Y_EXT 0x87DA
#define GL_NEGATIVE_Z_EXT 0x87DB
#define GL_NEGATIVE_W_EXT 0x87DC
#define GL_ZERO_EXT 0x87DD
#define GL_ONE_EXT 0x87DE
#define GL_NEGATIVE_ONE_EXT 0x87DF
#define GL_NORMALIZED_RANGE_EXT 0x87E0
#define GL_FULL_RANGE_EXT 0x87E1
#define GL_CURRENT_VERTEX_EXT 0x87E2
#define GL_MVP_MATRIX_EXT 0x87E3
#define GL_VARIANT_VALUE_EXT 0x87E4
#define GL_VARIANT_DATATYPE_EXT 0x87E5
#define GL_VARIANT_ARRAY_STRIDE_EXT 0x87E6
#define GL_VARIANT_ARRAY_TYPE_EXT 0x87E7
#define GL_VARIANT_ARRAY_EXT 0x87E8
#define GL_VARIANT_ARRAY_POINTER_EXT 0x87E9
#define GL_INVARIANT_VALUE_EXT 0x87EA
#define GL_INVARIANT_DATATYPE_EXT 0x87EB
#define GL_LOCAL_CONSTANT_VALUE_EXT 0x87EC
#define GL_LOCAL_CONSTANT_DATATYPE_EXT 0x87ED

typedef void (GLAPIENTRY * PFNGLBEGINVERTEXSHADEREXTPROC) (void);
typedef GLuint (GLAPIENTRY * PFNGLBINDLIGHTPARAMETEREXTPROC) (GLenum light, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDMATERIALPARAMETEREXTPROC) (GLenum face, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDPARAMETEREXTPROC) (GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDTEXGENPARAMETEREXTPROC) (GLenum unit, GLenum coord, GLenum value);
typedef GLuint (GLAPIENTRY * PFNGLBINDTEXTUREUNITPARAMETEREXTPROC) (GLenum unit, GLenum value);
typedef void (GLAPIENTRY * PFNGLBINDVERTEXSHADEREXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEVERTEXSHADEREXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLENABLEVARIANTCLIENTSTATEEXTPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLENDVERTEXSHADEREXTPROC) (void);
typedef void (GLAPIENTRY * PFNGLEXTRACTCOMPONENTEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLuint (GLAPIENTRY * PFNGLGENSYMBOLSEXTPROC) (GLenum dataType, GLenum storageType, GLenum range, GLuint components);
typedef GLuint (GLAPIENTRY * PFNGLGENVERTEXSHADERSEXTPROC) (GLuint range);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETINVARIANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETLOCALCONSTANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTBOOLEANVEXTPROC) (GLuint id, GLenum value, GLboolean *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTFLOATVEXTPROC) (GLuint id, GLenum value, GLfloat *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTINTEGERVEXTPROC) (GLuint id, GLenum value, GLint *data);
typedef void (GLAPIENTRY * PFNGLGETVARIANTPOINTERVEXTPROC) (GLuint id, GLenum value, GLvoid **data);
typedef void (GLAPIENTRY * PFNGLINSERTCOMPONENTEXTPROC) (GLuint res, GLuint src, GLuint num);
typedef GLboolean (GLAPIENTRY * PFNGLISVARIANTENABLEDEXTPROC) (GLuint id, GLenum cap);
typedef void (GLAPIENTRY * PFNGLSETINVARIANTEXTPROC) (GLuint id, GLenum type, GLvoid *addr);
typedef void (GLAPIENTRY * PFNGLSETLOCALCONSTANTEXTPROC) (GLuint id, GLenum type, GLvoid *addr);
typedef void (GLAPIENTRY * PFNGLSHADEROP1EXTPROC) (GLenum op, GLuint res, GLuint arg1);
typedef void (GLAPIENTRY * PFNGLSHADEROP2EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2);
typedef void (GLAPIENTRY * PFNGLSHADEROP3EXTPROC) (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3);
typedef void (GLAPIENTRY * PFNGLSWIZZLEEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);
typedef void (GLAPIENTRY * PFNGLVARIANTPOINTEREXTPROC) (GLuint id, GLenum type, GLuint stride, GLvoid *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTBVEXTPROC) (GLuint id, GLbyte *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTDVEXTPROC) (GLuint id, GLdouble *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTFVEXTPROC) (GLuint id, GLfloat *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTIVEXTPROC) (GLuint id, GLint *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTSVEXTPROC) (GLuint id, GLshort *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUBVEXTPROC) (GLuint id, GLubyte *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUIVEXTPROC) (GLuint id, GLuint *addr);
typedef void (GLAPIENTRY * PFNGLVARIANTUSVEXTPROC) (GLuint id, GLushort *addr);
typedef void (GLAPIENTRY * PFNGLWRITEMASKEXTPROC) (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW);

#define glBeginVertexShaderEXT GLEW_GET_FUN(G_glewBeginVertexShaderEXT)
#define glBindLightParameterEXT GLEW_GET_FUN(G_glewBindLightParameterEXT)
#define glBindMaterialParameterEXT GLEW_GET_FUN(G_glewBindMaterialParameterEXT)
#define glBindParameterEXT GLEW_GET_FUN(G_glewBindParameterEXT)
#define glBindTexGenParameterEXT GLEW_GET_FUN(G_glewBindTexGenParameterEXT)
#define glBindTextureUnitParameterEXT GLEW_GET_FUN(G_glewBindTextureUnitParameterEXT)
#define glBindVertexShaderEXT GLEW_GET_FUN(G_glewBindVertexShaderEXT)
#define glDeleteVertexShaderEXT GLEW_GET_FUN(G_glewDeleteVertexShaderEXT)
#define glDisableVariantClientStateEXT GLEW_GET_FUN(G_glewDisableVariantClientStateEXT)
#define glEnableVariantClientStateEXT GLEW_GET_FUN(G_glewEnableVariantClientStateEXT)
#define glEndVertexShaderEXT GLEW_GET_FUN(G_glewEndVertexShaderEXT)
#define glExtractComponentEXT GLEW_GET_FUN(G_glewExtractComponentEXT)
#define glGenSymbolsEXT GLEW_GET_FUN(G_glewGenSymbolsEXT)
#define glGenVertexShadersEXT GLEW_GET_FUN(G_glewGenVertexShadersEXT)
#define glGetInvariantBooleanvEXT GLEW_GET_FUN(G_glewGetInvariantBooleanvEXT)
#define glGetInvariantFloatvEXT GLEW_GET_FUN(G_glewGetInvariantFloatvEXT)
#define glGetInvariantIntegervEXT GLEW_GET_FUN(G_glewGetInvariantIntegervEXT)
#define glGetLocalConstantBooleanvEXT GLEW_GET_FUN(G_glewGetLocalConstantBooleanvEXT)
#define glGetLocalConstantFloatvEXT GLEW_GET_FUN(G_glewGetLocalConstantFloatvEXT)
#define glGetLocalConstantIntegervEXT GLEW_GET_FUN(G_glewGetLocalConstantIntegervEXT)
#define glGetVariantBooleanvEXT GLEW_GET_FUN(G_glewGetVariantBooleanvEXT)
#define glGetVariantFloatvEXT GLEW_GET_FUN(G_glewGetVariantFloatvEXT)
#define glGetVariantIntegervEXT GLEW_GET_FUN(G_glewGetVariantIntegervEXT)
#define glGetVariantPointervEXT GLEW_GET_FUN(G_glewGetVariantPointervEXT)
#define glInsertComponentEXT GLEW_GET_FUN(G_glewInsertComponentEXT)
#define glIsVariantEnabledEXT GLEW_GET_FUN(G_glewIsVariantEnabledEXT)
#define glSetInvariantEXT GLEW_GET_FUN(G_glewSetInvariantEXT)
#define glSetLocalConstantEXT GLEW_GET_FUN(G_glewSetLocalConstantEXT)
#define glShaderOp1EXT GLEW_GET_FUN(G_glewShaderOp1EXT)
#define glShaderOp2EXT GLEW_GET_FUN(G_glewShaderOp2EXT)
#define glShaderOp3EXT GLEW_GET_FUN(G_glewShaderOp3EXT)
#define glSwizzleEXT GLEW_GET_FUN(G_glewSwizzleEXT)
#define glVariantPointerEXT GLEW_GET_FUN(G_glewVariantPointerEXT)
#define glVariantbvEXT GLEW_GET_FUN(G_glewVariantbvEXT)
#define glVariantdvEXT GLEW_GET_FUN(G_glewVariantdvEXT)
#define glVariantfvEXT GLEW_GET_FUN(G_glewVariantfvEXT)
#define glVariantivEXT GLEW_GET_FUN(G_glewVariantivEXT)
#define glVariantsvEXT GLEW_GET_FUN(G_glewVariantsvEXT)
#define glVariantubvEXT GLEW_GET_FUN(G_glewVariantubvEXT)
#define glVariantuivEXT GLEW_GET_FUN(G_glewVariantuivEXT)
#define glVariantusvEXT GLEW_GET_FUN(G_glewVariantusvEXT)
#define glWriteMaskEXT GLEW_GET_FUN(G_glewWriteMaskEXT)

#define GLEW_EXT_vertex_shader GLEW_GET_VAR(G_GLEW_EXT_vertex_shader)

#endif /* GL_EXT_vertex_shader */

/* ------------------------ GL_EXT_vertex_weighting ------------------------ */

#ifndef GL_EXT_vertex_weighting
#define GL_EXT_vertex_weighting 1

#define GL_MODELVIEW0_STACK_DEPTH_EXT 0x0BA3
#define GL_MODELVIEW0_MATRIX_EXT 0x0BA6
#define GL_MODELVIEW0_EXT 0x1700
#define GL_MODELVIEW1_STACK_DEPTH_EXT 0x8502
#define GL_MODELVIEW1_MATRIX_EXT 0x8506
#define GL_VERTEX_WEIGHTING_EXT 0x8509
#define GL_MODELVIEW1_EXT 0x850A
#define GL_CURRENT_VERTEX_WEIGHT_EXT 0x850B
#define GL_VERTEX_WEIGHT_ARRAY_EXT 0x850C
#define GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT 0x850D
#define GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT 0x850E
#define GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT 0x850F
#define GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT 0x8510

typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTPOINTEREXTPROC) (GLint size, GLenum type, GLsizei stride, void* pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTFEXTPROC) (GLfloat weight);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTFVEXTPROC) (GLfloat* weight);

#define glVertexWeightPointerEXT GLEW_GET_FUN(G_glewVertexWeightPointerEXT)
#define glVertexWeightfEXT GLEW_GET_FUN(G_glewVertexWeightfEXT)
#define glVertexWeightfvEXT GLEW_GET_FUN(G_glewVertexWeightfvEXT)

#define GLEW_EXT_vertex_weighting GLEW_GET_VAR(G_GLEW_EXT_vertex_weighting)

#endif /* GL_EXT_vertex_weighting */

/* ------------------------- GL_EXT_x11_sync_object ------------------------ */

#ifndef GL_EXT_x11_sync_object
#define GL_EXT_x11_sync_object 1

#define GL_SYNC_X11_FENCE_EXT 0x90E1

typedef GLsync (GLAPIENTRY * PFNGLIMPORTSYNCEXTPROC) (GLenum external_sync_type, GLintptr external_sync, GLbitfield flags);

#define glImportSyncEXT GLEW_GET_FUN(G_glewImportSyncEXT)

#define GLEW_EXT_x11_sync_object GLEW_GET_VAR(G_GLEW_EXT_x11_sync_object)

#endif /* GL_EXT_x11_sync_object */

/* ---------------------- GL_GREMEDY_frame_terminator ---------------------- */

#ifndef GL_GREMEDY_frame_terminator
#define GL_GREMEDY_frame_terminator 1

typedef void (GLAPIENTRY * PFNGLFRAMETERMINATORGREMEDYPROC) (void);

#define glFrameTerminatorGREMEDY GLEW_GET_FUN(G_glewFrameTerminatorGREMEDY)

#define GLEW_GREMEDY_frame_terminator GLEW_GET_VAR(G_GLEW_GREMEDY_frame_terminator)

#endif /* GL_GREMEDY_frame_terminator */

/* ------------------------ GL_GREMEDY_string_marker ----------------------- */

#ifndef GL_GREMEDY_string_marker
#define GL_GREMEDY_string_marker 1

typedef void (GLAPIENTRY * PFNGLSTRINGMARKERGREMEDYPROC) (GLsizei len, const void* string);

#define glStringMarkerGREMEDY GLEW_GET_FUN(G_glewStringMarkerGREMEDY)

#define GLEW_GREMEDY_string_marker GLEW_GET_VAR(G_GLEW_GREMEDY_string_marker)

#endif /* GL_GREMEDY_string_marker */

/* --------------------- GL_HP_convolution_border_modes -------------------- */

#ifndef GL_HP_convolution_border_modes
#define GL_HP_convolution_border_modes 1

#define GLEW_HP_convolution_border_modes GLEW_GET_VAR(G_GLEW_HP_convolution_border_modes)

#endif /* GL_HP_convolution_border_modes */

/* ------------------------- GL_HP_image_transform ------------------------- */

#ifndef GL_HP_image_transform
#define GL_HP_image_transform 1

typedef void (GLAPIENTRY * PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLIMAGETRANSFORMPARAMETERFHPPROC) (GLenum target, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLIMAGETRANSFORMPARAMETERFVHPPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLIMAGETRANSFORMPARAMETERIHPPROC) (GLenum target, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLIMAGETRANSFORMPARAMETERIVHPPROC) (GLenum target, GLenum pname, const GLint* params);

#define glGetImageTransformParameterfvHP GLEW_GET_FUN(G_glewGetImageTransformParameterfvHP)
#define glGetImageTransformParameterivHP GLEW_GET_FUN(G_glewGetImageTransformParameterivHP)
#define glImageTransformParameterfHP GLEW_GET_FUN(G_glewImageTransformParameterfHP)
#define glImageTransformParameterfvHP GLEW_GET_FUN(G_glewImageTransformParameterfvHP)
#define glImageTransformParameteriHP GLEW_GET_FUN(G_glewImageTransformParameteriHP)
#define glImageTransformParameterivHP GLEW_GET_FUN(G_glewImageTransformParameterivHP)

#define GLEW_HP_image_transform GLEW_GET_VAR(G_GLEW_HP_image_transform)

#endif /* GL_HP_image_transform */

/* -------------------------- GL_HP_occlusion_test ------------------------- */

#ifndef GL_HP_occlusion_test
#define GL_HP_occlusion_test 1

#define GL_OCCLUSION_TEST_HP 0x8165
#define GL_OCCLUSION_TEST_RESULT_HP 0x8166

#define GLEW_HP_occlusion_test GLEW_GET_VAR(G_GLEW_HP_occlusion_test)

#endif /* GL_HP_occlusion_test */

/* ------------------------- GL_HP_texture_lighting ------------------------ */

#ifndef GL_HP_texture_lighting
#define GL_HP_texture_lighting 1

#define GLEW_HP_texture_lighting GLEW_GET_VAR(G_GLEW_HP_texture_lighting)

#endif /* GL_HP_texture_lighting */

/* --------------------------- GL_IBM_cull_vertex -------------------------- */

#ifndef GL_IBM_cull_vertex
#define GL_IBM_cull_vertex 1

#define GL_CULL_VERTEX_IBM 103050

#define GLEW_IBM_cull_vertex GLEW_GET_VAR(G_GLEW_IBM_cull_vertex)

#endif /* GL_IBM_cull_vertex */

/* ---------------------- GL_IBM_multimode_draw_arrays --------------------- */

#ifndef GL_IBM_multimode_draw_arrays
#define GL_IBM_multimode_draw_arrays 1

typedef void (GLAPIENTRY * PFNGLMULTIMODEDRAWARRAYSIBMPROC) (const GLenum* mode, const GLint *first, const GLsizei *count, GLsizei primcount, GLint modestride);
typedef void (GLAPIENTRY * PFNGLMULTIMODEDRAWELEMENTSIBMPROC) (const GLenum* mode, const GLsizei *count, GLenum type, const GLvoid * const *indices, GLsizei primcount, GLint modestride);

#define glMultiModeDrawArraysIBM GLEW_GET_FUN(G_glewMultiModeDrawArraysIBM)
#define glMultiModeDrawElementsIBM GLEW_GET_FUN(G_glewMultiModeDrawElementsIBM)

#define GLEW_IBM_multimode_draw_arrays GLEW_GET_VAR(G_GLEW_IBM_multimode_draw_arrays)

#endif /* GL_IBM_multimode_draw_arrays */

/* ------------------------- GL_IBM_rasterpos_clip ------------------------- */

#ifndef GL_IBM_rasterpos_clip
#define GL_IBM_rasterpos_clip 1

#define GL_RASTER_POSITION_UNCLIPPED_IBM 103010

#define GLEW_IBM_rasterpos_clip GLEW_GET_VAR(G_GLEW_IBM_rasterpos_clip)

#endif /* GL_IBM_rasterpos_clip */

/* --------------------------- GL_IBM_static_data -------------------------- */

#ifndef GL_IBM_static_data
#define GL_IBM_static_data 1

#define GL_ALL_STATIC_DATA_IBM 103060
#define GL_STATIC_VERTEX_ARRAY_IBM 103061

#define GLEW_IBM_static_data GLEW_GET_VAR(G_GLEW_IBM_static_data)

#endif /* GL_IBM_static_data */

/* --------------------- GL_IBM_texture_mirrored_repeat -------------------- */

#ifndef GL_IBM_texture_mirrored_repeat
#define GL_IBM_texture_mirrored_repeat 1

#define GL_MIRRORED_REPEAT_IBM 0x8370

#define GLEW_IBM_texture_mirrored_repeat GLEW_GET_VAR(G_GLEW_IBM_texture_mirrored_repeat)

#endif /* GL_IBM_texture_mirrored_repeat */

/* ----------------------- GL_IBM_vertex_array_lists ----------------------- */

#ifndef GL_IBM_vertex_array_lists
#define GL_IBM_vertex_array_lists 1

#define GL_VERTEX_ARRAY_LIST_IBM 103070
#define GL_NORMAL_ARRAY_LIST_IBM 103071
#define GL_COLOR_ARRAY_LIST_IBM 103072
#define GL_INDEX_ARRAY_LIST_IBM 103073
#define GL_TEXTURE_COORD_ARRAY_LIST_IBM 103074
#define GL_EDGE_FLAG_ARRAY_LIST_IBM 103075
#define GL_FOG_COORDINATE_ARRAY_LIST_IBM 103076
#define GL_SECONDARY_COLOR_ARRAY_LIST_IBM 103077
#define GL_VERTEX_ARRAY_LIST_STRIDE_IBM 103080
#define GL_NORMAL_ARRAY_LIST_STRIDE_IBM 103081
#define GL_COLOR_ARRAY_LIST_STRIDE_IBM 103082
#define GL_INDEX_ARRAY_LIST_STRIDE_IBM 103083
#define GL_TEXTURE_COORD_ARRAY_LIST_STRIDE_IBM 103084
#define GL_EDGE_FLAG_ARRAY_LIST_STRIDE_IBM 103085
#define GL_FOG_COORDINATE_ARRAY_LIST_STRIDE_IBM 103086
#define GL_SECONDARY_COLOR_ARRAY_LIST_STRIDE_IBM 103087

typedef void (GLAPIENTRY * PFNGLCOLORPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGPOINTERLISTIBMPROC) (GLint stride, const GLboolean ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLFOGCOORDPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLINDEXPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTERLISTIBMPROC) (GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTERLISTIBMPROC) (GLint size, GLenum type, GLint stride, const GLvoid ** pointer, GLint ptrstride);

#define glColorPointerListIBM GLEW_GET_FUN(G_glewColorPointerListIBM)
#define glEdgeFlagPointerListIBM GLEW_GET_FUN(G_glewEdgeFlagPointerListIBM)
#define glFogCoordPointerListIBM GLEW_GET_FUN(G_glewFogCoordPointerListIBM)
#define glIndexPointerListIBM GLEW_GET_FUN(G_glewIndexPointerListIBM)
#define glNormalPointerListIBM GLEW_GET_FUN(G_glewNormalPointerListIBM)
#define glSecondaryColorPointerListIBM GLEW_GET_FUN(G_glewSecondaryColorPointerListIBM)
#define glTexCoordPointerListIBM GLEW_GET_FUN(G_glewTexCoordPointerListIBM)
#define glVertexPointerListIBM GLEW_GET_FUN(G_glewVertexPointerListIBM)

#define GLEW_IBM_vertex_array_lists GLEW_GET_VAR(G_GLEW_IBM_vertex_array_lists)

#endif /* GL_IBM_vertex_array_lists */

/* -------------------------- GL_INGR_color_clamp -------------------------- */

#ifndef GL_INGR_color_clamp
#define GL_INGR_color_clamp 1

#define GL_RED_MIN_CLAMP_INGR 0x8560
#define GL_GREEN_MIN_CLAMP_INGR 0x8561
#define GL_BLUE_MIN_CLAMP_INGR 0x8562
#define GL_ALPHA_MIN_CLAMP_INGR 0x8563
#define GL_RED_MAX_CLAMP_INGR 0x8564
#define GL_GREEN_MAX_CLAMP_INGR 0x8565
#define GL_BLUE_MAX_CLAMP_INGR 0x8566
#define GL_ALPHA_MAX_CLAMP_INGR 0x8567

#define GLEW_INGR_color_clamp GLEW_GET_VAR(G_GLEW_INGR_color_clamp)

#endif /* GL_INGR_color_clamp */

/* ------------------------- GL_INGR_interlace_read ------------------------ */

#ifndef GL_INGR_interlace_read
#define GL_INGR_interlace_read 1

#define GL_INTERLACE_READ_INGR 0x8568

#define GLEW_INGR_interlace_read GLEW_GET_VAR(G_GLEW_INGR_interlace_read)

#endif /* GL_INGR_interlace_read */

/* ------------------------ GL_INTEL_parallel_arrays ----------------------- */

#ifndef GL_INTEL_parallel_arrays
#define GL_INTEL_parallel_arrays 1

#define GL_PARALLEL_ARRAYS_INTEL 0x83F4
#define GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL 0x83F5
#define GL_NORMAL_ARRAY_PARALLEL_POINTERS_INTEL 0x83F6
#define GL_COLOR_ARRAY_PARALLEL_POINTERS_INTEL 0x83F7
#define GL_TEXTURE_COORD_ARRAY_PARALLEL_POINTERS_INTEL 0x83F8

typedef void (GLAPIENTRY * PFNGLCOLORPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);
typedef void (GLAPIENTRY * PFNGLNORMALPOINTERVINTELPROC) (GLenum type, const void** pointer);
typedef void (GLAPIENTRY * PFNGLTEXCOORDPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXPOINTERVINTELPROC) (GLint size, GLenum type, const void** pointer);

#define glColorPointervINTEL GLEW_GET_FUN(G_glewColorPointervINTEL)
#define glNormalPointervINTEL GLEW_GET_FUN(G_glewNormalPointervINTEL)
#define glTexCoordPointervINTEL GLEW_GET_FUN(G_glewTexCoordPointervINTEL)
#define glVertexPointervINTEL GLEW_GET_FUN(G_glewVertexPointervINTEL)

#define GLEW_INTEL_parallel_arrays GLEW_GET_VAR(G_GLEW_INTEL_parallel_arrays)

#endif /* GL_INTEL_parallel_arrays */

/* ------------------------ GL_INTEL_texture_scissor ----------------------- */

#ifndef GL_INTEL_texture_scissor
#define GL_INTEL_texture_scissor 1

typedef void (GLAPIENTRY * PFNGLTEXSCISSORFUNCINTELPROC) (GLenum target, GLenum lfunc, GLenum hfunc);
typedef void (GLAPIENTRY * PFNGLTEXSCISSORINTELPROC) (GLenum target, GLclampf tlow, GLclampf thigh);

#define glTexScissorFuncINTEL GLEW_GET_FUN(G_glewTexScissorFuncINTEL)
#define glTexScissorINTEL GLEW_GET_FUN(G_glewTexScissorINTEL)

#define GLEW_INTEL_texture_scissor GLEW_GET_VAR(G_GLEW_INTEL_texture_scissor)

#endif /* GL_INTEL_texture_scissor */

/* ------------------------------ GL_KHR_debug ----------------------------- */

#ifndef GL_KHR_debug
#define GL_KHR_debug 1

#define GL_CONTEXT_FLAG_DEBUG_BIT 0x00000002
#define GL_STACK_OVERFLOW 0x0503
#define GL_STACK_UNDERFLOW 0x0504
#define GL_DEBUG_OUTPUT_SYNCHRONOUS 0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH 0x8243
#define GL_DEBUG_CALLBACK_FUNCTION 0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM 0x8245
#define GL_DEBUG_SOURCE_API 0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER 0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY 0x8249
#define GL_DEBUG_SOURCE_APPLICATION 0x824A
#define GL_DEBUG_SOURCE_OTHER 0x824B
#define GL_DEBUG_TYPE_ERROR 0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR 0x824E
#define GL_DEBUG_TYPE_PORTABILITY 0x824F
#define GL_DEBUG_TYPE_PERFORMANCE 0x8250
#define GL_DEBUG_TYPE_OTHER 0x8251
#define GL_DEBUG_TYPE_MARKER 0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP 0x8269
#define GL_DEBUG_TYPE_POP_GROUP 0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION 0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH 0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH 0x826D
#define GL_BUFFER 0x82E0
#define GL_SHADER 0x82E1
#define GL_PROGRAM 0x82E2
#define GL_QUERY 0x82E3
#define GL_PROGRAM_PIPELINE 0x82E4
#define GL_SAMPLER 0x82E6
#define GL_DISPLAY_LIST 0x82E7
#define GL_MAX_LABEL_LENGTH 0x82E8
#define GL_MAX_DEBUG_MESSAGE_LENGTH 0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES 0x9144
#define GL_DEBUG_LOGGED_MESSAGES 0x9145
#define GL_DEBUG_SEVERITY_HIGH 0x9146
#define GL_DEBUG_SEVERITY_MEDIUM 0x9147
#define GL_DEBUG_SEVERITY_LOW 0x9148
#define GL_DEBUG_OUTPUT 0x92E0

typedef void (APIENTRY *GLDEBUGPROC)(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);

typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, void* userParam);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGECONTROLPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint* ids, GLboolean enabled);
typedef void (GLAPIENTRY * PFNGLDEBUGMESSAGEINSERTPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* buf);
typedef GLuint (GLAPIENTRY * PFNGLGETDEBUGMESSAGELOGPROC) (GLuint count, GLsizei bufsize, GLenum* sources, GLenum* types, GLuint* ids, GLenum* severities, GLsizei* lengths, GLchar* messageLog);
typedef void (GLAPIENTRY * PFNGLGETOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei* length, GLchar *label);
typedef void (GLAPIENTRY * PFNGLGETOBJECTPTRLABELPROC) (void* ptr, GLsizei bufSize, GLsizei* length, GLchar *label);
typedef void (GLAPIENTRY * PFNGLGETPOINTERVPROC) (GLenum pname, void** params);
typedef void (GLAPIENTRY * PFNGLOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei length, const GLchar* label);
typedef void (GLAPIENTRY * PFNGLOBJECTPTRLABELPROC) (void* ptr, GLsizei length, const GLchar* label);
typedef void (GLAPIENTRY * PFNGLPUSHDEBUGGROUPPROC) (GLenum source, GLuint id, GLsizei length, const GLchar * message);

#define glDebugMessageCallback GLEW_GET_FUN(G_glewDebugMessageCallback)
#define glDebugMessageControl GLEW_GET_FUN(G_glewDebugMessageControl)
#define glDebugMessageInsert GLEW_GET_FUN(G_glewDebugMessageInsert)
#define glGetDebugMessageLog GLEW_GET_FUN(G_glewGetDebugMessageLog)
#define glGetObjectLabel GLEW_GET_FUN(G_glewGetObjectLabel)
#define glGetObjectPtrLabel GLEW_GET_FUN(G_glewGetObjectPtrLabel)
#define glGetPointerv GLEW_GET_FUN(G_glewGetPointerv)
#define glObjectLabel GLEW_GET_FUN(G_glewObjectLabel)
#define glObjectPtrLabel GLEW_GET_FUN(G_glewObjectPtrLabel)
#define glPushDebugGroup GLEW_GET_FUN(G_glewPushDebugGroup)

#define GLEW_KHR_debug GLEW_GET_VAR(G_GLEW_KHR_debug)

#endif /* GL_KHR_debug */

/* ------------------ GL_KHR_texture_compression_astc_ldr ------------------ */

#ifndef GL_KHR_texture_compression_astc_ldr
#define GL_KHR_texture_compression_astc_ldr 1

#define GL_COMPRESSED_RGBA_ASTC_4x4_KHR 0x93B0
#define GL_COMPRESSED_RGBA_ASTC_5x4_KHR 0x93B1
#define GL_COMPRESSED_RGBA_ASTC_5x5_KHR 0x93B2
#define GL_COMPRESSED_RGBA_ASTC_6x5_KHR 0x93B3
#define GL_COMPRESSED_RGBA_ASTC_6x6_KHR 0x93B4
#define GL_COMPRESSED_RGBA_ASTC_8x5_KHR 0x93B5
#define GL_COMPRESSED_RGBA_ASTC_8x6_KHR 0x93B6
#define GL_COMPRESSED_RGBA_ASTC_8x8_KHR 0x93B7
#define GL_COMPRESSED_RGBA_ASTC_10x5_KHR 0x93B8
#define GL_COMPRESSED_RGBA_ASTC_10x6_KHR 0x93B9
#define GL_COMPRESSED_RGBA_ASTC_10x8_KHR 0x93BA
#define GL_COMPRESSED_RGBA_ASTC_10x10_KHR 0x93BB
#define GL_COMPRESSED_RGBA_ASTC_12x10_KHR 0x93BC
#define GL_COMPRESSED_RGBA_ASTC_12x12_KHR 0x93BD
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR 0x93D0
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR 0x93D1
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR 0x93D2
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR 0x93D3
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR 0x93D4
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR 0x93D5
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR 0x93D6
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR 0x93D7
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR 0x93D8
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR 0x93D9
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR 0x93DA
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR 0x93DB
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR 0x93DC
#define GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR 0x93DD

#define GLEW_KHR_texture_compression_astc_ldr GLEW_GET_VAR(G_GLEW_KHR_texture_compression_astc_ldr)

#endif /* GL_KHR_texture_compression_astc_ldr */

/* -------------------------- GL_KTX_buffer_region ------------------------- */

#ifndef GL_KTX_buffer_region
#define GL_KTX_buffer_region 1

#define GL_KTX_FRONT_REGION 0x0
#define GL_KTX_BACK_REGION 0x1
#define GL_KTX_Z_REGION 0x2
#define GL_KTX_STENCIL_REGION 0x3

typedef GLuint (GLAPIENTRY * PFNGLBUFFERREGIONENABLEDPROC) (void);
typedef void (GLAPIENTRY * PFNGLDELETEBUFFERREGIONPROC) (GLenum region);
typedef void (GLAPIENTRY * PFNGLDRAWBUFFERREGIONPROC) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
typedef GLuint (GLAPIENTRY * PFNGLNEWBUFFERREGIONPROC) (GLenum region);
typedef void (GLAPIENTRY * PFNGLREADBUFFERREGIONPROC) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);

#define glBufferRegionEnabled GLEW_GET_FUN(G_glewBufferRegionEnabled)
#define glDeleteBufferRegion GLEW_GET_FUN(G_glewDeleteBufferRegion)
#define glDrawBufferRegion GLEW_GET_FUN(G_glewDrawBufferRegion)
#define glNewBufferRegion GLEW_GET_FUN(G_glewNewBufferRegion)
#define glReadBufferRegion GLEW_GET_FUN(G_glewReadBufferRegion)

#define GLEW_KTX_buffer_region GLEW_GET_VAR(G_GLEW_KTX_buffer_region)

#endif /* GL_KTX_buffer_region */

/* ------------------------- GL_MESAX_texture_stack ------------------------ */

#ifndef GL_MESAX_texture_stack
#define GL_MESAX_texture_stack 1

#define GL_TEXTURE_1D_STACK_MESAX 0x8759
#define GL_TEXTURE_2D_STACK_MESAX 0x875A
#define GL_PROXY_TEXTURE_1D_STACK_MESAX 0x875B
#define GL_PROXY_TEXTURE_2D_STACK_MESAX 0x875C
#define GL_TEXTURE_1D_STACK_BINDING_MESAX 0x875D
#define GL_TEXTURE_2D_STACK_BINDING_MESAX 0x875E

#define GLEW_MESAX_texture_stack GLEW_GET_VAR(G_GLEW_MESAX_texture_stack)

#endif /* GL_MESAX_texture_stack */

/* -------------------------- GL_MESA_pack_invert -------------------------- */

#ifndef GL_MESA_pack_invert
#define GL_MESA_pack_invert 1

#define GL_PACK_INVERT_MESA 0x8758

#define GLEW_MESA_pack_invert GLEW_GET_VAR(G_GLEW_MESA_pack_invert)

#endif /* GL_MESA_pack_invert */

/* ------------------------- GL_MESA_resize_buffers ------------------------ */

#ifndef GL_MESA_resize_buffers
#define GL_MESA_resize_buffers 1

typedef void (GLAPIENTRY * PFNGLRESIZEBUFFERSMESAPROC) (void);

#define glResizeBuffersMESA GLEW_GET_FUN(G_glewResizeBuffersMESA)

#define GLEW_MESA_resize_buffers GLEW_GET_VAR(G_GLEW_MESA_resize_buffers)

#endif /* GL_MESA_resize_buffers */

/* --------------------------- GL_MESA_window_pos -------------------------- */

#ifndef GL_MESA_window_pos
#define GL_MESA_window_pos 1

typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DMESAPROC) (GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2DVMESAPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FMESAPROC) (GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2FVMESAPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IMESAPROC) (GLint x, GLint y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2IVMESAPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SMESAPROC) (GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS2SVMESAPROC) (const GLshort* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DMESAPROC) (GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3DVMESAPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FMESAPROC) (GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3FVMESAPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IMESAPROC) (GLint x, GLint y, GLint z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3IVMESAPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SMESAPROC) (GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS3SVMESAPROC) (const GLshort* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4DMESAPROC) (GLdouble x, GLdouble y, GLdouble z, GLdouble);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4DVMESAPROC) (const GLdouble* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4FMESAPROC) (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4FVMESAPROC) (const GLfloat* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4IMESAPROC) (GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4IVMESAPROC) (const GLint* p);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4SMESAPROC) (GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLWINDOWPOS4SVMESAPROC) (const GLshort* p);

#define glWindowPos2dMESA GLEW_GET_FUN(G_glewWindowPos2dMESA)
#define glWindowPos2dvMESA GLEW_GET_FUN(G_glewWindowPos2dvMESA)
#define glWindowPos2fMESA GLEW_GET_FUN(G_glewWindowPos2fMESA)
#define glWindowPos2fvMESA GLEW_GET_FUN(G_glewWindowPos2fvMESA)
#define glWindowPos2iMESA GLEW_GET_FUN(G_glewWindowPos2iMESA)
#define glWindowPos2ivMESA GLEW_GET_FUN(G_glewWindowPos2ivMESA)
#define glWindowPos2sMESA GLEW_GET_FUN(G_glewWindowPos2sMESA)
#define glWindowPos2svMESA GLEW_GET_FUN(G_glewWindowPos2svMESA)
#define glWindowPos3dMESA GLEW_GET_FUN(G_glewWindowPos3dMESA)
#define glWindowPos3dvMESA GLEW_GET_FUN(G_glewWindowPos3dvMESA)
#define glWindowPos3fMESA GLEW_GET_FUN(G_glewWindowPos3fMESA)
#define glWindowPos3fvMESA GLEW_GET_FUN(G_glewWindowPos3fvMESA)
#define glWindowPos3iMESA GLEW_GET_FUN(G_glewWindowPos3iMESA)
#define glWindowPos3ivMESA GLEW_GET_FUN(G_glewWindowPos3ivMESA)
#define glWindowPos3sMESA GLEW_GET_FUN(G_glewWindowPos3sMESA)
#define glWindowPos3svMESA GLEW_GET_FUN(G_glewWindowPos3svMESA)
#define glWindowPos4dMESA GLEW_GET_FUN(G_glewWindowPos4dMESA)
#define glWindowPos4dvMESA GLEW_GET_FUN(G_glewWindowPos4dvMESA)
#define glWindowPos4fMESA GLEW_GET_FUN(G_glewWindowPos4fMESA)
#define glWindowPos4fvMESA GLEW_GET_FUN(G_glewWindowPos4fvMESA)
#define glWindowPos4iMESA GLEW_GET_FUN(G_glewWindowPos4iMESA)
#define glWindowPos4ivMESA GLEW_GET_FUN(G_glewWindowPos4ivMESA)
#define glWindowPos4sMESA GLEW_GET_FUN(G_glewWindowPos4sMESA)
#define glWindowPos4svMESA GLEW_GET_FUN(G_glewWindowPos4svMESA)

#define GLEW_MESA_window_pos GLEW_GET_VAR(G_GLEW_MESA_window_pos)

#endif /* GL_MESA_window_pos */

/* ------------------------- GL_MESA_ycbcr_texture ------------------------- */

#ifndef GL_MESA_ycbcr_texture
#define GL_MESA_ycbcr_texture 1

#define GL_UNSIGNED_SHORT_8_8_MESA 0x85BA
#define GL_UNSIGNED_SHORT_8_8_REV_MESA 0x85BB
#define GL_YCBCR_MESA 0x8757

#define GLEW_MESA_ycbcr_texture GLEW_GET_VAR(G_GLEW_MESA_ycbcr_texture)

#endif /* GL_MESA_ycbcr_texture */

/* ------------------------- GL_NVX_gpu_memory_info ------------------------ */

#ifndef GL_NVX_gpu_memory_info
#define GL_NVX_gpu_memory_info 1

#define GL_GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX 0x9047
#define GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX 0x9048
#define GL_GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX 0x9049
#define GL_GPU_MEMORY_INFO_EVICTION_COUNT_NVX 0x904A
#define GL_GPU_MEMORY_INFO_EVICTED_MEMORY_NVX 0x904B

#define GLEW_NVX_gpu_memory_info GLEW_GET_VAR(G_GLEW_NVX_gpu_memory_info)

#endif /* GL_NVX_gpu_memory_info */

/* ------------------------- GL_NV_bindless_texture ------------------------ */

#ifndef GL_NV_bindless_texture
#define GL_NV_bindless_texture 1

typedef GLuint64 (GLAPIENTRY * PFNGLGETIMAGEHANDLENVPROC) (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format);
typedef GLuint64 (GLAPIENTRY * PFNGLGETTEXTUREHANDLENVPROC) (GLuint texture);
typedef GLuint64 (GLAPIENTRY * PFNGLGETTEXTURESAMPLERHANDLENVPROC) (GLuint texture, GLuint sampler);
typedef GLboolean (GLAPIENTRY * PFNGLISIMAGEHANDLERESIDENTNVPROC) (GLuint64 handle);
typedef GLboolean (GLAPIENTRY * PFNGLISTEXTUREHANDLERESIDENTNVPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKEIMAGEHANDLERESIDENTNVPROC) (GLuint64 handle, GLenum access);
typedef void (GLAPIENTRY * PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLMAKETEXTUREHANDLERESIDENTNVPROC) (GLuint64 handle);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC) (GLuint program, GLint location, GLuint64 value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64* values);
typedef void (GLAPIENTRY * PFNGLUNIFORMHANDLEUI64NVPROC) (GLint location, GLuint64 value);
typedef void (GLAPIENTRY * PFNGLUNIFORMHANDLEUI64VNVPROC) (GLint location, GLsizei count, const GLuint64* value);

#define glGetImageHandleNV GLEW_GET_FUN(G_glewGetImageHandleNV)
#define glGetTextureHandleNV GLEW_GET_FUN(G_glewGetTextureHandleNV)
#define glGetTextureSamplerHandleNV GLEW_GET_FUN(G_glewGetTextureSamplerHandleNV)
#define glIsImageHandleResidentNV GLEW_GET_FUN(G_glewIsImageHandleResidentNV)
#define glIsTextureHandleResidentNV GLEW_GET_FUN(G_glewIsTextureHandleResidentNV)
#define glMakeImageHandleNonResidentNV GLEW_GET_FUN(G_glewMakeImageHandleNonResidentNV)
#define glMakeImageHandleResidentNV GLEW_GET_FUN(G_glewMakeImageHandleResidentNV)
#define glMakeTextureHandleNonResidentNV GLEW_GET_FUN(G_glewMakeTextureHandleNonResidentNV)
#define glMakeTextureHandleResidentNV GLEW_GET_FUN(G_glewMakeTextureHandleResidentNV)
#define glProgramUniformHandleui64NV GLEW_GET_FUN(G_glewProgramUniformHandleui64NV)
#define glProgramUniformHandleui64vNV GLEW_GET_FUN(G_glewProgramUniformHandleui64vNV)
#define glUniformHandleui64NV GLEW_GET_FUN(G_glewUniformHandleui64NV)
#define glUniformHandleui64vNV GLEW_GET_FUN(G_glewUniformHandleui64vNV)

#define GLEW_NV_bindless_texture GLEW_GET_VAR(G_GLEW_NV_bindless_texture)

#endif /* GL_NV_bindless_texture */

/* --------------------------- GL_NV_blend_square -------------------------- */

#ifndef GL_NV_blend_square
#define GL_NV_blend_square 1

#define GLEW_NV_blend_square GLEW_GET_VAR(G_GLEW_NV_blend_square)

#endif /* GL_NV_blend_square */

/* ------------------------ GL_NV_conditional_render ----------------------- */

#ifndef GL_NV_conditional_render
#define GL_NV_conditional_render 1

#define GL_QUERY_WAIT_NV 0x8E13
#define GL_QUERY_NO_WAIT_NV 0x8E14
#define GL_QUERY_BY_REGION_WAIT_NV 0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT_NV 0x8E16

typedef void (GLAPIENTRY * PFNGLBEGINCONDITIONALRENDERNVPROC) (GLuint id, GLenum mode);
typedef void (GLAPIENTRY * PFNGLENDCONDITIONALRENDERNVPROC) (void);

#define glBeginConditionalRenderNV GLEW_GET_FUN(G_glewBeginConditionalRenderNV)
#define glEndConditionalRenderNV GLEW_GET_FUN(G_glewEndConditionalRenderNV)

#define GLEW_NV_conditional_render GLEW_GET_VAR(G_GLEW_NV_conditional_render)

#endif /* GL_NV_conditional_render */

/* ----------------------- GL_NV_copy_depth_to_color ----------------------- */

#ifndef GL_NV_copy_depth_to_color
#define GL_NV_copy_depth_to_color 1

#define GL_DEPTH_STENCIL_TO_RGBA_NV 0x886E
#define GL_DEPTH_STENCIL_TO_BGRA_NV 0x886F

#define GLEW_NV_copy_depth_to_color GLEW_GET_VAR(G_GLEW_NV_copy_depth_to_color)

#endif /* GL_NV_copy_depth_to_color */

/* ---------------------------- GL_NV_copy_image --------------------------- */

#ifndef GL_NV_copy_image
#define GL_NV_copy_image 1

typedef void (GLAPIENTRY * PFNGLCOPYIMAGESUBDATANVPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);

#define glCopyImageSubDataNV GLEW_GET_FUN(G_glewCopyImageSubDataNV)

#define GLEW_NV_copy_image GLEW_GET_VAR(G_GLEW_NV_copy_image)

#endif /* GL_NV_copy_image */

/* ------------------------ GL_NV_depth_buffer_float ----------------------- */

#ifndef GL_NV_depth_buffer_float
#define GL_NV_depth_buffer_float 1

#define GL_DEPTH_COMPONENT32F_NV 0x8DAB
#define GL_DEPTH32F_STENCIL8_NV 0x8DAC
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV_NV 0x8DAD
#define GL_DEPTH_BUFFER_FLOAT_MODE_NV 0x8DAF

typedef void (GLAPIENTRY * PFNGLCLEARDEPTHDNVPROC) (GLdouble depth);
typedef void (GLAPIENTRY * PFNGLDEPTHBOUNDSDNVPROC) (GLdouble zmin, GLdouble zmax);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEDNVPROC) (GLdouble zNear, GLdouble zFar);

#define glClearDepthdNV GLEW_GET_FUN(G_glewClearDepthdNV)
#define glDepthBoundsdNV GLEW_GET_FUN(G_glewDepthBoundsdNV)
#define glDepthRangedNV GLEW_GET_FUN(G_glewDepthRangedNV)

#define GLEW_NV_depth_buffer_float GLEW_GET_VAR(G_GLEW_NV_depth_buffer_float)

#endif /* GL_NV_depth_buffer_float */

/* --------------------------- GL_NV_depth_clamp --------------------------- */

#ifndef GL_NV_depth_clamp
#define GL_NV_depth_clamp 1

#define GL_DEPTH_CLAMP_NV 0x864F

#define GLEW_NV_depth_clamp GLEW_GET_VAR(G_GLEW_NV_depth_clamp)

#endif /* GL_NV_depth_clamp */

/* ---------------------- GL_NV_depth_range_unclamped ---------------------- */

#ifndef GL_NV_depth_range_unclamped
#define GL_NV_depth_range_unclamped 1

#define GL_SAMPLE_COUNT_BITS_NV 0x8864
#define GL_CURRENT_SAMPLE_COUNT_QUERY_NV 0x8865
#define GL_QUERY_RESULT_NV 0x8866
#define GL_QUERY_RESULT_AVAILABLE_NV 0x8867
#define GL_SAMPLE_COUNT_NV 0x8914

#define GLEW_NV_depth_range_unclamped GLEW_GET_VAR(G_GLEW_NV_depth_range_unclamped)

#endif /* GL_NV_depth_range_unclamped */

/* ---------------------------- GL_NV_evaluators --------------------------- */

#ifndef GL_NV_evaluators
#define GL_NV_evaluators 1

#define GL_EVAL_2D_NV 0x86C0
#define GL_EVAL_TRIANGULAR_2D_NV 0x86C1
#define GL_MAP_TESSELLATION_NV 0x86C2
#define GL_MAP_ATTRIB_U_ORDER_NV 0x86C3
#define GL_MAP_ATTRIB_V_ORDER_NV 0x86C4
#define GL_EVAL_FRACTIONAL_TESSELLATION_NV 0x86C5
#define GL_EVAL_VERTEX_ATTRIB0_NV 0x86C6
#define GL_EVAL_VERTEX_ATTRIB1_NV 0x86C7
#define GL_EVAL_VERTEX_ATTRIB2_NV 0x86C8
#define GL_EVAL_VERTEX_ATTRIB3_NV 0x86C9
#define GL_EVAL_VERTEX_ATTRIB4_NV 0x86CA
#define GL_EVAL_VERTEX_ATTRIB5_NV 0x86CB
#define GL_EVAL_VERTEX_ATTRIB6_NV 0x86CC
#define GL_EVAL_VERTEX_ATTRIB7_NV 0x86CD
#define GL_EVAL_VERTEX_ATTRIB8_NV 0x86CE
#define GL_EVAL_VERTEX_ATTRIB9_NV 0x86CF
#define GL_EVAL_VERTEX_ATTRIB10_NV 0x86D0
#define GL_EVAL_VERTEX_ATTRIB11_NV 0x86D1
#define GL_EVAL_VERTEX_ATTRIB12_NV 0x86D2
#define GL_EVAL_VERTEX_ATTRIB13_NV 0x86D3
#define GL_EVAL_VERTEX_ATTRIB14_NV 0x86D4
#define GL_EVAL_VERTEX_ATTRIB15_NV 0x86D5
#define GL_MAX_MAP_TESSELLATION_NV 0x86D6
#define GL_MAX_RATIONAL_EVAL_ORDER_NV 0x86D7

typedef void (GLAPIENTRY * PFNGLEVALMAPSNVPROC) (GLenum target, GLenum mode);
typedef void (GLAPIENTRY * PFNGLGETMAPATTRIBPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMAPATTRIBPARAMETERIVNVPROC) (GLenum target, GLuint index, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETMAPCONTROLPOINTSNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void* points);
typedef void (GLAPIENTRY * PFNGLGETMAPPARAMETERFVNVPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETMAPPARAMETERIVNVPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLMAPCONTROLPOINTSNVPROC) (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void* points);
typedef void (GLAPIENTRY * PFNGLMAPPARAMETERFVNVPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLMAPPARAMETERIVNVPROC) (GLenum target, GLenum pname, const GLint* params);

#define glEvalMapsNV GLEW_GET_FUN(G_glewEvalMapsNV)
#define glGetMapAttribParameterfvNV GLEW_GET_FUN(G_glewGetMapAttribParameterfvNV)
#define glGetMapAttribParameterivNV GLEW_GET_FUN(G_glewGetMapAttribParameterivNV)
#define glGetMapControlPointsNV GLEW_GET_FUN(G_glewGetMapControlPointsNV)
#define glGetMapParameterfvNV GLEW_GET_FUN(G_glewGetMapParameterfvNV)
#define glGetMapParameterivNV GLEW_GET_FUN(G_glewGetMapParameterivNV)
#define glMapControlPointsNV GLEW_GET_FUN(G_glewMapControlPointsNV)
#define glMapParameterfvNV GLEW_GET_FUN(G_glewMapParameterfvNV)
#define glMapParameterivNV GLEW_GET_FUN(G_glewMapParameterivNV)

#define GLEW_NV_evaluators GLEW_GET_VAR(G_GLEW_NV_evaluators)

#endif /* GL_NV_evaluators */

/* ----------------------- GL_NV_explicit_multisample ---------------------- */

#ifndef GL_NV_explicit_multisample
#define GL_NV_explicit_multisample 1

#define GL_SAMPLE_POSITION_NV 0x8E50
#define GL_SAMPLE_MASK_NV 0x8E51
#define GL_SAMPLE_MASK_VALUE_NV 0x8E52
#define GL_TEXTURE_BINDING_RENDERBUFFER_NV 0x8E53
#define GL_TEXTURE_RENDERBUFFER_DATA_STORE_BINDING_NV 0x8E54
#define GL_TEXTURE_RENDERBUFFER_NV 0x8E55
#define GL_SAMPLER_RENDERBUFFER_NV 0x8E56
#define GL_INT_SAMPLER_RENDERBUFFER_NV 0x8E57
#define GL_UNSIGNED_INT_SAMPLER_RENDERBUFFER_NV 0x8E58
#define GL_MAX_SAMPLE_MASK_WORDS_NV 0x8E59

typedef void (GLAPIENTRY * PFNGLGETMULTISAMPLEFVNVPROC) (GLenum pname, GLuint index, GLfloat* val);
typedef void (GLAPIENTRY * PFNGLSAMPLEMASKINDEXEDNVPROC) (GLuint index, GLbitfield mask);
typedef void (GLAPIENTRY * PFNGLTEXRENDERBUFFERNVPROC) (GLenum target, GLuint renderbuffer);

#define glGetMultisamplefvNV GLEW_GET_FUN(G_glewGetMultisamplefvNV)
#define glSampleMaskIndexedNV GLEW_GET_FUN(G_glewSampleMaskIndexedNV)
#define glTexRenderbufferNV GLEW_GET_FUN(G_glewTexRenderbufferNV)

#define GLEW_NV_explicit_multisample GLEW_GET_VAR(G_GLEW_NV_explicit_multisample)

#endif /* GL_NV_explicit_multisample */

/* ------------------------------ GL_NV_fence ------------------------------ */

#ifndef GL_NV_fence
#define GL_NV_fence 1

#define GL_ALL_COMPLETED_NV 0x84F2
#define GL_FENCE_STATUS_NV 0x84F3
#define GL_FENCE_CONDITION_NV 0x84F4

typedef void (GLAPIENTRY * PFNGLDELETEFENCESNVPROC) (GLsizei n, const GLuint* fences);
typedef void (GLAPIENTRY * PFNGLFINISHFENCENVPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLGENFENCESNVPROC) (GLsizei n, GLuint* fences);
typedef void (GLAPIENTRY * PFNGLGETFENCEIVNVPROC) (GLuint fence, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISFENCENVPROC) (GLuint fence);
typedef void (GLAPIENTRY * PFNGLSETFENCENVPROC) (GLuint fence, GLenum condition);
typedef GLboolean (GLAPIENTRY * PFNGLTESTFENCENVPROC) (GLuint fence);

#define glDeleteFencesNV GLEW_GET_FUN(G_glewDeleteFencesNV)
#define glFinishFenceNV GLEW_GET_FUN(G_glewFinishFenceNV)
#define glGenFencesNV GLEW_GET_FUN(G_glewGenFencesNV)
#define glGetFenceivNV GLEW_GET_FUN(G_glewGetFenceivNV)
#define glIsFenceNV GLEW_GET_FUN(G_glewIsFenceNV)
#define glSetFenceNV GLEW_GET_FUN(G_glewSetFenceNV)
#define glTestFenceNV GLEW_GET_FUN(G_glewTestFenceNV)

#define GLEW_NV_fence GLEW_GET_VAR(G_GLEW_NV_fence)

#endif /* GL_NV_fence */

/* --------------------------- GL_NV_float_buffer -------------------------- */

#ifndef GL_NV_float_buffer
#define GL_NV_float_buffer 1

#define GL_FLOAT_R_NV 0x8880
#define GL_FLOAT_RG_NV 0x8881
#define GL_FLOAT_RGB_NV 0x8882
#define GL_FLOAT_RGBA_NV 0x8883
#define GL_FLOAT_R16_NV 0x8884
#define GL_FLOAT_R32_NV 0x8885
#define GL_FLOAT_RG16_NV 0x8886
#define GL_FLOAT_RG32_NV 0x8887
#define GL_FLOAT_RGB16_NV 0x8888
#define GL_FLOAT_RGB32_NV 0x8889
#define GL_FLOAT_RGBA16_NV 0x888A
#define GL_FLOAT_RGBA32_NV 0x888B
#define GL_TEXTURE_FLOAT_COMPONENTS_NV 0x888C
#define GL_FLOAT_CLEAR_COLOR_VALUE_NV 0x888D
#define GL_FLOAT_RGBA_MODE_NV 0x888E

#define GLEW_NV_float_buffer GLEW_GET_VAR(G_GLEW_NV_float_buffer)

#endif /* GL_NV_float_buffer */

/* --------------------------- GL_NV_fog_distance -------------------------- */

#ifndef GL_NV_fog_distance
#define GL_NV_fog_distance 1

#define GL_FOG_DISTANCE_MODE_NV 0x855A
#define GL_EYE_RADIAL_NV 0x855B
#define GL_EYE_PLANE_ABSOLUTE_NV 0x855C

#define GLEW_NV_fog_distance GLEW_GET_VAR(G_GLEW_NV_fog_distance)

#endif /* GL_NV_fog_distance */

/* ------------------------- GL_NV_fragment_program ------------------------ */

#ifndef GL_NV_fragment_program
#define GL_NV_fragment_program 1

#define GL_MAX_FRAGMENT_PROGRAM_LOCAL_PARAMETERS_NV 0x8868
#define GL_FRAGMENT_PROGRAM_NV 0x8870
#define GL_MAX_TEXTURE_COORDS_NV 0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_NV 0x8872
#define GL_FRAGMENT_PROGRAM_BINDING_NV 0x8873
#define GL_PROGRAM_ERROR_STRING_NV 0x8874

typedef void (GLAPIENTRY * PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLdouble *params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLfloat *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4DNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, const GLdouble v[]);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4FNVPROC) (GLuint id, GLsizei len, const GLubyte* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC) (GLuint id, GLsizei len, const GLubyte* name, const GLfloat v[]);

#define glGetProgramNamedParameterdvNV GLEW_GET_FUN(G_glewGetProgramNamedParameterdvNV)
#define glGetProgramNamedParameterfvNV GLEW_GET_FUN(G_glewGetProgramNamedParameterfvNV)
#define glProgramNamedParameter4dNV GLEW_GET_FUN(G_glewProgramNamedParameter4dNV)
#define glProgramNamedParameter4dvNV GLEW_GET_FUN(G_glewProgramNamedParameter4dvNV)
#define glProgramNamedParameter4fNV GLEW_GET_FUN(G_glewProgramNamedParameter4fNV)
#define glProgramNamedParameter4fvNV GLEW_GET_FUN(G_glewProgramNamedParameter4fvNV)

#define GLEW_NV_fragment_program GLEW_GET_VAR(G_GLEW_NV_fragment_program)

#endif /* GL_NV_fragment_program */

/* ------------------------ GL_NV_fragment_program2 ------------------------ */

#ifndef GL_NV_fragment_program2
#define GL_NV_fragment_program2 1

#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV 0x88F4
#define GL_MAX_PROGRAM_CALL_DEPTH_NV 0x88F5
#define GL_MAX_PROGRAM_IF_DEPTH_NV 0x88F6
#define GL_MAX_PROGRAM_LOOP_DEPTH_NV 0x88F7
#define GL_MAX_PROGRAM_LOOP_COUNT_NV 0x88F8

#define GLEW_NV_fragment_program2 GLEW_GET_VAR(G_GLEW_NV_fragment_program2)

#endif /* GL_NV_fragment_program2 */

/* ------------------------ GL_NV_fragment_program4 ------------------------ */

#ifndef GL_NV_fragment_program4
#define GL_NV_fragment_program4 1

#define GLEW_NV_fragment_program4 GLEW_GET_VAR(G_GLEW_NV_fragment_program4)

#endif /* GL_NV_fragment_program4 */

/* --------------------- GL_NV_fragment_program_option --------------------- */

#ifndef GL_NV_fragment_program_option
#define GL_NV_fragment_program_option 1

#define GLEW_NV_fragment_program_option GLEW_GET_VAR(G_GLEW_NV_fragment_program_option)

#endif /* GL_NV_fragment_program_option */

/* ----------------- GL_NV_framebuffer_multisample_coverage ---------------- */

#ifndef GL_NV_framebuffer_multisample_coverage
#define GL_NV_framebuffer_multisample_coverage 1

#define GL_RENDERBUFFER_COVERAGE_SAMPLES_NV 0x8CAB
#define GL_RENDERBUFFER_COLOR_SAMPLES_NV 0x8E10
#define GL_MAX_MULTISAMPLE_COVERAGE_MODES_NV 0x8E11
#define GL_MULTISAMPLE_COVERAGE_MODES_NV 0x8E12

typedef void (GLAPIENTRY * PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height);

#define glRenderbufferStorageMultisampleCoverageNV GLEW_GET_FUN(G_glewRenderbufferStorageMultisampleCoverageNV)

#define GLEW_NV_framebuffer_multisample_coverage GLEW_GET_VAR(G_GLEW_NV_framebuffer_multisample_coverage)

#endif /* GL_NV_framebuffer_multisample_coverage */

/* ------------------------ GL_NV_geometry_program4 ------------------------ */

#ifndef GL_NV_geometry_program4
#define GL_NV_geometry_program4 1

#define GL_GEOMETRY_PROGRAM_NV 0x8C26
#define GL_MAX_PROGRAM_OUTPUT_VERTICES_NV 0x8C27
#define GL_MAX_PROGRAM_TOTAL_OUTPUT_COMPONENTS_NV 0x8C28

typedef void (GLAPIENTRY * PFNGLPROGRAMVERTEXLIMITNVPROC) (GLenum target, GLint limit);

#define glProgramVertexLimitNV GLEW_GET_FUN(G_glewProgramVertexLimitNV)

#define GLEW_NV_geometry_program4 GLEW_GET_VAR(G_GLEW_NV_geometry_program4)

#endif /* GL_NV_geometry_program4 */

/* ------------------------- GL_NV_geometry_shader4 ------------------------ */

#ifndef GL_NV_geometry_shader4
#define GL_NV_geometry_shader4 1

#define GLEW_NV_geometry_shader4 GLEW_GET_VAR(G_GLEW_NV_geometry_shader4)

#endif /* GL_NV_geometry_shader4 */

/* --------------------------- GL_NV_gpu_program4 -------------------------- */

#ifndef GL_NV_gpu_program4
#define GL_NV_gpu_program4 1

#define GL_MIN_PROGRAM_TEXEL_OFFSET_NV 0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET_NV 0x8905
#define GL_PROGRAM_ATTRIB_COMPONENTS_NV 0x8906
#define GL_PROGRAM_RESULT_COMPONENTS_NV 0x8907
#define GL_MAX_PROGRAM_ATTRIB_COMPONENTS_NV 0x8908
#define GL_MAX_PROGRAM_RESULT_COMPONENTS_NV 0x8909
#define GL_MAX_PROGRAM_GENERIC_ATTRIBS_NV 0x8DA5
#define GL_MAX_PROGRAM_GENERIC_RESULTS_NV 0x8DA6

typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4IVNVPROC) (GLenum target, GLuint index, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, const GLuint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLuint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4INVPROC) (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC) (GLenum target, GLuint index, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4UINVPROC) (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC) (GLenum target, GLuint index, const GLuint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC) (GLenum target, GLuint index, GLsizei count, const GLuint *params);

#define glProgramEnvParameterI4iNV GLEW_GET_FUN(G_glewProgramEnvParameterI4iNV)
#define glProgramEnvParameterI4ivNV GLEW_GET_FUN(G_glewProgramEnvParameterI4ivNV)
#define glProgramEnvParameterI4uiNV GLEW_GET_FUN(G_glewProgramEnvParameterI4uiNV)
#define glProgramEnvParameterI4uivNV GLEW_GET_FUN(G_glewProgramEnvParameterI4uivNV)
#define glProgramEnvParametersI4ivNV GLEW_GET_FUN(G_glewProgramEnvParametersI4ivNV)
#define glProgramEnvParametersI4uivNV GLEW_GET_FUN(G_glewProgramEnvParametersI4uivNV)
#define glProgramLocalParameterI4iNV GLEW_GET_FUN(G_glewProgramLocalParameterI4iNV)
#define glProgramLocalParameterI4ivNV GLEW_GET_FUN(G_glewProgramLocalParameterI4ivNV)
#define glProgramLocalParameterI4uiNV GLEW_GET_FUN(G_glewProgramLocalParameterI4uiNV)
#define glProgramLocalParameterI4uivNV GLEW_GET_FUN(G_glewProgramLocalParameterI4uivNV)
#define glProgramLocalParametersI4ivNV GLEW_GET_FUN(G_glewProgramLocalParametersI4ivNV)
#define glProgramLocalParametersI4uivNV GLEW_GET_FUN(G_glewProgramLocalParametersI4uivNV)

#define GLEW_NV_gpu_program4 GLEW_GET_VAR(G_GLEW_NV_gpu_program4)

#endif /* GL_NV_gpu_program4 */

/* --------------------------- GL_NV_gpu_program5 -------------------------- */

#ifndef GL_NV_gpu_program5
#define GL_NV_gpu_program5 1

#define GL_MAX_GEOMETRY_PROGRAM_INVOCATIONS_NV 0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET_NV 0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET_NV 0x8E5C
#define GL_FRAGMENT_PROGRAM_INTERPOLATION_OFFSET_BITS_NV 0x8E5D
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_NV 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_NV 0x8E5F

#define GLEW_NV_gpu_program5 GLEW_GET_VAR(G_GLEW_NV_gpu_program5)

#endif /* GL_NV_gpu_program5 */

/* ------------------------- GL_NV_gpu_program_fp64 ------------------------ */

#ifndef GL_NV_gpu_program_fp64
#define GL_NV_gpu_program_fp64 1

#define GLEW_NV_gpu_program_fp64 GLEW_GET_VAR(G_GLEW_NV_gpu_program_fp64)

#endif /* GL_NV_gpu_program_fp64 */

/* --------------------------- GL_NV_gpu_shader5 --------------------------- */

#ifndef GL_NV_gpu_shader5
#define GL_NV_gpu_shader5 1

#define GL_INT64_NV 0x140E
#define GL_UNSIGNED_INT64_NV 0x140F
#define GL_INT8_NV 0x8FE0
#define GL_INT8_VEC2_NV 0x8FE1
#define GL_INT8_VEC3_NV 0x8FE2
#define GL_INT8_VEC4_NV 0x8FE3
#define GL_INT16_NV 0x8FE4
#define GL_INT16_VEC2_NV 0x8FE5
#define GL_INT16_VEC3_NV 0x8FE6
#define GL_INT16_VEC4_NV 0x8FE7
#define GL_INT64_VEC2_NV 0x8FE9
#define GL_INT64_VEC3_NV 0x8FEA
#define GL_INT64_VEC4_NV 0x8FEB
#define GL_UNSIGNED_INT8_NV 0x8FEC
#define GL_UNSIGNED_INT8_VEC2_NV 0x8FED
#define GL_UNSIGNED_INT8_VEC3_NV 0x8FEE
#define GL_UNSIGNED_INT8_VEC4_NV 0x8FEF
#define GL_UNSIGNED_INT16_NV 0x8FF0
#define GL_UNSIGNED_INT16_VEC2_NV 0x8FF1
#define GL_UNSIGNED_INT16_VEC3_NV 0x8FF2
#define GL_UNSIGNED_INT16_VEC4_NV 0x8FF3
#define GL_UNSIGNED_INT64_VEC2_NV 0x8FF5
#define GL_UNSIGNED_INT64_VEC3_NV 0x8FF6
#define GL_UNSIGNED_INT64_VEC4_NV 0x8FF7
#define GL_FLOAT16_NV 0x8FF8
#define GL_FLOAT16_VEC2_NV 0x8FF9
#define GL_FLOAT16_VEC3_NV 0x8FFA
#define GL_FLOAT16_VEC4_NV 0x8FFB

typedef void (GLAPIENTRY * PFNGLGETUNIFORMI64VNVPROC) (GLuint program, GLint location, GLint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLuint64EXT* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1I64NVPROC) (GLuint program, GLint location, GLint64EXT x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM1UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM2UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM3UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4I64NVPROC) (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4I64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UI64NVPROC) (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORM4UI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1I64NVPROC) (GLint location, GLint64EXT x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UI64NVPROC) (GLint location, GLuint64EXT x);
typedef void (GLAPIENTRY * PFNGLUNIFORM1UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y);
typedef void (GLAPIENTRY * PFNGLUNIFORM2UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (GLAPIENTRY * PFNGLUNIFORM3UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4I64NVPROC) (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4I64VNVPROC) (GLint location, GLsizei count, const GLint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UI64NVPROC) (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (GLAPIENTRY * PFNGLUNIFORM4UI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);

#define glGetUniformi64vNV GLEW_GET_FUN(G_glewGetUniformi64vNV)
#define glGetUniformui64vNV GLEW_GET_FUN(G_glewGetUniformui64vNV)
#define glProgramUniform1i64NV GLEW_GET_FUN(G_glewProgramUniform1i64NV)
#define glProgramUniform1i64vNV GLEW_GET_FUN(G_glewProgramUniform1i64vNV)
#define glProgramUniform1ui64NV GLEW_GET_FUN(G_glewProgramUniform1ui64NV)
#define glProgramUniform1ui64vNV GLEW_GET_FUN(G_glewProgramUniform1ui64vNV)
#define glProgramUniform2i64NV GLEW_GET_FUN(G_glewProgramUniform2i64NV)
#define glProgramUniform2i64vNV GLEW_GET_FUN(G_glewProgramUniform2i64vNV)
#define glProgramUniform2ui64NV GLEW_GET_FUN(G_glewProgramUniform2ui64NV)
#define glProgramUniform2ui64vNV GLEW_GET_FUN(G_glewProgramUniform2ui64vNV)
#define glProgramUniform3i64NV GLEW_GET_FUN(G_glewProgramUniform3i64NV)
#define glProgramUniform3i64vNV GLEW_GET_FUN(G_glewProgramUniform3i64vNV)
#define glProgramUniform3ui64NV GLEW_GET_FUN(G_glewProgramUniform3ui64NV)
#define glProgramUniform3ui64vNV GLEW_GET_FUN(G_glewProgramUniform3ui64vNV)
#define glProgramUniform4i64NV GLEW_GET_FUN(G_glewProgramUniform4i64NV)
#define glProgramUniform4i64vNV GLEW_GET_FUN(G_glewProgramUniform4i64vNV)
#define glProgramUniform4ui64NV GLEW_GET_FUN(G_glewProgramUniform4ui64NV)
#define glProgramUniform4ui64vNV GLEW_GET_FUN(G_glewProgramUniform4ui64vNV)
#define glUniform1i64NV GLEW_GET_FUN(G_glewUniform1i64NV)
#define glUniform1i64vNV GLEW_GET_FUN(G_glewUniform1i64vNV)
#define glUniform1ui64NV GLEW_GET_FUN(G_glewUniform1ui64NV)
#define glUniform1ui64vNV GLEW_GET_FUN(G_glewUniform1ui64vNV)
#define glUniform2i64NV GLEW_GET_FUN(G_glewUniform2i64NV)
#define glUniform2i64vNV GLEW_GET_FUN(G_glewUniform2i64vNV)
#define glUniform2ui64NV GLEW_GET_FUN(G_glewUniform2ui64NV)
#define glUniform2ui64vNV GLEW_GET_FUN(G_glewUniform2ui64vNV)
#define glUniform3i64NV GLEW_GET_FUN(G_glewUniform3i64NV)
#define glUniform3i64vNV GLEW_GET_FUN(G_glewUniform3i64vNV)
#define glUniform3ui64NV GLEW_GET_FUN(G_glewUniform3ui64NV)
#define glUniform3ui64vNV GLEW_GET_FUN(G_glewUniform3ui64vNV)
#define glUniform4i64NV GLEW_GET_FUN(G_glewUniform4i64NV)
#define glUniform4i64vNV GLEW_GET_FUN(G_glewUniform4i64vNV)
#define glUniform4ui64NV GLEW_GET_FUN(G_glewUniform4ui64NV)
#define glUniform4ui64vNV GLEW_GET_FUN(G_glewUniform4ui64vNV)

#define GLEW_NV_gpu_shader5 GLEW_GET_VAR(G_GLEW_NV_gpu_shader5)

#endif /* GL_NV_gpu_shader5 */

/* ---------------------------- GL_NV_half_float --------------------------- */

#ifndef GL_NV_half_float
#define GL_NV_half_float 1

#define GL_HALF_FLOAT_NV 0x140B

typedef unsigned short GLhalf;

typedef void (GLAPIENTRY * PFNGLCOLOR3HNVPROC) (GLhalf red, GLhalf green, GLhalf blue);
typedef void (GLAPIENTRY * PFNGLCOLOR3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLCOLOR4HNVPROC) (GLhalf red, GLhalf green, GLhalf blue, GLhalf alpha);
typedef void (GLAPIENTRY * PFNGLCOLOR4HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLFOGCOORDHNVPROC) (GLhalf fog);
typedef void (GLAPIENTRY * PFNGLFOGCOORDHVNVPROC) (const GLhalf* fog);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1HNVPROC) (GLenum target, GLhalf s);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD1HVNVPROC) (GLenum target, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2HNVPROC) (GLenum target, GLhalf s, GLhalf t);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD2HVNVPROC) (GLenum target, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3HNVPROC) (GLenum target, GLhalf s, GLhalf t, GLhalf r);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD3HVNVPROC) (GLenum target, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4HNVPROC) (GLenum target, GLhalf s, GLhalf t, GLhalf r, GLhalf q);
typedef void (GLAPIENTRY * PFNGLMULTITEXCOORD4HVNVPROC) (GLenum target, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLNORMAL3HNVPROC) (GLhalf nx, GLhalf ny, GLhalf nz);
typedef void (GLAPIENTRY * PFNGLNORMAL3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3HNVPROC) (GLhalf red, GLhalf green, GLhalf blue);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLOR3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1HNVPROC) (GLhalf s);
typedef void (GLAPIENTRY * PFNGLTEXCOORD1HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2HNVPROC) (GLhalf s, GLhalf t);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3HNVPROC) (GLhalf s, GLhalf t, GLhalf r);
typedef void (GLAPIENTRY * PFNGLTEXCOORD3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4HNVPROC) (GLhalf s, GLhalf t, GLhalf r, GLhalf q);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEX2HNVPROC) (GLhalf x, GLhalf y);
typedef void (GLAPIENTRY * PFNGLVERTEX2HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEX3HNVPROC) (GLhalf x, GLhalf y, GLhalf z);
typedef void (GLAPIENTRY * PFNGLVERTEX3HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEX4HNVPROC) (GLhalf x, GLhalf y, GLhalf z, GLhalf w);
typedef void (GLAPIENTRY * PFNGLVERTEX4HVNVPROC) (const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1HNVPROC) (GLuint index, GLhalf x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1HVNVPROC) (GLuint index, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2HNVPROC) (GLuint index, GLhalf x, GLhalf y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2HVNVPROC) (GLuint index, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3HNVPROC) (GLuint index, GLhalf x, GLhalf y, GLhalf z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3HVNVPROC) (GLuint index, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4HNVPROC) (GLuint index, GLhalf x, GLhalf y, GLhalf z, GLhalf w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4HVNVPROC) (GLuint index, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4HVNVPROC) (GLuint index, GLsizei n, const GLhalf* v);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTHNVPROC) (GLhalf weight);
typedef void (GLAPIENTRY * PFNGLVERTEXWEIGHTHVNVPROC) (const GLhalf* weight);

#define glColor3hNV GLEW_GET_FUN(G_glewColor3hNV)
#define glColor3hvNV GLEW_GET_FUN(G_glewColor3hvNV)
#define glColor4hNV GLEW_GET_FUN(G_glewColor4hNV)
#define glColor4hvNV GLEW_GET_FUN(G_glewColor4hvNV)
#define glFogCoordhNV GLEW_GET_FUN(G_glewFogCoordhNV)
#define glFogCoordhvNV GLEW_GET_FUN(G_glewFogCoordhvNV)
#define glMultiTexCoord1hNV GLEW_GET_FUN(G_glewMultiTexCoord1hNV)
#define glMultiTexCoord1hvNV GLEW_GET_FUN(G_glewMultiTexCoord1hvNV)
#define glMultiTexCoord2hNV GLEW_GET_FUN(G_glewMultiTexCoord2hNV)
#define glMultiTexCoord2hvNV GLEW_GET_FUN(G_glewMultiTexCoord2hvNV)
#define glMultiTexCoord3hNV GLEW_GET_FUN(G_glewMultiTexCoord3hNV)
#define glMultiTexCoord3hvNV GLEW_GET_FUN(G_glewMultiTexCoord3hvNV)
#define glMultiTexCoord4hNV GLEW_GET_FUN(G_glewMultiTexCoord4hNV)
#define glMultiTexCoord4hvNV GLEW_GET_FUN(G_glewMultiTexCoord4hvNV)
#define glNormal3hNV GLEW_GET_FUN(G_glewNormal3hNV)
#define glNormal3hvNV GLEW_GET_FUN(G_glewNormal3hvNV)
#define glSecondaryColor3hNV GLEW_GET_FUN(G_glewSecondaryColor3hNV)
#define glSecondaryColor3hvNV GLEW_GET_FUN(G_glewSecondaryColor3hvNV)
#define glTexCoord1hNV GLEW_GET_FUN(G_glewTexCoord1hNV)
#define glTexCoord1hvNV GLEW_GET_FUN(G_glewTexCoord1hvNV)
#define glTexCoord2hNV GLEW_GET_FUN(G_glewTexCoord2hNV)
#define glTexCoord2hvNV GLEW_GET_FUN(G_glewTexCoord2hvNV)
#define glTexCoord3hNV GLEW_GET_FUN(G_glewTexCoord3hNV)
#define glTexCoord3hvNV GLEW_GET_FUN(G_glewTexCoord3hvNV)
#define glTexCoord4hNV GLEW_GET_FUN(G_glewTexCoord4hNV)
#define glTexCoord4hvNV GLEW_GET_FUN(G_glewTexCoord4hvNV)
#define glVertex2hNV GLEW_GET_FUN(G_glewVertex2hNV)
#define glVertex2hvNV GLEW_GET_FUN(G_glewVertex2hvNV)
#define glVertex3hNV GLEW_GET_FUN(G_glewVertex3hNV)
#define glVertex3hvNV GLEW_GET_FUN(G_glewVertex3hvNV)
#define glVertex4hNV GLEW_GET_FUN(G_glewVertex4hNV)
#define glVertex4hvNV GLEW_GET_FUN(G_glewVertex4hvNV)
#define glVertexAttrib1hNV GLEW_GET_FUN(G_glewVertexAttrib1hNV)
#define glVertexAttrib1hvNV GLEW_GET_FUN(G_glewVertexAttrib1hvNV)
#define glVertexAttrib2hNV GLEW_GET_FUN(G_glewVertexAttrib2hNV)
#define glVertexAttrib2hvNV GLEW_GET_FUN(G_glewVertexAttrib2hvNV)
#define glVertexAttrib3hNV GLEW_GET_FUN(G_glewVertexAttrib3hNV)
#define glVertexAttrib3hvNV GLEW_GET_FUN(G_glewVertexAttrib3hvNV)
#define glVertexAttrib4hNV GLEW_GET_FUN(G_glewVertexAttrib4hNV)
#define glVertexAttrib4hvNV GLEW_GET_FUN(G_glewVertexAttrib4hvNV)
#define glVertexAttribs1hvNV GLEW_GET_FUN(G_glewVertexAttribs1hvNV)
#define glVertexAttribs2hvNV GLEW_GET_FUN(G_glewVertexAttribs2hvNV)
#define glVertexAttribs3hvNV GLEW_GET_FUN(G_glewVertexAttribs3hvNV)
#define glVertexAttribs4hvNV GLEW_GET_FUN(G_glewVertexAttribs4hvNV)
#define glVertexWeighthNV GLEW_GET_FUN(G_glewVertexWeighthNV)
#define glVertexWeighthvNV GLEW_GET_FUN(G_glewVertexWeighthvNV)

#define GLEW_NV_half_float GLEW_GET_VAR(G_GLEW_NV_half_float)

#endif /* GL_NV_half_float */

/* ------------------------ GL_NV_light_max_exponent ----------------------- */

#ifndef GL_NV_light_max_exponent
#define GL_NV_light_max_exponent 1

#define GL_MAX_SHININESS_NV 0x8504
#define GL_MAX_SPOT_EXPONENT_NV 0x8505

#define GLEW_NV_light_max_exponent GLEW_GET_VAR(G_GLEW_NV_light_max_exponent)

#endif /* GL_NV_light_max_exponent */

/* ----------------------- GL_NV_multisample_coverage ---------------------- */

#ifndef GL_NV_multisample_coverage
#define GL_NV_multisample_coverage 1

#define GL_COVERAGE_SAMPLES_NV 0x80A9
#define GL_COLOR_SAMPLES_NV 0x8E20

#define GLEW_NV_multisample_coverage GLEW_GET_VAR(G_GLEW_NV_multisample_coverage)

#endif /* GL_NV_multisample_coverage */

/* --------------------- GL_NV_multisample_filter_hint --------------------- */

#ifndef GL_NV_multisample_filter_hint
#define GL_NV_multisample_filter_hint 1

#define GL_MULTISAMPLE_FILTER_HINT_NV 0x8534

#define GLEW_NV_multisample_filter_hint GLEW_GET_VAR(G_GLEW_NV_multisample_filter_hint)

#endif /* GL_NV_multisample_filter_hint */

/* ------------------------- GL_NV_occlusion_query ------------------------- */

#ifndef GL_NV_occlusion_query
#define GL_NV_occlusion_query 1

#define GL_PIXEL_COUNTER_BITS_NV 0x8864
#define GL_CURRENT_OCCLUSION_QUERY_ID_NV 0x8865
#define GL_PIXEL_COUNT_NV 0x8866
#define GL_PIXEL_COUNT_AVAILABLE_NV 0x8867

typedef void (GLAPIENTRY * PFNGLBEGINOCCLUSIONQUERYNVPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEOCCLUSIONQUERIESNVPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLENDOCCLUSIONQUERYNVPROC) (void);
typedef void (GLAPIENTRY * PFNGLGENOCCLUSIONQUERIESNVPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETOCCLUSIONQUERYIVNVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETOCCLUSIONQUERYUIVNVPROC) (GLuint id, GLenum pname, GLuint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISOCCLUSIONQUERYNVPROC) (GLuint id);

#define glBeginOcclusionQueryNV GLEW_GET_FUN(G_glewBeginOcclusionQueryNV)
#define glDeleteOcclusionQueriesNV GLEW_GET_FUN(G_glewDeleteOcclusionQueriesNV)
#define glEndOcclusionQueryNV GLEW_GET_FUN(G_glewEndOcclusionQueryNV)
#define glGenOcclusionQueriesNV GLEW_GET_FUN(G_glewGenOcclusionQueriesNV)
#define glGetOcclusionQueryivNV GLEW_GET_FUN(G_glewGetOcclusionQueryivNV)
#define glGetOcclusionQueryuivNV GLEW_GET_FUN(G_glewGetOcclusionQueryuivNV)
#define glIsOcclusionQueryNV GLEW_GET_FUN(G_glewIsOcclusionQueryNV)

#define GLEW_NV_occlusion_query GLEW_GET_VAR(G_GLEW_NV_occlusion_query)

#endif /* GL_NV_occlusion_query */

/* ----------------------- GL_NV_packed_depth_stencil ---------------------- */

#ifndef GL_NV_packed_depth_stencil
#define GL_NV_packed_depth_stencil 1

#define GL_DEPTH_STENCIL_NV 0x84F9
#define GL_UNSIGNED_INT_24_8_NV 0x84FA

#define GLEW_NV_packed_depth_stencil GLEW_GET_VAR(G_GLEW_NV_packed_depth_stencil)

#endif /* GL_NV_packed_depth_stencil */

/* --------------------- GL_NV_parameter_buffer_object --------------------- */

#ifndef GL_NV_parameter_buffer_object
#define GL_NV_parameter_buffer_object 1

#define GL_MAX_PROGRAM_PARAMETER_BUFFER_BINDINGS_NV 0x8DA0
#define GL_MAX_PROGRAM_PARAMETER_BUFFER_SIZE_NV 0x8DA1
#define GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV 0x8DA2
#define GL_GEOMETRY_PROGRAM_PARAMETER_BUFFER_NV 0x8DA3
#define GL_FRAGMENT_PROGRAM_PARAMETER_BUFFER_NV 0x8DA4

typedef void (GLAPIENTRY * PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLuint *params);
typedef void (GLAPIENTRY * PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC) (GLenum target, GLuint buffer, GLuint index, GLsizei count, const GLfloat *params);

#define glProgramBufferParametersIivNV GLEW_GET_FUN(G_glewProgramBufferParametersIivNV)
#define glProgramBufferParametersIuivNV GLEW_GET_FUN(G_glewProgramBufferParametersIuivNV)
#define glProgramBufferParametersfvNV GLEW_GET_FUN(G_glewProgramBufferParametersfvNV)

#define GLEW_NV_parameter_buffer_object GLEW_GET_VAR(G_GLEW_NV_parameter_buffer_object)

#endif /* GL_NV_parameter_buffer_object */

/* --------------------- GL_NV_parameter_buffer_object2 -------------------- */

#ifndef GL_NV_parameter_buffer_object2
#define GL_NV_parameter_buffer_object2 1

#define GLEW_NV_parameter_buffer_object2 GLEW_GET_VAR(G_GLEW_NV_parameter_buffer_object2)

#endif /* GL_NV_parameter_buffer_object2 */

/* -------------------------- GL_NV_path_rendering ------------------------- */

#ifndef GL_NV_path_rendering
#define GL_NV_path_rendering 1

#define GL_CLOSE_PATH_NV 0x00
#define GL_BOLD_BIT_NV 0x01
#define GL_GLYPH_WIDTH_BIT_NV 0x01
#define GL_GLYPH_HEIGHT_BIT_NV 0x02
#define GL_ITALIC_BIT_NV 0x02
#define GL_MOVE_TO_NV 0x02
#define GL_RELATIVE_MOVE_TO_NV 0x03
#define GL_LINE_TO_NV 0x04
#define GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV 0x04
#define GL_RELATIVE_LINE_TO_NV 0x05
#define GL_HORIZONTAL_LINE_TO_NV 0x06
#define GL_RELATIVE_HORIZONTAL_LINE_TO_NV 0x07
#define GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV 0x08
#define GL_VERTICAL_LINE_TO_NV 0x08
#define GL_RELATIVE_VERTICAL_LINE_TO_NV 0x09
#define GL_QUADRATIC_CURVE_TO_NV 0x0A
#define GL_RELATIVE_QUADRATIC_CURVE_TO_NV 0x0B
#define GL_CUBIC_CURVE_TO_NV 0x0C
#define GL_RELATIVE_CUBIC_CURVE_TO_NV 0x0D
#define GL_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0E
#define GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV 0x0F
#define GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV 0x10
#define GL_SMOOTH_CUBIC_CURVE_TO_NV 0x10
#define GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV 0x11
#define GL_SMALL_CCW_ARC_TO_NV 0x12
#define GL_RELATIVE_SMALL_CCW_ARC_TO_NV 0x13
#define GL_SMALL_CW_ARC_TO_NV 0x14
#define GL_RELATIVE_SMALL_CW_ARC_TO_NV 0x15
#define GL_LARGE_CCW_ARC_TO_NV 0x16
#define GL_RELATIVE_LARGE_CCW_ARC_TO_NV 0x17
#define GL_LARGE_CW_ARC_TO_NV 0x18
#define GL_RELATIVE_LARGE_CW_ARC_TO_NV 0x19
#define GL_GLYPH_VERTICAL_BEARING_X_BIT_NV 0x20
#define GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV 0x40
#define GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV 0x80
#define GL_RESTART_PATH_NV 0xF0
#define GL_DUP_FIRST_CUBIC_CURVE_TO_NV 0xF2
#define GL_DUP_LAST_CUBIC_CURVE_TO_NV 0xF4
#define GL_RECT_NV 0xF6
#define GL_CIRCULAR_CCW_ARC_TO_NV 0xF8
#define GL_CIRCULAR_CW_ARC_TO_NV 0xFA
#define GL_CIRCULAR_TANGENT_ARC_TO_NV 0xFC
#define GL_ARC_TO_NV 0xFE
#define GL_RELATIVE_ARC_TO_NV 0xFF
#define GL_GLYPH_HAS_KERNING_BIT_NV 0x100
#define GL_PRIMARY_COLOR_NV 0x852C
#define GL_SECONDARY_COLOR_NV 0x852D
#define GL_PRIMARY_COLOR 0x8577
#define GL_PATH_FORMAT_SVG_NV 0x9070
#define GL_PATH_FORMAT_PS_NV 0x9071
#define GL_STANDARD_FONT_NAME_NV 0x9072
#define GL_SYSTEM_FONT_NAME_NV 0x9073
#define GL_FILE_NAME_NV 0x9074
#define GL_PATH_STROKE_WIDTH_NV 0x9075
#define GL_PATH_END_CAPS_NV 0x9076
#define GL_PATH_INITIAL_END_CAP_NV 0x9077
#define GL_PATH_TERMINAL_END_CAP_NV 0x9078
#define GL_PATH_JOIN_STYLE_NV 0x9079
#define GL_PATH_MITER_LIMIT_NV 0x907A
#define GL_PATH_DASH_CAPS_NV 0x907B
#define GL_PATH_INITIAL_DASH_CAP_NV 0x907C
#define GL_PATH_TERMINAL_DASH_CAP_NV 0x907D
#define GL_PATH_DASH_OFFSET_NV 0x907E
#define GL_PATH_CLIENT_LENGTH_NV 0x907F
#define GL_PATH_FILL_MODE_NV 0x9080
#define GL_PATH_FILL_MASK_NV 0x9081
#define GL_PATH_FILL_COVER_MODE_NV 0x9082
#define GL_PATH_STROKE_COVER_MODE_NV 0x9083
#define GL_PATH_STROKE_MASK_NV 0x9084
#define GL_COUNT_UP_NV 0x9088
#define GL_COUNT_DOWN_NV 0x9089
#define GL_PATH_OBJECT_BOUNDING_BOX_NV 0x908A
#define GL_CONVEX_HULL_NV 0x908B
#define GL_BOUNDING_BOX_NV 0x908D
#define GL_TRANSLATE_X_NV 0x908E
#define GL_TRANSLATE_Y_NV 0x908F
#define GL_TRANSLATE_2D_NV 0x9090
#define GL_TRANSLATE_3D_NV 0x9091
#define GL_AFFINE_2D_NV 0x9092
#define GL_AFFINE_3D_NV 0x9094
#define GL_TRANSPOSE_AFFINE_2D_NV 0x9096
#define GL_TRANSPOSE_AFFINE_3D_NV 0x9098
#define GL_UTF8_NV 0x909A
#define GL_UTF16_NV 0x909B
#define GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV 0x909C
#define GL_PATH_COMMAND_COUNT_NV 0x909D
#define GL_PATH_COORD_COUNT_NV 0x909E
#define GL_PATH_DASH_ARRAY_COUNT_NV 0x909F
#define GL_PATH_COMPUTED_LENGTH_NV 0x90A0
#define GL_PATH_FILL_BOUNDING_BOX_NV 0x90A1
#define GL_PATH_STROKE_BOUNDING_BOX_NV 0x90A2
#define GL_SQUARE_NV 0x90A3
#define GL_ROUND_NV 0x90A4
#define GL_TRIANGULAR_NV 0x90A5
#define GL_BEVEL_NV 0x90A6
#define GL_MITER_REVERT_NV 0x90A7
#define GL_MITER_TRUNCATE_NV 0x90A8
#define GL_SKIP_MISSING_GLYPH_NV 0x90A9
#define GL_USE_MISSING_GLYPH_NV 0x90AA
#define GL_PATH_ERROR_POSITION_NV 0x90AB
#define GL_PATH_FOG_GEN_MODE_NV 0x90AC
#define GL_ACCUM_ADJACENT_PAIRS_NV 0x90AD
#define GL_ADJACENT_PAIRS_NV 0x90AE
#define GL_FIRST_TO_REST_NV 0x90AF
#define GL_PATH_GEN_MODE_NV 0x90B0
#define GL_PATH_GEN_COEFF_NV 0x90B1
#define GL_PATH_GEN_COLOR_FORMAT_NV 0x90B2
#define GL_PATH_GEN_COMPONENTS_NV 0x90B3
#define GL_PATH_DASH_OFFSET_RESET_NV 0x90B4
#define GL_MOVE_TO_RESETS_NV 0x90B5
#define GL_MOVE_TO_CONTINUES_NV 0x90B6
#define GL_PATH_STENCIL_FUNC_NV 0x90B7
#define GL_PATH_STENCIL_REF_NV 0x90B8
#define GL_PATH_STENCIL_VALUE_MASK_NV 0x90B9
#define GL_PATH_STENCIL_DEPTH_OFFSET_FACTOR_NV 0x90BD
#define GL_PATH_STENCIL_DEPTH_OFFSET_UNITS_NV 0x90BE
#define GL_PATH_COVER_DEPTH_FUNC_NV 0x90BF
#define GL_FONT_X_MIN_BOUNDS_BIT_NV 0x00010000
#define GL_FONT_Y_MIN_BOUNDS_BIT_NV 0x00020000
#define GL_FONT_X_MAX_BOUNDS_BIT_NV 0x00040000
#define GL_FONT_Y_MAX_BOUNDS_BIT_NV 0x00080000
#define GL_FONT_UNITS_PER_EM_BIT_NV 0x00100000
#define GL_FONT_ASCENDER_BIT_NV 0x00200000
#define GL_FONT_DESCENDER_BIT_NV 0x00400000
#define GL_FONT_HEIGHT_BIT_NV 0x00800000
#define GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV 0x01000000
#define GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV 0x02000000
#define GL_FONT_UNDERLINE_POSITION_BIT_NV 0x04000000
#define GL_FONT_UNDERLINE_THICKNESS_BIT_NV 0x08000000
#define GL_FONT_HAS_KERNING_BIT_NV 0x10000000

typedef void (GLAPIENTRY * PFNGLCOPYPATHNVPROC) (GLuint resultPath, GLuint srcPath);
typedef void (GLAPIENTRY * PFNGLCOVERFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * PFNGLCOVERFILLPATHNVPROC) (GLuint path, GLenum coverMode);
typedef void (GLAPIENTRY * PFNGLCOVERSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * PFNGLCOVERSTROKEPATHNVPROC) (GLuint name, GLenum coverMode);
typedef void (GLAPIENTRY * PFNGLDELETEPATHSNVPROC) (GLuint path, GLsizei range);
typedef GLuint (GLAPIENTRY * PFNGLGENPATHSNVPROC) (GLsizei range);
typedef void (GLAPIENTRY * PFNGLGETPATHCOLORGENFVNVPROC) (GLenum color, GLenum pname, GLfloat* value);
typedef void (GLAPIENTRY * PFNGLGETPATHCOLORGENIVNVPROC) (GLenum color, GLenum pname, GLint* value);
typedef void (GLAPIENTRY * PFNGLGETPATHCOMMANDSNVPROC) (GLuint name, GLubyte* commands);
typedef void (GLAPIENTRY * PFNGLGETPATHCOORDSNVPROC) (GLuint name, GLfloat* coords);
typedef void (GLAPIENTRY * PFNGLGETPATHDASHARRAYNVPROC) (GLuint name, GLfloat* dashArray);
typedef GLfloat (GLAPIENTRY * PFNGLGETPATHLENGTHNVPROC) (GLuint path, GLsizei startSegment, GLsizei numSegments);
typedef void (GLAPIENTRY * PFNGLGETPATHMETRICRANGENVPROC) (GLbitfield metricQueryMask, GLuint fistPathName, GLsizei numPaths, GLsizei stride, GLfloat* metrics);
typedef void (GLAPIENTRY * PFNGLGETPATHMETRICSNVPROC) (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLsizei stride, GLfloat *metrics);
typedef void (GLAPIENTRY * PFNGLGETPATHPARAMETERFVNVPROC) (GLuint name, GLenum param, GLfloat* value);
typedef void (GLAPIENTRY * PFNGLGETPATHPARAMETERIVNVPROC) (GLuint name, GLenum param, GLint* value);
typedef void (GLAPIENTRY * PFNGLGETPATHSPACINGNVPROC) (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat *returnedSpacing);
typedef void (GLAPIENTRY * PFNGLGETPATHTEXGENFVNVPROC) (GLenum texCoordSet, GLenum pname, GLfloat* value);
typedef void (GLAPIENTRY * PFNGLGETPATHTEXGENIVNVPROC) (GLenum texCoordSet, GLenum pname, GLint* value);
typedef void (GLAPIENTRY * PFNGLINTERPOLATEPATHSNVPROC) (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight);
typedef GLboolean (GLAPIENTRY * PFNGLISPATHNVPROC) (GLuint path);
typedef GLboolean (GLAPIENTRY * PFNGLISPOINTINFILLPATHNVPROC) (GLuint path, GLuint mask, GLfloat x, GLfloat y);
typedef GLboolean (GLAPIENTRY * PFNGLISPOINTINSTROKEPATHNVPROC) (GLuint path, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLPATHCOLORGENNVPROC) (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat* coeffs);
typedef void (GLAPIENTRY * PFNGLPATHCOMMANDSNVPROC) (GLuint path, GLsizei numCommands, const GLubyte* commands, GLsizei numCoords, GLenum coordType, const GLvoid*coords);
typedef void (GLAPIENTRY * PFNGLPATHCOORDSNVPROC) (GLuint path, GLsizei numCoords, GLenum coordType, const void* coords);
typedef void (GLAPIENTRY * PFNGLPATHCOVERDEPTHFUNCNVPROC) (GLenum zfunc);
typedef void (GLAPIENTRY * PFNGLPATHDASHARRAYNVPROC) (GLuint path, GLsizei dashCount, const GLfloat* dashArray);
typedef void (GLAPIENTRY * PFNGLPATHFOGGENNVPROC) (GLenum genMode);
typedef void (GLAPIENTRY * PFNGLPATHGLYPHRANGENVPROC) (GLuint firstPathName, GLenum fontTarget, const void* fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (GLAPIENTRY * PFNGLPATHGLYPHSNVPROC) (GLuint firstPathName, GLenum fontTarget, const void* fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const GLvoid*charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale);
typedef void (GLAPIENTRY * PFNGLPATHPARAMETERFNVPROC) (GLuint path, GLenum pname, GLfloat value);
typedef void (GLAPIENTRY * PFNGLPATHPARAMETERFVNVPROC) (GLuint path, GLenum pname, const GLfloat* value);
typedef void (GLAPIENTRY * PFNGLPATHPARAMETERINVPROC) (GLuint path, GLenum pname, GLint value);
typedef void (GLAPIENTRY * PFNGLPATHPARAMETERIVNVPROC) (GLuint path, GLenum pname, const GLint* value);
typedef void (GLAPIENTRY * PFNGLPATHSTENCILDEPTHOFFSETNVPROC) (GLfloat factor, GLfloat units);
typedef void (GLAPIENTRY * PFNGLPATHSTENCILFUNCNVPROC) (GLenum func, GLint ref, GLuint mask);
typedef void (GLAPIENTRY * PFNGLPATHSTRINGNVPROC) (GLuint path, GLenum format, GLsizei length, const void* pathString);
typedef void (GLAPIENTRY * PFNGLPATHSUBCOMMANDSNVPROC) (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte* commands, GLsizei numCoords, GLenum coordType, const GLvoid*coords);
typedef void (GLAPIENTRY * PFNGLPATHSUBCOORDSNVPROC) (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void* coords);
typedef void (GLAPIENTRY * PFNGLPATHTEXGENNVPROC) (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat* coeffs);
typedef GLboolean (GLAPIENTRY * PFNGLPOINTALONGPATHNVPROC) (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat* x, GLfloat *y, GLfloat *tangentX, GLfloat *tangentY);
typedef void (GLAPIENTRY * PFNGLSTENCILFILLPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * PFNGLSTENCILFILLPATHNVPROC) (GLuint path, GLenum fillMode, GLuint mask);
typedef void (GLAPIENTRY * PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC) (GLsizei numPaths, GLenum pathNameType, const void* paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat *transformValues);
typedef void (GLAPIENTRY * PFNGLSTENCILSTROKEPATHNVPROC) (GLuint path, GLint reference, GLuint mask);
typedef void (GLAPIENTRY * PFNGLTRANSFORMPATHNVPROC) (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat* transformValues);
typedef void (GLAPIENTRY * PFNGLWEIGHTPATHSNVPROC) (GLuint resultPath, GLsizei numPaths, const GLuint paths[], const GLfloat weights[]);

#define glCopyPathNV GLEW_GET_FUN(G_glewCopyPathNV)
#define glCoverFillPathInstancedNV GLEW_GET_FUN(G_glewCoverFillPathInstancedNV)
#define glCoverFillPathNV GLEW_GET_FUN(G_glewCoverFillPathNV)
#define glCoverStrokePathInstancedNV GLEW_GET_FUN(G_glewCoverStrokePathInstancedNV)
#define glCoverStrokePathNV GLEW_GET_FUN(G_glewCoverStrokePathNV)
#define glDeletePathsNV GLEW_GET_FUN(G_glewDeletePathsNV)
#define glGenPathsNV GLEW_GET_FUN(G_glewGenPathsNV)
#define glGetPathColorGenfvNV GLEW_GET_FUN(G_glewGetPathColorGenfvNV)
#define glGetPathColorGenivNV GLEW_GET_FUN(G_glewGetPathColorGenivNV)
#define glGetPathCommandsNV GLEW_GET_FUN(G_glewGetPathCommandsNV)
#define glGetPathCoordsNV GLEW_GET_FUN(G_glewGetPathCoordsNV)
#define glGetPathDashArrayNV GLEW_GET_FUN(G_glewGetPathDashArrayNV)
#define glGetPathLengthNV GLEW_GET_FUN(G_glewGetPathLengthNV)
#define glGetPathMetricRangeNV GLEW_GET_FUN(G_glewGetPathMetricRangeNV)
#define glGetPathMetricsNV GLEW_GET_FUN(G_glewGetPathMetricsNV)
#define glGetPathParameterfvNV GLEW_GET_FUN(G_glewGetPathParameterfvNV)
#define glGetPathParameterivNV GLEW_GET_FUN(G_glewGetPathParameterivNV)
#define glGetPathSpacingNV GLEW_GET_FUN(G_glewGetPathSpacingNV)
#define glGetPathTexGenfvNV GLEW_GET_FUN(G_glewGetPathTexGenfvNV)
#define glGetPathTexGenivNV GLEW_GET_FUN(G_glewGetPathTexGenivNV)
#define glInterpolatePathsNV GLEW_GET_FUN(G_glewInterpolatePathsNV)
#define glIsPathNV GLEW_GET_FUN(G_glewIsPathNV)
#define glIsPointInFillPathNV GLEW_GET_FUN(G_glewIsPointInFillPathNV)
#define glIsPointInStrokePathNV GLEW_GET_FUN(G_glewIsPointInStrokePathNV)
#define glPathColorGenNV GLEW_GET_FUN(G_glewPathColorGenNV)
#define glPathCommandsNV GLEW_GET_FUN(G_glewPathCommandsNV)
#define glPathCoordsNV GLEW_GET_FUN(G_glewPathCoordsNV)
#define glPathCoverDepthFuncNV GLEW_GET_FUN(G_glewPathCoverDepthFuncNV)
#define glPathDashArrayNV GLEW_GET_FUN(G_glewPathDashArrayNV)
#define glPathFogGenNV GLEW_GET_FUN(G_glewPathFogGenNV)
#define glPathGlyphRangeNV GLEW_GET_FUN(G_glewPathGlyphRangeNV)
#define glPathGlyphsNV GLEW_GET_FUN(G_glewPathGlyphsNV)
#define glPathParameterfNV GLEW_GET_FUN(G_glewPathParameterfNV)
#define glPathParameterfvNV GLEW_GET_FUN(G_glewPathParameterfvNV)
#define glPathParameteriNV GLEW_GET_FUN(G_glewPathParameteriNV)
#define glPathParameterivNV GLEW_GET_FUN(G_glewPathParameterivNV)
#define glPathStencilDepthOffsetNV GLEW_GET_FUN(G_glewPathStencilDepthOffsetNV)
#define glPathStencilFuncNV GLEW_GET_FUN(G_glewPathStencilFuncNV)
#define glPathStringNV GLEW_GET_FUN(G_glewPathStringNV)
#define glPathSubCommandsNV GLEW_GET_FUN(G_glewPathSubCommandsNV)
#define glPathSubCoordsNV GLEW_GET_FUN(G_glewPathSubCoordsNV)
#define glPathTexGenNV GLEW_GET_FUN(G_glewPathTexGenNV)
#define glPointAlongPathNV GLEW_GET_FUN(G_glewPointAlongPathNV)
#define glStencilFillPathInstancedNV GLEW_GET_FUN(G_glewStencilFillPathInstancedNV)
#define glStencilFillPathNV GLEW_GET_FUN(G_glewStencilFillPathNV)
#define glStencilStrokePathInstancedNV GLEW_GET_FUN(G_glewStencilStrokePathInstancedNV)
#define glStencilStrokePathNV GLEW_GET_FUN(G_glewStencilStrokePathNV)
#define glTransformPathNV GLEW_GET_FUN(G_glewTransformPathNV)
#define glWeightPathsNV GLEW_GET_FUN(G_glewWeightPathsNV)

#define GLEW_NV_path_rendering GLEW_GET_VAR(G_GLEW_NV_path_rendering)

#endif /* GL_NV_path_rendering */

/* ------------------------- GL_NV_pixel_data_range ------------------------ */

#ifndef GL_NV_pixel_data_range
#define GL_NV_pixel_data_range 1

#define GL_WRITE_PIXEL_DATA_RANGE_NV 0x8878
#define GL_READ_PIXEL_DATA_RANGE_NV 0x8879
#define GL_WRITE_PIXEL_DATA_RANGE_LENGTH_NV 0x887A
#define GL_READ_PIXEL_DATA_RANGE_LENGTH_NV 0x887B
#define GL_WRITE_PIXEL_DATA_RANGE_POINTER_NV 0x887C
#define GL_READ_PIXEL_DATA_RANGE_POINTER_NV 0x887D

typedef void (GLAPIENTRY * PFNGLFLUSHPIXELDATARANGENVPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLPIXELDATARANGENVPROC) (GLenum target, GLsizei length, void* pointer);

#define glFlushPixelDataRangeNV GLEW_GET_FUN(G_glewFlushPixelDataRangeNV)
#define glPixelDataRangeNV GLEW_GET_FUN(G_glewPixelDataRangeNV)

#define GLEW_NV_pixel_data_range GLEW_GET_VAR(G_GLEW_NV_pixel_data_range)

#endif /* GL_NV_pixel_data_range */

/* --------------------------- GL_NV_point_sprite -------------------------- */

#ifndef GL_NV_point_sprite
#define GL_NV_point_sprite 1

#define GL_POINT_SPRITE_NV 0x8861
#define GL_COORD_REPLACE_NV 0x8862
#define GL_POINT_SPRITE_R_MODE_NV 0x8863

typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLPOINTPARAMETERIVNVPROC) (GLenum pname, const GLint* params);

#define glPointParameteriNV GLEW_GET_FUN(G_glewPointParameteriNV)
#define glPointParameterivNV GLEW_GET_FUN(G_glewPointParameterivNV)

#define GLEW_NV_point_sprite GLEW_GET_VAR(G_GLEW_NV_point_sprite)

#endif /* GL_NV_point_sprite */

/* -------------------------- GL_NV_present_video -------------------------- */

#ifndef GL_NV_present_video
#define GL_NV_present_video 1

#define GL_FRAME_NV 0x8E26
#define GL_FIELDS_NV 0x8E27
#define GL_CURRENT_TIME_NV 0x8E28
#define GL_NUM_FILL_STREAMS_NV 0x8E29
#define GL_PRESENT_TIME_NV 0x8E2A
#define GL_PRESENT_DURATION_NV 0x8E2B

typedef void (GLAPIENTRY * PFNGLGETVIDEOI64VNVPROC) (GLuint video_slot, GLenum pname, GLint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOIVNVPROC) (GLuint video_slot, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOUI64VNVPROC) (GLuint video_slot, GLenum pname, GLuint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOUIVNVPROC) (GLuint video_slot, GLenum pname, GLuint* params);
typedef void (GLAPIENTRY * PFNGLPRESENTFRAMEDUALFILLNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3);
typedef void (GLAPIENTRY * PFNGLPRESENTFRAMEKEYEDNVPROC) (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1);

#define glGetVideoi64vNV GLEW_GET_FUN(G_glewGetVideoi64vNV)
#define glGetVideoivNV GLEW_GET_FUN(G_glewGetVideoivNV)
#define glGetVideoui64vNV GLEW_GET_FUN(G_glewGetVideoui64vNV)
#define glGetVideouivNV GLEW_GET_FUN(G_glewGetVideouivNV)
#define glPresentFrameDualFillNV GLEW_GET_FUN(G_glewPresentFrameDualFillNV)
#define glPresentFrameKeyedNV GLEW_GET_FUN(G_glewPresentFrameKeyedNV)

#define GLEW_NV_present_video GLEW_GET_VAR(G_GLEW_NV_present_video)

#endif /* GL_NV_present_video */

/* ------------------------ GL_NV_primitive_restart ------------------------ */

#ifndef GL_NV_primitive_restart
#define GL_NV_primitive_restart 1

#define GL_PRIMITIVE_RESTART_NV 0x8558
#define GL_PRIMITIVE_RESTART_INDEX_NV 0x8559

typedef void (GLAPIENTRY * PFNGLPRIMITIVERESTARTINDEXNVPROC) (GLuint index);
typedef void (GLAPIENTRY * PFNGLPRIMITIVERESTARTNVPROC) (void);

#define glPrimitiveRestartIndexNV GLEW_GET_FUN(G_glewPrimitiveRestartIndexNV)
#define glPrimitiveRestartNV GLEW_GET_FUN(G_glewPrimitiveRestartNV)

#define GLEW_NV_primitive_restart GLEW_GET_VAR(G_GLEW_NV_primitive_restart)

#endif /* GL_NV_primitive_restart */

/* ------------------------ GL_NV_register_combiners ----------------------- */

#ifndef GL_NV_register_combiners
#define GL_NV_register_combiners 1

#define GL_REGISTER_COMBINERS_NV 0x8522
#define GL_VARIABLE_A_NV 0x8523
#define GL_VARIABLE_B_NV 0x8524
#define GL_VARIABLE_C_NV 0x8525
#define GL_VARIABLE_D_NV 0x8526
#define GL_VARIABLE_E_NV 0x8527
#define GL_VARIABLE_F_NV 0x8528
#define GL_VARIABLE_G_NV 0x8529
#define GL_CONSTANT_COLOR0_NV 0x852A
#define GL_CONSTANT_COLOR1_NV 0x852B
#define GL_PRIMARY_COLOR_NV 0x852C
#define GL_SECONDARY_COLOR_NV 0x852D
#define GL_SPARE0_NV 0x852E
#define GL_SPARE1_NV 0x852F
#define GL_DISCARD_NV 0x8530
#define GL_E_TIMES_F_NV 0x8531
#define GL_SPARE0_PLUS_SECONDARY_COLOR_NV 0x8532
#define GL_UNSIGNED_IDENTITY_NV 0x8536
#define GL_UNSIGNED_INVERT_NV 0x8537
#define GL_EXPAND_NORMAL_NV 0x8538
#define GL_EXPAND_NEGATE_NV 0x8539
#define GL_HALF_BIAS_NORMAL_NV 0x853A
#define GL_HALF_BIAS_NEGATE_NV 0x853B
#define GL_SIGNED_IDENTITY_NV 0x853C
#define GL_SIGNED_NEGATE_NV 0x853D
#define GL_SCALE_BY_TWO_NV 0x853E
#define GL_SCALE_BY_FOUR_NV 0x853F
#define GL_SCALE_BY_ONE_HALF_NV 0x8540
#define GL_BIAS_BY_NEGATIVE_ONE_HALF_NV 0x8541
#define GL_COMBINER_INPUT_NV 0x8542
#define GL_COMBINER_MAPPING_NV 0x8543
#define GL_COMBINER_COMPONENT_USAGE_NV 0x8544
#define GL_COMBINER_AB_DOT_PRODUCT_NV 0x8545
#define GL_COMBINER_CD_DOT_PRODUCT_NV 0x8546
#define GL_COMBINER_MUX_SUM_NV 0x8547
#define GL_COMBINER_SCALE_NV 0x8548
#define GL_COMBINER_BIAS_NV 0x8549
#define GL_COMBINER_AB_OUTPUT_NV 0x854A
#define GL_COMBINER_CD_OUTPUT_NV 0x854B
#define GL_COMBINER_SUM_OUTPUT_NV 0x854C
#define GL_MAX_GENERAL_COMBINERS_NV 0x854D
#define GL_NUM_GENERAL_COMBINERS_NV 0x854E
#define GL_COLOR_SUM_CLAMP_NV 0x854F
#define GL_COMBINER0_NV 0x8550
#define GL_COMBINER1_NV 0x8551
#define GL_COMBINER2_NV 0x8552
#define GL_COMBINER3_NV 0x8553
#define GL_COMBINER4_NV 0x8554
#define GL_COMBINER5_NV 0x8555
#define GL_COMBINER6_NV 0x8556
#define GL_COMBINER7_NV 0x8557

typedef void (GLAPIENTRY * PFNGLCOMBINERINPUTNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (GLAPIENTRY * PFNGLCOMBINEROUTPUTNVPROC) (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERFNVPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERFVNVPROC) (GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERINVPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLCOMBINERPARAMETERIVNVPROC) (GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLFINALCOMBINERINPUTNVPROC) (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC) (GLenum stage, GLenum portion, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC) (GLenum variable, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC) (GLenum variable, GLenum pname, GLint* params);

#define glCombinerInputNV GLEW_GET_FUN(G_glewCombinerInputNV)
#define glCombinerOutputNV GLEW_GET_FUN(G_glewCombinerOutputNV)
#define glCombinerParameterfNV GLEW_GET_FUN(G_glewCombinerParameterfNV)
#define glCombinerParameterfvNV GLEW_GET_FUN(G_glewCombinerParameterfvNV)
#define glCombinerParameteriNV GLEW_GET_FUN(G_glewCombinerParameteriNV)
#define glCombinerParameterivNV GLEW_GET_FUN(G_glewCombinerParameterivNV)
#define glFinalCombinerInputNV GLEW_GET_FUN(G_glewFinalCombinerInputNV)
#define glGetCombinerInputParameterfvNV GLEW_GET_FUN(G_glewGetCombinerInputParameterfvNV)
#define glGetCombinerInputParameterivNV GLEW_GET_FUN(G_glewGetCombinerInputParameterivNV)
#define glGetCombinerOutputParameterfvNV GLEW_GET_FUN(G_glewGetCombinerOutputParameterfvNV)
#define glGetCombinerOutputParameterivNV GLEW_GET_FUN(G_glewGetCombinerOutputParameterivNV)
#define glGetFinalCombinerInputParameterfvNV GLEW_GET_FUN(G_glewGetFinalCombinerInputParameterfvNV)
#define glGetFinalCombinerInputParameterivNV GLEW_GET_FUN(G_glewGetFinalCombinerInputParameterivNV)

#define GLEW_NV_register_combiners GLEW_GET_VAR(G_GLEW_NV_register_combiners)

#endif /* GL_NV_register_combiners */

/* ----------------------- GL_NV_register_combiners2 ----------------------- */

#ifndef GL_NV_register_combiners2
#define GL_NV_register_combiners2 1

#define GL_PER_STAGE_CONSTANTS_NV 0x8535

typedef void (GLAPIENTRY * PFNGLCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC) (GLenum stage, GLenum pname, GLfloat* params);

#define glCombinerStageParameterfvNV GLEW_GET_FUN(G_glewCombinerStageParameterfvNV)
#define glGetCombinerStageParameterfvNV GLEW_GET_FUN(G_glewGetCombinerStageParameterfvNV)

#define GLEW_NV_register_combiners2 GLEW_GET_VAR(G_GLEW_NV_register_combiners2)

#endif /* GL_NV_register_combiners2 */

/* ----------------------- GL_NV_shader_atomic_float ----------------------- */

#ifndef GL_NV_shader_atomic_float
#define GL_NV_shader_atomic_float 1

#define GLEW_NV_shader_atomic_float GLEW_GET_VAR(G_GLEW_NV_shader_atomic_float)

#endif /* GL_NV_shader_atomic_float */

/* ------------------------ GL_NV_shader_buffer_load ----------------------- */

#ifndef GL_NV_shader_buffer_load
#define GL_NV_shader_buffer_load 1

#define GL_BUFFER_GPU_ADDRESS_NV 0x8F1D
#define GL_GPU_ADDRESS_NV 0x8F34
#define GL_MAX_SHADER_BUFFER_ADDRESS_NV 0x8F35

typedef void (GLAPIENTRY * PFNGLGETBUFFERPARAMETERUI64VNVPROC) (GLenum target, GLenum pname, GLuint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETINTEGERUI64VNVPROC) (GLenum value, GLuint64EXT* result);
typedef void (GLAPIENTRY * PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC) (GLuint buffer, GLenum pname, GLuint64EXT* params);
typedef GLboolean (GLAPIENTRY * PFNGLISBUFFERRESIDENTNVPROC) (GLenum target);
typedef GLboolean (GLAPIENTRY * PFNGLISNAMEDBUFFERRESIDENTNVPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMAKEBUFFERNONRESIDENTNVPROC) (GLenum target);
typedef void (GLAPIENTRY * PFNGLMAKEBUFFERRESIDENTNVPROC) (GLenum target, GLenum access);
typedef void (GLAPIENTRY * PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC) (GLuint buffer);
typedef void (GLAPIENTRY * PFNGLMAKENAMEDBUFFERRESIDENTNVPROC) (GLuint buffer, GLenum access);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMUI64NVPROC) (GLuint program, GLint location, GLuint64EXT value);
typedef void (GLAPIENTRY * PFNGLPROGRAMUNIFORMUI64VNVPROC) (GLuint program, GLint location, GLsizei count, const GLuint64EXT* value);
typedef void (GLAPIENTRY * PFNGLUNIFORMUI64NVPROC) (GLint location, GLuint64EXT value);
typedef void (GLAPIENTRY * PFNGLUNIFORMUI64VNVPROC) (GLint location, GLsizei count, const GLuint64EXT* value);

#define glGetBufferParameterui64vNV GLEW_GET_FUN(G_glewGetBufferParameterui64vNV)
#define glGetIntegerui64vNV GLEW_GET_FUN(G_glewGetIntegerui64vNV)
#define glGetNamedBufferParameterui64vNV GLEW_GET_FUN(G_glewGetNamedBufferParameterui64vNV)
#define glIsBufferResidentNV GLEW_GET_FUN(G_glewIsBufferResidentNV)
#define glIsNamedBufferResidentNV GLEW_GET_FUN(G_glewIsNamedBufferResidentNV)
#define glMakeBufferNonResidentNV GLEW_GET_FUN(G_glewMakeBufferNonResidentNV)
#define glMakeBufferResidentNV GLEW_GET_FUN(G_glewMakeBufferResidentNV)
#define glMakeNamedBufferNonResidentNV GLEW_GET_FUN(G_glewMakeNamedBufferNonResidentNV)
#define glMakeNamedBufferResidentNV GLEW_GET_FUN(G_glewMakeNamedBufferResidentNV)
#define glProgramUniformui64NV GLEW_GET_FUN(G_glewProgramUniformui64NV)
#define glProgramUniformui64vNV GLEW_GET_FUN(G_glewProgramUniformui64vNV)
#define glUniformui64NV GLEW_GET_FUN(G_glewUniformui64NV)
#define glUniformui64vNV GLEW_GET_FUN(G_glewUniformui64vNV)

#define GLEW_NV_shader_buffer_load GLEW_GET_VAR(G_GLEW_NV_shader_buffer_load)

#endif /* GL_NV_shader_buffer_load */

/* ---------------------- GL_NV_tessellation_program5 ---------------------- */

#ifndef GL_NV_tessellation_program5
#define GL_NV_tessellation_program5 1

#define GL_MAX_PROGRAM_PATCH_ATTRIBS_NV 0x86D8
#define GL_TESS_CONTROL_PROGRAM_NV 0x891E
#define GL_TESS_EVALUATION_PROGRAM_NV 0x891F
#define GL_TESS_CONTROL_PROGRAM_PARAMETER_BUFFER_NV 0x8C74
#define GL_TESS_EVALUATION_PROGRAM_PARAMETER_BUFFER_NV 0x8C75

#define GLEW_NV_tessellation_program5 GLEW_GET_VAR(G_GLEW_NV_tessellation_program5)

#endif /* GL_NV_tessellation_program5 */

/* -------------------------- GL_NV_texgen_emboss -------------------------- */

#ifndef GL_NV_texgen_emboss
#define GL_NV_texgen_emboss 1

#define GL_EMBOSS_LIGHT_NV 0x855D
#define GL_EMBOSS_CONSTANT_NV 0x855E
#define GL_EMBOSS_MAP_NV 0x855F

#define GLEW_NV_texgen_emboss GLEW_GET_VAR(G_GLEW_NV_texgen_emboss)

#endif /* GL_NV_texgen_emboss */

/* ------------------------ GL_NV_texgen_reflection ------------------------ */

#ifndef GL_NV_texgen_reflection
#define GL_NV_texgen_reflection 1

#define GL_NORMAL_MAP_NV 0x8511
#define GL_REFLECTION_MAP_NV 0x8512

#define GLEW_NV_texgen_reflection GLEW_GET_VAR(G_GLEW_NV_texgen_reflection)

#endif /* GL_NV_texgen_reflection */

/* ------------------------- GL_NV_texture_barrier ------------------------- */

#ifndef GL_NV_texture_barrier
#define GL_NV_texture_barrier 1

typedef void (GLAPIENTRY * PFNGLTEXTUREBARRIERNVPROC) (void);

#define glTextureBarrierNV GLEW_GET_FUN(G_glewTextureBarrierNV)

#define GLEW_NV_texture_barrier GLEW_GET_VAR(G_GLEW_NV_texture_barrier)

#endif /* GL_NV_texture_barrier */

/* --------------------- GL_NV_texture_compression_vtc --------------------- */

#ifndef GL_NV_texture_compression_vtc
#define GL_NV_texture_compression_vtc 1

#define GLEW_NV_texture_compression_vtc GLEW_GET_VAR(G_GLEW_NV_texture_compression_vtc)

#endif /* GL_NV_texture_compression_vtc */

/* ----------------------- GL_NV_texture_env_combine4 ---------------------- */

#ifndef GL_NV_texture_env_combine4
#define GL_NV_texture_env_combine4 1

#define GL_COMBINE4_NV 0x8503
#define GL_SOURCE3_RGB_NV 0x8583
#define GL_SOURCE3_ALPHA_NV 0x858B
#define GL_OPERAND3_RGB_NV 0x8593
#define GL_OPERAND3_ALPHA_NV 0x859B

#define GLEW_NV_texture_env_combine4 GLEW_GET_VAR(G_GLEW_NV_texture_env_combine4)

#endif /* GL_NV_texture_env_combine4 */

/* ---------------------- GL_NV_texture_expand_normal ---------------------- */

#ifndef GL_NV_texture_expand_normal
#define GL_NV_texture_expand_normal 1

#define GL_TEXTURE_UNSIGNED_REMAP_MODE_NV 0x888F

#define GLEW_NV_texture_expand_normal GLEW_GET_VAR(G_GLEW_NV_texture_expand_normal)

#endif /* GL_NV_texture_expand_normal */

/* ----------------------- GL_NV_texture_multisample ----------------------- */

#ifndef GL_NV_texture_multisample
#define GL_NV_texture_multisample 1

#define GL_TEXTURE_COVERAGE_SAMPLES_NV 0x9045
#define GL_TEXTURE_COLOR_SAMPLES_NV 0x9046

typedef void (GLAPIENTRY * PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC) (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);
typedef void (GLAPIENTRY * PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC) (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations);

#define glTexImage2DMultisampleCoverageNV GLEW_GET_FUN(G_glewTexImage2DMultisampleCoverageNV)
#define glTexImage3DMultisampleCoverageNV GLEW_GET_FUN(G_glewTexImage3DMultisampleCoverageNV)
#define glTextureImage2DMultisampleCoverageNV GLEW_GET_FUN(G_glewTextureImage2DMultisampleCoverageNV)
#define glTextureImage2DMultisampleNV GLEW_GET_FUN(G_glewTextureImage2DMultisampleNV)
#define glTextureImage3DMultisampleCoverageNV GLEW_GET_FUN(G_glewTextureImage3DMultisampleCoverageNV)
#define glTextureImage3DMultisampleNV GLEW_GET_FUN(G_glewTextureImage3DMultisampleNV)

#define GLEW_NV_texture_multisample GLEW_GET_VAR(G_GLEW_NV_texture_multisample)

#endif /* GL_NV_texture_multisample */

/* ------------------------ GL_NV_texture_rectangle ------------------------ */

#ifndef GL_NV_texture_rectangle
#define GL_NV_texture_rectangle 1

#define GL_TEXTURE_RECTANGLE_NV 0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE_NV 0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE_NV 0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE_NV 0x84F8

#define GLEW_NV_texture_rectangle GLEW_GET_VAR(G_GLEW_NV_texture_rectangle)

#endif /* GL_NV_texture_rectangle */

/* -------------------------- GL_NV_texture_shader ------------------------- */

#ifndef GL_NV_texture_shader
#define GL_NV_texture_shader 1

#define GL_OFFSET_TEXTURE_RECTANGLE_NV 0x864C
#define GL_OFFSET_TEXTURE_RECTANGLE_SCALE_NV 0x864D
#define GL_DOT_PRODUCT_TEXTURE_RECTANGLE_NV 0x864E
#define GL_RGBA_UNSIGNED_DOT_PRODUCT_MAPPING_NV 0x86D9
#define GL_UNSIGNED_INT_S8_S8_8_8_NV 0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV 0x86DB
#define GL_DSDT_MAG_INTENSITY_NV 0x86DC
#define GL_SHADER_CONSISTENT_NV 0x86DD
#define GL_TEXTURE_SHADER_NV 0x86DE
#define GL_SHADER_OPERATION_NV 0x86DF
#define GL_CULL_MODES_NV 0x86E0
#define GL_OFFSET_TEXTURE_2D_MATRIX_NV 0x86E1
#define GL_OFFSET_TEXTURE_MATRIX_NV 0x86E1
#define GL_OFFSET_TEXTURE_2D_SCALE_NV 0x86E2
#define GL_OFFSET_TEXTURE_SCALE_NV 0x86E2
#define GL_OFFSET_TEXTURE_BIAS_NV 0x86E3
#define GL_OFFSET_TEXTURE_2D_BIAS_NV 0x86E3
#define GL_PREVIOUS_TEXTURE_INPUT_NV 0x86E4
#define GL_CONST_EYE_NV 0x86E5
#define GL_PASS_THROUGH_NV 0x86E6
#define GL_CULL_FRAGMENT_NV 0x86E7
#define GL_OFFSET_TEXTURE_2D_NV 0x86E8
#define GL_DEPENDENT_AR_TEXTURE_2D_NV 0x86E9
#define GL_DEPENDENT_GB_TEXTURE_2D_NV 0x86EA
#define GL_DOT_PRODUCT_NV 0x86EC
#define GL_DOT_PRODUCT_DEPTH_REPLACE_NV 0x86ED
#define GL_DOT_PRODUCT_TEXTURE_2D_NV 0x86EE
#define GL_DOT_PRODUCT_TEXTURE_CUBE_MAP_NV 0x86F0
#define GL_DOT_PRODUCT_DIFFUSE_CUBE_MAP_NV 0x86F1
#define GL_DOT_PRODUCT_REFLECT_CUBE_MAP_NV 0x86F2
#define GL_DOT_PRODUCT_CONST_EYE_REFLECT_CUBE_MAP_NV 0x86F3
#define GL_HILO_NV 0x86F4
#define GL_DSDT_NV 0x86F5
#define GL_DSDT_MAG_NV 0x86F6
#define GL_DSDT_MAG_VIB_NV 0x86F7
#define GL_HILO16_NV 0x86F8
#define GL_SIGNED_HILO_NV 0x86F9
#define GL_SIGNED_HILO16_NV 0x86FA
#define GL_SIGNED_RGBA_NV 0x86FB
#define GL_SIGNED_RGBA8_NV 0x86FC
#define GL_SIGNED_RGB_NV 0x86FE
#define GL_SIGNED_RGB8_NV 0x86FF
#define GL_SIGNED_LUMINANCE_NV 0x8701
#define GL_SIGNED_LUMINANCE8_NV 0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV 0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV 0x8704
#define GL_SIGNED_ALPHA_NV 0x8705
#define GL_SIGNED_ALPHA8_NV 0x8706
#define GL_SIGNED_INTENSITY_NV 0x8707
#define GL_SIGNED_INTENSITY8_NV 0x8708
#define GL_DSDT8_NV 0x8709
#define GL_DSDT8_MAG8_NV 0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV 0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV 0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D
#define GL_HI_SCALE_NV 0x870E
#define GL_LO_SCALE_NV 0x870F
#define GL_DS_SCALE_NV 0x8710
#define GL_DT_SCALE_NV 0x8711
#define GL_MAGNITUDE_SCALE_NV 0x8712
#define GL_VIBRANCE_SCALE_NV 0x8713
#define GL_HI_BIAS_NV 0x8714
#define GL_LO_BIAS_NV 0x8715
#define GL_DS_BIAS_NV 0x8716
#define GL_DT_BIAS_NV 0x8717
#define GL_MAGNITUDE_BIAS_NV 0x8718
#define GL_VIBRANCE_BIAS_NV 0x8719
#define GL_TEXTURE_BORDER_VALUES_NV 0x871A
#define GL_TEXTURE_HI_SIZE_NV 0x871B
#define GL_TEXTURE_LO_SIZE_NV 0x871C
#define GL_TEXTURE_DS_SIZE_NV 0x871D
#define GL_TEXTURE_DT_SIZE_NV 0x871E
#define GL_TEXTURE_MAG_SIZE_NV 0x871F

#define GLEW_NV_texture_shader GLEW_GET_VAR(G_GLEW_NV_texture_shader)

#endif /* GL_NV_texture_shader */

/* ------------------------- GL_NV_texture_shader2 ------------------------- */

#ifndef GL_NV_texture_shader2
#define GL_NV_texture_shader2 1

#define GL_UNSIGNED_INT_S8_S8_8_8_NV 0x86DA
#define GL_UNSIGNED_INT_8_8_S8_S8_REV_NV 0x86DB
#define GL_DSDT_MAG_INTENSITY_NV 0x86DC
#define GL_DOT_PRODUCT_TEXTURE_3D_NV 0x86EF
#define GL_HILO_NV 0x86F4
#define GL_DSDT_NV 0x86F5
#define GL_DSDT_MAG_NV 0x86F6
#define GL_DSDT_MAG_VIB_NV 0x86F7
#define GL_HILO16_NV 0x86F8
#define GL_SIGNED_HILO_NV 0x86F9
#define GL_SIGNED_HILO16_NV 0x86FA
#define GL_SIGNED_RGBA_NV 0x86FB
#define GL_SIGNED_RGBA8_NV 0x86FC
#define GL_SIGNED_RGB_NV 0x86FE
#define GL_SIGNED_RGB8_NV 0x86FF
#define GL_SIGNED_LUMINANCE_NV 0x8701
#define GL_SIGNED_LUMINANCE8_NV 0x8702
#define GL_SIGNED_LUMINANCE_ALPHA_NV 0x8703
#define GL_SIGNED_LUMINANCE8_ALPHA8_NV 0x8704
#define GL_SIGNED_ALPHA_NV 0x8705
#define GL_SIGNED_ALPHA8_NV 0x8706
#define GL_SIGNED_INTENSITY_NV 0x8707
#define GL_SIGNED_INTENSITY8_NV 0x8708
#define GL_DSDT8_NV 0x8709
#define GL_DSDT8_MAG8_NV 0x870A
#define GL_DSDT8_MAG8_INTENSITY8_NV 0x870B
#define GL_SIGNED_RGB_UNSIGNED_ALPHA_NV 0x870C
#define GL_SIGNED_RGB8_UNSIGNED_ALPHA8_NV 0x870D

#define GLEW_NV_texture_shader2 GLEW_GET_VAR(G_GLEW_NV_texture_shader2)

#endif /* GL_NV_texture_shader2 */

/* ------------------------- GL_NV_texture_shader3 ------------------------- */

#ifndef GL_NV_texture_shader3
#define GL_NV_texture_shader3 1

#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_NV 0x8850
#define GL_OFFSET_PROJECTIVE_TEXTURE_2D_SCALE_NV 0x8851
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8852
#define GL_OFFSET_PROJECTIVE_TEXTURE_RECTANGLE_SCALE_NV 0x8853
#define GL_OFFSET_HILO_TEXTURE_2D_NV 0x8854
#define GL_OFFSET_HILO_TEXTURE_RECTANGLE_NV 0x8855
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_2D_NV 0x8856
#define GL_OFFSET_HILO_PROJECTIVE_TEXTURE_RECTANGLE_NV 0x8857
#define GL_DEPENDENT_HILO_TEXTURE_2D_NV 0x8858
#define GL_DEPENDENT_RGB_TEXTURE_3D_NV 0x8859
#define GL_DEPENDENT_RGB_TEXTURE_CUBE_MAP_NV 0x885A
#define GL_DOT_PRODUCT_PASS_THROUGH_NV 0x885B
#define GL_DOT_PRODUCT_TEXTURE_1D_NV 0x885C
#define GL_DOT_PRODUCT_AFFINE_DEPTH_REPLACE_NV 0x885D
#define GL_HILO8_NV 0x885E
#define GL_SIGNED_HILO8_NV 0x885F
#define GL_FORCE_BLUE_TO_ONE_NV 0x8860

#define GLEW_NV_texture_shader3 GLEW_GET_VAR(G_GLEW_NV_texture_shader3)

#endif /* GL_NV_texture_shader3 */

/* ------------------------ GL_NV_transform_feedback ----------------------- */

#ifndef GL_NV_transform_feedback
#define GL_NV_transform_feedback 1

#define GL_BACK_PRIMARY_COLOR_NV 0x8C77
#define GL_BACK_SECONDARY_COLOR_NV 0x8C78
#define GL_TEXTURE_COORD_NV 0x8C79
#define GL_CLIP_DISTANCE_NV 0x8C7A
#define GL_VERTEX_ID_NV 0x8C7B
#define GL_PRIMITIVE_ID_NV 0x8C7C
#define GL_GENERIC_ATTRIB_NV 0x8C7D
#define GL_TRANSFORM_FEEDBACK_ATTRIBS_NV 0x8C7E
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE_NV 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS_NV 0x8C80
#define GL_ACTIVE_VARYINGS_NV 0x8C81
#define GL_ACTIVE_VARYING_MAX_LENGTH_NV 0x8C82
#define GL_TRANSFORM_FEEDBACK_VARYINGS_NV 0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START_NV 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE_NV 0x8C85
#define GL_TRANSFORM_FEEDBACK_RECORD_NV 0x8C86
#define GL_PRIMITIVES_GENERATED_NV 0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN_NV 0x8C88
#define GL_RASTERIZER_DISCARD_NV 0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS_NV 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS_NV 0x8C8B
#define GL_INTERLEAVED_ATTRIBS_NV 0x8C8C
#define GL_SEPARATE_ATTRIBS_NV 0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER_NV 0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING_NV 0x8C8F

typedef void (GLAPIENTRY * PFNGLACTIVEVARYINGNVPROC) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLBEGINTRANSFORMFEEDBACKNVPROC) (GLenum primitiveMode);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERBASENVPROC) (GLenum target, GLuint index, GLuint buffer);
typedef void (GLAPIENTRY * PFNGLBINDBUFFEROFFSETNVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset);
typedef void (GLAPIENTRY * PFNGLBINDBUFFERRANGENVPROC) (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void (GLAPIENTRY * PFNGLENDTRANSFORMFEEDBACKNVPROC) (void);
typedef void (GLAPIENTRY * PFNGLGETACTIVEVARYINGNVPROC) (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
typedef void (GLAPIENTRY * PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC) (GLuint program, GLuint index, GLint *location);
typedef GLint (GLAPIENTRY * PFNGLGETVARYINGLOCATIONNVPROC) (GLuint program, const GLchar *name);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC) (GLuint count, const GLint *attribs, GLenum bufferMode);
typedef void (GLAPIENTRY * PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC) (GLuint program, GLsizei count, const GLint *locations, GLenum bufferMode);

#define glActiveVaryingNV GLEW_GET_FUN(G_glewActiveVaryingNV)
#define glBeginTransformFeedbackNV GLEW_GET_FUN(G_glewBeginTransformFeedbackNV)
#define glBindBufferBaseNV GLEW_GET_FUN(G_glewBindBufferBaseNV)
#define glBindBufferOffsetNV GLEW_GET_FUN(G_glewBindBufferOffsetNV)
#define glBindBufferRangeNV GLEW_GET_FUN(G_glewBindBufferRangeNV)
#define glEndTransformFeedbackNV GLEW_GET_FUN(G_glewEndTransformFeedbackNV)
#define glGetActiveVaryingNV GLEW_GET_FUN(G_glewGetActiveVaryingNV)
#define glGetTransformFeedbackVaryingNV GLEW_GET_FUN(G_glewGetTransformFeedbackVaryingNV)
#define glGetVaryingLocationNV GLEW_GET_FUN(G_glewGetVaryingLocationNV)
#define glTransformFeedbackAttribsNV GLEW_GET_FUN(G_glewTransformFeedbackAttribsNV)
#define glTransformFeedbackVaryingsNV GLEW_GET_FUN(G_glewTransformFeedbackVaryingsNV)

#define GLEW_NV_transform_feedback GLEW_GET_VAR(G_GLEW_NV_transform_feedback)

#endif /* GL_NV_transform_feedback */

/* ----------------------- GL_NV_transform_feedback2 ----------------------- */

#ifndef GL_NV_transform_feedback2
#define GL_NV_transform_feedback2 1

#define GL_TRANSFORM_FEEDBACK_NV 0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED_NV 0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE_NV 0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING_NV 0x8E25

typedef void (GLAPIENTRY * PFNGLBINDTRANSFORMFEEDBACKNVPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETETRANSFORMFEEDBACKSNVPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLDRAWTRANSFORMFEEDBACKNVPROC) (GLenum mode, GLuint id);
typedef void (GLAPIENTRY * PFNGLGENTRANSFORMFEEDBACKSNVPROC) (GLsizei n, GLuint* ids);
typedef GLboolean (GLAPIENTRY * PFNGLISTRANSFORMFEEDBACKNVPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLPAUSETRANSFORMFEEDBACKNVPROC) (void);
typedef void (GLAPIENTRY * PFNGLRESUMETRANSFORMFEEDBACKNVPROC) (void);

#define glBindTransformFeedbackNV GLEW_GET_FUN(G_glewBindTransformFeedbackNV)
#define glDeleteTransformFeedbacksNV GLEW_GET_FUN(G_glewDeleteTransformFeedbacksNV)
#define glDrawTransformFeedbackNV GLEW_GET_FUN(G_glewDrawTransformFeedbackNV)
#define glGenTransformFeedbacksNV GLEW_GET_FUN(G_glewGenTransformFeedbacksNV)
#define glIsTransformFeedbackNV GLEW_GET_FUN(G_glewIsTransformFeedbackNV)
#define glPauseTransformFeedbackNV GLEW_GET_FUN(G_glewPauseTransformFeedbackNV)
#define glResumeTransformFeedbackNV GLEW_GET_FUN(G_glewResumeTransformFeedbackNV)

#define GLEW_NV_transform_feedback2 GLEW_GET_VAR(G_GLEW_NV_transform_feedback2)

#endif /* GL_NV_transform_feedback2 */

/* -------------------------- GL_NV_vdpau_interop -------------------------- */

#ifndef GL_NV_vdpau_interop
#define GL_NV_vdpau_interop 1

#define GL_SURFACE_STATE_NV 0x86EB
#define GL_SURFACE_REGISTERED_NV 0x86FD
#define GL_SURFACE_MAPPED_NV 0x8700
#define GL_WRITE_DISCARD_NV 0x88BE

typedef GLintptr GLvdpauSurfaceNV;

typedef void (GLAPIENTRY * PFNGLVDPAUFININVPROC) (void);
typedef void (GLAPIENTRY * PFNGLVDPAUGETSURFACEIVNVPROC) (GLvdpauSurfaceNV surface, GLenum pname, GLsizei bufSize, GLsizei* length, GLint *values);
typedef void (GLAPIENTRY * PFNGLVDPAUINITNVPROC) (const void* vdpDevice, const GLvoid*getProcAddress);
typedef void (GLAPIENTRY * PFNGLVDPAUISSURFACENVPROC) (GLvdpauSurfaceNV surface);
typedef void (GLAPIENTRY * PFNGLVDPAUMAPSURFACESNVPROC) (GLsizei numSurfaces, const GLvdpauSurfaceNV* surfaces);
typedef GLvdpauSurfaceNV (GLAPIENTRY * PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC) (const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
typedef GLvdpauSurfaceNV (GLAPIENTRY * PFNGLVDPAUREGISTERVIDEOSURFACENVPROC) (const void* vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint *textureNames);
typedef void (GLAPIENTRY * PFNGLVDPAUSURFACEACCESSNVPROC) (GLvdpauSurfaceNV surface, GLenum access);
typedef void (GLAPIENTRY * PFNGLVDPAUUNMAPSURFACESNVPROC) (GLsizei numSurface, const GLvdpauSurfaceNV* surfaces);
typedef void (GLAPIENTRY * PFNGLVDPAUUNREGISTERSURFACENVPROC) (GLvdpauSurfaceNV surface);

#define glVDPAUFiniNV GLEW_GET_FUN(G_glewVDPAUFiniNV)
#define glVDPAUGetSurfaceivNV GLEW_GET_FUN(G_glewVDPAUGetSurfaceivNV)
#define glVDPAUInitNV GLEW_GET_FUN(G_glewVDPAUInitNV)
#define glVDPAUIsSurfaceNV GLEW_GET_FUN(G_glewVDPAUIsSurfaceNV)
#define glVDPAUMapSurfacesNV GLEW_GET_FUN(G_glewVDPAUMapSurfacesNV)
#define glVDPAURegisterOutputSurfaceNV GLEW_GET_FUN(G_glewVDPAURegisterOutputSurfaceNV)
#define glVDPAURegisterVideoSurfaceNV GLEW_GET_FUN(G_glewVDPAURegisterVideoSurfaceNV)
#define glVDPAUSurfaceAccessNV GLEW_GET_FUN(G_glewVDPAUSurfaceAccessNV)
#define glVDPAUUnmapSurfacesNV GLEW_GET_FUN(G_glewVDPAUUnmapSurfacesNV)
#define glVDPAUUnregisterSurfaceNV GLEW_GET_FUN(G_glewVDPAUUnregisterSurfaceNV)

#define GLEW_NV_vdpau_interop GLEW_GET_VAR(G_GLEW_NV_vdpau_interop)

#endif /* GL_NV_vdpau_interop */

/* ------------------------ GL_NV_vertex_array_range ----------------------- */

#ifndef GL_NV_vertex_array_range
#define GL_NV_vertex_array_range 1

#define GL_VERTEX_ARRAY_RANGE_NV 0x851D
#define GL_VERTEX_ARRAY_RANGE_LENGTH_NV 0x851E
#define GL_VERTEX_ARRAY_RANGE_VALID_NV 0x851F
#define GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV 0x8520
#define GL_VERTEX_ARRAY_RANGE_POINTER_NV 0x8521

typedef void (GLAPIENTRY * PFNGLFLUSHVERTEXARRAYRANGENVPROC) (void);
typedef void (GLAPIENTRY * PFNGLVERTEXARRAYRANGENVPROC) (GLsizei length, void* pointer);

#define glFlushVertexArrayRangeNV GLEW_GET_FUN(G_glewFlushVertexArrayRangeNV)
#define glVertexArrayRangeNV GLEW_GET_FUN(G_glewVertexArrayRangeNV)

#define GLEW_NV_vertex_array_range GLEW_GET_VAR(G_GLEW_NV_vertex_array_range)

#endif /* GL_NV_vertex_array_range */

/* ----------------------- GL_NV_vertex_array_range2 ----------------------- */

#ifndef GL_NV_vertex_array_range2
#define GL_NV_vertex_array_range2 1

#define GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV 0x8533

#define GLEW_NV_vertex_array_range2 GLEW_GET_VAR(G_GLEW_NV_vertex_array_range2)

#endif /* GL_NV_vertex_array_range2 */

/* ------------------- GL_NV_vertex_attrib_integer_64bit ------------------- */

#ifndef GL_NV_vertex_attrib_integer_64bit
#define GL_NV_vertex_attrib_integer_64bit 1

#define GL_INT64_NV 0x140E
#define GL_UNSIGNED_INT64_NV 0x140F

typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLI64VNVPROC) (GLuint index, GLenum pname, GLint64EXT* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBLUI64VNVPROC) (GLuint index, GLenum pname, GLuint64EXT* params);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1I64NVPROC) (GLuint index, GLint64EXT x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1UI64NVPROC) (GLuint index, GLuint64EXT x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL1UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL2UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL3UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4I64NVPROC) (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4I64VNVPROC) (GLuint index, const GLint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4UI64NVPROC) (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBL4UI64VNVPROC) (GLuint index, const GLuint64EXT* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBLFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);

#define glGetVertexAttribLi64vNV GLEW_GET_FUN(G_glewGetVertexAttribLi64vNV)
#define glGetVertexAttribLui64vNV GLEW_GET_FUN(G_glewGetVertexAttribLui64vNV)
#define glVertexAttribL1i64NV GLEW_GET_FUN(G_glewVertexAttribL1i64NV)
#define glVertexAttribL1i64vNV GLEW_GET_FUN(G_glewVertexAttribL1i64vNV)
#define glVertexAttribL1ui64NV GLEW_GET_FUN(G_glewVertexAttribL1ui64NV)
#define glVertexAttribL1ui64vNV GLEW_GET_FUN(G_glewVertexAttribL1ui64vNV)
#define glVertexAttribL2i64NV GLEW_GET_FUN(G_glewVertexAttribL2i64NV)
#define glVertexAttribL2i64vNV GLEW_GET_FUN(G_glewVertexAttribL2i64vNV)
#define glVertexAttribL2ui64NV GLEW_GET_FUN(G_glewVertexAttribL2ui64NV)
#define glVertexAttribL2ui64vNV GLEW_GET_FUN(G_glewVertexAttribL2ui64vNV)
#define glVertexAttribL3i64NV GLEW_GET_FUN(G_glewVertexAttribL3i64NV)
#define glVertexAttribL3i64vNV GLEW_GET_FUN(G_glewVertexAttribL3i64vNV)
#define glVertexAttribL3ui64NV GLEW_GET_FUN(G_glewVertexAttribL3ui64NV)
#define glVertexAttribL3ui64vNV GLEW_GET_FUN(G_glewVertexAttribL3ui64vNV)
#define glVertexAttribL4i64NV GLEW_GET_FUN(G_glewVertexAttribL4i64NV)
#define glVertexAttribL4i64vNV GLEW_GET_FUN(G_glewVertexAttribL4i64vNV)
#define glVertexAttribL4ui64NV GLEW_GET_FUN(G_glewVertexAttribL4ui64NV)
#define glVertexAttribL4ui64vNV GLEW_GET_FUN(G_glewVertexAttribL4ui64vNV)
#define glVertexAttribLFormatNV GLEW_GET_FUN(G_glewVertexAttribLFormatNV)

#define GLEW_NV_vertex_attrib_integer_64bit GLEW_GET_VAR(G_GLEW_NV_vertex_attrib_integer_64bit)

#endif /* GL_NV_vertex_attrib_integer_64bit */

/* ------------------- GL_NV_vertex_buffer_unified_memory ------------------ */

#ifndef GL_NV_vertex_buffer_unified_memory
#define GL_NV_vertex_buffer_unified_memory 1

#define GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV 0x8F1E
#define GL_ELEMENT_ARRAY_UNIFIED_NV 0x8F1F
#define GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV 0x8F20
#define GL_VERTEX_ARRAY_ADDRESS_NV 0x8F21
#define GL_NORMAL_ARRAY_ADDRESS_NV 0x8F22
#define GL_COLOR_ARRAY_ADDRESS_NV 0x8F23
#define GL_INDEX_ARRAY_ADDRESS_NV 0x8F24
#define GL_TEXTURE_COORD_ARRAY_ADDRESS_NV 0x8F25
#define GL_EDGE_FLAG_ARRAY_ADDRESS_NV 0x8F26
#define GL_SECONDARY_COLOR_ARRAY_ADDRESS_NV 0x8F27
#define GL_FOG_COORD_ARRAY_ADDRESS_NV 0x8F28
#define GL_ELEMENT_ARRAY_ADDRESS_NV 0x8F29
#define GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV 0x8F2A
#define GL_VERTEX_ARRAY_LENGTH_NV 0x8F2B
#define GL_NORMAL_ARRAY_LENGTH_NV 0x8F2C
#define GL_COLOR_ARRAY_LENGTH_NV 0x8F2D
#define GL_INDEX_ARRAY_LENGTH_NV 0x8F2E
#define GL_TEXTURE_COORD_ARRAY_LENGTH_NV 0x8F2F
#define GL_EDGE_FLAG_ARRAY_LENGTH_NV 0x8F30
#define GL_SECONDARY_COLOR_ARRAY_LENGTH_NV 0x8F31
#define GL_FOG_COORD_ARRAY_LENGTH_NV 0x8F32
#define GL_ELEMENT_ARRAY_LENGTH_NV 0x8F33
#define GL_DRAW_INDIRECT_UNIFIED_NV 0x8F40
#define GL_DRAW_INDIRECT_ADDRESS_NV 0x8F41
#define GL_DRAW_INDIRECT_LENGTH_NV 0x8F42

typedef void (GLAPIENTRY * PFNGLBUFFERADDRESSRANGENVPROC) (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length);
typedef void (GLAPIENTRY * PFNGLCOLORFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLEDGEFLAGFORMATNVPROC) (GLsizei stride);
typedef void (GLAPIENTRY * PFNGLFOGCOORDFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLGETINTEGERUI64I_VNVPROC) (GLenum value, GLuint index, GLuint64EXT result[]);
typedef void (GLAPIENTRY * PFNGLINDEXFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLNORMALFORMATNVPROC) (GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLSECONDARYCOLORFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLTEXCOORDFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBIFORMATNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride);
typedef void (GLAPIENTRY * PFNGLVERTEXFORMATNVPROC) (GLint size, GLenum type, GLsizei stride);

#define glBufferAddressRangeNV GLEW_GET_FUN(G_glewBufferAddressRangeNV)
#define glColorFormatNV GLEW_GET_FUN(G_glewColorFormatNV)
#define glEdgeFlagFormatNV GLEW_GET_FUN(G_glewEdgeFlagFormatNV)
#define glFogCoordFormatNV GLEW_GET_FUN(G_glewFogCoordFormatNV)
#define glGetIntegerui64i_vNV GLEW_GET_FUN(G_glewGetIntegerui64i_vNV)
#define glIndexFormatNV GLEW_GET_FUN(G_glewIndexFormatNV)
#define glNormalFormatNV GLEW_GET_FUN(G_glewNormalFormatNV)
#define glSecondaryColorFormatNV GLEW_GET_FUN(G_glewSecondaryColorFormatNV)
#define glTexCoordFormatNV GLEW_GET_FUN(G_glewTexCoordFormatNV)
#define glVertexAttribFormatNV GLEW_GET_FUN(G_glewVertexAttribFormatNV)
#define glVertexAttribIFormatNV GLEW_GET_FUN(G_glewVertexAttribIFormatNV)
#define glVertexFormatNV GLEW_GET_FUN(G_glewVertexFormatNV)

#define GLEW_NV_vertex_buffer_unified_memory GLEW_GET_VAR(G_GLEW_NV_vertex_buffer_unified_memory)

#endif /* GL_NV_vertex_buffer_unified_memory */

/* -------------------------- GL_NV_vertex_program ------------------------- */

#ifndef GL_NV_vertex_program
#define GL_NV_vertex_program 1

#define GL_VERTEX_PROGRAM_NV 0x8620
#define GL_VERTEX_STATE_PROGRAM_NV 0x8621
#define GL_ATTRIB_ARRAY_SIZE_NV 0x8623
#define GL_ATTRIB_ARRAY_STRIDE_NV 0x8624
#define GL_ATTRIB_ARRAY_TYPE_NV 0x8625
#define GL_CURRENT_ATTRIB_NV 0x8626
#define GL_PROGRAM_LENGTH_NV 0x8627
#define GL_PROGRAM_STRING_NV 0x8628
#define GL_MODELVIEW_PROJECTION_NV 0x8629
#define GL_IDENTITY_NV 0x862A
#define GL_INVERSE_NV 0x862B
#define GL_TRANSPOSE_NV 0x862C
#define GL_INVERSE_TRANSPOSE_NV 0x862D
#define GL_MAX_TRACK_MATRIX_STACK_DEPTH_NV 0x862E
#define GL_MAX_TRACK_MATRICES_NV 0x862F
#define GL_MATRIX0_NV 0x8630
#define GL_MATRIX1_NV 0x8631
#define GL_MATRIX2_NV 0x8632
#define GL_MATRIX3_NV 0x8633
#define GL_MATRIX4_NV 0x8634
#define GL_MATRIX5_NV 0x8635
#define GL_MATRIX6_NV 0x8636
#define GL_MATRIX7_NV 0x8637
#define GL_CURRENT_MATRIX_STACK_DEPTH_NV 0x8640
#define GL_CURRENT_MATRIX_NV 0x8641
#define GL_VERTEX_PROGRAM_POINT_SIZE_NV 0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_NV 0x8643
#define GL_PROGRAM_PARAMETER_NV 0x8644
#define GL_ATTRIB_ARRAY_POINTER_NV 0x8645
#define GL_PROGRAM_TARGET_NV 0x8646
#define GL_PROGRAM_RESIDENT_NV 0x8647
#define GL_TRACK_MATRIX_NV 0x8648
#define GL_TRACK_MATRIX_TRANSFORM_NV 0x8649
#define GL_VERTEX_PROGRAM_BINDING_NV 0x864A
#define GL_PROGRAM_ERROR_POSITION_NV 0x864B
#define GL_VERTEX_ATTRIB_ARRAY0_NV 0x8650
#define GL_VERTEX_ATTRIB_ARRAY1_NV 0x8651
#define GL_VERTEX_ATTRIB_ARRAY2_NV 0x8652
#define GL_VERTEX_ATTRIB_ARRAY3_NV 0x8653
#define GL_VERTEX_ATTRIB_ARRAY4_NV 0x8654
#define GL_VERTEX_ATTRIB_ARRAY5_NV 0x8655
#define GL_VERTEX_ATTRIB_ARRAY6_NV 0x8656
#define GL_VERTEX_ATTRIB_ARRAY7_NV 0x8657
#define GL_VERTEX_ATTRIB_ARRAY8_NV 0x8658
#define GL_VERTEX_ATTRIB_ARRAY9_NV 0x8659
#define GL_VERTEX_ATTRIB_ARRAY10_NV 0x865A
#define GL_VERTEX_ATTRIB_ARRAY11_NV 0x865B
#define GL_VERTEX_ATTRIB_ARRAY12_NV 0x865C
#define GL_VERTEX_ATTRIB_ARRAY13_NV 0x865D
#define GL_VERTEX_ATTRIB_ARRAY14_NV 0x865E
#define GL_VERTEX_ATTRIB_ARRAY15_NV 0x865F
#define GL_MAP1_VERTEX_ATTRIB0_4_NV 0x8660
#define GL_MAP1_VERTEX_ATTRIB1_4_NV 0x8661
#define GL_MAP1_VERTEX_ATTRIB2_4_NV 0x8662
#define GL_MAP1_VERTEX_ATTRIB3_4_NV 0x8663
#define GL_MAP1_VERTEX_ATTRIB4_4_NV 0x8664
#define GL_MAP1_VERTEX_ATTRIB5_4_NV 0x8665
#define GL_MAP1_VERTEX_ATTRIB6_4_NV 0x8666
#define GL_MAP1_VERTEX_ATTRIB7_4_NV 0x8667
#define GL_MAP1_VERTEX_ATTRIB8_4_NV 0x8668
#define GL_MAP1_VERTEX_ATTRIB9_4_NV 0x8669
#define GL_MAP1_VERTEX_ATTRIB10_4_NV 0x866A
#define GL_MAP1_VERTEX_ATTRIB11_4_NV 0x866B
#define GL_MAP1_VERTEX_ATTRIB12_4_NV 0x866C
#define GL_MAP1_VERTEX_ATTRIB13_4_NV 0x866D
#define GL_MAP1_VERTEX_ATTRIB14_4_NV 0x866E
#define GL_MAP1_VERTEX_ATTRIB15_4_NV 0x866F
#define GL_MAP2_VERTEX_ATTRIB0_4_NV 0x8670
#define GL_MAP2_VERTEX_ATTRIB1_4_NV 0x8671
#define GL_MAP2_VERTEX_ATTRIB2_4_NV 0x8672
#define GL_MAP2_VERTEX_ATTRIB3_4_NV 0x8673
#define GL_MAP2_VERTEX_ATTRIB4_4_NV 0x8674
#define GL_MAP2_VERTEX_ATTRIB5_4_NV 0x8675
#define GL_MAP2_VERTEX_ATTRIB6_4_NV 0x8676
#define GL_MAP2_VERTEX_ATTRIB7_4_NV 0x8677
#define GL_MAP2_VERTEX_ATTRIB8_4_NV 0x8678
#define GL_MAP2_VERTEX_ATTRIB9_4_NV 0x8679
#define GL_MAP2_VERTEX_ATTRIB10_4_NV 0x867A
#define GL_MAP2_VERTEX_ATTRIB11_4_NV 0x867B
#define GL_MAP2_VERTEX_ATTRIB12_4_NV 0x867C
#define GL_MAP2_VERTEX_ATTRIB13_4_NV 0x867D
#define GL_MAP2_VERTEX_ATTRIB14_4_NV 0x867E
#define GL_MAP2_VERTEX_ATTRIB15_4_NV 0x867F

typedef GLboolean (GLAPIENTRY * PFNGLAREPROGRAMSRESIDENTNVPROC) (GLsizei n, const GLuint* ids, GLboolean *residences);
typedef void (GLAPIENTRY * PFNGLBINDPROGRAMNVPROC) (GLenum target, GLuint id);
typedef void (GLAPIENTRY * PFNGLDELETEPROGRAMSNVPROC) (GLsizei n, const GLuint* ids);
typedef void (GLAPIENTRY * PFNGLEXECUTEPROGRAMNVPROC) (GLenum target, GLuint id, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGENPROGRAMSNVPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPARAMETERDVNVPROC) (GLenum target, GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMPARAMETERFVNVPROC) (GLenum target, GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMSTRINGNVPROC) (GLuint id, GLenum pname, GLubyte* program);
typedef void (GLAPIENTRY * PFNGLGETPROGRAMIVNVPROC) (GLuint id, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETTRACKMATRIXIVNVPROC) (GLenum target, GLuint address, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBPOINTERVNVPROC) (GLuint index, GLenum pname, GLvoid** pointer);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBDVNVPROC) (GLuint index, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBFVNVPROC) (GLuint index, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVERTEXATTRIBIVNVPROC) (GLuint index, GLenum pname, GLint* params);
typedef GLboolean (GLAPIENTRY * PFNGLISPROGRAMNVPROC) (GLuint id);
typedef void (GLAPIENTRY * PFNGLLOADPROGRAMNVPROC) (GLenum target, GLuint id, GLsizei len, const GLubyte* program);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4DNVPROC) (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4DVNVPROC) (GLenum target, GLuint index, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4FNVPROC) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETER4FVNVPROC) (GLenum target, GLuint index, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERS4DVNVPROC) (GLenum target, GLuint index, GLsizei num, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLPROGRAMPARAMETERS4FVNVPROC) (GLenum target, GLuint index, GLsizei num, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLREQUESTRESIDENTPROGRAMSNVPROC) (GLsizei n, GLuint* ids);
typedef void (GLAPIENTRY * PFNGLTRACKMATRIXNVPROC) (GLenum target, GLuint address, GLenum matrix, GLenum transform);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DNVPROC) (GLuint index, GLdouble x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1DVNVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FNVPROC) (GLuint index, GLfloat x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1FVNVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SNVPROC) (GLuint index, GLshort x);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB1SVNVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DNVPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2DVNVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FNVPROC) (GLuint index, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2FVNVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SNVPROC) (GLuint index, GLshort x, GLshort y);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB2SVNVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3DVNVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3FVNVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB3SVNVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DNVPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4DVNVPROC) (GLuint index, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FNVPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4FVNVPROC) (GLuint index, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SNVPROC) (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4SVNVPROC) (GLuint index, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBNVPROC) (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIB4UBVNVPROC) (GLuint index, const GLubyte* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBPOINTERNVPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void* pointer);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS1SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS2SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS3SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4DVNVPROC) (GLuint index, GLsizei n, const GLdouble* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4FVNVPROC) (GLuint index, GLsizei n, const GLfloat* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4SVNVPROC) (GLuint index, GLsizei n, const GLshort* v);
typedef void (GLAPIENTRY * PFNGLVERTEXATTRIBS4UBVNVPROC) (GLuint index, GLsizei n, const GLubyte* v);

#define glAreProgramsResidentNV GLEW_GET_FUN(G_glewAreProgramsResidentNV)
#define glBindProgramNV GLEW_GET_FUN(G_glewBindProgramNV)
#define glDeleteProgramsNV GLEW_GET_FUN(G_glewDeleteProgramsNV)
#define glExecuteProgramNV GLEW_GET_FUN(G_glewExecuteProgramNV)
#define glGenProgramsNV GLEW_GET_FUN(G_glewGenProgramsNV)
#define glGetProgramParameterdvNV GLEW_GET_FUN(G_glewGetProgramParameterdvNV)
#define glGetProgramParameterfvNV GLEW_GET_FUN(G_glewGetProgramParameterfvNV)
#define glGetProgramStringNV GLEW_GET_FUN(G_glewGetProgramStringNV)
#define glGetProgramivNV GLEW_GET_FUN(G_glewGetProgramivNV)
#define glGetTrackMatrixivNV GLEW_GET_FUN(G_glewGetTrackMatrixivNV)
#define glGetVertexAttribPointervNV GLEW_GET_FUN(G_glewGetVertexAttribPointervNV)
#define glGetVertexAttribdvNV GLEW_GET_FUN(G_glewGetVertexAttribdvNV)
#define glGetVertexAttribfvNV GLEW_GET_FUN(G_glewGetVertexAttribfvNV)
#define glGetVertexAttribivNV GLEW_GET_FUN(G_glewGetVertexAttribivNV)
#define glIsProgramNV GLEW_GET_FUN(G_glewIsProgramNV)
#define glLoadProgramNV GLEW_GET_FUN(G_glewLoadProgramNV)
#define glProgramParameter4dNV GLEW_GET_FUN(G_glewProgramParameter4dNV)
#define glProgramParameter4dvNV GLEW_GET_FUN(G_glewProgramParameter4dvNV)
#define glProgramParameter4fNV GLEW_GET_FUN(G_glewProgramParameter4fNV)
#define glProgramParameter4fvNV GLEW_GET_FUN(G_glewProgramParameter4fvNV)
#define glProgramParameters4dvNV GLEW_GET_FUN(G_glewProgramParameters4dvNV)
#define glProgramParameters4fvNV GLEW_GET_FUN(G_glewProgramParameters4fvNV)
#define glRequestResidentProgramsNV GLEW_GET_FUN(G_glewRequestResidentProgramsNV)
#define glTrackMatrixNV GLEW_GET_FUN(G_glewTrackMatrixNV)
#define glVertexAttrib1dNV GLEW_GET_FUN(G_glewVertexAttrib1dNV)
#define glVertexAttrib1dvNV GLEW_GET_FUN(G_glewVertexAttrib1dvNV)
#define glVertexAttrib1fNV GLEW_GET_FUN(G_glewVertexAttrib1fNV)
#define glVertexAttrib1fvNV GLEW_GET_FUN(G_glewVertexAttrib1fvNV)
#define glVertexAttrib1sNV GLEW_GET_FUN(G_glewVertexAttrib1sNV)
#define glVertexAttrib1svNV GLEW_GET_FUN(G_glewVertexAttrib1svNV)
#define glVertexAttrib2dNV GLEW_GET_FUN(G_glewVertexAttrib2dNV)
#define glVertexAttrib2dvNV GLEW_GET_FUN(G_glewVertexAttrib2dvNV)
#define glVertexAttrib2fNV GLEW_GET_FUN(G_glewVertexAttrib2fNV)
#define glVertexAttrib2fvNV GLEW_GET_FUN(G_glewVertexAttrib2fvNV)
#define glVertexAttrib2sNV GLEW_GET_FUN(G_glewVertexAttrib2sNV)
#define glVertexAttrib2svNV GLEW_GET_FUN(G_glewVertexAttrib2svNV)
#define glVertexAttrib3dNV GLEW_GET_FUN(G_glewVertexAttrib3dNV)
#define glVertexAttrib3dvNV GLEW_GET_FUN(G_glewVertexAttrib3dvNV)
#define glVertexAttrib3fNV GLEW_GET_FUN(G_glewVertexAttrib3fNV)
#define glVertexAttrib3fvNV GLEW_GET_FUN(G_glewVertexAttrib3fvNV)
#define glVertexAttrib3sNV GLEW_GET_FUN(G_glewVertexAttrib3sNV)
#define glVertexAttrib3svNV GLEW_GET_FUN(G_glewVertexAttrib3svNV)
#define glVertexAttrib4dNV GLEW_GET_FUN(G_glewVertexAttrib4dNV)
#define glVertexAttrib4dvNV GLEW_GET_FUN(G_glewVertexAttrib4dvNV)
#define glVertexAttrib4fNV GLEW_GET_FUN(G_glewVertexAttrib4fNV)
#define glVertexAttrib4fvNV GLEW_GET_FUN(G_glewVertexAttrib4fvNV)
#define glVertexAttrib4sNV GLEW_GET_FUN(G_glewVertexAttrib4sNV)
#define glVertexAttrib4svNV GLEW_GET_FUN(G_glewVertexAttrib4svNV)
#define glVertexAttrib4ubNV GLEW_GET_FUN(G_glewVertexAttrib4ubNV)
#define glVertexAttrib4ubvNV GLEW_GET_FUN(G_glewVertexAttrib4ubvNV)
#define glVertexAttribPointerNV GLEW_GET_FUN(G_glewVertexAttribPointerNV)
#define glVertexAttribs1dvNV GLEW_GET_FUN(G_glewVertexAttribs1dvNV)
#define glVertexAttribs1fvNV GLEW_GET_FUN(G_glewVertexAttribs1fvNV)
#define glVertexAttribs1svNV GLEW_GET_FUN(G_glewVertexAttribs1svNV)
#define glVertexAttribs2dvNV GLEW_GET_FUN(G_glewVertexAttribs2dvNV)
#define glVertexAttribs2fvNV GLEW_GET_FUN(G_glewVertexAttribs2fvNV)
#define glVertexAttribs2svNV GLEW_GET_FUN(G_glewVertexAttribs2svNV)
#define glVertexAttribs3dvNV GLEW_GET_FUN(G_glewVertexAttribs3dvNV)
#define glVertexAttribs3fvNV GLEW_GET_FUN(G_glewVertexAttribs3fvNV)
#define glVertexAttribs3svNV GLEW_GET_FUN(G_glewVertexAttribs3svNV)
#define glVertexAttribs4dvNV GLEW_GET_FUN(G_glewVertexAttribs4dvNV)
#define glVertexAttribs4fvNV GLEW_GET_FUN(G_glewVertexAttribs4fvNV)
#define glVertexAttribs4svNV GLEW_GET_FUN(G_glewVertexAttribs4svNV)
#define glVertexAttribs4ubvNV GLEW_GET_FUN(G_glewVertexAttribs4ubvNV)

#define GLEW_NV_vertex_program GLEW_GET_VAR(G_GLEW_NV_vertex_program)

#endif /* GL_NV_vertex_program */

/* ------------------------ GL_NV_vertex_program1_1 ------------------------ */

#ifndef GL_NV_vertex_program1_1
#define GL_NV_vertex_program1_1 1

#define GLEW_NV_vertex_program1_1 GLEW_GET_VAR(G_GLEW_NV_vertex_program1_1)

#endif /* GL_NV_vertex_program1_1 */

/* ------------------------- GL_NV_vertex_program2 ------------------------- */

#ifndef GL_NV_vertex_program2
#define GL_NV_vertex_program2 1

#define GLEW_NV_vertex_program2 GLEW_GET_VAR(G_GLEW_NV_vertex_program2)

#endif /* GL_NV_vertex_program2 */

/* ---------------------- GL_NV_vertex_program2_option --------------------- */

#ifndef GL_NV_vertex_program2_option
#define GL_NV_vertex_program2_option 1

#define GL_MAX_PROGRAM_EXEC_INSTRUCTIONS_NV 0x88F4
#define GL_MAX_PROGRAM_CALL_DEPTH_NV 0x88F5

#define GLEW_NV_vertex_program2_option GLEW_GET_VAR(G_GLEW_NV_vertex_program2_option)

#endif /* GL_NV_vertex_program2_option */

/* ------------------------- GL_NV_vertex_program3 ------------------------- */

#ifndef GL_NV_vertex_program3
#define GL_NV_vertex_program3 1

#define MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB 0x8B4C

#define GLEW_NV_vertex_program3 GLEW_GET_VAR(G_GLEW_NV_vertex_program3)

#endif /* GL_NV_vertex_program3 */

/* ------------------------- GL_NV_vertex_program4 ------------------------- */

#ifndef GL_NV_vertex_program4
#define GL_NV_vertex_program4 1

#define GL_VERTEX_ATTRIB_ARRAY_INTEGER_NV 0x88FD

#define GLEW_NV_vertex_program4 GLEW_GET_VAR(G_GLEW_NV_vertex_program4)

#endif /* GL_NV_vertex_program4 */

/* -------------------------- GL_NV_video_capture -------------------------- */

#ifndef GL_NV_video_capture
#define GL_NV_video_capture 1

#define GL_VIDEO_BUFFER_NV 0x9020
#define GL_VIDEO_BUFFER_BINDING_NV 0x9021
#define GL_FIELD_UPPER_NV 0x9022
#define GL_FIELD_LOWER_NV 0x9023
#define GL_NUM_VIDEO_CAPTURE_STREAMS_NV 0x9024
#define GL_NEXT_VIDEO_CAPTURE_BUFFER_STATUS_NV 0x9025
#define GL_VIDEO_CAPTURE_TO_422_SUPPORTED_NV 0x9026
#define GL_LAST_VIDEO_CAPTURE_STATUS_NV 0x9027
#define GL_VIDEO_BUFFER_PITCH_NV 0x9028
#define GL_VIDEO_COLOR_CONVERSION_MATRIX_NV 0x9029
#define GL_VIDEO_COLOR_CONVERSION_MAX_NV 0x902A
#define GL_VIDEO_COLOR_CONVERSION_MIN_NV 0x902B
#define GL_VIDEO_COLOR_CONVERSION_OFFSET_NV 0x902C
#define GL_VIDEO_BUFFER_INTERNAL_FORMAT_NV 0x902D
#define GL_PARTIAL_SUCCESS_NV 0x902E
#define GL_SUCCESS_NV 0x902F
#define GL_FAILURE_NV 0x9030
#define GL_YCBYCR8_422_NV 0x9031
#define GL_YCBAYCR8A_4224_NV 0x9032
#define GL_Z6Y10Z6CB10Z6Y10Z6CR10_422_NV 0x9033
#define GL_Z6Y10Z6CB10Z6A10Z6Y10Z6CR10Z6A10_4224_NV 0x9034
#define GL_Z4Y12Z4CB12Z4Y12Z4CR12_422_NV 0x9035
#define GL_Z4Y12Z4CB12Z4A12Z4Y12Z4CR12Z4A12_4224_NV 0x9036
#define GL_Z4Y12Z4CB12Z4CR12_444_NV 0x9037
#define GL_VIDEO_CAPTURE_FRAME_WIDTH_NV 0x9038
#define GL_VIDEO_CAPTURE_FRAME_HEIGHT_NV 0x9039
#define GL_VIDEO_CAPTURE_FIELD_UPPER_HEIGHT_NV 0x903A
#define GL_VIDEO_CAPTURE_FIELD_LOWER_HEIGHT_NV 0x903B
#define GL_VIDEO_CAPTURE_SURFACE_ORIGIN_NV 0x903C

typedef void (GLAPIENTRY * PFNGLBEGINVIDEOCAPTURENVPROC) (GLuint video_capture_slot);
typedef void (GLAPIENTRY * PFNGLBINDVIDEOCAPTURESTREAMBUFFERNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset);
typedef void (GLAPIENTRY * PFNGLBINDVIDEOCAPTURESTREAMTEXTURENVPROC) (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture);
typedef void (GLAPIENTRY * PFNGLENDVIDEOCAPTURENVPROC) (GLuint video_capture_slot);
typedef void (GLAPIENTRY * PFNGLGETVIDEOCAPTURESTREAMDVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOCAPTURESTREAMFVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOCAPTURESTREAMIVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETVIDEOCAPTUREIVNVPROC) (GLuint video_capture_slot, GLenum pname, GLint* params);
typedef GLenum (GLAPIENTRY * PFNGLVIDEOCAPTURENVPROC) (GLuint video_capture_slot, GLuint* sequence_num, GLuint64EXT *capture_time);
typedef void (GLAPIENTRY * PFNGLVIDEOCAPTURESTREAMPARAMETERDVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble* params);
typedef void (GLAPIENTRY * PFNGLVIDEOCAPTURESTREAMPARAMETERFVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLVIDEOCAPTURESTREAMPARAMETERIVNVPROC) (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint* params);

#define glBeginVideoCaptureNV GLEW_GET_FUN(G_glewBeginVideoCaptureNV)
#define glBindVideoCaptureStreamBufferNV GLEW_GET_FUN(G_glewBindVideoCaptureStreamBufferNV)
#define glBindVideoCaptureStreamTextureNV GLEW_GET_FUN(G_glewBindVideoCaptureStreamTextureNV)
#define glEndVideoCaptureNV GLEW_GET_FUN(G_glewEndVideoCaptureNV)
#define glGetVideoCaptureStreamdvNV GLEW_GET_FUN(G_glewGetVideoCaptureStreamdvNV)
#define glGetVideoCaptureStreamfvNV GLEW_GET_FUN(G_glewGetVideoCaptureStreamfvNV)
#define glGetVideoCaptureStreamivNV GLEW_GET_FUN(G_glewGetVideoCaptureStreamivNV)
#define glGetVideoCaptureivNV GLEW_GET_FUN(G_glewGetVideoCaptureivNV)
#define glVideoCaptureNV GLEW_GET_FUN(G_glewVideoCaptureNV)
#define glVideoCaptureStreamParameterdvNV GLEW_GET_FUN(G_glewVideoCaptureStreamParameterdvNV)
#define glVideoCaptureStreamParameterfvNV GLEW_GET_FUN(G_glewVideoCaptureStreamParameterfvNV)
#define glVideoCaptureStreamParameterivNV GLEW_GET_FUN(G_glewVideoCaptureStreamParameterivNV)

#define GLEW_NV_video_capture GLEW_GET_VAR(G_GLEW_NV_video_capture)

#endif /* GL_NV_video_capture */

/* ------------------------ GL_OES_byte_coordinates ------------------------ */

#ifndef GL_OES_byte_coordinates
#define GL_OES_byte_coordinates 1

#define GL_BYTE 0x1400

#define GLEW_OES_byte_coordinates GLEW_GET_VAR(G_GLEW_OES_byte_coordinates)

#endif /* GL_OES_byte_coordinates */

/* ------------------- GL_OES_compressed_paletted_texture ------------------ */

#ifndef GL_OES_compressed_paletted_texture
#define GL_OES_compressed_paletted_texture 1

#define GL_PALETTE4_RGB8_OES 0x8B90
#define GL_PALETTE4_RGBA8_OES 0x8B91
#define GL_PALETTE4_R5_G6_B5_OES 0x8B92
#define GL_PALETTE4_RGBA4_OES 0x8B93
#define GL_PALETTE4_RGB5_A1_OES 0x8B94
#define GL_PALETTE8_RGB8_OES 0x8B95
#define GL_PALETTE8_RGBA8_OES 0x8B96
#define GL_PALETTE8_R5_G6_B5_OES 0x8B97
#define GL_PALETTE8_RGBA4_OES 0x8B98
#define GL_PALETTE8_RGB5_A1_OES 0x8B99

#define GLEW_OES_compressed_paletted_texture GLEW_GET_VAR(G_GLEW_OES_compressed_paletted_texture)

#endif /* GL_OES_compressed_paletted_texture */

/* --------------------------- GL_OES_read_format -------------------------- */

#ifndef GL_OES_read_format
#define GL_OES_read_format 1

#define GL_IMPLEMENTATION_COLOR_READ_TYPE_OES 0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT_OES 0x8B9B

#define GLEW_OES_read_format GLEW_GET_VAR(G_GLEW_OES_read_format)

#endif /* GL_OES_read_format */

/* ------------------------ GL_OES_single_precision ------------------------ */

#ifndef GL_OES_single_precision
#define GL_OES_single_precision 1

typedef void (GLAPIENTRY * PFNGLCLEARDEPTHFOESPROC) (GLclampd depth);
typedef void (GLAPIENTRY * PFNGLCLIPPLANEFOESPROC) (GLenum plane, const GLfloat* equation);
typedef void (GLAPIENTRY * PFNGLDEPTHRANGEFOESPROC) (GLclampf n, GLclampf f);
typedef void (GLAPIENTRY * PFNGLFRUSTUMFOESPROC) (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);
typedef void (GLAPIENTRY * PFNGLGETCLIPPLANEFOESPROC) (GLenum plane, GLfloat* equation);
typedef void (GLAPIENTRY * PFNGLORTHOFOESPROC) (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f);

#define glClearDepthfOES GLEW_GET_FUN(G_glewClearDepthfOES)
#define glClipPlanefOES GLEW_GET_FUN(G_glewClipPlanefOES)
#define glDepthRangefOES GLEW_GET_FUN(G_glewDepthRangefOES)
#define glFrustumfOES GLEW_GET_FUN(G_glewFrustumfOES)
#define glGetClipPlanefOES GLEW_GET_FUN(G_glewGetClipPlanefOES)
#define glOrthofOES GLEW_GET_FUN(G_glewOrthofOES)

#define GLEW_OES_single_precision GLEW_GET_VAR(G_GLEW_OES_single_precision)

#endif /* GL_OES_single_precision */

/* ---------------------------- GL_OML_interlace --------------------------- */

#ifndef GL_OML_interlace
#define GL_OML_interlace 1

#define GL_INTERLACE_OML 0x8980
#define GL_INTERLACE_READ_OML 0x8981

#define GLEW_OML_interlace GLEW_GET_VAR(G_GLEW_OML_interlace)

#endif /* GL_OML_interlace */

/* ---------------------------- GL_OML_resample ---------------------------- */

#ifndef GL_OML_resample
#define GL_OML_resample 1

#define GL_PACK_RESAMPLE_OML 0x8984
#define GL_UNPACK_RESAMPLE_OML 0x8985
#define GL_RESAMPLE_REPLICATE_OML 0x8986
#define GL_RESAMPLE_ZERO_FILL_OML 0x8987
#define GL_RESAMPLE_AVERAGE_OML 0x8988
#define GL_RESAMPLE_DECIMATE_OML 0x8989

#define GLEW_OML_resample GLEW_GET_VAR(G_GLEW_OML_resample)

#endif /* GL_OML_resample */

/* ---------------------------- GL_OML_subsample --------------------------- */

#ifndef GL_OML_subsample
#define GL_OML_subsample 1

#define GL_FORMAT_SUBSAMPLE_24_24_OML 0x8982
#define GL_FORMAT_SUBSAMPLE_244_244_OML 0x8983

#define GLEW_OML_subsample GLEW_GET_VAR(G_GLEW_OML_subsample)

#endif /* GL_OML_subsample */

/* --------------------------- GL_PGI_misc_hints --------------------------- */

#ifndef GL_PGI_misc_hints
#define GL_PGI_misc_hints 1

#define GL_PREFER_DOUBLEBUFFER_HINT_PGI 107000
#define GL_CONSERVE_MEMORY_HINT_PGI 107005
#define GL_RECLAIM_MEMORY_HINT_PGI 107006
#define GL_NATIVE_GRAPHICS_HANDLE_PGI 107010
#define GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI 107011
#define GL_NATIVE_GRAPHICS_END_HINT_PGI 107012
#define GL_ALWAYS_FAST_HINT_PGI 107020
#define GL_ALWAYS_SOFT_HINT_PGI 107021
#define GL_ALLOW_DRAW_OBJ_HINT_PGI 107022
#define GL_ALLOW_DRAW_WIN_HINT_PGI 107023
#define GL_ALLOW_DRAW_FRG_HINT_PGI 107024
#define GL_ALLOW_DRAW_MEM_HINT_PGI 107025
#define GL_STRICT_DEPTHFUNC_HINT_PGI 107030
#define GL_STRICT_LIGHTING_HINT_PGI 107031
#define GL_STRICT_SCISSOR_HINT_PGI 107032
#define GL_FULL_STIPPLE_HINT_PGI 107033
#define GL_CLIP_NEAR_HINT_PGI 107040
#define GL_CLIP_FAR_HINT_PGI 107041
#define GL_WIDE_LINE_HINT_PGI 107042
#define GL_BACK_NORMALS_HINT_PGI 107043

#define GLEW_PGI_misc_hints GLEW_GET_VAR(G_GLEW_PGI_misc_hints)

#endif /* GL_PGI_misc_hints */

/* -------------------------- GL_PGI_vertex_hints -------------------------- */

#ifndef GL_PGI_vertex_hints
#define GL_PGI_vertex_hints 1

#define GL_VERTEX23_BIT_PGI 0x00000004
#define GL_VERTEX4_BIT_PGI 0x00000008
#define GL_COLOR3_BIT_PGI 0x00010000
#define GL_COLOR4_BIT_PGI 0x00020000
#define GL_EDGEFLAG_BIT_PGI 0x00040000
#define GL_INDEX_BIT_PGI 0x00080000
#define GL_MAT_AMBIENT_BIT_PGI 0x00100000
#define GL_VERTEX_DATA_HINT_PGI 107050
#define GL_VERTEX_CONSISTENT_HINT_PGI 107051
#define GL_MATERIAL_SIDE_HINT_PGI 107052
#define GL_MAX_VERTEX_HINT_PGI 107053
#define GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI 0x00200000
#define GL_MAT_DIFFUSE_BIT_PGI 0x00400000
#define GL_MAT_EMISSION_BIT_PGI 0x00800000
#define GL_MAT_COLOR_INDEXES_BIT_PGI 0x01000000
#define GL_MAT_SHININESS_BIT_PGI 0x02000000
#define GL_MAT_SPECULAR_BIT_PGI 0x04000000
#define GL_NORMAL_BIT_PGI 0x08000000
#define GL_TEXCOORD1_BIT_PGI 0x10000000
#define GL_TEXCOORD2_BIT_PGI 0x20000000
#define GL_TEXCOORD3_BIT_PGI 0x40000000
#define GL_TEXCOORD4_BIT_PGI 0x80000000

#define GLEW_PGI_vertex_hints GLEW_GET_VAR(G_GLEW_PGI_vertex_hints)

#endif /* GL_PGI_vertex_hints */

/* ------------------------- GL_REGAL_error_string ------------------------- */

#ifndef GL_REGAL_error_string
#define GL_REGAL_error_string 1

typedef const GLchar* (GLAPIENTRY * PFNGLERRORSTRINGREGALPROC) (GLenum error);

#define glErrorStringREGAL GLEW_GET_FUN(G_glewErrorStringREGAL)

#define GLEW_REGAL_error_string GLEW_GET_VAR(G_GLEW_REGAL_error_string)

#endif /* GL_REGAL_error_string */

/* ------------------------ GL_REGAL_extension_query ----------------------- */

#ifndef GL_REGAL_extension_query
#define GL_REGAL_extension_query 1

typedef GLboolean (GLAPIENTRY * PFNGLGETEXTENSIONREGALPROC) (const GLchar* ext);
typedef GLboolean (GLAPIENTRY * PFNGLISSUPPORTEDREGALPROC) (const GLchar* ext);

#define glGetExtensionREGAL GLEW_GET_FUN(G_glewGetExtensionREGAL)
#define glIsSupportedREGAL GLEW_GET_FUN(G_glewIsSupportedREGAL)

#define GLEW_REGAL_extension_query GLEW_GET_VAR(G_GLEW_REGAL_extension_query)

#endif /* GL_REGAL_extension_query */

/* ------------------------------ GL_REGAL_log ----------------------------- */

#ifndef GL_REGAL_log
#define GL_REGAL_log 1

#define GL_LOG_ERROR_REGAL 0x9319
#define GL_LOG_WARNING_REGAL 0x931A
#define GL_LOG_INFO_REGAL 0x931B
#define GL_LOG_APP_REGAL 0x931C
#define GL_LOG_DRIVER_REGAL 0x931D
#define GL_LOG_INTERNAL_REGAL 0x931E
#define GL_LOG_DEBUG_REGAL 0x931F
#define GL_LOG_STATUS_REGAL 0x9320
#define GL_LOG_HTTP_REGAL 0x9321

#define GLEW_REGAL_log GLEW_GET_VAR(G_GLEW_REGAL_log)

#endif /* GL_REGAL_log */

/* ----------------------- GL_REND_screen_coordinates ---------------------- */

#ifndef GL_REND_screen_coordinates
#define GL_REND_screen_coordinates 1

#define GL_SCREEN_COORDINATES_REND 0x8490
#define GL_INVERTED_SCREEN_W_REND 0x8491

#define GLEW_REND_screen_coordinates GLEW_GET_VAR(G_GLEW_REND_screen_coordinates)

#endif /* GL_REND_screen_coordinates */

/* ------------------------------- GL_S3_s3tc ------------------------------ */

#ifndef GL_S3_s3tc
#define GL_S3_s3tc 1

#define GL_RGB_S3TC 0x83A0
#define GL_RGB4_S3TC 0x83A1
#define GL_RGBA_S3TC 0x83A2
#define GL_RGBA4_S3TC 0x83A3
#define GL_RGBA_DXT5_S3TC 0x83A4
#define GL_RGBA4_DXT5_S3TC 0x83A5

#define GLEW_S3_s3tc GLEW_GET_VAR(G_GLEW_S3_s3tc)

#endif /* GL_S3_s3tc */

/* -------------------------- GL_SGIS_color_range -------------------------- */

#ifndef GL_SGIS_color_range
#define GL_SGIS_color_range 1

#define GL_EXTENDED_RANGE_SGIS 0x85A5
#define GL_MIN_RED_SGIS 0x85A6
#define GL_MAX_RED_SGIS 0x85A7
#define GL_MIN_GREEN_SGIS 0x85A8
#define GL_MAX_GREEN_SGIS 0x85A9
#define GL_MIN_BLUE_SGIS 0x85AA
#define GL_MAX_BLUE_SGIS 0x85AB
#define GL_MIN_ALPHA_SGIS 0x85AC
#define GL_MAX_ALPHA_SGIS 0x85AD

#define GLEW_SGIS_color_range GLEW_GET_VAR(G_GLEW_SGIS_color_range)

#endif /* GL_SGIS_color_range */

/* ------------------------- GL_SGIS_detail_texture ------------------------ */

#ifndef GL_SGIS_detail_texture
#define GL_SGIS_detail_texture 1

typedef void (GLAPIENTRY * PFNGLDETAILTEXFUNCSGISPROC) (GLenum target, GLsizei n, const GLfloat* points);
typedef void (GLAPIENTRY * PFNGLGETDETAILTEXFUNCSGISPROC) (GLenum target, GLfloat* points);

#define glDetailTexFuncSGIS GLEW_GET_FUN(G_glewDetailTexFuncSGIS)
#define glGetDetailTexFuncSGIS GLEW_GET_FUN(G_glewGetDetailTexFuncSGIS)

#define GLEW_SGIS_detail_texture GLEW_GET_VAR(G_GLEW_SGIS_detail_texture)

#endif /* GL_SGIS_detail_texture */

/* -------------------------- GL_SGIS_fog_function ------------------------- */

#ifndef GL_SGIS_fog_function
#define GL_SGIS_fog_function 1

typedef void (GLAPIENTRY * PFNGLFOGFUNCSGISPROC) (GLsizei n, const GLfloat* points);
typedef void (GLAPIENTRY * PFNGLGETFOGFUNCSGISPROC) (GLfloat* points);

#define glFogFuncSGIS GLEW_GET_FUN(G_glewFogFuncSGIS)
#define glGetFogFuncSGIS GLEW_GET_FUN(G_glewGetFogFuncSGIS)

#define GLEW_SGIS_fog_function GLEW_GET_VAR(G_GLEW_SGIS_fog_function)

#endif /* GL_SGIS_fog_function */

/* ------------------------ GL_SGIS_generate_mipmap ------------------------ */

#ifndef GL_SGIS_generate_mipmap
#define GL_SGIS_generate_mipmap 1

#define GL_GENERATE_MIPMAP_SGIS 0x8191
#define GL_GENERATE_MIPMAP_HINT_SGIS 0x8192

#define GLEW_SGIS_generate_mipmap GLEW_GET_VAR(G_GLEW_SGIS_generate_mipmap)

#endif /* GL_SGIS_generate_mipmap */

/* -------------------------- GL_SGIS_multisample -------------------------- */

#ifndef GL_SGIS_multisample
#define GL_SGIS_multisample 1

#define GL_MULTISAMPLE_SGIS 0x809D
#define GL_SAMPLE_ALPHA_TO_MASK_SGIS 0x809E
#define GL_SAMPLE_ALPHA_TO_ONE_SGIS 0x809F
#define GL_SAMPLE_MASK_SGIS 0x80A0
#define GL_1PASS_SGIS 0x80A1
#define GL_2PASS_0_SGIS 0x80A2
#define GL_2PASS_1_SGIS 0x80A3
#define GL_4PASS_0_SGIS 0x80A4
#define GL_4PASS_1_SGIS 0x80A5
#define GL_4PASS_2_SGIS 0x80A6
#define GL_4PASS_3_SGIS 0x80A7
#define GL_SAMPLE_BUFFERS_SGIS 0x80A8
#define GL_SAMPLES_SGIS 0x80A9
#define GL_SAMPLE_MASK_VALUE_SGIS 0x80AA
#define GL_SAMPLE_MASK_INVERT_SGIS 0x80AB
#define GL_SAMPLE_PATTERN_SGIS 0x80AC

typedef void (GLAPIENTRY * PFNGLSAMPLEMASKSGISPROC) (GLclampf value, GLboolean invert);
typedef void (GLAPIENTRY * PFNGLSAMPLEPATTERNSGISPROC) (GLenum pattern);

#define glSampleMaskSGIS GLEW_GET_FUN(G_glewSampleMaskSGIS)
#define glSamplePatternSGIS GLEW_GET_FUN(G_glewSamplePatternSGIS)

#define GLEW_SGIS_multisample GLEW_GET_VAR(G_GLEW_SGIS_multisample)

#endif /* GL_SGIS_multisample */

/* ------------------------- GL_SGIS_pixel_texture ------------------------- */

#ifndef GL_SGIS_pixel_texture
#define GL_SGIS_pixel_texture 1

#define GLEW_SGIS_pixel_texture GLEW_GET_VAR(G_GLEW_SGIS_pixel_texture)

#endif /* GL_SGIS_pixel_texture */

/* ----------------------- GL_SGIS_point_line_texgen ----------------------- */

#ifndef GL_SGIS_point_line_texgen
#define GL_SGIS_point_line_texgen 1

#define GL_EYE_DISTANCE_TO_POINT_SGIS 0x81F0
#define GL_OBJECT_DISTANCE_TO_POINT_SGIS 0x81F1
#define GL_EYE_DISTANCE_TO_LINE_SGIS 0x81F2
#define GL_OBJECT_DISTANCE_TO_LINE_SGIS 0x81F3
#define GL_EYE_POINT_SGIS 0x81F4
#define GL_OBJECT_POINT_SGIS 0x81F5
#define GL_EYE_LINE_SGIS 0x81F6
#define GL_OBJECT_LINE_SGIS 0x81F7

#define GLEW_SGIS_point_line_texgen GLEW_GET_VAR(G_GLEW_SGIS_point_line_texgen)

#endif /* GL_SGIS_point_line_texgen */

/* ------------------------ GL_SGIS_sharpen_texture ------------------------ */

#ifndef GL_SGIS_sharpen_texture
#define GL_SGIS_sharpen_texture 1

typedef void (GLAPIENTRY * PFNGLGETSHARPENTEXFUNCSGISPROC) (GLenum target, GLfloat* points);
typedef void (GLAPIENTRY * PFNGLSHARPENTEXFUNCSGISPROC) (GLenum target, GLsizei n, const GLfloat* points);

#define glGetSharpenTexFuncSGIS GLEW_GET_FUN(G_glewGetSharpenTexFuncSGIS)
#define glSharpenTexFuncSGIS GLEW_GET_FUN(G_glewSharpenTexFuncSGIS)

#define GLEW_SGIS_sharpen_texture GLEW_GET_VAR(G_GLEW_SGIS_sharpen_texture)

#endif /* GL_SGIS_sharpen_texture */

/* --------------------------- GL_SGIS_texture4D --------------------------- */

#ifndef GL_SGIS_texture4D
#define GL_SGIS_texture4D 1

typedef void (GLAPIENTRY * PFNGLTEXIMAGE4DSGISPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei extent, GLint border, GLenum format, GLenum type, const void* pixels);
typedef void (GLAPIENTRY * PFNGLTEXSUBIMAGE4DSGISPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei extent, GLenum format, GLenum type, const void* pixels);

#define glTexImage4DSGIS GLEW_GET_FUN(G_glewTexImage4DSGIS)
#define glTexSubImage4DSGIS GLEW_GET_FUN(G_glewTexSubImage4DSGIS)

#define GLEW_SGIS_texture4D GLEW_GET_VAR(G_GLEW_SGIS_texture4D)

#endif /* GL_SGIS_texture4D */

/* ---------------------- GL_SGIS_texture_border_clamp --------------------- */

#ifndef GL_SGIS_texture_border_clamp
#define GL_SGIS_texture_border_clamp 1

#define GL_CLAMP_TO_BORDER_SGIS 0x812D

#define GLEW_SGIS_texture_border_clamp GLEW_GET_VAR(G_GLEW_SGIS_texture_border_clamp)

#endif /* GL_SGIS_texture_border_clamp */

/* ----------------------- GL_SGIS_texture_edge_clamp ---------------------- */

#ifndef GL_SGIS_texture_edge_clamp
#define GL_SGIS_texture_edge_clamp 1

#define GL_CLAMP_TO_EDGE_SGIS 0x812F

#define GLEW_SGIS_texture_edge_clamp GLEW_GET_VAR(G_GLEW_SGIS_texture_edge_clamp)

#endif /* GL_SGIS_texture_edge_clamp */

/* ------------------------ GL_SGIS_texture_filter4 ------------------------ */

#ifndef GL_SGIS_texture_filter4
#define GL_SGIS_texture_filter4 1

typedef void (GLAPIENTRY * PFNGLGETTEXFILTERFUNCSGISPROC) (GLenum target, GLenum filter, GLfloat* weights);
typedef void (GLAPIENTRY * PFNGLTEXFILTERFUNCSGISPROC) (GLenum target, GLenum filter, GLsizei n, const GLfloat* weights);

#define glGetTexFilterFuncSGIS GLEW_GET_FUN(G_glewGetTexFilterFuncSGIS)
#define glTexFilterFuncSGIS GLEW_GET_FUN(G_glewTexFilterFuncSGIS)

#define GLEW_SGIS_texture_filter4 GLEW_GET_VAR(G_GLEW_SGIS_texture_filter4)

#endif /* GL_SGIS_texture_filter4 */

/* -------------------------- GL_SGIS_texture_lod -------------------------- */

#ifndef GL_SGIS_texture_lod
#define GL_SGIS_texture_lod 1

#define GL_TEXTURE_MIN_LOD_SGIS 0x813A
#define GL_TEXTURE_MAX_LOD_SGIS 0x813B
#define GL_TEXTURE_BASE_LEVEL_SGIS 0x813C
#define GL_TEXTURE_MAX_LEVEL_SGIS 0x813D

#define GLEW_SGIS_texture_lod GLEW_GET_VAR(G_GLEW_SGIS_texture_lod)

#endif /* GL_SGIS_texture_lod */

/* ------------------------- GL_SGIS_texture_select ------------------------ */

#ifndef GL_SGIS_texture_select
#define GL_SGIS_texture_select 1

#define GLEW_SGIS_texture_select GLEW_GET_VAR(G_GLEW_SGIS_texture_select)

#endif /* GL_SGIS_texture_select */

/* ----------------------------- GL_SGIX_async ----------------------------- */

#ifndef GL_SGIX_async
#define GL_SGIX_async 1

#define GL_ASYNC_MARKER_SGIX 0x8329

typedef void (GLAPIENTRY * PFNGLASYNCMARKERSGIXPROC) (GLuint marker);
typedef void (GLAPIENTRY * PFNGLDELETEASYNCMARKERSSGIXPROC) (GLuint marker, GLsizei range);
typedef GLint (GLAPIENTRY * PFNGLFINISHASYNCSGIXPROC) (GLuint* markerp);
typedef GLuint (GLAPIENTRY * PFNGLGENASYNCMARKERSSGIXPROC) (GLsizei range);
typedef GLboolean (GLAPIENTRY * PFNGLISASYNCMARKERSGIXPROC) (GLuint marker);
typedef GLint (GLAPIENTRY * PFNGLPOLLASYNCSGIXPROC) (GLuint* markerp);

#define glAsyncMarkerSGIX GLEW_GET_FUN(G_glewAsyncMarkerSGIX)
#define glDeleteAsyncMarkersSGIX GLEW_GET_FUN(G_glewDeleteAsyncMarkersSGIX)
#define glFinishAsyncSGIX GLEW_GET_FUN(G_glewFinishAsyncSGIX)
#define glGenAsyncMarkersSGIX GLEW_GET_FUN(G_glewGenAsyncMarkersSGIX)
#define glIsAsyncMarkerSGIX GLEW_GET_FUN(G_glewIsAsyncMarkerSGIX)
#define glPollAsyncSGIX GLEW_GET_FUN(G_glewPollAsyncSGIX)

#define GLEW_SGIX_async GLEW_GET_VAR(G_GLEW_SGIX_async)

#endif /* GL_SGIX_async */

/* ------------------------ GL_SGIX_async_histogram ------------------------ */

#ifndef GL_SGIX_async_histogram
#define GL_SGIX_async_histogram 1

#define GL_ASYNC_HISTOGRAM_SGIX 0x832C
#define GL_MAX_ASYNC_HISTOGRAM_SGIX 0x832D

#define GLEW_SGIX_async_histogram GLEW_GET_VAR(G_GLEW_SGIX_async_histogram)

#endif /* GL_SGIX_async_histogram */

/* -------------------------- GL_SGIX_async_pixel -------------------------- */

#ifndef GL_SGIX_async_pixel
#define GL_SGIX_async_pixel 1

#define GL_ASYNC_TEX_IMAGE_SGIX 0x835C
#define GL_ASYNC_DRAW_PIXELS_SGIX 0x835D
#define GL_ASYNC_READ_PIXELS_SGIX 0x835E
#define GL_MAX_ASYNC_TEX_IMAGE_SGIX 0x835F
#define GL_MAX_ASYNC_DRAW_PIXELS_SGIX 0x8360
#define GL_MAX_ASYNC_READ_PIXELS_SGIX 0x8361

#define GLEW_SGIX_async_pixel GLEW_GET_VAR(G_GLEW_SGIX_async_pixel)

#endif /* GL_SGIX_async_pixel */

/* ----------------------- GL_SGIX_blend_alpha_minmax ---------------------- */

#ifndef GL_SGIX_blend_alpha_minmax
#define GL_SGIX_blend_alpha_minmax 1

#define GL_ALPHA_MIN_SGIX 0x8320
#define GL_ALPHA_MAX_SGIX 0x8321

#define GLEW_SGIX_blend_alpha_minmax GLEW_GET_VAR(G_GLEW_SGIX_blend_alpha_minmax)

#endif /* GL_SGIX_blend_alpha_minmax */

/* ---------------------------- GL_SGIX_clipmap ---------------------------- */

#ifndef GL_SGIX_clipmap
#define GL_SGIX_clipmap 1

#define GLEW_SGIX_clipmap GLEW_GET_VAR(G_GLEW_SGIX_clipmap)

#endif /* GL_SGIX_clipmap */

/* ---------------------- GL_SGIX_convolution_accuracy --------------------- */

#ifndef GL_SGIX_convolution_accuracy
#define GL_SGIX_convolution_accuracy 1

#define GL_CONVOLUTION_HINT_SGIX 0x8316

#define GLEW_SGIX_convolution_accuracy GLEW_GET_VAR(G_GLEW_SGIX_convolution_accuracy)

#endif /* GL_SGIX_convolution_accuracy */

/* ------------------------- GL_SGIX_depth_texture ------------------------- */

#ifndef GL_SGIX_depth_texture
#define GL_SGIX_depth_texture 1

#define GL_DEPTH_COMPONENT16_SGIX 0x81A5
#define GL_DEPTH_COMPONENT24_SGIX 0x81A6
#define GL_DEPTH_COMPONENT32_SGIX 0x81A7

#define GLEW_SGIX_depth_texture GLEW_GET_VAR(G_GLEW_SGIX_depth_texture)

#endif /* GL_SGIX_depth_texture */

/* -------------------------- GL_SGIX_flush_raster ------------------------- */

#ifndef GL_SGIX_flush_raster
#define GL_SGIX_flush_raster 1

typedef void (GLAPIENTRY * PFNGLFLUSHRASTERSGIXPROC) (void);

#define glFlushRasterSGIX GLEW_GET_FUN(G_glewFlushRasterSGIX)

#define GLEW_SGIX_flush_raster GLEW_GET_VAR(G_GLEW_SGIX_flush_raster)

#endif /* GL_SGIX_flush_raster */

/* --------------------------- GL_SGIX_fog_offset -------------------------- */

#ifndef GL_SGIX_fog_offset
#define GL_SGIX_fog_offset 1

#define GL_FOG_OFFSET_SGIX 0x8198
#define GL_FOG_OFFSET_VALUE_SGIX 0x8199

#define GLEW_SGIX_fog_offset GLEW_GET_VAR(G_GLEW_SGIX_fog_offset)

#endif /* GL_SGIX_fog_offset */

/* -------------------------- GL_SGIX_fog_texture -------------------------- */

#ifndef GL_SGIX_fog_texture
#define GL_SGIX_fog_texture 1

#define GL_TEXTURE_FOG_SGIX 0
#define GL_FOG_PATCHY_FACTOR_SGIX 0
#define GL_FRAGMENT_FOG_SGIX 0

typedef void (GLAPIENTRY * PFNGLTEXTUREFOGSGIXPROC) (GLenum pname);

#define glTextureFogSGIX GLEW_GET_FUN(G_glewTextureFogSGIX)

#define GLEW_SGIX_fog_texture GLEW_GET_VAR(G_GLEW_SGIX_fog_texture)

#endif /* GL_SGIX_fog_texture */

/* ------------------- GL_SGIX_fragment_specular_lighting ------------------ */

#ifndef GL_SGIX_fragment_specular_lighting
#define GL_SGIX_fragment_specular_lighting 1

typedef void (GLAPIENTRY * PFNGLFRAGMENTCOLORMATERIALSGIXPROC) (GLenum face, GLenum mode);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFSGIXPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELFVSGIXPROC) (GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELISGIXPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTMODELIVSGIXPROC) (GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFSGIXPROC) (GLenum light, GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTFVSGIXPROC) (GLenum light, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTISGIXPROC) (GLenum light, GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTLIGHTIVSGIXPROC) (GLenum light, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFSGIXPROC) (GLenum face, GLenum pname, const GLfloat param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALFVSGIXPROC) (GLenum face, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALISGIXPROC) (GLenum face, GLenum pname, const GLint param);
typedef void (GLAPIENTRY * PFNGLFRAGMENTMATERIALIVSGIXPROC) (GLenum face, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTFVSGIXPROC) (GLenum light, GLenum value, GLfloat* data);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTLIGHTIVSGIXPROC) (GLenum light, GLenum value, GLint* data);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALFVSGIXPROC) (GLenum face, GLenum pname, const GLfloat* data);
typedef void (GLAPIENTRY * PFNGLGETFRAGMENTMATERIALIVSGIXPROC) (GLenum face, GLenum pname, const GLint* data);

#define glFragmentColorMaterialSGIX GLEW_GET_FUN(G_glewFragmentColorMaterialSGIX)
#define glFragmentLightModelfSGIX GLEW_GET_FUN(G_glewFragmentLightModelfSGIX)
#define glFragmentLightModelfvSGIX GLEW_GET_FUN(G_glewFragmentLightModelfvSGIX)
#define glFragmentLightModeliSGIX GLEW_GET_FUN(G_glewFragmentLightModeliSGIX)
#define glFragmentLightModelivSGIX GLEW_GET_FUN(G_glewFragmentLightModelivSGIX)
#define glFragmentLightfSGIX GLEW_GET_FUN(G_glewFragmentLightfSGIX)
#define glFragmentLightfvSGIX GLEW_GET_FUN(G_glewFragmentLightfvSGIX)
#define glFragmentLightiSGIX GLEW_GET_FUN(G_glewFragmentLightiSGIX)
#define glFragmentLightivSGIX GLEW_GET_FUN(G_glewFragmentLightivSGIX)
#define glFragmentMaterialfSGIX GLEW_GET_FUN(G_glewFragmentMaterialfSGIX)
#define glFragmentMaterialfvSGIX GLEW_GET_FUN(G_glewFragmentMaterialfvSGIX)
#define glFragmentMaterialiSGIX GLEW_GET_FUN(G_glewFragmentMaterialiSGIX)
#define glFragmentMaterialivSGIX GLEW_GET_FUN(G_glewFragmentMaterialivSGIX)
#define glGetFragmentLightfvSGIX GLEW_GET_FUN(G_glewGetFragmentLightfvSGIX)
#define glGetFragmentLightivSGIX GLEW_GET_FUN(G_glewGetFragmentLightivSGIX)
#define glGetFragmentMaterialfvSGIX GLEW_GET_FUN(G_glewGetFragmentMaterialfvSGIX)
#define glGetFragmentMaterialivSGIX GLEW_GET_FUN(G_glewGetFragmentMaterialivSGIX)

#define GLEW_SGIX_fragment_specular_lighting GLEW_GET_VAR(G_GLEW_SGIX_fragment_specular_lighting)

#endif /* GL_SGIX_fragment_specular_lighting */

/* --------------------------- GL_SGIX_framezoom --------------------------- */

#ifndef GL_SGIX_framezoom
#define GL_SGIX_framezoom 1

typedef void (GLAPIENTRY * PFNGLFRAMEZOOMSGIXPROC) (GLint factor);

#define glFrameZoomSGIX GLEW_GET_FUN(G_glewFrameZoomSGIX)

#define GLEW_SGIX_framezoom GLEW_GET_VAR(G_GLEW_SGIX_framezoom)

#endif /* GL_SGIX_framezoom */

/* --------------------------- GL_SGIX_interlace --------------------------- */

#ifndef GL_SGIX_interlace
#define GL_SGIX_interlace 1

#define GL_INTERLACE_SGIX 0x8094

#define GLEW_SGIX_interlace GLEW_GET_VAR(G_GLEW_SGIX_interlace)

#endif /* GL_SGIX_interlace */

/* ------------------------- GL_SGIX_ir_instrument1 ------------------------ */

#ifndef GL_SGIX_ir_instrument1
#define GL_SGIX_ir_instrument1 1

#define GLEW_SGIX_ir_instrument1 GLEW_GET_VAR(G_GLEW_SGIX_ir_instrument1)

#endif /* GL_SGIX_ir_instrument1 */

/* ------------------------- GL_SGIX_list_priority ------------------------- */

#ifndef GL_SGIX_list_priority
#define GL_SGIX_list_priority 1

#define GLEW_SGIX_list_priority GLEW_GET_VAR(G_GLEW_SGIX_list_priority)

#endif /* GL_SGIX_list_priority */

/* ------------------------- GL_SGIX_pixel_texture ------------------------- */

#ifndef GL_SGIX_pixel_texture
#define GL_SGIX_pixel_texture 1

typedef void (GLAPIENTRY * PFNGLPIXELTEXGENSGIXPROC) (GLenum mode);

#define glPixelTexGenSGIX GLEW_GET_FUN(G_glewPixelTexGenSGIX)

#define GLEW_SGIX_pixel_texture GLEW_GET_VAR(G_GLEW_SGIX_pixel_texture)

#endif /* GL_SGIX_pixel_texture */

/* ----------------------- GL_SGIX_pixel_texture_bits ---------------------- */

#ifndef GL_SGIX_pixel_texture_bits
#define GL_SGIX_pixel_texture_bits 1

#define GLEW_SGIX_pixel_texture_bits GLEW_GET_VAR(G_GLEW_SGIX_pixel_texture_bits)

#endif /* GL_SGIX_pixel_texture_bits */

/* ------------------------ GL_SGIX_reference_plane ------------------------ */

#ifndef GL_SGIX_reference_plane
#define GL_SGIX_reference_plane 1

typedef void (GLAPIENTRY * PFNGLREFERENCEPLANESGIXPROC) (const GLdouble* equation);

#define glReferencePlaneSGIX GLEW_GET_FUN(G_glewReferencePlaneSGIX)

#define GLEW_SGIX_reference_plane GLEW_GET_VAR(G_GLEW_SGIX_reference_plane)

#endif /* GL_SGIX_reference_plane */

/* ---------------------------- GL_SGIX_resample --------------------------- */

#ifndef GL_SGIX_resample
#define GL_SGIX_resample 1

#define GL_PACK_RESAMPLE_SGIX 0x842E
#define GL_UNPACK_RESAMPLE_SGIX 0x842F
#define GL_RESAMPLE_DECIMATE_SGIX 0x8430
#define GL_RESAMPLE_REPLICATE_SGIX 0x8433
#define GL_RESAMPLE_ZERO_FILL_SGIX 0x8434

#define GLEW_SGIX_resample GLEW_GET_VAR(G_GLEW_SGIX_resample)

#endif /* GL_SGIX_resample */

/* ----------------------------- GL_SGIX_shadow ---------------------------- */

#ifndef GL_SGIX_shadow
#define GL_SGIX_shadow 1

#define GL_TEXTURE_COMPARE_SGIX 0x819A
#define GL_TEXTURE_COMPARE_OPERATOR_SGIX 0x819B
#define GL_TEXTURE_LEQUAL_R_SGIX 0x819C
#define GL_TEXTURE_GEQUAL_R_SGIX 0x819D

#define GLEW_SGIX_shadow GLEW_GET_VAR(G_GLEW_SGIX_shadow)

#endif /* GL_SGIX_shadow */

/* ------------------------- GL_SGIX_shadow_ambient ------------------------ */

#ifndef GL_SGIX_shadow_ambient
#define GL_SGIX_shadow_ambient 1

#define GL_SHADOW_AMBIENT_SGIX 0x80BF

#define GLEW_SGIX_shadow_ambient GLEW_GET_VAR(G_GLEW_SGIX_shadow_ambient)

#endif /* GL_SGIX_shadow_ambient */

/* ----------------------------- GL_SGIX_sprite ---------------------------- */

#ifndef GL_SGIX_sprite
#define GL_SGIX_sprite 1

typedef void (GLAPIENTRY * PFNGLSPRITEPARAMETERFSGIXPROC) (GLenum pname, GLfloat param);
typedef void (GLAPIENTRY * PFNGLSPRITEPARAMETERFVSGIXPROC) (GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLSPRITEPARAMETERISGIXPROC) (GLenum pname, GLint param);
typedef void (GLAPIENTRY * PFNGLSPRITEPARAMETERIVSGIXPROC) (GLenum pname, GLint* params);

#define glSpriteParameterfSGIX GLEW_GET_FUN(G_glewSpriteParameterfSGIX)
#define glSpriteParameterfvSGIX GLEW_GET_FUN(G_glewSpriteParameterfvSGIX)
#define glSpriteParameteriSGIX GLEW_GET_FUN(G_glewSpriteParameteriSGIX)
#define glSpriteParameterivSGIX GLEW_GET_FUN(G_glewSpriteParameterivSGIX)

#define GLEW_SGIX_sprite GLEW_GET_VAR(G_GLEW_SGIX_sprite)

#endif /* GL_SGIX_sprite */

/* ----------------------- GL_SGIX_tag_sample_buffer ----------------------- */

#ifndef GL_SGIX_tag_sample_buffer
#define GL_SGIX_tag_sample_buffer 1

typedef void (GLAPIENTRY * PFNGLTAGSAMPLEBUFFERSGIXPROC) (void);

#define glTagSampleBufferSGIX GLEW_GET_FUN(G_glewTagSampleBufferSGIX)

#define GLEW_SGIX_tag_sample_buffer GLEW_GET_VAR(G_GLEW_SGIX_tag_sample_buffer)

#endif /* GL_SGIX_tag_sample_buffer */

/* ------------------------ GL_SGIX_texture_add_env ------------------------ */

#ifndef GL_SGIX_texture_add_env
#define GL_SGIX_texture_add_env 1

#define GLEW_SGIX_texture_add_env GLEW_GET_VAR(G_GLEW_SGIX_texture_add_env)

#endif /* GL_SGIX_texture_add_env */

/* -------------------- GL_SGIX_texture_coordinate_clamp ------------------- */

#ifndef GL_SGIX_texture_coordinate_clamp
#define GL_SGIX_texture_coordinate_clamp 1

#define GL_TEXTURE_MAX_CLAMP_S_SGIX 0x8369
#define GL_TEXTURE_MAX_CLAMP_T_SGIX 0x836A
#define GL_TEXTURE_MAX_CLAMP_R_SGIX 0x836B

#define GLEW_SGIX_texture_coordinate_clamp GLEW_GET_VAR(G_GLEW_SGIX_texture_coordinate_clamp)

#endif /* GL_SGIX_texture_coordinate_clamp */

/* ------------------------ GL_SGIX_texture_lod_bias ----------------------- */

#ifndef GL_SGIX_texture_lod_bias
#define GL_SGIX_texture_lod_bias 1

#define GLEW_SGIX_texture_lod_bias GLEW_GET_VAR(G_GLEW_SGIX_texture_lod_bias)

#endif /* GL_SGIX_texture_lod_bias */

/* ---------------------- GL_SGIX_texture_multi_buffer --------------------- */

#ifndef GL_SGIX_texture_multi_buffer
#define GL_SGIX_texture_multi_buffer 1

#define GL_TEXTURE_MULTI_BUFFER_HINT_SGIX 0x812E

#define GLEW_SGIX_texture_multi_buffer GLEW_GET_VAR(G_GLEW_SGIX_texture_multi_buffer)

#endif /* GL_SGIX_texture_multi_buffer */

/* ------------------------- GL_SGIX_texture_range ------------------------- */

#ifndef GL_SGIX_texture_range
#define GL_SGIX_texture_range 1

#define GL_RGB_SIGNED_SGIX 0x85E0
#define GL_RGBA_SIGNED_SGIX 0x85E1
#define GL_ALPHA_SIGNED_SGIX 0x85E2
#define GL_LUMINANCE_SIGNED_SGIX 0x85E3
#define GL_INTENSITY_SIGNED_SGIX 0x85E4
#define GL_LUMINANCE_ALPHA_SIGNED_SGIX 0x85E5
#define GL_RGB16_SIGNED_SGIX 0x85E6
#define GL_RGBA16_SIGNED_SGIX 0x85E7
#define GL_ALPHA16_SIGNED_SGIX 0x85E8
#define GL_LUMINANCE16_SIGNED_SGIX 0x85E9
#define GL_INTENSITY16_SIGNED_SGIX 0x85EA
#define GL_LUMINANCE16_ALPHA16_SIGNED_SGIX 0x85EB
#define GL_RGB_EXTENDED_RANGE_SGIX 0x85EC
#define GL_RGBA_EXTENDED_RANGE_SGIX 0x85ED
#define GL_ALPHA_EXTENDED_RANGE_SGIX 0x85EE
#define GL_LUMINANCE_EXTENDED_RANGE_SGIX 0x85EF
#define GL_INTENSITY_EXTENDED_RANGE_SGIX 0x85F0
#define GL_LUMINANCE_ALPHA_EXTENDED_RANGE_SGIX 0x85F1
#define GL_RGB16_EXTENDED_RANGE_SGIX 0x85F2
#define GL_RGBA16_EXTENDED_RANGE_SGIX 0x85F3
#define GL_ALPHA16_EXTENDED_RANGE_SGIX 0x85F4
#define GL_LUMINANCE16_EXTENDED_RANGE_SGIX 0x85F5
#define GL_INTENSITY16_EXTENDED_RANGE_SGIX 0x85F6
#define GL_LUMINANCE16_ALPHA16_EXTENDED_RANGE_SGIX 0x85F7
#define GL_MIN_LUMINANCE_SGIS 0x85F8
#define GL_MAX_LUMINANCE_SGIS 0x85F9
#define GL_MIN_INTENSITY_SGIS 0x85FA
#define GL_MAX_INTENSITY_SGIS 0x85FB

#define GLEW_SGIX_texture_range GLEW_GET_VAR(G_GLEW_SGIX_texture_range)

#endif /* GL_SGIX_texture_range */

/* ----------------------- GL_SGIX_texture_scale_bias ---------------------- */

#ifndef GL_SGIX_texture_scale_bias
#define GL_SGIX_texture_scale_bias 1

#define GL_POST_TEXTURE_FILTER_BIAS_SGIX 0x8179
#define GL_POST_TEXTURE_FILTER_SCALE_SGIX 0x817A
#define GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX 0x817B
#define GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX 0x817C

#define GLEW_SGIX_texture_scale_bias GLEW_GET_VAR(G_GLEW_SGIX_texture_scale_bias)

#endif /* GL_SGIX_texture_scale_bias */

/* ------------------------- GL_SGIX_vertex_preclip ------------------------ */

#ifndef GL_SGIX_vertex_preclip
#define GL_SGIX_vertex_preclip 1

#define GL_VERTEX_PRECLIP_SGIX 0x83EE
#define GL_VERTEX_PRECLIP_HINT_SGIX 0x83EF

#define GLEW_SGIX_vertex_preclip GLEW_GET_VAR(G_GLEW_SGIX_vertex_preclip)

#endif /* GL_SGIX_vertex_preclip */

/* ---------------------- GL_SGIX_vertex_preclip_hint ---------------------- */

#ifndef GL_SGIX_vertex_preclip_hint
#define GL_SGIX_vertex_preclip_hint 1

#define GL_VERTEX_PRECLIP_SGIX 0x83EE
#define GL_VERTEX_PRECLIP_HINT_SGIX 0x83EF

#define GLEW_SGIX_vertex_preclip_hint GLEW_GET_VAR(G_GLEW_SGIX_vertex_preclip_hint)

#endif /* GL_SGIX_vertex_preclip_hint */

/* ----------------------------- GL_SGIX_ycrcb ----------------------------- */

#ifndef GL_SGIX_ycrcb
#define GL_SGIX_ycrcb 1

#define GLEW_SGIX_ycrcb GLEW_GET_VAR(G_GLEW_SGIX_ycrcb)

#endif /* GL_SGIX_ycrcb */

/* -------------------------- GL_SGI_color_matrix -------------------------- */

#ifndef GL_SGI_color_matrix
#define GL_SGI_color_matrix 1

#define GL_COLOR_MATRIX_SGI 0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI 0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE_SGI 0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI 0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI 0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI 0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS_SGI 0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI 0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI 0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI 0x80BB

#define GLEW_SGI_color_matrix GLEW_GET_VAR(G_GLEW_SGI_color_matrix)

#endif /* GL_SGI_color_matrix */

/* --------------------------- GL_SGI_color_table -------------------------- */

#ifndef GL_SGI_color_table
#define GL_SGI_color_table 1

#define GL_COLOR_TABLE_SGI 0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D2
#define GL_PROXY_COLOR_TABLE_SGI 0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI 0x80D5
#define GL_COLOR_TABLE_SCALE_SGI 0x80D6
#define GL_COLOR_TABLE_BIAS_SGI 0x80D7
#define GL_COLOR_TABLE_FORMAT_SGI 0x80D8
#define GL_COLOR_TABLE_WIDTH_SGI 0x80D9
#define GL_COLOR_TABLE_RED_SIZE_SGI 0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE_SGI 0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE_SGI 0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE_SGI 0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE_SGI 0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE_SGI 0x80DF

typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERFVSGIPROC) (GLenum target, GLenum pname, const GLfloat* params);
typedef void (GLAPIENTRY * PFNGLCOLORTABLEPARAMETERIVSGIPROC) (GLenum target, GLenum pname, const GLint* params);
typedef void (GLAPIENTRY * PFNGLCOLORTABLESGIPROC) (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void* table);
typedef void (GLAPIENTRY * PFNGLCOPYCOLORTABLESGIPROC) (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERFVSGIPROC) (GLenum target, GLenum pname, GLfloat* params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLEPARAMETERIVSGIPROC) (GLenum target, GLenum pname, GLint* params);
typedef void (GLAPIENTRY * PFNGLGETCOLORTABLESGIPROC) (GLenum target, GLenum format, GLenum type, void* table);

#define glColorTableParameterfvSGI GLEW_GET_FUN(G_glewColorTableParameterfvSGI)
#define glColorTableParameterivSGI GLEW_GET_FUN(G_glewColorTableParameterivSGI)
#define glColorTableSGI GLEW_GET_FUN(G_glewColorTableSGI)
#define glCopyColorTableSGI GLEW_GET_FUN(G_glewCopyColorTableSGI)
#define glGetColorTableParameterfvSGI GLEW_GET_FUN(G_glewGetColorTableParameterfvSGI)
#define glGetColorTableParameterivSGI GLEW_GET_FUN(G_glewGetColorTableParameterivSGI)
#define glGetColorTableSGI GLEW_GET_FUN(G_glewGetColorTableSGI)

#define GLEW_SGI_color_table GLEW_GET_VAR(G_GLEW_SGI_color_table)

#endif /* GL_SGI_color_table */

/* ----------------------- GL_SGI_texture_color_table ---------------------- */

#ifndef GL_SGI_texture_color_table
#define GL_SGI_texture_color_table 1

#define GL_TEXTURE_COLOR_TABLE_SGI 0x80BC
#define GL_PROXY_TEXTURE_COLOR_TABLE_SGI 0x80BD

#define GLEW_SGI_texture_color_table GLEW_GET_VAR(G_GLEW_SGI_texture_color_table)

#endif /* GL_SGI_texture_color_table */

/* ------------------------- GL_SUNX_constant_data ------------------------- */

#ifndef GL_SUNX_constant_data
#define GL_SUNX_constant_data 1

#define GL_UNPACK_CONSTANT_DATA_SUNX 0x81D5
#define GL_TEXTURE_CONSTANT_DATA_SUNX 0x81D6

typedef void (GLAPIENTRY * PFNGLFINISHTEXTURESUNXPROC) (void);

#define glFinishTextureSUNX GLEW_GET_FUN(G_glewFinishTextureSUNX)

#define GLEW_SUNX_constant_data GLEW_GET_VAR(G_GLEW_SUNX_constant_data)

#endif /* GL_SUNX_constant_data */

/* -------------------- GL_SUN_convolution_border_modes -------------------- */

#ifndef GL_SUN_convolution_border_modes
#define GL_SUN_convolution_border_modes 1

#define GL_WRAP_BORDER_SUN 0x81D4

#define GLEW_SUN_convolution_border_modes GLEW_GET_VAR(G_GLEW_SUN_convolution_border_modes)

#endif /* GL_SUN_convolution_border_modes */

/* -------------------------- GL_SUN_global_alpha -------------------------- */

#ifndef GL_SUN_global_alpha
#define GL_SUN_global_alpha 1

#define GL_GLOBAL_ALPHA_SUN 0x81D9
#define GL_GLOBAL_ALPHA_FACTOR_SUN 0x81DA

typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORBSUNPROC) (GLbyte factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORDSUNPROC) (GLdouble factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORFSUNPROC) (GLfloat factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORISUNPROC) (GLint factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORSSUNPROC) (GLshort factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORUBSUNPROC) (GLubyte factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORUISUNPROC) (GLuint factor);
typedef void (GLAPIENTRY * PFNGLGLOBALALPHAFACTORUSSUNPROC) (GLushort factor);

#define glGlobalAlphaFactorbSUN GLEW_GET_FUN(G_glewGlobalAlphaFactorbSUN)
#define glGlobalAlphaFactordSUN GLEW_GET_FUN(G_glewGlobalAlphaFactordSUN)
#define glGlobalAlphaFactorfSUN GLEW_GET_FUN(G_glewGlobalAlphaFactorfSUN)
#define glGlobalAlphaFactoriSUN GLEW_GET_FUN(G_glewGlobalAlphaFactoriSUN)
#define glGlobalAlphaFactorsSUN GLEW_GET_FUN(G_glewGlobalAlphaFactorsSUN)
#define glGlobalAlphaFactorubSUN GLEW_GET_FUN(G_glewGlobalAlphaFactorubSUN)
#define glGlobalAlphaFactoruiSUN GLEW_GET_FUN(G_glewGlobalAlphaFactoruiSUN)
#define glGlobalAlphaFactorusSUN GLEW_GET_FUN(G_glewGlobalAlphaFactorusSUN)

#define GLEW_SUN_global_alpha GLEW_GET_VAR(G_GLEW_SUN_global_alpha)

#endif /* GL_SUN_global_alpha */

/* --------------------------- GL_SUN_mesh_array --------------------------- */

#ifndef GL_SUN_mesh_array
#define GL_SUN_mesh_array 1

#define GL_QUAD_MESH_SUN 0x8614
#define GL_TRIANGLE_MESH_SUN 0x8615

#define GLEW_SUN_mesh_array GLEW_GET_VAR(G_GLEW_SUN_mesh_array)

#endif /* GL_SUN_mesh_array */

/* ------------------------ GL_SUN_read_video_pixels ----------------------- */

#ifndef GL_SUN_read_video_pixels
#define GL_SUN_read_video_pixels 1

typedef void (GLAPIENTRY * PFNGLREADVIDEOPIXELSSUNPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid* pixels);

#define glReadVideoPixelsSUN GLEW_GET_FUN(G_glewReadVideoPixelsSUN)

#define GLEW_SUN_read_video_pixels GLEW_GET_VAR(G_GLEW_SUN_read_video_pixels)

#endif /* GL_SUN_read_video_pixels */

/* --------------------------- GL_SUN_slice_accum -------------------------- */

#ifndef GL_SUN_slice_accum
#define GL_SUN_slice_accum 1

#define GL_SLICE_ACCUM_SUN 0x85CC

#define GLEW_SUN_slice_accum GLEW_GET_VAR(G_GLEW_SUN_slice_accum)

#endif /* GL_SUN_slice_accum */

/* -------------------------- GL_SUN_triangle_list ------------------------- */

#ifndef GL_SUN_triangle_list
#define GL_SUN_triangle_list 1

#define GL_RESTART_SUN 0x01
#define GL_REPLACE_MIDDLE_SUN 0x02
#define GL_REPLACE_OLDEST_SUN 0x03
#define GL_TRIANGLE_LIST_SUN 0x81D7
#define GL_REPLACEMENT_CODE_SUN 0x81D8
#define GL_REPLACEMENT_CODE_ARRAY_SUN 0x85C0
#define GL_REPLACEMENT_CODE_ARRAY_TYPE_SUN 0x85C1
#define GL_REPLACEMENT_CODE_ARRAY_STRIDE_SUN 0x85C2
#define GL_REPLACEMENT_CODE_ARRAY_POINTER_SUN 0x85C3
#define GL_R1UI_V3F_SUN 0x85C4
#define GL_R1UI_C4UB_V3F_SUN 0x85C5
#define GL_R1UI_C3F_V3F_SUN 0x85C6
#define GL_R1UI_N3F_V3F_SUN 0x85C7
#define GL_R1UI_C4F_N3F_V3F_SUN 0x85C8
#define GL_R1UI_T2F_V3F_SUN 0x85C9
#define GL_R1UI_T2F_N3F_V3F_SUN 0x85CA
#define GL_R1UI_T2F_C4F_N3F_V3F_SUN 0x85CB

typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEPOINTERSUNPROC) (GLenum type, GLsizei stride, const void* pointer);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUBSUNPROC) (GLubyte code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUBVSUNPROC) (const GLubyte* code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUISUNPROC) (GLuint code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUIVSUNPROC) (const GLuint* code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUSSUNPROC) (GLushort code);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUSVSUNPROC) (const GLushort* code);

#define glReplacementCodePointerSUN GLEW_GET_FUN(G_glewReplacementCodePointerSUN)
#define glReplacementCodeubSUN GLEW_GET_FUN(G_glewReplacementCodeubSUN)
#define glReplacementCodeubvSUN GLEW_GET_FUN(G_glewReplacementCodeubvSUN)
#define glReplacementCodeuiSUN GLEW_GET_FUN(G_glewReplacementCodeuiSUN)
#define glReplacementCodeuivSUN GLEW_GET_FUN(G_glewReplacementCodeuivSUN)
#define glReplacementCodeusSUN GLEW_GET_FUN(G_glewReplacementCodeusSUN)
#define glReplacementCodeusvSUN GLEW_GET_FUN(G_glewReplacementCodeusvSUN)

#define GLEW_SUN_triangle_list GLEW_GET_VAR(G_GLEW_SUN_triangle_list)

#endif /* GL_SUN_triangle_list */

/* ----------------------------- GL_SUN_vertex ----------------------------- */

#ifndef GL_SUN_vertex
#define GL_SUN_vertex 1

typedef void (GLAPIENTRY * PFNGLCOLOR3FVERTEX3FSUNPROC) (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLCOLOR3FVERTEX3FVSUNPROC) (const GLfloat* c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVERTEX2FSUNPROC) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVERTEX2FVSUNPROC) (const GLubyte* c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVERTEX3FSUNPROC) (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLCOLOR4UBVERTEX3FVSUNPROC) (const GLubyte* c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLNORMAL3FVERTEX3FSUNPROC) (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC) (GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC) (const GLuint* rc, const GLubyte *c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC) (GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *tc, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC) (GLuint rc, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC) (const GLuint* rc, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC) (const GLfloat* tc, const GLubyte *c, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FVERTEX3FSUNPROC) (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z);
typedef void (GLAPIENTRY * PFNGLTEXCOORD2FVERTEX3FVSUNPROC) (const GLfloat* tc, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC) (const GLfloat* tc, const GLfloat *c, const GLfloat *n, const GLfloat *v);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FVERTEX4FSUNPROC) (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (GLAPIENTRY * PFNGLTEXCOORD4FVERTEX4FVSUNPROC) (const GLfloat* tc, const GLfloat *v);

#define glColor3fVertex3fSUN GLEW_GET_FUN(G_glewColor3fVertex3fSUN)
#define glColor3fVertex3fvSUN GLEW_GET_FUN(G_glewColor3fVertex3fvSUN)
#define glColor4fNormal3fVertex3fSUN GLEW_GET_FUN(G_glewColor4fNormal3fVertex3fSUN)
#define glColor4fNormal3fVertex3fvSUN GLEW_GET_FUN(G_glewColor4fNormal3fVertex3fvSUN)
#define glColor4ubVertex2fSUN GLEW_GET_FUN(G_glewColor4ubVertex2fSUN)
#define glColor4ubVertex2fvSUN GLEW_GET_FUN(G_glewColor4ubVertex2fvSUN)
#define glColor4ubVertex3fSUN GLEW_GET_FUN(G_glewColor4ubVertex3fSUN)
#define glColor4ubVertex3fvSUN GLEW_GET_FUN(G_glewColor4ubVertex3fvSUN)
#define glNormal3fVertex3fSUN GLEW_GET_FUN(G_glewNormal3fVertex3fSUN)
#define glNormal3fVertex3fvSUN GLEW_GET_FUN(G_glewNormal3fVertex3fvSUN)
#define glReplacementCodeuiColor3fVertex3fSUN GLEW_GET_FUN(G_glewReplacementCodeuiColor3fVertex3fSUN)
#define glReplacementCodeuiColor3fVertex3fvSUN GLEW_GET_FUN(G_glewReplacementCodeuiColor3fVertex3fvSUN)
#define glReplacementCodeuiColor4fNormal3fVertex3fSUN GLEW_GET_FUN(G_glewReplacementCodeuiColor4fNormal3fVertex3fSUN)
#define glReplacementCodeuiColor4fNormal3fVertex3fvSUN GLEW_GET_FUN(G_glewReplacementCodeuiColor4fNormal3fVertex3fvSUN)
#define glReplacementCodeuiColor4ubVertex3fSUN GLEW_GET_FUN(G_glewReplacementCodeuiColor4ubVertex3fSUN)
#define glReplacementCodeuiColor4ubVertex3fvSUN GLEW_GET_FUN(G_glewReplacementCodeuiColor4ubVertex3fvSUN)
#define glReplacementCodeuiNormal3fVertex3fSUN GLEW_GET_FUN(G_glewReplacementCodeuiNormal3fVertex3fSUN)
#define glReplacementCodeuiNormal3fVertex3fvSUN GLEW_GET_FUN(G_glewReplacementCodeuiNormal3fVertex3fvSUN)
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN GLEW_GET_FUN(G_glewReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN)
#define glReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN GLEW_GET_FUN(G_glewReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN)
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fSUN GLEW_GET_FUN(G_glewReplacementCodeuiTexCoord2fNormal3fVertex3fSUN)
#define glReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN GLEW_GET_FUN(G_glewReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN)
#define glReplacementCodeuiTexCoord2fVertex3fSUN GLEW_GET_FUN(G_glewReplacementCodeuiTexCoord2fVertex3fSUN)
#define glReplacementCodeuiTexCoord2fVertex3fvSUN GLEW_GET_FUN(G_glewReplacementCodeuiTexCoord2fVertex3fvSUN)
#define glReplacementCodeuiVertex3fSUN GLEW_GET_FUN(G_glewReplacementCodeuiVertex3fSUN)
#define glReplacementCodeuiVertex3fvSUN GLEW_GET_FUN(G_glewReplacementCodeuiVertex3fvSUN)
#define glTexCoord2fColor3fVertex3fSUN GLEW_GET_FUN(G_glewTexCoord2fColor3fVertex3fSUN)
#define glTexCoord2fColor3fVertex3fvSUN GLEW_GET_FUN(G_glewTexCoord2fColor3fVertex3fvSUN)
#define glTexCoord2fColor4fNormal3fVertex3fSUN GLEW_GET_FUN(G_glewTexCoord2fColor4fNormal3fVertex3fSUN)
#define glTexCoord2fColor4fNormal3fVertex3fvSUN GLEW_GET_FUN(G_glewTexCoord2fColor4fNormal3fVertex3fvSUN)
#define glTexCoord2fColor4ubVertex3fSUN GLEW_GET_FUN(G_glewTexCoord2fColor4ubVertex3fSUN)
#define glTexCoord2fColor4ubVertex3fvSUN GLEW_GET_FUN(G_glewTexCoord2fColor4ubVertex3fvSUN)
#define glTexCoord2fNormal3fVertex3fSUN GLEW_GET_FUN(G_glewTexCoord2fNormal3fVertex3fSUN)
#define glTexCoord2fNormal3fVertex3fvSUN GLEW_GET_FUN(G_glewTexCoord2fNormal3fVertex3fvSUN)
#define glTexCoord2fVertex3fSUN GLEW_GET_FUN(G_glewTexCoord2fVertex3fSUN)
#define glTexCoord2fVertex3fvSUN GLEW_GET_FUN(G_glewTexCoord2fVertex3fvSUN)
#define glTexCoord4fColor4fNormal3fVertex4fSUN GLEW_GET_FUN(G_glewTexCoord4fColor4fNormal3fVertex4fSUN)
#define glTexCoord4fColor4fNormal3fVertex4fvSUN GLEW_GET_FUN(G_glewTexCoord4fColor4fNormal3fVertex4fvSUN)
#define glTexCoord4fVertex4fSUN GLEW_GET_FUN(G_glewTexCoord4fVertex4fSUN)
#define glTexCoord4fVertex4fvSUN GLEW_GET_FUN(G_glewTexCoord4fVertex4fvSUN)

#define GLEW_SUN_vertex GLEW_GET_VAR(G_GLEW_SUN_vertex)

#endif /* GL_SUN_vertex */

/* -------------------------- GL_WIN_phong_shading ------------------------- */

#ifndef GL_WIN_phong_shading
#define GL_WIN_phong_shading 1

#define GL_PHONG_WIN 0x80EA
#define GL_PHONG_HINT_WIN 0x80EB

#define GLEW_WIN_phong_shading GLEW_GET_VAR(G_GLEW_WIN_phong_shading)

#endif /* GL_WIN_phong_shading */

/* -------------------------- GL_WIN_specular_fog -------------------------- */

#ifndef GL_WIN_specular_fog
#define GL_WIN_specular_fog 1

#define GL_FOG_SPECULAR_TEXTURE_WIN 0x80EC

#define GLEW_WIN_specular_fog GLEW_GET_VAR(G_GLEW_WIN_specular_fog)

#endif /* GL_WIN_specular_fog */

/* ---------------------------- GL_WIN_swap_hint --------------------------- */

#ifndef GL_WIN_swap_hint
#define GL_WIN_swap_hint 1

typedef void (GLAPIENTRY * PFNGLADDSWAPHINTRECTWINPROC) (GLint x, GLint y, GLsizei width, GLsizei height);

#define glAddSwapHintRectWIN GLEW_GET_FUN(G_glewAddSwapHintRectWIN)

#define GLEW_WIN_swap_hint GLEW_GET_VAR(G_GLEW_WIN_swap_hint)

#endif /* GL_WIN_swap_hint */

/* ------------------------------------------------------------------------- */

#if defined(GLEW_MX) && defined(_WIN32)
#define GLEW_FUN_EXPORT
#else
#define GLEW_FUN_EXPORT GLEWAPI
#endif /* GLEW_MX */

#if defined(GLEW_MX)
#define GLEW_VAR_EXPORT
#else
#define GLEW_VAR_EXPORT GLEWAPI
#endif /* GLEW_MX */

#if defined(GLEW_MX) && defined(_WIN32)
struct GLEWContextStruct
{
#endif /* GLEW_MX */

GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE3DPROC G_glewCopyTexSubImage3D;
GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTSPROC G_glewDrawRangeElements;
GLEW_FUN_EXPORT PFNGLTEXIMAGE3DPROC G_glewTexImage3D;
GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE3DPROC G_glewTexSubImage3D;

GLEW_FUN_EXPORT PFNGLACTIVETEXTUREPROC G_glewActiveTexture;
GLEW_FUN_EXPORT PFNGLCLIENTACTIVETEXTUREPROC G_glewClientActiveTexture;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE1DPROC G_glewCompressedTexImage1D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE2DPROC G_glewCompressedTexImage2D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE3DPROC G_glewCompressedTexImage3D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC G_glewCompressedTexSubImage1D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC G_glewCompressedTexSubImage2D;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC G_glewCompressedTexSubImage3D;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDTEXIMAGEPROC G_glewGetCompressedTexImage;
GLEW_FUN_EXPORT PFNGLLOADTRANSPOSEMATRIXDPROC G_glewLoadTransposeMatrixd;
GLEW_FUN_EXPORT PFNGLLOADTRANSPOSEMATRIXFPROC G_glewLoadTransposeMatrixf;
GLEW_FUN_EXPORT PFNGLMULTTRANSPOSEMATRIXDPROC G_glewMultTransposeMatrixd;
GLEW_FUN_EXPORT PFNGLMULTTRANSPOSEMATRIXFPROC G_glewMultTransposeMatrixf;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1DPROC G_glewMultiTexCoord1d;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1DVPROC G_glewMultiTexCoord1dv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1FPROC G_glewMultiTexCoord1f;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1FVPROC G_glewMultiTexCoord1fv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1IPROC G_glewMultiTexCoord1i;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1IVPROC G_glewMultiTexCoord1iv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1SPROC G_glewMultiTexCoord1s;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1SVPROC G_glewMultiTexCoord1sv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2DPROC G_glewMultiTexCoord2d;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2DVPROC G_glewMultiTexCoord2dv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2FPROC G_glewMultiTexCoord2f;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2FVPROC G_glewMultiTexCoord2fv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2IPROC G_glewMultiTexCoord2i;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2IVPROC G_glewMultiTexCoord2iv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2SPROC G_glewMultiTexCoord2s;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2SVPROC G_glewMultiTexCoord2sv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3DPROC G_glewMultiTexCoord3d;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3DVPROC G_glewMultiTexCoord3dv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3FPROC G_glewMultiTexCoord3f;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3FVPROC G_glewMultiTexCoord3fv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3IPROC G_glewMultiTexCoord3i;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3IVPROC G_glewMultiTexCoord3iv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3SPROC G_glewMultiTexCoord3s;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3SVPROC G_glewMultiTexCoord3sv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4DPROC G_glewMultiTexCoord4d;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4DVPROC G_glewMultiTexCoord4dv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4FPROC G_glewMultiTexCoord4f;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4FVPROC G_glewMultiTexCoord4fv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4IPROC G_glewMultiTexCoord4i;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4IVPROC G_glewMultiTexCoord4iv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4SPROC G_glewMultiTexCoord4s;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4SVPROC G_glewMultiTexCoord4sv;
GLEW_FUN_EXPORT PFNGLSAMPLECOVERAGEPROC G_glewSampleCoverage;

GLEW_FUN_EXPORT PFNGLBLENDCOLORPROC G_glewBlendColor;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONPROC G_glewBlendEquation;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEPROC G_glewBlendFuncSeparate;
GLEW_FUN_EXPORT PFNGLFOGCOORDPOINTERPROC G_glewFogCoordPointer;
GLEW_FUN_EXPORT PFNGLFOGCOORDDPROC G_glewFogCoordd;
GLEW_FUN_EXPORT PFNGLFOGCOORDDVPROC G_glewFogCoorddv;
GLEW_FUN_EXPORT PFNGLFOGCOORDFPROC G_glewFogCoordf;
GLEW_FUN_EXPORT PFNGLFOGCOORDFVPROC G_glewFogCoordfv;
GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSPROC G_glewMultiDrawArrays;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSPROC G_glewMultiDrawElements;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFPROC G_glewPointParameterf;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFVPROC G_glewPointParameterfv;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERIPROC G_glewPointParameteri;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERIVPROC G_glewPointParameteriv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3BPROC G_glewSecondaryColor3b;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3BVPROC G_glewSecondaryColor3bv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3DPROC G_glewSecondaryColor3d;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3DVPROC G_glewSecondaryColor3dv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3FPROC G_glewSecondaryColor3f;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3FVPROC G_glewSecondaryColor3fv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3IPROC G_glewSecondaryColor3i;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3IVPROC G_glewSecondaryColor3iv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3SPROC G_glewSecondaryColor3s;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3SVPROC G_glewSecondaryColor3sv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UBPROC G_glewSecondaryColor3ub;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UBVPROC G_glewSecondaryColor3ubv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UIPROC G_glewSecondaryColor3ui;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UIVPROC G_glewSecondaryColor3uiv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3USPROC G_glewSecondaryColor3us;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3USVPROC G_glewSecondaryColor3usv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORPOINTERPROC G_glewSecondaryColorPointer;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2DPROC G_glewWindowPos2d;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2DVPROC G_glewWindowPos2dv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FPROC G_glewWindowPos2f;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FVPROC G_glewWindowPos2fv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IPROC G_glewWindowPos2i;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IVPROC G_glewWindowPos2iv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SPROC G_glewWindowPos2s;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SVPROC G_glewWindowPos2sv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DPROC G_glewWindowPos3d;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DVPROC G_glewWindowPos3dv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FPROC G_glewWindowPos3f;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FVPROC G_glewWindowPos3fv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IPROC G_glewWindowPos3i;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IVPROC G_glewWindowPos3iv;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SPROC G_glewWindowPos3s;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SVPROC G_glewWindowPos3sv;

GLEW_FUN_EXPORT PFNGLBEGINQUERYPROC G_glewBeginQuery;
GLEW_FUN_EXPORT PFNGLBINDBUFFERPROC G_glewBindBuffer;
GLEW_FUN_EXPORT PFNGLBUFFERDATAPROC G_glewBufferData;
GLEW_FUN_EXPORT PFNGLBUFFERSUBDATAPROC G_glewBufferSubData;
GLEW_FUN_EXPORT PFNGLDELETEBUFFERSPROC G_glewDeleteBuffers;
GLEW_FUN_EXPORT PFNGLDELETEQUERIESPROC G_glewDeleteQueries;
GLEW_FUN_EXPORT PFNGLENDQUERYPROC G_glewEndQuery;
GLEW_FUN_EXPORT PFNGLGENBUFFERSPROC G_glewGenBuffers;
GLEW_FUN_EXPORT PFNGLGENQUERIESPROC G_glewGenQueries;
GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERIVPROC G_glewGetBufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETBUFFERPOINTERVPROC G_glewGetBufferPointerv;
GLEW_FUN_EXPORT PFNGLGETBUFFERSUBDATAPROC G_glewGetBufferSubData;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTIVPROC G_glewGetQueryObjectiv;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUIVPROC G_glewGetQueryObjectuiv;
GLEW_FUN_EXPORT PFNGLGETQUERYIVPROC G_glewGetQueryiv;
GLEW_FUN_EXPORT PFNGLISBUFFERPROC G_glewIsBuffer;
GLEW_FUN_EXPORT PFNGLISQUERYPROC G_glewIsQuery;
GLEW_FUN_EXPORT PFNGLMAPBUFFERPROC G_glewMapBuffer;
GLEW_FUN_EXPORT PFNGLUNMAPBUFFERPROC G_glewUnmapBuffer;

GLEW_FUN_EXPORT PFNGLATTACHSHADERPROC G_glewAttachShader;
GLEW_FUN_EXPORT PFNGLBINDATTRIBLOCATIONPROC G_glewBindAttribLocation;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEPROC G_glewBlendEquationSeparate;
GLEW_FUN_EXPORT PFNGLCOMPILESHADERPROC G_glewCompileShader;
GLEW_FUN_EXPORT PFNGLCREATEPROGRAMPROC G_glewCreateProgram;
GLEW_FUN_EXPORT PFNGLCREATESHADERPROC G_glewCreateShader;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMPROC G_glewDeleteProgram;
GLEW_FUN_EXPORT PFNGLDELETESHADERPROC G_glewDeleteShader;
GLEW_FUN_EXPORT PFNGLDETACHSHADERPROC G_glewDetachShader;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXATTRIBARRAYPROC G_glewDisableVertexAttribArray;
GLEW_FUN_EXPORT PFNGLDRAWBUFFERSPROC G_glewDrawBuffers;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXATTRIBARRAYPROC G_glewEnableVertexAttribArray;
GLEW_FUN_EXPORT PFNGLGETACTIVEATTRIBPROC G_glewGetActiveAttrib;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMPROC G_glewGetActiveUniform;
GLEW_FUN_EXPORT PFNGLGETATTACHEDSHADERSPROC G_glewGetAttachedShaders;
GLEW_FUN_EXPORT PFNGLGETATTRIBLOCATIONPROC G_glewGetAttribLocation;
GLEW_FUN_EXPORT PFNGLGETPROGRAMINFOLOGPROC G_glewGetProgramInfoLog;
GLEW_FUN_EXPORT PFNGLGETPROGRAMIVPROC G_glewGetProgramiv;
GLEW_FUN_EXPORT PFNGLGETSHADERINFOLOGPROC G_glewGetShaderInfoLog;
GLEW_FUN_EXPORT PFNGLGETSHADERSOURCEPROC G_glewGetShaderSource;
GLEW_FUN_EXPORT PFNGLGETSHADERIVPROC G_glewGetShaderiv;
GLEW_FUN_EXPORT PFNGLGETUNIFORMLOCATIONPROC G_glewGetUniformLocation;
GLEW_FUN_EXPORT PFNGLGETUNIFORMFVPROC G_glewGetUniformfv;
GLEW_FUN_EXPORT PFNGLGETUNIFORMIVPROC G_glewGetUniformiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBPOINTERVPROC G_glewGetVertexAttribPointerv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBDVPROC G_glewGetVertexAttribdv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBFVPROC G_glewGetVertexAttribfv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIVPROC G_glewGetVertexAttribiv;
GLEW_FUN_EXPORT PFNGLISPROGRAMPROC G_glewIsProgram;
GLEW_FUN_EXPORT PFNGLISSHADERPROC G_glewIsShader;
GLEW_FUN_EXPORT PFNGLLINKPROGRAMPROC G_glewLinkProgram;
GLEW_FUN_EXPORT PFNGLSHADERSOURCEPROC G_glewShaderSource;
GLEW_FUN_EXPORT PFNGLSTENCILFUNCSEPARATEPROC G_glewStencilFuncSeparate;
GLEW_FUN_EXPORT PFNGLSTENCILMASKSEPARATEPROC G_glewStencilMaskSeparate;
GLEW_FUN_EXPORT PFNGLSTENCILOPSEPARATEPROC G_glewStencilOpSeparate;
GLEW_FUN_EXPORT PFNGLUNIFORM1FPROC G_glewUniform1f;
GLEW_FUN_EXPORT PFNGLUNIFORM1FVPROC G_glewUniform1fv;
GLEW_FUN_EXPORT PFNGLUNIFORM1IPROC G_glewUniform1i;
GLEW_FUN_EXPORT PFNGLUNIFORM1IVPROC G_glewUniform1iv;
GLEW_FUN_EXPORT PFNGLUNIFORM2FPROC G_glewUniform2f;
GLEW_FUN_EXPORT PFNGLUNIFORM2FVPROC G_glewUniform2fv;
GLEW_FUN_EXPORT PFNGLUNIFORM2IPROC G_glewUniform2i;
GLEW_FUN_EXPORT PFNGLUNIFORM2IVPROC G_glewUniform2iv;
GLEW_FUN_EXPORT PFNGLUNIFORM3FPROC G_glewUniform3f;
GLEW_FUN_EXPORT PFNGLUNIFORM3FVPROC G_glewUniform3fv;
GLEW_FUN_EXPORT PFNGLUNIFORM3IPROC G_glewUniform3i;
GLEW_FUN_EXPORT PFNGLUNIFORM3IVPROC G_glewUniform3iv;
GLEW_FUN_EXPORT PFNGLUNIFORM4FPROC G_glewUniform4f;
GLEW_FUN_EXPORT PFNGLUNIFORM4FVPROC G_glewUniform4fv;
GLEW_FUN_EXPORT PFNGLUNIFORM4IPROC G_glewUniform4i;
GLEW_FUN_EXPORT PFNGLUNIFORM4IVPROC G_glewUniform4iv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2FVPROC G_glewUniformMatrix2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3FVPROC G_glewUniformMatrix3fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4FVPROC G_glewUniformMatrix4fv;
GLEW_FUN_EXPORT PFNGLUSEPROGRAMPROC G_glewUseProgram;
GLEW_FUN_EXPORT PFNGLVALIDATEPROGRAMPROC G_glewValidateProgram;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DPROC G_glewVertexAttrib1d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DVPROC G_glewVertexAttrib1dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FPROC G_glewVertexAttrib1f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FVPROC G_glewVertexAttrib1fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SPROC G_glewVertexAttrib1s;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SVPROC G_glewVertexAttrib1sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DPROC G_glewVertexAttrib2d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DVPROC G_glewVertexAttrib2dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FPROC G_glewVertexAttrib2f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FVPROC G_glewVertexAttrib2fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SPROC G_glewVertexAttrib2s;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SVPROC G_glewVertexAttrib2sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DPROC G_glewVertexAttrib3d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DVPROC G_glewVertexAttrib3dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FPROC G_glewVertexAttrib3f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FVPROC G_glewVertexAttrib3fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SPROC G_glewVertexAttrib3s;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SVPROC G_glewVertexAttrib3sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NBVPROC G_glewVertexAttrib4Nbv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NIVPROC G_glewVertexAttrib4Niv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NSVPROC G_glewVertexAttrib4Nsv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUBPROC G_glewVertexAttrib4Nub;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUBVPROC G_glewVertexAttrib4Nubv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUIVPROC G_glewVertexAttrib4Nuiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUSVPROC G_glewVertexAttrib4Nusv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4BVPROC G_glewVertexAttrib4bv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DPROC G_glewVertexAttrib4d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DVPROC G_glewVertexAttrib4dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FPROC G_glewVertexAttrib4f;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FVPROC G_glewVertexAttrib4fv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4IVPROC G_glewVertexAttrib4iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SPROC G_glewVertexAttrib4s;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SVPROC G_glewVertexAttrib4sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UBVPROC G_glewVertexAttrib4ubv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UIVPROC G_glewVertexAttrib4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4USVPROC G_glewVertexAttrib4usv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBPOINTERPROC G_glewVertexAttribPointer;

GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X3FVPROC G_glewUniformMatrix2x3fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X4FVPROC G_glewUniformMatrix2x4fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X2FVPROC G_glewUniformMatrix3x2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X4FVPROC G_glewUniformMatrix3x4fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X2FVPROC G_glewUniformMatrix4x2fv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X3FVPROC G_glewUniformMatrix4x3fv;

GLEW_FUN_EXPORT PFNGLBEGINCONDITIONALRENDERPROC G_glewBeginConditionalRender;
GLEW_FUN_EXPORT PFNGLBEGINTRANSFORMFEEDBACKPROC G_glewBeginTransformFeedback;
GLEW_FUN_EXPORT PFNGLBINDFRAGDATALOCATIONPROC G_glewBindFragDataLocation;
GLEW_FUN_EXPORT PFNGLCLAMPCOLORPROC G_glewClampColor;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERFIPROC G_glewClearBufferfi;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERFVPROC G_glewClearBufferfv;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERIVPROC G_glewClearBufferiv;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERUIVPROC G_glewClearBufferuiv;
GLEW_FUN_EXPORT PFNGLCOLORMASKIPROC G_glewColorMaski;
GLEW_FUN_EXPORT PFNGLDISABLEIPROC G_glewDisablei;
GLEW_FUN_EXPORT PFNGLENABLEIPROC G_glewEnablei;
GLEW_FUN_EXPORT PFNGLENDCONDITIONALRENDERPROC G_glewEndConditionalRender;
GLEW_FUN_EXPORT PFNGLENDTRANSFORMFEEDBACKPROC G_glewEndTransformFeedback;
GLEW_FUN_EXPORT PFNGLGETBOOLEANI_VPROC G_glewGetBooleani_v;
GLEW_FUN_EXPORT PFNGLGETFRAGDATALOCATIONPROC G_glewGetFragDataLocation;
GLEW_FUN_EXPORT PFNGLGETSTRINGIPROC G_glewGetStringi;
GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERIIVPROC G_glewGetTexParameterIiv;
GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERIUIVPROC G_glewGetTexParameterIuiv;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKVARYINGPROC G_glewGetTransformFeedbackVarying;
GLEW_FUN_EXPORT PFNGLGETUNIFORMUIVPROC G_glewGetUniformuiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIIVPROC G_glewGetVertexAttribIiv;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIUIVPROC G_glewGetVertexAttribIuiv;
GLEW_FUN_EXPORT PFNGLISENABLEDIPROC G_glewIsEnabledi;
GLEW_FUN_EXPORT PFNGLTEXPARAMETERIIVPROC G_glewTexParameterIiv;
GLEW_FUN_EXPORT PFNGLTEXPARAMETERIUIVPROC G_glewTexParameterIuiv;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKVARYINGSPROC G_glewTransformFeedbackVaryings;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIPROC G_glewUniform1ui;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIVPROC G_glewUniform1uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIPROC G_glewUniform2ui;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIVPROC G_glewUniform2uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIPROC G_glewUniform3ui;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIVPROC G_glewUniform3uiv;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIPROC G_glewUniform4ui;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIVPROC G_glewUniform4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1IPROC G_glewVertexAttribI1i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1IVPROC G_glewVertexAttribI1iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1UIPROC G_glewVertexAttribI1ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1UIVPROC G_glewVertexAttribI1uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2IPROC G_glewVertexAttribI2i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2IVPROC G_glewVertexAttribI2iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2UIPROC G_glewVertexAttribI2ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2UIVPROC G_glewVertexAttribI2uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3IPROC G_glewVertexAttribI3i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3IVPROC G_glewVertexAttribI3iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3UIPROC G_glewVertexAttribI3ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3UIVPROC G_glewVertexAttribI3uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4BVPROC G_glewVertexAttribI4bv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IPROC G_glewVertexAttribI4i;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IVPROC G_glewVertexAttribI4iv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4SVPROC G_glewVertexAttribI4sv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UBVPROC G_glewVertexAttribI4ubv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIPROC G_glewVertexAttribI4ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIVPROC G_glewVertexAttribI4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4USVPROC G_glewVertexAttribI4usv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIPOINTERPROC G_glewVertexAttribIPointer;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDPROC G_glewDrawArraysInstanced;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDPROC G_glewDrawElementsInstanced;
GLEW_FUN_EXPORT PFNGLPRIMITIVERESTARTINDEXPROC G_glewPrimitiveRestartIndex;
GLEW_FUN_EXPORT PFNGLTEXBUFFERPROC G_glewTexBuffer;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREPROC G_glewFramebufferTexture;
GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERI64VPROC G_glewGetBufferParameteri64v;
GLEW_FUN_EXPORT PFNGLGETINTEGER64I_VPROC G_glewGetInteger64i_v;

GLEW_FUN_EXPORT PFNGLVERTEXATTRIBDIVISORPROC G_glewVertexAttribDivisor;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEIPROC G_glewBlendEquationSeparatei;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONIPROC G_glewBlendEquationi;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEIPROC G_glewBlendFuncSeparatei;
GLEW_FUN_EXPORT PFNGLBLENDFUNCIPROC G_glewBlendFunci;
GLEW_FUN_EXPORT PFNGLMINSAMPLESHADINGPROC G_glewMinSampleShading;

GLEW_FUN_EXPORT PFNGLTBUFFERMASK3DFXPROC G_glewTbufferMask3DFX;

GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECALLBACKAMDPROC G_glewDebugMessageCallbackAMD;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGEENABLEAMDPROC G_glewDebugMessageEnableAMD;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGEINSERTAMDPROC G_glewDebugMessageInsertAMD;
GLEW_FUN_EXPORT PFNGLGETDEBUGMESSAGELOGAMDPROC G_glewGetDebugMessageLogAMD;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONINDEXEDAMDPROC G_glewBlendEquationIndexedAMD;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEINDEXEDAMDPROC G_glewBlendEquationSeparateIndexedAMD;
GLEW_FUN_EXPORT PFNGLBLENDFUNCINDEXEDAMDPROC G_glewBlendFuncIndexedAMD;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEINDEXEDAMDPROC G_glewBlendFuncSeparateIndexedAMD;

GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSINDIRECTAMDPROC G_glewMultiDrawArraysIndirectAMD;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSINDIRECTAMDPROC G_glewMultiDrawElementsIndirectAMD;

GLEW_FUN_EXPORT PFNGLDELETENAMESAMDPROC G_glewDeleteNamesAMD;
GLEW_FUN_EXPORT PFNGLGENNAMESAMDPROC G_glewGenNamesAMD;
GLEW_FUN_EXPORT PFNGLISNAMEAMDPROC G_glewIsNameAMD;

GLEW_FUN_EXPORT PFNGLBEGINPERFMONITORAMDPROC G_glewBeginPerfMonitorAMD;
GLEW_FUN_EXPORT PFNGLDELETEPERFMONITORSAMDPROC G_glewDeletePerfMonitorsAMD;
GLEW_FUN_EXPORT PFNGLENDPERFMONITORAMDPROC G_glewEndPerfMonitorAMD;
GLEW_FUN_EXPORT PFNGLGENPERFMONITORSAMDPROC G_glewGenPerfMonitorsAMD;
GLEW_FUN_EXPORT PFNGLGETPERFMONITORCOUNTERDATAAMDPROC G_glewGetPerfMonitorCounterDataAMD;
GLEW_FUN_EXPORT PFNGLGETPERFMONITORCOUNTERINFOAMDPROC G_glewGetPerfMonitorCounterInfoAMD;
GLEW_FUN_EXPORT PFNGLGETPERFMONITORCOUNTERSTRINGAMDPROC G_glewGetPerfMonitorCounterStringAMD;
GLEW_FUN_EXPORT PFNGLGETPERFMONITORCOUNTERSAMDPROC G_glewGetPerfMonitorCountersAMD;
GLEW_FUN_EXPORT PFNGLGETPERFMONITORGROUPSTRINGAMDPROC G_glewGetPerfMonitorGroupStringAMD;
GLEW_FUN_EXPORT PFNGLGETPERFMONITORGROUPSAMDPROC G_glewGetPerfMonitorGroupsAMD;
GLEW_FUN_EXPORT PFNGLSELECTPERFMONITORCOUNTERSAMDPROC G_glewSelectPerfMonitorCountersAMD;

GLEW_FUN_EXPORT PFNGLSETMULTISAMPLEFVAMDPROC G_glewSetMultisamplefvAMD;

GLEW_FUN_EXPORT PFNGLSTENCILOPVALUEAMDPROC G_glewStencilOpValueAMD;

GLEW_FUN_EXPORT PFNGLTESSELLATIONFACTORAMDPROC G_glewTessellationFactorAMD;
GLEW_FUN_EXPORT PFNGLTESSELLATIONMODEAMDPROC G_glewTessellationModeAMD;

GLEW_FUN_EXPORT PFNGLDRAWELEMENTARRAYAPPLEPROC G_glewDrawElementArrayAPPLE;
GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTARRAYAPPLEPROC G_glewDrawRangeElementArrayAPPLE;
GLEW_FUN_EXPORT PFNGLELEMENTPOINTERAPPLEPROC G_glewElementPointerAPPLE;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTARRAYAPPLEPROC G_glewMultiDrawElementArrayAPPLE;
GLEW_FUN_EXPORT PFNGLMULTIDRAWRANGEELEMENTARRAYAPPLEPROC G_glewMultiDrawRangeElementArrayAPPLE;

GLEW_FUN_EXPORT PFNGLDELETEFENCESAPPLEPROC G_glewDeleteFencesAPPLE;
GLEW_FUN_EXPORT PFNGLFINISHFENCEAPPLEPROC G_glewFinishFenceAPPLE;
GLEW_FUN_EXPORT PFNGLFINISHOBJECTAPPLEPROC G_glewFinishObjectAPPLE;
GLEW_FUN_EXPORT PFNGLGENFENCESAPPLEPROC G_glewGenFencesAPPLE;
GLEW_FUN_EXPORT PFNGLISFENCEAPPLEPROC G_glewIsFenceAPPLE;
GLEW_FUN_EXPORT PFNGLSETFENCEAPPLEPROC G_glewSetFenceAPPLE;
GLEW_FUN_EXPORT PFNGLTESTFENCEAPPLEPROC G_glewTestFenceAPPLE;
GLEW_FUN_EXPORT PFNGLTESTOBJECTAPPLEPROC G_glewTestObjectAPPLE;

GLEW_FUN_EXPORT PFNGLBUFFERPARAMETERIAPPLEPROC G_glewBufferParameteriAPPLE;
GLEW_FUN_EXPORT PFNGLFLUSHMAPPEDBUFFERRANGEAPPLEPROC G_glewFlushMappedBufferRangeAPPLE;

GLEW_FUN_EXPORT PFNGLGETOBJECTPARAMETERIVAPPLEPROC G_glewGetObjectParameterivAPPLE;
GLEW_FUN_EXPORT PFNGLOBJECTPURGEABLEAPPLEPROC G_glewObjectPurgeableAPPLE;
GLEW_FUN_EXPORT PFNGLOBJECTUNPURGEABLEAPPLEPROC G_glewObjectUnpurgeableAPPLE;

GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERPOINTERVAPPLEPROC G_glewGetTexParameterPointervAPPLE;
GLEW_FUN_EXPORT PFNGLTEXTURERANGEAPPLEPROC G_glewTextureRangeAPPLE;

GLEW_FUN_EXPORT PFNGLBINDVERTEXARRAYAPPLEPROC G_glewBindVertexArrayAPPLE;
GLEW_FUN_EXPORT PFNGLDELETEVERTEXARRAYSAPPLEPROC G_glewDeleteVertexArraysAPPLE;
GLEW_FUN_EXPORT PFNGLGENVERTEXARRAYSAPPLEPROC G_glewGenVertexArraysAPPLE;
GLEW_FUN_EXPORT PFNGLISVERTEXARRAYAPPLEPROC G_glewIsVertexArrayAPPLE;

GLEW_FUN_EXPORT PFNGLFLUSHVERTEXARRAYRANGEAPPLEPROC G_glewFlushVertexArrayRangeAPPLE;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYPARAMETERIAPPLEPROC G_glewVertexArrayParameteriAPPLE;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYRANGEAPPLEPROC G_glewVertexArrayRangeAPPLE;

GLEW_FUN_EXPORT PFNGLDISABLEVERTEXATTRIBAPPLEPROC G_glewDisableVertexAttribAPPLE;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXATTRIBAPPLEPROC G_glewEnableVertexAttribAPPLE;
GLEW_FUN_EXPORT PFNGLISVERTEXATTRIBENABLEDAPPLEPROC G_glewIsVertexAttribEnabledAPPLE;
GLEW_FUN_EXPORT PFNGLMAPVERTEXATTRIB1DAPPLEPROC G_glewMapVertexAttrib1dAPPLE;
GLEW_FUN_EXPORT PFNGLMAPVERTEXATTRIB1FAPPLEPROC G_glewMapVertexAttrib1fAPPLE;
GLEW_FUN_EXPORT PFNGLMAPVERTEXATTRIB2DAPPLEPROC G_glewMapVertexAttrib2dAPPLE;
GLEW_FUN_EXPORT PFNGLMAPVERTEXATTRIB2FAPPLEPROC G_glewMapVertexAttrib2fAPPLE;

GLEW_FUN_EXPORT PFNGLCLEARDEPTHFPROC G_glewClearDepthf;
GLEW_FUN_EXPORT PFNGLDEPTHRANGEFPROC G_glewDepthRangef;
GLEW_FUN_EXPORT PFNGLGETSHADERPRECISIONFORMATPROC G_glewGetShaderPrecisionFormat;
GLEW_FUN_EXPORT PFNGLRELEASESHADERCOMPILERPROC G_glewReleaseShaderCompiler;
GLEW_FUN_EXPORT PFNGLSHADERBINARYPROC G_glewShaderBinary;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC G_glewDrawArraysInstancedBaseInstance;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC G_glewDrawElementsInstancedBaseInstance;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC G_glewDrawElementsInstancedBaseVertexBaseInstance;

GLEW_FUN_EXPORT PFNGLBINDFRAGDATALOCATIONINDEXEDPROC G_glewBindFragDataLocationIndexed;
GLEW_FUN_EXPORT PFNGLGETFRAGDATAINDEXPROC G_glewGetFragDataIndex;

GLEW_FUN_EXPORT PFNGLCREATESYNCFROMCLEVENTARBPROC G_glewCreateSyncFromCLeventARB;

GLEW_FUN_EXPORT PFNGLCLEARBUFFERDATAPROC G_glewClearBufferData;
GLEW_FUN_EXPORT PFNGLCLEARBUFFERSUBDATAPROC G_glewClearBufferSubData;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDBUFFERDATAEXTPROC G_glewClearNamedBufferDataEXT;
GLEW_FUN_EXPORT PFNGLCLEARNAMEDBUFFERSUBDATAEXTPROC G_glewClearNamedBufferSubDataEXT;

GLEW_FUN_EXPORT PFNGLCLAMPCOLORARBPROC G_glewClampColorARB;

GLEW_FUN_EXPORT PFNGLDISPATCHCOMPUTEPROC G_glewDispatchCompute;
GLEW_FUN_EXPORT PFNGLDISPATCHCOMPUTEINDIRECTPROC G_glewDispatchComputeIndirect;

GLEW_FUN_EXPORT PFNGLCOPYBUFFERSUBDATAPROC G_glewCopyBufferSubData;

GLEW_FUN_EXPORT PFNGLCOPYIMAGESUBDATAPROC G_glewCopyImageSubData;

GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECALLBACKARBPROC G_glewDebugMessageCallbackARB;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECONTROLARBPROC G_glewDebugMessageControlARB;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGEINSERTARBPROC G_glewDebugMessageInsertARB;
GLEW_FUN_EXPORT PFNGLGETDEBUGMESSAGELOGARBPROC G_glewGetDebugMessageLogARB;

GLEW_FUN_EXPORT PFNGLDRAWBUFFERSARBPROC G_glewDrawBuffersARB;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEIARBPROC G_glewBlendEquationSeparateiARB;
GLEW_FUN_EXPORT PFNGLBLENDEQUATIONIARBPROC G_glewBlendEquationiARB;
GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEIARBPROC G_glewBlendFuncSeparateiARB;
GLEW_FUN_EXPORT PFNGLBLENDFUNCIARBPROC G_glewBlendFunciARB;

GLEW_FUN_EXPORT PFNGLDRAWELEMENTSBASEVERTEXPROC G_glewDrawElementsBaseVertex;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC G_glewDrawElementsInstancedBaseVertex;
GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC G_glewDrawRangeElementsBaseVertex;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC G_glewMultiDrawElementsBaseVertex;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINDIRECTPROC G_glewDrawArraysIndirect;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINDIRECTPROC G_glewDrawElementsIndirect;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERPARAMETERIPROC G_glewFramebufferParameteri;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERPARAMETERIVPROC G_glewGetFramebufferParameteriv;
GLEW_FUN_EXPORT PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVEXTPROC G_glewGetNamedFramebufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERPARAMETERIEXTPROC G_glewNamedFramebufferParameteriEXT;

GLEW_FUN_EXPORT PFNGLBINDFRAMEBUFFERPROC G_glewBindFramebuffer;
GLEW_FUN_EXPORT PFNGLBINDRENDERBUFFERPROC G_glewBindRenderbuffer;
GLEW_FUN_EXPORT PFNGLBLITFRAMEBUFFERPROC G_glewBlitFramebuffer;
GLEW_FUN_EXPORT PFNGLCHECKFRAMEBUFFERSTATUSPROC G_glewCheckFramebufferStatus;
GLEW_FUN_EXPORT PFNGLDELETEFRAMEBUFFERSPROC G_glewDeleteFramebuffers;
GLEW_FUN_EXPORT PFNGLDELETERENDERBUFFERSPROC G_glewDeleteRenderbuffers;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERRENDERBUFFERPROC G_glewFramebufferRenderbuffer;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE1DPROC G_glewFramebufferTexture1D;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE2DPROC G_glewFramebufferTexture2D;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE3DPROC G_glewFramebufferTexture3D;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURELAYERPROC G_glewFramebufferTextureLayer;
GLEW_FUN_EXPORT PFNGLGENFRAMEBUFFERSPROC G_glewGenFramebuffers;
GLEW_FUN_EXPORT PFNGLGENRENDERBUFFERSPROC G_glewGenRenderbuffers;
GLEW_FUN_EXPORT PFNGLGENERATEMIPMAPPROC G_glewGenerateMipmap;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC G_glewGetFramebufferAttachmentParameteriv;
GLEW_FUN_EXPORT PFNGLGETRENDERBUFFERPARAMETERIVPROC G_glewGetRenderbufferParameteriv;
GLEW_FUN_EXPORT PFNGLISFRAMEBUFFERPROC G_glewIsFramebuffer;
GLEW_FUN_EXPORT PFNGLISRENDERBUFFERPROC G_glewIsRenderbuffer;
GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEPROC G_glewRenderbufferStorage;
GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC G_glewRenderbufferStorageMultisample;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREARBPROC G_glewFramebufferTextureARB;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREFACEARBPROC G_glewFramebufferTextureFaceARB;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURELAYERARBPROC G_glewFramebufferTextureLayerARB;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERIARBPROC G_glewProgramParameteriARB;

GLEW_FUN_EXPORT PFNGLGETPROGRAMBINARYPROC G_glewGetProgramBinary;
GLEW_FUN_EXPORT PFNGLPROGRAMBINARYPROC G_glewProgramBinary;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERIPROC G_glewProgramParameteri;

GLEW_FUN_EXPORT PFNGLGETUNIFORMDVPROC G_glewGetUniformdv;
GLEW_FUN_EXPORT PFNGLUNIFORM1DPROC G_glewUniform1d;
GLEW_FUN_EXPORT PFNGLUNIFORM1DVPROC G_glewUniform1dv;
GLEW_FUN_EXPORT PFNGLUNIFORM2DPROC G_glewUniform2d;
GLEW_FUN_EXPORT PFNGLUNIFORM2DVPROC G_glewUniform2dv;
GLEW_FUN_EXPORT PFNGLUNIFORM3DPROC G_glewUniform3d;
GLEW_FUN_EXPORT PFNGLUNIFORM3DVPROC G_glewUniform3dv;
GLEW_FUN_EXPORT PFNGLUNIFORM4DPROC G_glewUniform4d;
GLEW_FUN_EXPORT PFNGLUNIFORM4DVPROC G_glewUniform4dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2DVPROC G_glewUniformMatrix2dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X3DVPROC G_glewUniformMatrix2x3dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2X4DVPROC G_glewUniformMatrix2x4dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3DVPROC G_glewUniformMatrix3dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X2DVPROC G_glewUniformMatrix3x2dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3X4DVPROC G_glewUniformMatrix3x4dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4DVPROC G_glewUniformMatrix4dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X2DVPROC G_glewUniformMatrix4x2dv;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4X3DVPROC G_glewUniformMatrix4x3dv;

GLEW_FUN_EXPORT PFNGLCOLORSUBTABLEPROC G_glewColorSubTable;
GLEW_FUN_EXPORT PFNGLCOLORTABLEPROC G_glewColorTable;
GLEW_FUN_EXPORT PFNGLCOLORTABLEPARAMETERFVPROC G_glewColorTableParameterfv;
GLEW_FUN_EXPORT PFNGLCOLORTABLEPARAMETERIVPROC G_glewColorTableParameteriv;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONFILTER1DPROC G_glewConvolutionFilter1D;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONFILTER2DPROC G_glewConvolutionFilter2D;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERFPROC G_glewConvolutionParameterf;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERFVPROC G_glewConvolutionParameterfv;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERIPROC G_glewConvolutionParameteri;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERIVPROC G_glewConvolutionParameteriv;
GLEW_FUN_EXPORT PFNGLCOPYCOLORSUBTABLEPROC G_glewCopyColorSubTable;
GLEW_FUN_EXPORT PFNGLCOPYCOLORTABLEPROC G_glewCopyColorTable;
GLEW_FUN_EXPORT PFNGLCOPYCONVOLUTIONFILTER1DPROC G_glewCopyConvolutionFilter1D;
GLEW_FUN_EXPORT PFNGLCOPYCONVOLUTIONFILTER2DPROC G_glewCopyConvolutionFilter2D;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPROC G_glewGetColorTable;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERFVPROC G_glewGetColorTableParameterfv;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERIVPROC G_glewGetColorTableParameteriv;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONFILTERPROC G_glewGetConvolutionFilter;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONPARAMETERFVPROC G_glewGetConvolutionParameterfv;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONPARAMETERIVPROC G_glewGetConvolutionParameteriv;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPROC G_glewGetHistogram;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPARAMETERFVPROC G_glewGetHistogramParameterfv;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPARAMETERIVPROC G_glewGetHistogramParameteriv;
GLEW_FUN_EXPORT PFNGLGETMINMAXPROC G_glewGetMinmax;
GLEW_FUN_EXPORT PFNGLGETMINMAXPARAMETERFVPROC G_glewGetMinmaxParameterfv;
GLEW_FUN_EXPORT PFNGLGETMINMAXPARAMETERIVPROC G_glewGetMinmaxParameteriv;
GLEW_FUN_EXPORT PFNGLGETSEPARABLEFILTERPROC G_glewGetSeparableFilter;
GLEW_FUN_EXPORT PFNGLHISTOGRAMPROC G_glewHistogram;
GLEW_FUN_EXPORT PFNGLMINMAXPROC G_glewMinmax;
GLEW_FUN_EXPORT PFNGLRESETHISTOGRAMPROC G_glewResetHistogram;
GLEW_FUN_EXPORT PFNGLRESETMINMAXPROC G_glewResetMinmax;
GLEW_FUN_EXPORT PFNGLSEPARABLEFILTER2DPROC G_glewSeparableFilter2D;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDARBPROC G_glewDrawArraysInstancedARB;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDARBPROC G_glewDrawElementsInstancedARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBDIVISORARBPROC G_glewVertexAttribDivisorARB;

GLEW_FUN_EXPORT PFNGLGETINTERNALFORMATIVPROC G_glewGetInternalformativ;

GLEW_FUN_EXPORT PFNGLGETINTERNALFORMATI64VPROC G_glewGetInternalformati64v;

GLEW_FUN_EXPORT PFNGLINVALIDATEBUFFERDATAPROC G_glewInvalidateBufferData;
GLEW_FUN_EXPORT PFNGLINVALIDATEBUFFERSUBDATAPROC G_glewInvalidateBufferSubData;
GLEW_FUN_EXPORT PFNGLINVALIDATEFRAMEBUFFERPROC G_glewInvalidateFramebuffer;
GLEW_FUN_EXPORT PFNGLINVALIDATESUBFRAMEBUFFERPROC G_glewInvalidateSubFramebuffer;
GLEW_FUN_EXPORT PFNGLINVALIDATETEXIMAGEPROC G_glewInvalidateTexImage;
GLEW_FUN_EXPORT PFNGLINVALIDATETEXSUBIMAGEPROC G_glewInvalidateTexSubImage;

GLEW_FUN_EXPORT PFNGLFLUSHMAPPEDBUFFERRANGEPROC G_glewFlushMappedBufferRange;
GLEW_FUN_EXPORT PFNGLMAPBUFFERRANGEPROC G_glewMapBufferRange;

GLEW_FUN_EXPORT PFNGLCURRENTPALETTEMATRIXARBPROC G_glewCurrentPaletteMatrixARB;
GLEW_FUN_EXPORT PFNGLMATRIXINDEXPOINTERARBPROC G_glewMatrixIndexPointerARB;
GLEW_FUN_EXPORT PFNGLMATRIXINDEXUBVARBPROC G_glewMatrixIndexubvARB;
GLEW_FUN_EXPORT PFNGLMATRIXINDEXUIVARBPROC G_glewMatrixIndexuivARB;
GLEW_FUN_EXPORT PFNGLMATRIXINDEXUSVARBPROC G_glewMatrixIndexusvARB;

GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSINDIRECTPROC G_glewMultiDrawArraysIndirect;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSINDIRECTPROC G_glewMultiDrawElementsIndirect;

GLEW_FUN_EXPORT PFNGLSAMPLECOVERAGEARBPROC G_glewSampleCoverageARB;

GLEW_FUN_EXPORT PFNGLACTIVETEXTUREARBPROC G_glewActiveTextureARB;
GLEW_FUN_EXPORT PFNGLCLIENTACTIVETEXTUREARBPROC G_glewClientActiveTextureARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1DARBPROC G_glewMultiTexCoord1dARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1DVARBPROC G_glewMultiTexCoord1dvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1FARBPROC G_glewMultiTexCoord1fARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1FVARBPROC G_glewMultiTexCoord1fvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1IARBPROC G_glewMultiTexCoord1iARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1IVARBPROC G_glewMultiTexCoord1ivARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1SARBPROC G_glewMultiTexCoord1sARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1SVARBPROC G_glewMultiTexCoord1svARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2DARBPROC G_glewMultiTexCoord2dARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2DVARBPROC G_glewMultiTexCoord2dvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2FARBPROC G_glewMultiTexCoord2fARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2FVARBPROC G_glewMultiTexCoord2fvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2IARBPROC G_glewMultiTexCoord2iARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2IVARBPROC G_glewMultiTexCoord2ivARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2SARBPROC G_glewMultiTexCoord2sARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2SVARBPROC G_glewMultiTexCoord2svARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3DARBPROC G_glewMultiTexCoord3dARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3DVARBPROC G_glewMultiTexCoord3dvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3FARBPROC G_glewMultiTexCoord3fARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3FVARBPROC G_glewMultiTexCoord3fvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3IARBPROC G_glewMultiTexCoord3iARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3IVARBPROC G_glewMultiTexCoord3ivARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3SARBPROC G_glewMultiTexCoord3sARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3SVARBPROC G_glewMultiTexCoord3svARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4DARBPROC G_glewMultiTexCoord4dARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4DVARBPROC G_glewMultiTexCoord4dvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4FARBPROC G_glewMultiTexCoord4fARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4FVARBPROC G_glewMultiTexCoord4fvARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4IARBPROC G_glewMultiTexCoord4iARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4IVARBPROC G_glewMultiTexCoord4ivARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4SARBPROC G_glewMultiTexCoord4sARB;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4SVARBPROC G_glewMultiTexCoord4svARB;

GLEW_FUN_EXPORT PFNGLBEGINQUERYARBPROC G_glewBeginQueryARB;
GLEW_FUN_EXPORT PFNGLDELETEQUERIESARBPROC G_glewDeleteQueriesARB;
GLEW_FUN_EXPORT PFNGLENDQUERYARBPROC G_glewEndQueryARB;
GLEW_FUN_EXPORT PFNGLGENQUERIESARBPROC G_glewGenQueriesARB;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTIVARBPROC G_glewGetQueryObjectivARB;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUIVARBPROC G_glewGetQueryObjectuivARB;
GLEW_FUN_EXPORT PFNGLGETQUERYIVARBPROC G_glewGetQueryivARB;
GLEW_FUN_EXPORT PFNGLISQUERYARBPROC G_glewIsQueryARB;

GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFARBPROC G_glewPointParameterfARB;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFVARBPROC G_glewPointParameterfvARB;

GLEW_FUN_EXPORT PFNGLGETPROGRAMINTERFACEIVPROC G_glewGetProgramInterfaceiv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCEINDEXPROC G_glewGetProgramResourceIndex;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCELOCATIONPROC G_glewGetProgramResourceLocation;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC G_glewGetProgramResourceLocationIndex;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCENAMEPROC G_glewGetProgramResourceName;
GLEW_FUN_EXPORT PFNGLGETPROGRAMRESOURCEIVPROC G_glewGetProgramResourceiv;

GLEW_FUN_EXPORT PFNGLPROVOKINGVERTEXPROC G_glewProvokingVertex;

GLEW_FUN_EXPORT PFNGLGETGRAPHICSRESETSTATUSARBPROC G_glewGetGraphicsResetStatusARB;
GLEW_FUN_EXPORT PFNGLGETNCOLORTABLEARBPROC G_glewGetnColorTableARB;
GLEW_FUN_EXPORT PFNGLGETNCOMPRESSEDTEXIMAGEARBPROC G_glewGetnCompressedTexImageARB;
GLEW_FUN_EXPORT PFNGLGETNCONVOLUTIONFILTERARBPROC G_glewGetnConvolutionFilterARB;
GLEW_FUN_EXPORT PFNGLGETNHISTOGRAMARBPROC G_glewGetnHistogramARB;
GLEW_FUN_EXPORT PFNGLGETNMAPDVARBPROC G_glewGetnMapdvARB;
GLEW_FUN_EXPORT PFNGLGETNMAPFVARBPROC G_glewGetnMapfvARB;
GLEW_FUN_EXPORT PFNGLGETNMAPIVARBPROC G_glewGetnMapivARB;
GLEW_FUN_EXPORT PFNGLGETNMINMAXARBPROC G_glewGetnMinmaxARB;
GLEW_FUN_EXPORT PFNGLGETNPIXELMAPFVARBPROC G_glewGetnPixelMapfvARB;
GLEW_FUN_EXPORT PFNGLGETNPIXELMAPUIVARBPROC G_glewGetnPixelMapuivARB;
GLEW_FUN_EXPORT PFNGLGETNPIXELMAPUSVARBPROC G_glewGetnPixelMapusvARB;
GLEW_FUN_EXPORT PFNGLGETNPOLYGONSTIPPLEARBPROC G_glewGetnPolygonStippleARB;
GLEW_FUN_EXPORT PFNGLGETNSEPARABLEFILTERARBPROC G_glewGetnSeparableFilterARB;
GLEW_FUN_EXPORT PFNGLGETNTEXIMAGEARBPROC G_glewGetnTexImageARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMDVARBPROC G_glewGetnUniformdvARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMFVARBPROC G_glewGetnUniformfvARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMIVARBPROC G_glewGetnUniformivARB;
GLEW_FUN_EXPORT PFNGLGETNUNIFORMUIVARBPROC G_glewGetnUniformuivARB;
GLEW_FUN_EXPORT PFNGLREADNPIXELSARBPROC G_glewReadnPixelsARB;

GLEW_FUN_EXPORT PFNGLMINSAMPLESHADINGARBPROC G_glewMinSampleShadingARB;

GLEW_FUN_EXPORT PFNGLBINDSAMPLERPROC G_glewBindSampler;
GLEW_FUN_EXPORT PFNGLDELETESAMPLERSPROC G_glewDeleteSamplers;
GLEW_FUN_EXPORT PFNGLGENSAMPLERSPROC G_glewGenSamplers;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERIIVPROC G_glewGetSamplerParameterIiv;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERIUIVPROC G_glewGetSamplerParameterIuiv;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERFVPROC G_glewGetSamplerParameterfv;
GLEW_FUN_EXPORT PFNGLGETSAMPLERPARAMETERIVPROC G_glewGetSamplerParameteriv;
GLEW_FUN_EXPORT PFNGLISSAMPLERPROC G_glewIsSampler;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIIVPROC G_glewSamplerParameterIiv;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIUIVPROC G_glewSamplerParameterIuiv;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERFPROC G_glewSamplerParameterf;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERFVPROC G_glewSamplerParameterfv;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIPROC G_glewSamplerParameteri;
GLEW_FUN_EXPORT PFNGLSAMPLERPARAMETERIVPROC G_glewSamplerParameteriv;

GLEW_FUN_EXPORT PFNGLACTIVESHADERPROGRAMPROC G_glewActiveShaderProgram;
GLEW_FUN_EXPORT PFNGLBINDPROGRAMPIPELINEPROC G_glewBindProgramPipeline;
GLEW_FUN_EXPORT PFNGLCREATESHADERPROGRAMVPROC G_glewCreateShaderProgramv;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMPIPELINESPROC G_glewDeleteProgramPipelines;
GLEW_FUN_EXPORT PFNGLGENPROGRAMPIPELINESPROC G_glewGenProgramPipelines;
GLEW_FUN_EXPORT PFNGLGETPROGRAMPIPELINEINFOLOGPROC G_glewGetProgramPipelineInfoLog;
GLEW_FUN_EXPORT PFNGLGETPROGRAMPIPELINEIVPROC G_glewGetProgramPipelineiv;
GLEW_FUN_EXPORT PFNGLISPROGRAMPIPELINEPROC G_glewIsProgramPipeline;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1DPROC G_glewProgramUniform1d;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1DVPROC G_glewProgramUniform1dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FPROC G_glewProgramUniform1f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FVPROC G_glewProgramUniform1fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IPROC G_glewProgramUniform1i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IVPROC G_glewProgramUniform1iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIPROC G_glewProgramUniform1ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIVPROC G_glewProgramUniform1uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2DPROC G_glewProgramUniform2d;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2DVPROC G_glewProgramUniform2dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FPROC G_glewProgramUniform2f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FVPROC G_glewProgramUniform2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IPROC G_glewProgramUniform2i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IVPROC G_glewProgramUniform2iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIPROC G_glewProgramUniform2ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIVPROC G_glewProgramUniform2uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3DPROC G_glewProgramUniform3d;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3DVPROC G_glewProgramUniform3dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FPROC G_glewProgramUniform3f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FVPROC G_glewProgramUniform3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IPROC G_glewProgramUniform3i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IVPROC G_glewProgramUniform3iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIPROC G_glewProgramUniform3ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIVPROC G_glewProgramUniform3uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4DPROC G_glewProgramUniform4d;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4DVPROC G_glewProgramUniform4dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FPROC G_glewProgramUniform4f;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FVPROC G_glewProgramUniform4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IPROC G_glewProgramUniform4i;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IVPROC G_glewProgramUniform4iv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIPROC G_glewProgramUniform4ui;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIVPROC G_glewProgramUniform4uiv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2DVPROC G_glewProgramUniformMatrix2dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2FVPROC G_glewProgramUniformMatrix2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC G_glewProgramUniformMatrix2x3dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC G_glewProgramUniformMatrix2x3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC G_glewProgramUniformMatrix2x4dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC G_glewProgramUniformMatrix2x4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3DVPROC G_glewProgramUniformMatrix3dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3FVPROC G_glewProgramUniformMatrix3fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC G_glewProgramUniformMatrix3x2dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC G_glewProgramUniformMatrix3x2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC G_glewProgramUniformMatrix3x4dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC G_glewProgramUniformMatrix3x4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4DVPROC G_glewProgramUniformMatrix4dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4FVPROC G_glewProgramUniformMatrix4fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC G_glewProgramUniformMatrix4x2dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC G_glewProgramUniformMatrix4x2fv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC G_glewProgramUniformMatrix4x3dv;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC G_glewProgramUniformMatrix4x3fv;
GLEW_FUN_EXPORT PFNGLUSEPROGRAMSTAGESPROC G_glewUseProgramStages;
GLEW_FUN_EXPORT PFNGLVALIDATEPROGRAMPIPELINEPROC G_glewValidateProgramPipeline;

GLEW_FUN_EXPORT PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC G_glewGetActiveAtomicCounterBufferiv;

GLEW_FUN_EXPORT PFNGLBINDIMAGETEXTUREPROC G_glewBindImageTexture;
GLEW_FUN_EXPORT PFNGLMEMORYBARRIERPROC G_glewMemoryBarrier;

GLEW_FUN_EXPORT PFNGLATTACHOBJECTARBPROC G_glewAttachObjectARB;
GLEW_FUN_EXPORT PFNGLCOMPILESHADERARBPROC G_glewCompileShaderARB;
GLEW_FUN_EXPORT PFNGLCREATEPROGRAMOBJECTARBPROC G_glewCreateProgramObjectARB;
GLEW_FUN_EXPORT PFNGLCREATESHADEROBJECTARBPROC G_glewCreateShaderObjectARB;
GLEW_FUN_EXPORT PFNGLDELETEOBJECTARBPROC G_glewDeleteObjectARB;
GLEW_FUN_EXPORT PFNGLDETACHOBJECTARBPROC G_glewDetachObjectARB;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMARBPROC G_glewGetActiveUniformARB;
GLEW_FUN_EXPORT PFNGLGETATTACHEDOBJECTSARBPROC G_glewGetAttachedObjectsARB;
GLEW_FUN_EXPORT PFNGLGETHANDLEARBPROC G_glewGetHandleARB;
GLEW_FUN_EXPORT PFNGLGETINFOLOGARBPROC G_glewGetInfoLogARB;
GLEW_FUN_EXPORT PFNGLGETOBJECTPARAMETERFVARBPROC G_glewGetObjectParameterfvARB;
GLEW_FUN_EXPORT PFNGLGETOBJECTPARAMETERIVARBPROC G_glewGetObjectParameterivARB;
GLEW_FUN_EXPORT PFNGLGETSHADERSOURCEARBPROC G_glewGetShaderSourceARB;
GLEW_FUN_EXPORT PFNGLGETUNIFORMLOCATIONARBPROC G_glewGetUniformLocationARB;
GLEW_FUN_EXPORT PFNGLGETUNIFORMFVARBPROC G_glewGetUniformfvARB;
GLEW_FUN_EXPORT PFNGLGETUNIFORMIVARBPROC G_glewGetUniformivARB;
GLEW_FUN_EXPORT PFNGLLINKPROGRAMARBPROC G_glewLinkProgramARB;
GLEW_FUN_EXPORT PFNGLSHADERSOURCEARBPROC G_glewShaderSourceARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1FARBPROC G_glewUniform1fARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1FVARBPROC G_glewUniform1fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1IARBPROC G_glewUniform1iARB;
GLEW_FUN_EXPORT PFNGLUNIFORM1IVARBPROC G_glewUniform1ivARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2FARBPROC G_glewUniform2fARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2FVARBPROC G_glewUniform2fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2IARBPROC G_glewUniform2iARB;
GLEW_FUN_EXPORT PFNGLUNIFORM2IVARBPROC G_glewUniform2ivARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3FARBPROC G_glewUniform3fARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3FVARBPROC G_glewUniform3fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3IARBPROC G_glewUniform3iARB;
GLEW_FUN_EXPORT PFNGLUNIFORM3IVARBPROC G_glewUniform3ivARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4FARBPROC G_glewUniform4fARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4FVARBPROC G_glewUniform4fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4IARBPROC G_glewUniform4iARB;
GLEW_FUN_EXPORT PFNGLUNIFORM4IVARBPROC G_glewUniform4ivARB;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX2FVARBPROC G_glewUniformMatrix2fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX3FVARBPROC G_glewUniformMatrix3fvARB;
GLEW_FUN_EXPORT PFNGLUNIFORMMATRIX4FVARBPROC G_glewUniformMatrix4fvARB;
GLEW_FUN_EXPORT PFNGLUSEPROGRAMOBJECTARBPROC G_glewUseProgramObjectARB;
GLEW_FUN_EXPORT PFNGLVALIDATEPROGRAMARBPROC G_glewValidateProgramARB;

GLEW_FUN_EXPORT PFNGLSHADERSTORAGEBLOCKBINDINGPROC G_glewShaderStorageBlockBinding;

GLEW_FUN_EXPORT PFNGLGETACTIVESUBROUTINENAMEPROC G_glewGetActiveSubroutineName;
GLEW_FUN_EXPORT PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC G_glewGetActiveSubroutineUniformName;
GLEW_FUN_EXPORT PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC G_glewGetActiveSubroutineUniformiv;
GLEW_FUN_EXPORT PFNGLGETPROGRAMSTAGEIVPROC G_glewGetProgramStageiv;
GLEW_FUN_EXPORT PFNGLGETSUBROUTINEINDEXPROC G_glewGetSubroutineIndex;
GLEW_FUN_EXPORT PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC G_glewGetSubroutineUniformLocation;
GLEW_FUN_EXPORT PFNGLGETUNIFORMSUBROUTINEUIVPROC G_glewGetUniformSubroutineuiv;
GLEW_FUN_EXPORT PFNGLUNIFORMSUBROUTINESUIVPROC G_glewUniformSubroutinesuiv;

GLEW_FUN_EXPORT PFNGLCOMPILESHADERINCLUDEARBPROC G_glewCompileShaderIncludeARB;
GLEW_FUN_EXPORT PFNGLDELETENAMEDSTRINGARBPROC G_glewDeleteNamedStringARB;
GLEW_FUN_EXPORT PFNGLGETNAMEDSTRINGARBPROC G_glewGetNamedStringARB;
GLEW_FUN_EXPORT PFNGLGETNAMEDSTRINGIVARBPROC G_glewGetNamedStringivARB;
GLEW_FUN_EXPORT PFNGLISNAMEDSTRINGARBPROC G_glewIsNamedStringARB;
GLEW_FUN_EXPORT PFNGLNAMEDSTRINGARBPROC G_glewNamedStringARB;

GLEW_FUN_EXPORT PFNGLCLIENTWAITSYNCPROC G_glewClientWaitSync;
GLEW_FUN_EXPORT PFNGLDELETESYNCPROC G_glewDeleteSync;
GLEW_FUN_EXPORT PFNGLFENCESYNCPROC G_glewFenceSync;
GLEW_FUN_EXPORT PFNGLGETINTEGER64VPROC G_glewGetInteger64v;
GLEW_FUN_EXPORT PFNGLGETSYNCIVPROC G_glewGetSynciv;
GLEW_FUN_EXPORT PFNGLISSYNCPROC G_glewIsSync;
GLEW_FUN_EXPORT PFNGLWAITSYNCPROC G_glewWaitSync;

GLEW_FUN_EXPORT PFNGLPATCHPARAMETERFVPROC G_glewPatchParameterfv;
GLEW_FUN_EXPORT PFNGLPATCHPARAMETERIPROC G_glewPatchParameteri;

GLEW_FUN_EXPORT PFNGLTEXBUFFERARBPROC G_glewTexBufferARB;

GLEW_FUN_EXPORT PFNGLTEXBUFFERRANGEPROC G_glewTexBufferRange;
GLEW_FUN_EXPORT PFNGLTEXTUREBUFFERRANGEEXTPROC G_glewTextureBufferRangeEXT;

GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE1DARBPROC G_glewCompressedTexImage1DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE2DARBPROC G_glewCompressedTexImage2DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXIMAGE3DARBPROC G_glewCompressedTexImage3DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE1DARBPROC G_glewCompressedTexSubImage1DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE2DARBPROC G_glewCompressedTexSubImage2DARB;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXSUBIMAGE3DARBPROC G_glewCompressedTexSubImage3DARB;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDTEXIMAGEARBPROC G_glewGetCompressedTexImageARB;

GLEW_FUN_EXPORT PFNGLGETMULTISAMPLEFVPROC G_glewGetMultisamplefv;
GLEW_FUN_EXPORT PFNGLSAMPLEMASKIPROC G_glewSampleMaski;
GLEW_FUN_EXPORT PFNGLTEXIMAGE2DMULTISAMPLEPROC G_glewTexImage2DMultisample;
GLEW_FUN_EXPORT PFNGLTEXIMAGE3DMULTISAMPLEPROC G_glewTexImage3DMultisample;

GLEW_FUN_EXPORT PFNGLTEXSTORAGE1DPROC G_glewTexStorage1D;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE2DPROC G_glewTexStorage2D;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE3DPROC G_glewTexStorage3D;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE1DEXTPROC G_glewTextureStorage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE2DEXTPROC G_glewTextureStorage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE3DEXTPROC G_glewTextureStorage3DEXT;

GLEW_FUN_EXPORT PFNGLTEXSTORAGE2DMULTISAMPLEPROC G_glewTexStorage2DMultisample;
GLEW_FUN_EXPORT PFNGLTEXSTORAGE3DMULTISAMPLEPROC G_glewTexStorage3DMultisample;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE2DMULTISAMPLEEXTPROC G_glewTextureStorage2DMultisampleEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESTORAGE3DMULTISAMPLEEXTPROC G_glewTextureStorage3DMultisampleEXT;

GLEW_FUN_EXPORT PFNGLTEXTUREVIEWPROC G_glewTextureView;

GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTI64VPROC G_glewGetQueryObjecti64v;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUI64VPROC G_glewGetQueryObjectui64v;
GLEW_FUN_EXPORT PFNGLQUERYCOUNTERPROC G_glewQueryCounter;

GLEW_FUN_EXPORT PFNGLBINDTRANSFORMFEEDBACKPROC G_glewBindTransformFeedback;
GLEW_FUN_EXPORT PFNGLDELETETRANSFORMFEEDBACKSPROC G_glewDeleteTransformFeedbacks;
GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKPROC G_glewDrawTransformFeedback;
GLEW_FUN_EXPORT PFNGLGENTRANSFORMFEEDBACKSPROC G_glewGenTransformFeedbacks;
GLEW_FUN_EXPORT PFNGLISTRANSFORMFEEDBACKPROC G_glewIsTransformFeedback;
GLEW_FUN_EXPORT PFNGLPAUSETRANSFORMFEEDBACKPROC G_glewPauseTransformFeedback;
GLEW_FUN_EXPORT PFNGLRESUMETRANSFORMFEEDBACKPROC G_glewResumeTransformFeedback;

GLEW_FUN_EXPORT PFNGLBEGINQUERYINDEXEDPROC G_glewBeginQueryIndexed;
GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC G_glewDrawTransformFeedbackStream;
GLEW_FUN_EXPORT PFNGLENDQUERYINDEXEDPROC G_glewEndQueryIndexed;
GLEW_FUN_EXPORT PFNGLGETQUERYINDEXEDIVPROC G_glewGetQueryIndexediv;

GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC G_glewDrawTransformFeedbackInstanced;
GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC G_glewDrawTransformFeedbackStreamInstanced;

GLEW_FUN_EXPORT PFNGLLOADTRANSPOSEMATRIXDARBPROC G_glewLoadTransposeMatrixdARB;
GLEW_FUN_EXPORT PFNGLLOADTRANSPOSEMATRIXFARBPROC G_glewLoadTransposeMatrixfARB;
GLEW_FUN_EXPORT PFNGLMULTTRANSPOSEMATRIXDARBPROC G_glewMultTransposeMatrixdARB;
GLEW_FUN_EXPORT PFNGLMULTTRANSPOSEMATRIXFARBPROC G_glewMultTransposeMatrixfARB;

GLEW_FUN_EXPORT PFNGLBINDBUFFERBASEPROC G_glewBindBufferBase;
GLEW_FUN_EXPORT PFNGLBINDBUFFERRANGEPROC G_glewBindBufferRange;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC G_glewGetActiveUniformBlockName;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMBLOCKIVPROC G_glewGetActiveUniformBlockiv;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMNAMEPROC G_glewGetActiveUniformName;
GLEW_FUN_EXPORT PFNGLGETACTIVEUNIFORMSIVPROC G_glewGetActiveUniformsiv;
GLEW_FUN_EXPORT PFNGLGETINTEGERI_VPROC G_glewGetIntegeri_v;
GLEW_FUN_EXPORT PFNGLGETUNIFORMBLOCKINDEXPROC G_glewGetUniformBlockIndex;
GLEW_FUN_EXPORT PFNGLGETUNIFORMINDICESPROC G_glewGetUniformIndices;
GLEW_FUN_EXPORT PFNGLUNIFORMBLOCKBINDINGPROC G_glewUniformBlockBinding;

GLEW_FUN_EXPORT PFNGLBINDVERTEXARRAYPROC G_glewBindVertexArray;
GLEW_FUN_EXPORT PFNGLDELETEVERTEXARRAYSPROC G_glewDeleteVertexArrays;
GLEW_FUN_EXPORT PFNGLGENVERTEXARRAYSPROC G_glewGenVertexArrays;
GLEW_FUN_EXPORT PFNGLISVERTEXARRAYPROC G_glewIsVertexArray;

GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBLDVPROC G_glewGetVertexAttribLdv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1DPROC G_glewVertexAttribL1d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1DVPROC G_glewVertexAttribL1dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2DPROC G_glewVertexAttribL2d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2DVPROC G_glewVertexAttribL2dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3DPROC G_glewVertexAttribL3d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3DVPROC G_glewVertexAttribL3dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4DPROC G_glewVertexAttribL4d;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4DVPROC G_glewVertexAttribL4dv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBLPOINTERPROC G_glewVertexAttribLPointer;

GLEW_FUN_EXPORT PFNGLBINDVERTEXBUFFERPROC G_glewBindVertexBuffer;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBBINDINGPROC G_glewVertexAttribBinding;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBFORMATPROC G_glewVertexAttribFormat;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIFORMATPROC G_glewVertexAttribIFormat;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBLFORMATPROC G_glewVertexAttribLFormat;
GLEW_FUN_EXPORT PFNGLVERTEXBINDINGDIVISORPROC G_glewVertexBindingDivisor;

GLEW_FUN_EXPORT PFNGLVERTEXBLENDARBPROC G_glewVertexBlendARB;
GLEW_FUN_EXPORT PFNGLWEIGHTPOINTERARBPROC G_glewWeightPointerARB;
GLEW_FUN_EXPORT PFNGLWEIGHTBVARBPROC G_glewWeightbvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTDVARBPROC G_glewWeightdvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTFVARBPROC G_glewWeightfvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTIVARBPROC G_glewWeightivARB;
GLEW_FUN_EXPORT PFNGLWEIGHTSVARBPROC G_glewWeightsvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTUBVARBPROC G_glewWeightubvARB;
GLEW_FUN_EXPORT PFNGLWEIGHTUIVARBPROC G_glewWeightuivARB;
GLEW_FUN_EXPORT PFNGLWEIGHTUSVARBPROC G_glewWeightusvARB;

GLEW_FUN_EXPORT PFNGLBINDBUFFERARBPROC G_glewBindBufferARB;
GLEW_FUN_EXPORT PFNGLBUFFERDATAARBPROC G_glewBufferDataARB;
GLEW_FUN_EXPORT PFNGLBUFFERSUBDATAARBPROC G_glewBufferSubDataARB;
GLEW_FUN_EXPORT PFNGLDELETEBUFFERSARBPROC G_glewDeleteBuffersARB;
GLEW_FUN_EXPORT PFNGLGENBUFFERSARBPROC G_glewGenBuffersARB;
GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERIVARBPROC G_glewGetBufferParameterivARB;
GLEW_FUN_EXPORT PFNGLGETBUFFERPOINTERVARBPROC G_glewGetBufferPointervARB;
GLEW_FUN_EXPORT PFNGLGETBUFFERSUBDATAARBPROC G_glewGetBufferSubDataARB;
GLEW_FUN_EXPORT PFNGLISBUFFERARBPROC G_glewIsBufferARB;
GLEW_FUN_EXPORT PFNGLMAPBUFFERARBPROC G_glewMapBufferARB;
GLEW_FUN_EXPORT PFNGLUNMAPBUFFERARBPROC G_glewUnmapBufferARB;

GLEW_FUN_EXPORT PFNGLBINDPROGRAMARBPROC G_glewBindProgramARB;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMSARBPROC G_glewDeleteProgramsARB;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXATTRIBARRAYARBPROC G_glewDisableVertexAttribArrayARB;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXATTRIBARRAYARBPROC G_glewEnableVertexAttribArrayARB;
GLEW_FUN_EXPORT PFNGLGENPROGRAMSARBPROC G_glewGenProgramsARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMENVPARAMETERDVARBPROC G_glewGetProgramEnvParameterdvARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMENVPARAMETERFVARBPROC G_glewGetProgramEnvParameterfvARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMLOCALPARAMETERDVARBPROC G_glewGetProgramLocalParameterdvARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMLOCALPARAMETERFVARBPROC G_glewGetProgramLocalParameterfvARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMSTRINGARBPROC G_glewGetProgramStringARB;
GLEW_FUN_EXPORT PFNGLGETPROGRAMIVARBPROC G_glewGetProgramivARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBPOINTERVARBPROC G_glewGetVertexAttribPointervARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBDVARBPROC G_glewGetVertexAttribdvARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBFVARBPROC G_glewGetVertexAttribfvARB;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIVARBPROC G_glewGetVertexAttribivARB;
GLEW_FUN_EXPORT PFNGLISPROGRAMARBPROC G_glewIsProgramARB;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETER4DARBPROC G_glewProgramEnvParameter4dARB;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETER4DVARBPROC G_glewProgramEnvParameter4dvARB;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETER4FARBPROC G_glewProgramEnvParameter4fARB;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETER4FVARBPROC G_glewProgramEnvParameter4fvARB;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETER4DARBPROC G_glewProgramLocalParameter4dARB;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETER4DVARBPROC G_glewProgramLocalParameter4dvARB;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETER4FARBPROC G_glewProgramLocalParameter4fARB;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETER4FVARBPROC G_glewProgramLocalParameter4fvARB;
GLEW_FUN_EXPORT PFNGLPROGRAMSTRINGARBPROC G_glewProgramStringARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DARBPROC G_glewVertexAttrib1dARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DVARBPROC G_glewVertexAttrib1dvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FARBPROC G_glewVertexAttrib1fARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FVARBPROC G_glewVertexAttrib1fvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SARBPROC G_glewVertexAttrib1sARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SVARBPROC G_glewVertexAttrib1svARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DARBPROC G_glewVertexAttrib2dARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DVARBPROC G_glewVertexAttrib2dvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FARBPROC G_glewVertexAttrib2fARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FVARBPROC G_glewVertexAttrib2fvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SARBPROC G_glewVertexAttrib2sARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SVARBPROC G_glewVertexAttrib2svARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DARBPROC G_glewVertexAttrib3dARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DVARBPROC G_glewVertexAttrib3dvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FARBPROC G_glewVertexAttrib3fARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FVARBPROC G_glewVertexAttrib3fvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SARBPROC G_glewVertexAttrib3sARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SVARBPROC G_glewVertexAttrib3svARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NBVARBPROC G_glewVertexAttrib4NbvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NIVARBPROC G_glewVertexAttrib4NivARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NSVARBPROC G_glewVertexAttrib4NsvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUBARBPROC G_glewVertexAttrib4NubARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUBVARBPROC G_glewVertexAttrib4NubvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUIVARBPROC G_glewVertexAttrib4NuivARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4NUSVARBPROC G_glewVertexAttrib4NusvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4BVARBPROC G_glewVertexAttrib4bvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DARBPROC G_glewVertexAttrib4dARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DVARBPROC G_glewVertexAttrib4dvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FARBPROC G_glewVertexAttrib4fARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FVARBPROC G_glewVertexAttrib4fvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4IVARBPROC G_glewVertexAttrib4ivARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SARBPROC G_glewVertexAttrib4sARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SVARBPROC G_glewVertexAttrib4svARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UBVARBPROC G_glewVertexAttrib4ubvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UIVARBPROC G_glewVertexAttrib4uivARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4USVARBPROC G_glewVertexAttrib4usvARB;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBPOINTERARBPROC G_glewVertexAttribPointerARB;

GLEW_FUN_EXPORT PFNGLBINDATTRIBLOCATIONARBPROC G_glewBindAttribLocationARB;
GLEW_FUN_EXPORT PFNGLGETACTIVEATTRIBARBPROC G_glewGetActiveAttribARB;
GLEW_FUN_EXPORT PFNGLGETATTRIBLOCATIONARBPROC G_glewGetAttribLocationARB;

GLEW_FUN_EXPORT PFNGLCOLORP3UIPROC G_glewColorP3ui;
GLEW_FUN_EXPORT PFNGLCOLORP3UIVPROC G_glewColorP3uiv;
GLEW_FUN_EXPORT PFNGLCOLORP4UIPROC G_glewColorP4ui;
GLEW_FUN_EXPORT PFNGLCOLORP4UIVPROC G_glewColorP4uiv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP1UIPROC G_glewMultiTexCoordP1ui;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP1UIVPROC G_glewMultiTexCoordP1uiv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP2UIPROC G_glewMultiTexCoordP2ui;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP2UIVPROC G_glewMultiTexCoordP2uiv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP3UIPROC G_glewMultiTexCoordP3ui;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP3UIVPROC G_glewMultiTexCoordP3uiv;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP4UIPROC G_glewMultiTexCoordP4ui;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDP4UIVPROC G_glewMultiTexCoordP4uiv;
GLEW_FUN_EXPORT PFNGLNORMALP3UIPROC G_glewNormalP3ui;
GLEW_FUN_EXPORT PFNGLNORMALP3UIVPROC G_glewNormalP3uiv;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORP3UIPROC G_glewSecondaryColorP3ui;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORP3UIVPROC G_glewSecondaryColorP3uiv;
GLEW_FUN_EXPORT PFNGLTEXCOORDP1UIPROC G_glewTexCoordP1ui;
GLEW_FUN_EXPORT PFNGLTEXCOORDP1UIVPROC G_glewTexCoordP1uiv;
GLEW_FUN_EXPORT PFNGLTEXCOORDP2UIPROC G_glewTexCoordP2ui;
GLEW_FUN_EXPORT PFNGLTEXCOORDP2UIVPROC G_glewTexCoordP2uiv;
GLEW_FUN_EXPORT PFNGLTEXCOORDP3UIPROC G_glewTexCoordP3ui;
GLEW_FUN_EXPORT PFNGLTEXCOORDP3UIVPROC G_glewTexCoordP3uiv;
GLEW_FUN_EXPORT PFNGLTEXCOORDP4UIPROC G_glewTexCoordP4ui;
GLEW_FUN_EXPORT PFNGLTEXCOORDP4UIVPROC G_glewTexCoordP4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP1UIPROC G_glewVertexAttribP1ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP1UIVPROC G_glewVertexAttribP1uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP2UIPROC G_glewVertexAttribP2ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP2UIVPROC G_glewVertexAttribP2uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP3UIPROC G_glewVertexAttribP3ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP3UIVPROC G_glewVertexAttribP3uiv;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP4UIPROC G_glewVertexAttribP4ui;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBP4UIVPROC G_glewVertexAttribP4uiv;
GLEW_FUN_EXPORT PFNGLVERTEXP2UIPROC G_glewVertexP2ui;
GLEW_FUN_EXPORT PFNGLVERTEXP2UIVPROC G_glewVertexP2uiv;
GLEW_FUN_EXPORT PFNGLVERTEXP3UIPROC G_glewVertexP3ui;
GLEW_FUN_EXPORT PFNGLVERTEXP3UIVPROC G_glewVertexP3uiv;
GLEW_FUN_EXPORT PFNGLVERTEXP4UIPROC G_glewVertexP4ui;
GLEW_FUN_EXPORT PFNGLVERTEXP4UIVPROC G_glewVertexP4uiv;

GLEW_FUN_EXPORT PFNGLDEPTHRANGEARRAYVPROC G_glewDepthRangeArrayv;
GLEW_FUN_EXPORT PFNGLDEPTHRANGEINDEXEDPROC G_glewDepthRangeIndexed;
GLEW_FUN_EXPORT PFNGLGETDOUBLEI_VPROC G_glewGetDoublei_v;
GLEW_FUN_EXPORT PFNGLGETFLOATI_VPROC G_glewGetFloati_v;
GLEW_FUN_EXPORT PFNGLSCISSORARRAYVPROC G_glewScissorArrayv;
GLEW_FUN_EXPORT PFNGLSCISSORINDEXEDPROC G_glewScissorIndexed;
GLEW_FUN_EXPORT PFNGLSCISSORINDEXEDVPROC G_glewScissorIndexedv;
GLEW_FUN_EXPORT PFNGLVIEWPORTARRAYVPROC G_glewViewportArrayv;
GLEW_FUN_EXPORT PFNGLVIEWPORTINDEXEDFPROC G_glewViewportIndexedf;
GLEW_FUN_EXPORT PFNGLVIEWPORTINDEXEDFVPROC G_glewViewportIndexedfv;

GLEW_FUN_EXPORT PFNGLWINDOWPOS2DARBPROC G_glewWindowPos2dARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2DVARBPROC G_glewWindowPos2dvARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FARBPROC G_glewWindowPos2fARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FVARBPROC G_glewWindowPos2fvARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IARBPROC G_glewWindowPos2iARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IVARBPROC G_glewWindowPos2ivARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SARBPROC G_glewWindowPos2sARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SVARBPROC G_glewWindowPos2svARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DARBPROC G_glewWindowPos3dARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DVARBPROC G_glewWindowPos3dvARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FARBPROC G_glewWindowPos3fARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FVARBPROC G_glewWindowPos3fvARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IARBPROC G_glewWindowPos3iARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IVARBPROC G_glewWindowPos3ivARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SARBPROC G_glewWindowPos3sARB;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SVARBPROC G_glewWindowPos3svARB;

GLEW_FUN_EXPORT PFNGLDRAWBUFFERSATIPROC G_glewDrawBuffersATI;

GLEW_FUN_EXPORT PFNGLDRAWELEMENTARRAYATIPROC G_glewDrawElementArrayATI;
GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTARRAYATIPROC G_glewDrawRangeElementArrayATI;
GLEW_FUN_EXPORT PFNGLELEMENTPOINTERATIPROC G_glewElementPointerATI;

GLEW_FUN_EXPORT PFNGLGETTEXBUMPPARAMETERFVATIPROC G_glewGetTexBumpParameterfvATI;
GLEW_FUN_EXPORT PFNGLGETTEXBUMPPARAMETERIVATIPROC G_glewGetTexBumpParameterivATI;
GLEW_FUN_EXPORT PFNGLTEXBUMPPARAMETERFVATIPROC G_glewTexBumpParameterfvATI;
GLEW_FUN_EXPORT PFNGLTEXBUMPPARAMETERIVATIPROC G_glewTexBumpParameterivATI;

GLEW_FUN_EXPORT PFNGLALPHAFRAGMENTOP1ATIPROC G_glewAlphaFragmentOp1ATI;
GLEW_FUN_EXPORT PFNGLALPHAFRAGMENTOP2ATIPROC G_glewAlphaFragmentOp2ATI;
GLEW_FUN_EXPORT PFNGLALPHAFRAGMENTOP3ATIPROC G_glewAlphaFragmentOp3ATI;
GLEW_FUN_EXPORT PFNGLBEGINFRAGMENTSHADERATIPROC G_glewBeginFragmentShaderATI;
GLEW_FUN_EXPORT PFNGLBINDFRAGMENTSHADERATIPROC G_glewBindFragmentShaderATI;
GLEW_FUN_EXPORT PFNGLCOLORFRAGMENTOP1ATIPROC G_glewColorFragmentOp1ATI;
GLEW_FUN_EXPORT PFNGLCOLORFRAGMENTOP2ATIPROC G_glewColorFragmentOp2ATI;
GLEW_FUN_EXPORT PFNGLCOLORFRAGMENTOP3ATIPROC G_glewColorFragmentOp3ATI;
GLEW_FUN_EXPORT PFNGLDELETEFRAGMENTSHADERATIPROC G_glewDeleteFragmentShaderATI;
GLEW_FUN_EXPORT PFNGLENDFRAGMENTSHADERATIPROC G_glewEndFragmentShaderATI;
GLEW_FUN_EXPORT PFNGLGENFRAGMENTSHADERSATIPROC G_glewGenFragmentShadersATI;
GLEW_FUN_EXPORT PFNGLPASSTEXCOORDATIPROC G_glewPassTexCoordATI;
GLEW_FUN_EXPORT PFNGLSAMPLEMAPATIPROC G_glewSampleMapATI;
GLEW_FUN_EXPORT PFNGLSETFRAGMENTSHADERCONSTANTATIPROC G_glewSetFragmentShaderConstantATI;

GLEW_FUN_EXPORT PFNGLMAPOBJECTBUFFERATIPROC G_glewMapObjectBufferATI;
GLEW_FUN_EXPORT PFNGLUNMAPOBJECTBUFFERATIPROC G_glewUnmapObjectBufferATI;

GLEW_FUN_EXPORT PFNGLPNTRIANGLESFATIPROC G_glewPNTrianglesfATI;
GLEW_FUN_EXPORT PFNGLPNTRIANGLESIATIPROC G_glewPNTrianglesiATI;

GLEW_FUN_EXPORT PFNGLSTENCILFUNCSEPARATEATIPROC G_glewStencilFuncSeparateATI;
GLEW_FUN_EXPORT PFNGLSTENCILOPSEPARATEATIPROC G_glewStencilOpSeparateATI;

GLEW_FUN_EXPORT PFNGLARRAYOBJECTATIPROC G_glewArrayObjectATI;
GLEW_FUN_EXPORT PFNGLFREEOBJECTBUFFERATIPROC G_glewFreeObjectBufferATI;
GLEW_FUN_EXPORT PFNGLGETARRAYOBJECTFVATIPROC G_glewGetArrayObjectfvATI;
GLEW_FUN_EXPORT PFNGLGETARRAYOBJECTIVATIPROC G_glewGetArrayObjectivATI;
GLEW_FUN_EXPORT PFNGLGETOBJECTBUFFERFVATIPROC G_glewGetObjectBufferfvATI;
GLEW_FUN_EXPORT PFNGLGETOBJECTBUFFERIVATIPROC G_glewGetObjectBufferivATI;
GLEW_FUN_EXPORT PFNGLGETVARIANTARRAYOBJECTFVATIPROC G_glewGetVariantArrayObjectfvATI;
GLEW_FUN_EXPORT PFNGLGETVARIANTARRAYOBJECTIVATIPROC G_glewGetVariantArrayObjectivATI;
GLEW_FUN_EXPORT PFNGLISOBJECTBUFFERATIPROC G_glewIsObjectBufferATI;
GLEW_FUN_EXPORT PFNGLNEWOBJECTBUFFERATIPROC G_glewNewObjectBufferATI;
GLEW_FUN_EXPORT PFNGLUPDATEOBJECTBUFFERATIPROC G_glewUpdateObjectBufferATI;
GLEW_FUN_EXPORT PFNGLVARIANTARRAYOBJECTATIPROC G_glewVariantArrayObjectATI;

GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBARRAYOBJECTFVATIPROC G_glewGetVertexAttribArrayObjectfvATI;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBARRAYOBJECTIVATIPROC G_glewGetVertexAttribArrayObjectivATI;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBARRAYOBJECTATIPROC G_glewVertexAttribArrayObjectATI;

GLEW_FUN_EXPORT PFNGLCLIENTACTIVEVERTEXSTREAMATIPROC G_glewClientActiveVertexStreamATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3BATIPROC G_glewNormalStream3bATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3BVATIPROC G_glewNormalStream3bvATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3DATIPROC G_glewNormalStream3dATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3DVATIPROC G_glewNormalStream3dvATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3FATIPROC G_glewNormalStream3fATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3FVATIPROC G_glewNormalStream3fvATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3IATIPROC G_glewNormalStream3iATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3IVATIPROC G_glewNormalStream3ivATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3SATIPROC G_glewNormalStream3sATI;
GLEW_FUN_EXPORT PFNGLNORMALSTREAM3SVATIPROC G_glewNormalStream3svATI;
GLEW_FUN_EXPORT PFNGLVERTEXBLENDENVFATIPROC G_glewVertexBlendEnvfATI;
GLEW_FUN_EXPORT PFNGLVERTEXBLENDENVIATIPROC G_glewVertexBlendEnviATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM1DATIPROC G_glewVertexStream1dATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM1DVATIPROC G_glewVertexStream1dvATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM1FATIPROC G_glewVertexStream1fATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM1FVATIPROC G_glewVertexStream1fvATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM1IATIPROC G_glewVertexStream1iATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM1IVATIPROC G_glewVertexStream1ivATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM1SATIPROC G_glewVertexStream1sATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM1SVATIPROC G_glewVertexStream1svATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM2DATIPROC G_glewVertexStream2dATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM2DVATIPROC G_glewVertexStream2dvATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM2FATIPROC G_glewVertexStream2fATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM2FVATIPROC G_glewVertexStream2fvATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM2IATIPROC G_glewVertexStream2iATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM2IVATIPROC G_glewVertexStream2ivATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM2SATIPROC G_glewVertexStream2sATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM2SVATIPROC G_glewVertexStream2svATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM3DATIPROC G_glewVertexStream3dATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM3DVATIPROC G_glewVertexStream3dvATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM3FATIPROC G_glewVertexStream3fATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM3FVATIPROC G_glewVertexStream3fvATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM3IATIPROC G_glewVertexStream3iATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM3IVATIPROC G_glewVertexStream3ivATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM3SATIPROC G_glewVertexStream3sATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM3SVATIPROC G_glewVertexStream3svATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM4DATIPROC G_glewVertexStream4dATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM4DVATIPROC G_glewVertexStream4dvATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM4FATIPROC G_glewVertexStream4fATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM4FVATIPROC G_glewVertexStream4fvATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM4IATIPROC G_glewVertexStream4iATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM4IVATIPROC G_glewVertexStream4ivATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM4SATIPROC G_glewVertexStream4sATI;
GLEW_FUN_EXPORT PFNGLVERTEXSTREAM4SVATIPROC G_glewVertexStream4svATI;

GLEW_FUN_EXPORT PFNGLGETUNIFORMBUFFERSIZEEXTPROC G_glewGetUniformBufferSizeEXT;
GLEW_FUN_EXPORT PFNGLGETUNIFORMOFFSETEXTPROC G_glewGetUniformOffsetEXT;
GLEW_FUN_EXPORT PFNGLUNIFORMBUFFEREXTPROC G_glewUniformBufferEXT;

GLEW_FUN_EXPORT PFNGLBLENDCOLOREXTPROC G_glewBlendColorEXT;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONSEPARATEEXTPROC G_glewBlendEquationSeparateEXT;

GLEW_FUN_EXPORT PFNGLBLENDFUNCSEPARATEEXTPROC G_glewBlendFuncSeparateEXT;

GLEW_FUN_EXPORT PFNGLBLENDEQUATIONEXTPROC G_glewBlendEquationEXT;

GLEW_FUN_EXPORT PFNGLCOLORSUBTABLEEXTPROC G_glewColorSubTableEXT;
GLEW_FUN_EXPORT PFNGLCOPYCOLORSUBTABLEEXTPROC G_glewCopyColorSubTableEXT;

GLEW_FUN_EXPORT PFNGLLOCKARRAYSEXTPROC G_glewLockArraysEXT;
GLEW_FUN_EXPORT PFNGLUNLOCKARRAYSEXTPROC G_glewUnlockArraysEXT;

GLEW_FUN_EXPORT PFNGLCONVOLUTIONFILTER1DEXTPROC G_glewConvolutionFilter1DEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONFILTER2DEXTPROC G_glewConvolutionFilter2DEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERFEXTPROC G_glewConvolutionParameterfEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERFVEXTPROC G_glewConvolutionParameterfvEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERIEXTPROC G_glewConvolutionParameteriEXT;
GLEW_FUN_EXPORT PFNGLCONVOLUTIONPARAMETERIVEXTPROC G_glewConvolutionParameterivEXT;
GLEW_FUN_EXPORT PFNGLCOPYCONVOLUTIONFILTER1DEXTPROC G_glewCopyConvolutionFilter1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYCONVOLUTIONFILTER2DEXTPROC G_glewCopyConvolutionFilter2DEXT;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONFILTEREXTPROC G_glewGetConvolutionFilterEXT;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONPARAMETERFVEXTPROC G_glewGetConvolutionParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETCONVOLUTIONPARAMETERIVEXTPROC G_glewGetConvolutionParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETSEPARABLEFILTEREXTPROC G_glewGetSeparableFilterEXT;
GLEW_FUN_EXPORT PFNGLSEPARABLEFILTER2DEXTPROC G_glewSeparableFilter2DEXT;

GLEW_FUN_EXPORT PFNGLBINORMALPOINTEREXTPROC G_glewBinormalPointerEXT;
GLEW_FUN_EXPORT PFNGLTANGENTPOINTEREXTPROC G_glewTangentPointerEXT;

GLEW_FUN_EXPORT PFNGLCOPYTEXIMAGE1DEXTPROC G_glewCopyTexImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXIMAGE2DEXTPROC G_glewCopyTexImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE1DEXTPROC G_glewCopyTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE2DEXTPROC G_glewCopyTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXSUBIMAGE3DEXTPROC G_glewCopyTexSubImage3DEXT;

GLEW_FUN_EXPORT PFNGLCULLPARAMETERDVEXTPROC G_glewCullParameterdvEXT;
GLEW_FUN_EXPORT PFNGLCULLPARAMETERFVEXTPROC G_glewCullParameterfvEXT;

GLEW_FUN_EXPORT PFNGLINSERTEVENTMARKEREXTPROC G_glewInsertEventMarkerEXT;
GLEW_FUN_EXPORT PFNGLPOPGROUPMARKEREXTPROC G_glewPopGroupMarkerEXT;
GLEW_FUN_EXPORT PFNGLPUSHGROUPMARKEREXTPROC G_glewPushGroupMarkerEXT;

GLEW_FUN_EXPORT PFNGLDEPTHBOUNDSEXTPROC G_glewDepthBoundsEXT;

GLEW_FUN_EXPORT PFNGLBINDMULTITEXTUREEXTPROC G_glewBindMultiTextureEXT;
GLEW_FUN_EXPORT PFNGLCHECKNAMEDFRAMEBUFFERSTATUSEXTPROC G_glewCheckNamedFramebufferStatusEXT;
GLEW_FUN_EXPORT PFNGLCLIENTATTRIBDEFAULTEXTPROC G_glewClientAttribDefaultEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXIMAGE1DEXTPROC G_glewCompressedMultiTexImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXIMAGE2DEXTPROC G_glewCompressedMultiTexImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXIMAGE3DEXTPROC G_glewCompressedMultiTexImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXSUBIMAGE1DEXTPROC G_glewCompressedMultiTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXSUBIMAGE2DEXTPROC G_glewCompressedMultiTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDMULTITEXSUBIMAGE3DEXTPROC G_glewCompressedMultiTexSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTUREIMAGE1DEXTPROC G_glewCompressedTextureImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTUREIMAGE2DEXTPROC G_glewCompressedTextureImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTUREIMAGE3DEXTPROC G_glewCompressedTextureImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE1DEXTPROC G_glewCompressedTextureSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE2DEXTPROC G_glewCompressedTextureSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOMPRESSEDTEXTURESUBIMAGE3DEXTPROC G_glewCompressedTextureSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXIMAGE1DEXTPROC G_glewCopyMultiTexImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXIMAGE2DEXTPROC G_glewCopyMultiTexImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXSUBIMAGE1DEXTPROC G_glewCopyMultiTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXSUBIMAGE2DEXTPROC G_glewCopyMultiTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYMULTITEXSUBIMAGE3DEXTPROC G_glewCopyMultiTexSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTUREIMAGE1DEXTPROC G_glewCopyTextureImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTUREIMAGE2DEXTPROC G_glewCopyTextureImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE1DEXTPROC G_glewCopyTextureSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE2DEXTPROC G_glewCopyTextureSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLCOPYTEXTURESUBIMAGE3DEXTPROC G_glewCopyTextureSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLDISABLECLIENTSTATEINDEXEDEXTPROC G_glewDisableClientStateIndexedEXT;
GLEW_FUN_EXPORT PFNGLDISABLECLIENTSTATEIEXTPROC G_glewDisableClientStateiEXT;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXARRAYATTRIBEXTPROC G_glewDisableVertexArrayAttribEXT;
GLEW_FUN_EXPORT PFNGLDISABLEVERTEXARRAYEXTPROC G_glewDisableVertexArrayEXT;
GLEW_FUN_EXPORT PFNGLENABLECLIENTSTATEINDEXEDEXTPROC G_glewEnableClientStateIndexedEXT;
GLEW_FUN_EXPORT PFNGLENABLECLIENTSTATEIEXTPROC G_glewEnableClientStateiEXT;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXARRAYATTRIBEXTPROC G_glewEnableVertexArrayAttribEXT;
GLEW_FUN_EXPORT PFNGLENABLEVERTEXARRAYEXTPROC G_glewEnableVertexArrayEXT;
GLEW_FUN_EXPORT PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEEXTPROC G_glewFlushMappedNamedBufferRangeEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERDRAWBUFFEREXTPROC G_glewFramebufferDrawBufferEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERDRAWBUFFERSEXTPROC G_glewFramebufferDrawBuffersEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERREADBUFFEREXTPROC G_glewFramebufferReadBufferEXT;
GLEW_FUN_EXPORT PFNGLGENERATEMULTITEXMIPMAPEXTPROC G_glewGenerateMultiTexMipmapEXT;
GLEW_FUN_EXPORT PFNGLGENERATETEXTUREMIPMAPEXTPROC G_glewGenerateTextureMipmapEXT;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDMULTITEXIMAGEEXTPROC G_glewGetCompressedMultiTexImageEXT;
GLEW_FUN_EXPORT PFNGLGETCOMPRESSEDTEXTUREIMAGEEXTPROC G_glewGetCompressedTextureImageEXT;
GLEW_FUN_EXPORT PFNGLGETDOUBLEINDEXEDVEXTPROC G_glewGetDoubleIndexedvEXT;
GLEW_FUN_EXPORT PFNGLGETDOUBLEI_VEXTPROC G_glewGetDoublei_vEXT;
GLEW_FUN_EXPORT PFNGLGETFLOATINDEXEDVEXTPROC G_glewGetFloatIndexedvEXT;
GLEW_FUN_EXPORT PFNGLGETFLOATI_VEXTPROC G_glewGetFloati_vEXT;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERPARAMETERIVEXTPROC G_glewGetFramebufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXENVFVEXTPROC G_glewGetMultiTexEnvfvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXENVIVEXTPROC G_glewGetMultiTexEnvivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXGENDVEXTPROC G_glewGetMultiTexGendvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXGENFVEXTPROC G_glewGetMultiTexGenfvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXGENIVEXTPROC G_glewGetMultiTexGenivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXIMAGEEXTPROC G_glewGetMultiTexImageEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXLEVELPARAMETERFVEXTPROC G_glewGetMultiTexLevelParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXLEVELPARAMETERIVEXTPROC G_glewGetMultiTexLevelParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXPARAMETERIIVEXTPROC G_glewGetMultiTexParameterIivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXPARAMETERIUIVEXTPROC G_glewGetMultiTexParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXPARAMETERFVEXTPROC G_glewGetMultiTexParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETMULTITEXPARAMETERIVEXTPROC G_glewGetMultiTexParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERPARAMETERIVEXTPROC G_glewGetNamedBufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERPOINTERVEXTPROC G_glewGetNamedBufferPointervEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERSUBDATAEXTPROC G_glewGetNamedBufferSubDataEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC G_glewGetNamedFramebufferAttachmentParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMLOCALPARAMETERIIVEXTPROC G_glewGetNamedProgramLocalParameterIivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMLOCALPARAMETERIUIVEXTPROC G_glewGetNamedProgramLocalParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMLOCALPARAMETERDVEXTPROC G_glewGetNamedProgramLocalParameterdvEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMLOCALPARAMETERFVEXTPROC G_glewGetNamedProgramLocalParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMSTRINGEXTPROC G_glewGetNamedProgramStringEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDPROGRAMIVEXTPROC G_glewGetNamedProgramivEXT;
GLEW_FUN_EXPORT PFNGLGETNAMEDRENDERBUFFERPARAMETERIVEXTPROC G_glewGetNamedRenderbufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETPOINTERINDEXEDVEXTPROC G_glewGetPointerIndexedvEXT;
GLEW_FUN_EXPORT PFNGLGETPOINTERI_VEXTPROC G_glewGetPointeri_vEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREIMAGEEXTPROC G_glewGetTextureImageEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTURELEVELPARAMETERFVEXTPROC G_glewGetTextureLevelParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTURELEVELPARAMETERIVEXTPROC G_glewGetTextureLevelParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIIVEXTPROC G_glewGetTextureParameterIivEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIUIVEXTPROC G_glewGetTextureParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERFVEXTPROC G_glewGetTextureParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETTEXTUREPARAMETERIVEXTPROC G_glewGetTextureParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYINTEGERI_VEXTPROC G_glewGetVertexArrayIntegeri_vEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYINTEGERVEXTPROC G_glewGetVertexArrayIntegervEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYPOINTERI_VEXTPROC G_glewGetVertexArrayPointeri_vEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXARRAYPOINTERVEXTPROC G_glewGetVertexArrayPointervEXT;
GLEW_FUN_EXPORT PFNGLMAPNAMEDBUFFEREXTPROC G_glewMapNamedBufferEXT;
GLEW_FUN_EXPORT PFNGLMAPNAMEDBUFFERRANGEEXTPROC G_glewMapNamedBufferRangeEXT;
GLEW_FUN_EXPORT PFNGLMATRIXFRUSTUMEXTPROC G_glewMatrixFrustumEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADIDENTITYEXTPROC G_glewMatrixLoadIdentityEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADTRANSPOSEDEXTPROC G_glewMatrixLoadTransposedEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADTRANSPOSEFEXTPROC G_glewMatrixLoadTransposefEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADDEXTPROC G_glewMatrixLoaddEXT;
GLEW_FUN_EXPORT PFNGLMATRIXLOADFEXTPROC G_glewMatrixLoadfEXT;
GLEW_FUN_EXPORT PFNGLMATRIXMULTTRANSPOSEDEXTPROC G_glewMatrixMultTransposedEXT;
GLEW_FUN_EXPORT PFNGLMATRIXMULTTRANSPOSEFEXTPROC G_glewMatrixMultTransposefEXT;
GLEW_FUN_EXPORT PFNGLMATRIXMULTDEXTPROC G_glewMatrixMultdEXT;
GLEW_FUN_EXPORT PFNGLMATRIXMULTFEXTPROC G_glewMatrixMultfEXT;
GLEW_FUN_EXPORT PFNGLMATRIXORTHOEXTPROC G_glewMatrixOrthoEXT;
GLEW_FUN_EXPORT PFNGLMATRIXPOPEXTPROC G_glewMatrixPopEXT;
GLEW_FUN_EXPORT PFNGLMATRIXPUSHEXTPROC G_glewMatrixPushEXT;
GLEW_FUN_EXPORT PFNGLMATRIXROTATEDEXTPROC G_glewMatrixRotatedEXT;
GLEW_FUN_EXPORT PFNGLMATRIXROTATEFEXTPROC G_glewMatrixRotatefEXT;
GLEW_FUN_EXPORT PFNGLMATRIXSCALEDEXTPROC G_glewMatrixScaledEXT;
GLEW_FUN_EXPORT PFNGLMATRIXSCALEFEXTPROC G_glewMatrixScalefEXT;
GLEW_FUN_EXPORT PFNGLMATRIXTRANSLATEDEXTPROC G_glewMatrixTranslatedEXT;
GLEW_FUN_EXPORT PFNGLMATRIXTRANSLATEFEXTPROC G_glewMatrixTranslatefEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXBUFFEREXTPROC G_glewMultiTexBufferEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORDPOINTEREXTPROC G_glewMultiTexCoordPointerEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXENVFEXTPROC G_glewMultiTexEnvfEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXENVFVEXTPROC G_glewMultiTexEnvfvEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXENVIEXTPROC G_glewMultiTexEnviEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXENVIVEXTPROC G_glewMultiTexEnvivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENDEXTPROC G_glewMultiTexGendEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENDVEXTPROC G_glewMultiTexGendvEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENFEXTPROC G_glewMultiTexGenfEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENFVEXTPROC G_glewMultiTexGenfvEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENIEXTPROC G_glewMultiTexGeniEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXGENIVEXTPROC G_glewMultiTexGenivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXIMAGE1DEXTPROC G_glewMultiTexImage1DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXIMAGE2DEXTPROC G_glewMultiTexImage2DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXIMAGE3DEXTPROC G_glewMultiTexImage3DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERIIVEXTPROC G_glewMultiTexParameterIivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERIUIVEXTPROC G_glewMultiTexParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERFEXTPROC G_glewMultiTexParameterfEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERFVEXTPROC G_glewMultiTexParameterfvEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERIEXTPROC G_glewMultiTexParameteriEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXPARAMETERIVEXTPROC G_glewMultiTexParameterivEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXRENDERBUFFEREXTPROC G_glewMultiTexRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXSUBIMAGE1DEXTPROC G_glewMultiTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXSUBIMAGE2DEXTPROC G_glewMultiTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLMULTITEXSUBIMAGE3DEXTPROC G_glewMultiTexSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERDATAEXTPROC G_glewNamedBufferDataEXT;
GLEW_FUN_EXPORT PFNGLNAMEDBUFFERSUBDATAEXTPROC G_glewNamedBufferSubDataEXT;
GLEW_FUN_EXPORT PFNGLNAMEDCOPYBUFFERSUBDATAEXTPROC G_glewNamedCopyBufferSubDataEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERRENDERBUFFEREXTPROC G_glewNamedFramebufferRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURE1DEXTPROC G_glewNamedFramebufferTexture1DEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURE2DEXTPROC G_glewNamedFramebufferTexture2DEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURE3DEXTPROC G_glewNamedFramebufferTexture3DEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTUREEXTPROC G_glewNamedFramebufferTextureEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTUREFACEEXTPROC G_glewNamedFramebufferTextureFaceEXT;
GLEW_FUN_EXPORT PFNGLNAMEDFRAMEBUFFERTEXTURELAYEREXTPROC G_glewNamedFramebufferTextureLayerEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETER4DEXTPROC G_glewNamedProgramLocalParameter4dEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETER4DVEXTPROC G_glewNamedProgramLocalParameter4dvEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETER4FEXTPROC G_glewNamedProgramLocalParameter4fEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETER4FVEXTPROC G_glewNamedProgramLocalParameter4fvEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERI4IEXTPROC G_glewNamedProgramLocalParameterI4iEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERI4IVEXTPROC G_glewNamedProgramLocalParameterI4ivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIEXTPROC G_glewNamedProgramLocalParameterI4uiEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERI4UIVEXTPROC G_glewNamedProgramLocalParameterI4uivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERS4FVEXTPROC G_glewNamedProgramLocalParameters4fvEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERSI4IVEXTPROC G_glewNamedProgramLocalParametersI4ivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMLOCALPARAMETERSI4UIVEXTPROC G_glewNamedProgramLocalParametersI4uivEXT;
GLEW_FUN_EXPORT PFNGLNAMEDPROGRAMSTRINGEXTPROC G_glewNamedProgramStringEXT;
GLEW_FUN_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEEXTPROC G_glewNamedRenderbufferStorageEXT;
GLEW_FUN_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLECOVERAGEEXTPROC G_glewNamedRenderbufferStorageMultisampleCoverageEXT;
GLEW_FUN_EXPORT PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC G_glewNamedRenderbufferStorageMultisampleEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1DEXTPROC G_glewProgramUniform1dEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1DVEXTPROC G_glewProgramUniform1dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FEXTPROC G_glewProgramUniform1fEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1FVEXTPROC G_glewProgramUniform1fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IEXTPROC G_glewProgramUniform1iEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1IVEXTPROC G_glewProgramUniform1ivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIEXTPROC G_glewProgramUniform1uiEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UIVEXTPROC G_glewProgramUniform1uivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2DEXTPROC G_glewProgramUniform2dEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2DVEXTPROC G_glewProgramUniform2dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FEXTPROC G_glewProgramUniform2fEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2FVEXTPROC G_glewProgramUniform2fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IEXTPROC G_glewProgramUniform2iEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2IVEXTPROC G_glewProgramUniform2ivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIEXTPROC G_glewProgramUniform2uiEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UIVEXTPROC G_glewProgramUniform2uivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3DEXTPROC G_glewProgramUniform3dEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3DVEXTPROC G_glewProgramUniform3dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FEXTPROC G_glewProgramUniform3fEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3FVEXTPROC G_glewProgramUniform3fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IEXTPROC G_glewProgramUniform3iEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3IVEXTPROC G_glewProgramUniform3ivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIEXTPROC G_glewProgramUniform3uiEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UIVEXTPROC G_glewProgramUniform3uivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4DEXTPROC G_glewProgramUniform4dEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4DVEXTPROC G_glewProgramUniform4dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FEXTPROC G_glewProgramUniform4fEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4FVEXTPROC G_glewProgramUniform4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IEXTPROC G_glewProgramUniform4iEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4IVEXTPROC G_glewProgramUniform4ivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIEXTPROC G_glewProgramUniform4uiEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UIVEXTPROC G_glewProgramUniform4uivEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2DVEXTPROC G_glewProgramUniformMatrix2dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2FVEXTPROC G_glewProgramUniformMatrix2fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3DVEXTPROC G_glewProgramUniformMatrix2x3dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X3FVEXTPROC G_glewProgramUniformMatrix2x3fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4DVEXTPROC G_glewProgramUniformMatrix2x4dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX2X4FVEXTPROC G_glewProgramUniformMatrix2x4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3DVEXTPROC G_glewProgramUniformMatrix3dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3FVEXTPROC G_glewProgramUniformMatrix3fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2DVEXTPROC G_glewProgramUniformMatrix3x2dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X2FVEXTPROC G_glewProgramUniformMatrix3x2fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4DVEXTPROC G_glewProgramUniformMatrix3x4dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX3X4FVEXTPROC G_glewProgramUniformMatrix3x4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4DVEXTPROC G_glewProgramUniformMatrix4dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4FVEXTPROC G_glewProgramUniformMatrix4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2DVEXTPROC G_glewProgramUniformMatrix4x2dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X2FVEXTPROC G_glewProgramUniformMatrix4x2fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3DVEXTPROC G_glewProgramUniformMatrix4x3dvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMMATRIX4X3FVEXTPROC G_glewProgramUniformMatrix4x3fvEXT;
GLEW_FUN_EXPORT PFNGLPUSHCLIENTATTRIBDEFAULTEXTPROC G_glewPushClientAttribDefaultEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREBUFFEREXTPROC G_glewTextureBufferEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE1DEXTPROC G_glewTextureImage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE2DEXTPROC G_glewTextureImage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE3DEXTPROC G_glewTextureImage3DEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIIVEXTPROC G_glewTextureParameterIivEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIUIVEXTPROC G_glewTextureParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERFEXTPROC G_glewTextureParameterfEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERFVEXTPROC G_glewTextureParameterfvEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIEXTPROC G_glewTextureParameteriEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREPARAMETERIVEXTPROC G_glewTextureParameterivEXT;
GLEW_FUN_EXPORT PFNGLTEXTURERENDERBUFFEREXTPROC G_glewTextureRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE1DEXTPROC G_glewTextureSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE2DEXTPROC G_glewTextureSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXTURESUBIMAGE3DEXTPROC G_glewTextureSubImage3DEXT;
GLEW_FUN_EXPORT PFNGLUNMAPNAMEDBUFFEREXTPROC G_glewUnmapNamedBufferEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYCOLOROFFSETEXTPROC G_glewVertexArrayColorOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYEDGEFLAGOFFSETEXTPROC G_glewVertexArrayEdgeFlagOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYFOGCOORDOFFSETEXTPROC G_glewVertexArrayFogCoordOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYINDEXOFFSETEXTPROC G_glewVertexArrayIndexOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYMULTITEXCOORDOFFSETEXTPROC G_glewVertexArrayMultiTexCoordOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYNORMALOFFSETEXTPROC G_glewVertexArrayNormalOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYSECONDARYCOLOROFFSETEXTPROC G_glewVertexArraySecondaryColorOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYTEXCOORDOFFSETEXTPROC G_glewVertexArrayTexCoordOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBIOFFSETEXTPROC G_glewVertexArrayVertexAttribIOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBOFFSETEXTPROC G_glewVertexArrayVertexAttribOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXOFFSETEXTPROC G_glewVertexArrayVertexOffsetEXT;

GLEW_FUN_EXPORT PFNGLCOLORMASKINDEXEDEXTPROC G_glewColorMaskIndexedEXT;
GLEW_FUN_EXPORT PFNGLDISABLEINDEXEDEXTPROC G_glewDisableIndexedEXT;
GLEW_FUN_EXPORT PFNGLENABLEINDEXEDEXTPROC G_glewEnableIndexedEXT;
GLEW_FUN_EXPORT PFNGLGETBOOLEANINDEXEDVEXTPROC G_glewGetBooleanIndexedvEXT;
GLEW_FUN_EXPORT PFNGLGETINTEGERINDEXEDVEXTPROC G_glewGetIntegerIndexedvEXT;
GLEW_FUN_EXPORT PFNGLISENABLEDINDEXEDEXTPROC G_glewIsEnabledIndexedEXT;

GLEW_FUN_EXPORT PFNGLDRAWARRAYSINSTANCEDEXTPROC G_glewDrawArraysInstancedEXT;
GLEW_FUN_EXPORT PFNGLDRAWELEMENTSINSTANCEDEXTPROC G_glewDrawElementsInstancedEXT;

GLEW_FUN_EXPORT PFNGLDRAWRANGEELEMENTSEXTPROC G_glewDrawRangeElementsEXT;

GLEW_FUN_EXPORT PFNGLFOGCOORDPOINTEREXTPROC G_glewFogCoordPointerEXT;
GLEW_FUN_EXPORT PFNGLFOGCOORDDEXTPROC G_glewFogCoorddEXT;
GLEW_FUN_EXPORT PFNGLFOGCOORDDVEXTPROC G_glewFogCoorddvEXT;
GLEW_FUN_EXPORT PFNGLFOGCOORDFEXTPROC G_glewFogCoordfEXT;
GLEW_FUN_EXPORT PFNGLFOGCOORDFVEXTPROC G_glewFogCoordfvEXT;

GLEW_FUN_EXPORT PFNGLFRAGMENTCOLORMATERIALEXTPROC G_glewFragmentColorMaterialEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELFEXTPROC G_glewFragmentLightModelfEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELFVEXTPROC G_glewFragmentLightModelfvEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELIEXTPROC G_glewFragmentLightModeliEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELIVEXTPROC G_glewFragmentLightModelivEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTFEXTPROC G_glewFragmentLightfEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTFVEXTPROC G_glewFragmentLightfvEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTIEXTPROC G_glewFragmentLightiEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTIVEXTPROC G_glewFragmentLightivEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALFEXTPROC G_glewFragmentMaterialfEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALFVEXTPROC G_glewFragmentMaterialfvEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALIEXTPROC G_glewFragmentMaterialiEXT;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALIVEXTPROC G_glewFragmentMaterialivEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTLIGHTFVEXTPROC G_glewGetFragmentLightfvEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTLIGHTIVEXTPROC G_glewGetFragmentLightivEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTMATERIALFVEXTPROC G_glewGetFragmentMaterialfvEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTMATERIALIVEXTPROC G_glewGetFragmentMaterialivEXT;
GLEW_FUN_EXPORT PFNGLLIGHTENVIEXTPROC G_glewLightEnviEXT;

GLEW_FUN_EXPORT PFNGLBLITFRAMEBUFFEREXTPROC G_glewBlitFramebufferEXT;

GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLEEXTPROC G_glewRenderbufferStorageMultisampleEXT;

GLEW_FUN_EXPORT PFNGLBINDFRAMEBUFFEREXTPROC G_glewBindFramebufferEXT;
GLEW_FUN_EXPORT PFNGLBINDRENDERBUFFEREXTPROC G_glewBindRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC G_glewCheckFramebufferStatusEXT;
GLEW_FUN_EXPORT PFNGLDELETEFRAMEBUFFERSEXTPROC G_glewDeleteFramebuffersEXT;
GLEW_FUN_EXPORT PFNGLDELETERENDERBUFFERSEXTPROC G_glewDeleteRenderbuffersEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC G_glewFramebufferRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE1DEXTPROC G_glewFramebufferTexture1DEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE2DEXTPROC G_glewFramebufferTexture2DEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURE3DEXTPROC G_glewFramebufferTexture3DEXT;
GLEW_FUN_EXPORT PFNGLGENFRAMEBUFFERSEXTPROC G_glewGenFramebuffersEXT;
GLEW_FUN_EXPORT PFNGLGENRENDERBUFFERSEXTPROC G_glewGenRenderbuffersEXT;
GLEW_FUN_EXPORT PFNGLGENERATEMIPMAPEXTPROC G_glewGenerateMipmapEXT;
GLEW_FUN_EXPORT PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC G_glewGetFramebufferAttachmentParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC G_glewGetRenderbufferParameterivEXT;
GLEW_FUN_EXPORT PFNGLISFRAMEBUFFEREXTPROC G_glewIsFramebufferEXT;
GLEW_FUN_EXPORT PFNGLISRENDERBUFFEREXTPROC G_glewIsRenderbufferEXT;
GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEEXTPROC G_glewRenderbufferStorageEXT;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREEXTPROC G_glewFramebufferTextureEXT;
GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTUREFACEEXTPROC G_glewFramebufferTextureFaceEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERIEXTPROC G_glewProgramParameteriEXT;

GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETERS4FVEXTPROC G_glewProgramEnvParameters4fvEXT;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETERS4FVEXTPROC G_glewProgramLocalParameters4fvEXT;

GLEW_FUN_EXPORT PFNGLBINDFRAGDATALOCATIONEXTPROC G_glewBindFragDataLocationEXT;
GLEW_FUN_EXPORT PFNGLGETFRAGDATALOCATIONEXTPROC G_glewGetFragDataLocationEXT;
GLEW_FUN_EXPORT PFNGLGETUNIFORMUIVEXTPROC G_glewGetUniformuivEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIIVEXTPROC G_glewGetVertexAttribIivEXT;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIUIVEXTPROC G_glewGetVertexAttribIuivEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIEXTPROC G_glewUniform1uiEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM1UIVEXTPROC G_glewUniform1uivEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIEXTPROC G_glewUniform2uiEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM2UIVEXTPROC G_glewUniform2uivEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIEXTPROC G_glewUniform3uiEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM3UIVEXTPROC G_glewUniform3uivEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIEXTPROC G_glewUniform4uiEXT;
GLEW_FUN_EXPORT PFNGLUNIFORM4UIVEXTPROC G_glewUniform4uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1IEXTPROC G_glewVertexAttribI1iEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1IVEXTPROC G_glewVertexAttribI1ivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1UIEXTPROC G_glewVertexAttribI1uiEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI1UIVEXTPROC G_glewVertexAttribI1uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2IEXTPROC G_glewVertexAttribI2iEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2IVEXTPROC G_glewVertexAttribI2ivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2UIEXTPROC G_glewVertexAttribI2uiEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI2UIVEXTPROC G_glewVertexAttribI2uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3IEXTPROC G_glewVertexAttribI3iEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3IVEXTPROC G_glewVertexAttribI3ivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3UIEXTPROC G_glewVertexAttribI3uiEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI3UIVEXTPROC G_glewVertexAttribI3uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4BVEXTPROC G_glewVertexAttribI4bvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IEXTPROC G_glewVertexAttribI4iEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4IVEXTPROC G_glewVertexAttribI4ivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4SVEXTPROC G_glewVertexAttribI4svEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UBVEXTPROC G_glewVertexAttribI4ubvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIEXTPROC G_glewVertexAttribI4uiEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4UIVEXTPROC G_glewVertexAttribI4uivEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBI4USVEXTPROC G_glewVertexAttribI4usvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIPOINTEREXTPROC G_glewVertexAttribIPointerEXT;

GLEW_FUN_EXPORT PFNGLGETHISTOGRAMEXTPROC G_glewGetHistogramEXT;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPARAMETERFVEXTPROC G_glewGetHistogramParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETHISTOGRAMPARAMETERIVEXTPROC G_glewGetHistogramParameterivEXT;
GLEW_FUN_EXPORT PFNGLGETMINMAXEXTPROC G_glewGetMinmaxEXT;
GLEW_FUN_EXPORT PFNGLGETMINMAXPARAMETERFVEXTPROC G_glewGetMinmaxParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETMINMAXPARAMETERIVEXTPROC G_glewGetMinmaxParameterivEXT;
GLEW_FUN_EXPORT PFNGLHISTOGRAMEXTPROC G_glewHistogramEXT;
GLEW_FUN_EXPORT PFNGLMINMAXEXTPROC G_glewMinmaxEXT;
GLEW_FUN_EXPORT PFNGLRESETHISTOGRAMEXTPROC G_glewResetHistogramEXT;
GLEW_FUN_EXPORT PFNGLRESETMINMAXEXTPROC G_glewResetMinmaxEXT;

GLEW_FUN_EXPORT PFNGLINDEXFUNCEXTPROC G_glewIndexFuncEXT;

GLEW_FUN_EXPORT PFNGLINDEXMATERIALEXTPROC G_glewIndexMaterialEXT;

GLEW_FUN_EXPORT PFNGLAPPLYTEXTUREEXTPROC G_glewApplyTextureEXT;
GLEW_FUN_EXPORT PFNGLTEXTURELIGHTEXTPROC G_glewTextureLightEXT;
GLEW_FUN_EXPORT PFNGLTEXTUREMATERIALEXTPROC G_glewTextureMaterialEXT;

GLEW_FUN_EXPORT PFNGLMULTIDRAWARRAYSEXTPROC G_glewMultiDrawArraysEXT;
GLEW_FUN_EXPORT PFNGLMULTIDRAWELEMENTSEXTPROC G_glewMultiDrawElementsEXT;

GLEW_FUN_EXPORT PFNGLSAMPLEMASKEXTPROC G_glewSampleMaskEXT;
GLEW_FUN_EXPORT PFNGLSAMPLEPATTERNEXTPROC G_glewSamplePatternEXT;

GLEW_FUN_EXPORT PFNGLCOLORTABLEEXTPROC G_glewColorTableEXT;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEEXTPROC G_glewGetColorTableEXT;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERFVEXTPROC G_glewGetColorTableParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERIVEXTPROC G_glewGetColorTableParameterivEXT;

GLEW_FUN_EXPORT PFNGLGETPIXELTRANSFORMPARAMETERFVEXTPROC G_glewGetPixelTransformParameterfvEXT;
GLEW_FUN_EXPORT PFNGLGETPIXELTRANSFORMPARAMETERIVEXTPROC G_glewGetPixelTransformParameterivEXT;
GLEW_FUN_EXPORT PFNGLPIXELTRANSFORMPARAMETERFEXTPROC G_glewPixelTransformParameterfEXT;
GLEW_FUN_EXPORT PFNGLPIXELTRANSFORMPARAMETERFVEXTPROC G_glewPixelTransformParameterfvEXT;
GLEW_FUN_EXPORT PFNGLPIXELTRANSFORMPARAMETERIEXTPROC G_glewPixelTransformParameteriEXT;
GLEW_FUN_EXPORT PFNGLPIXELTRANSFORMPARAMETERIVEXTPROC G_glewPixelTransformParameterivEXT;

GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFEXTPROC G_glewPointParameterfEXT;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERFVEXTPROC G_glewPointParameterfvEXT;

GLEW_FUN_EXPORT PFNGLPOLYGONOFFSETEXTPROC G_glewPolygonOffsetEXT;

GLEW_FUN_EXPORT PFNGLPROVOKINGVERTEXEXTPROC G_glewProvokingVertexEXT;

GLEW_FUN_EXPORT PFNGLBEGINSCENEEXTPROC G_glewBeginSceneEXT;
GLEW_FUN_EXPORT PFNGLENDSCENEEXTPROC G_glewEndSceneEXT;

GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3BEXTPROC G_glewSecondaryColor3bEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3BVEXTPROC G_glewSecondaryColor3bvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3DEXTPROC G_glewSecondaryColor3dEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3DVEXTPROC G_glewSecondaryColor3dvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3FEXTPROC G_glewSecondaryColor3fEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3FVEXTPROC G_glewSecondaryColor3fvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3IEXTPROC G_glewSecondaryColor3iEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3IVEXTPROC G_glewSecondaryColor3ivEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3SEXTPROC G_glewSecondaryColor3sEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3SVEXTPROC G_glewSecondaryColor3svEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UBEXTPROC G_glewSecondaryColor3ubEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UBVEXTPROC G_glewSecondaryColor3ubvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UIEXTPROC G_glewSecondaryColor3uiEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3UIVEXTPROC G_glewSecondaryColor3uivEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3USEXTPROC G_glewSecondaryColor3usEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3USVEXTPROC G_glewSecondaryColor3usvEXT;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORPOINTEREXTPROC G_glewSecondaryColorPointerEXT;

GLEW_FUN_EXPORT PFNGLACTIVEPROGRAMEXTPROC G_glewActiveProgramEXT;
GLEW_FUN_EXPORT PFNGLCREATESHADERPROGRAMEXTPROC G_glewCreateShaderProgramEXT;
GLEW_FUN_EXPORT PFNGLUSESHADERPROGRAMEXTPROC G_glewUseShaderProgramEXT;

GLEW_FUN_EXPORT PFNGLBINDIMAGETEXTUREEXTPROC G_glewBindImageTextureEXT;
GLEW_FUN_EXPORT PFNGLMEMORYBARRIEREXTPROC G_glewMemoryBarrierEXT;

GLEW_FUN_EXPORT PFNGLACTIVESTENCILFACEEXTPROC G_glewActiveStencilFaceEXT;

GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE1DEXTPROC G_glewTexSubImage1DEXT;
GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE2DEXTPROC G_glewTexSubImage2DEXT;
GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE3DEXTPROC G_glewTexSubImage3DEXT;

GLEW_FUN_EXPORT PFNGLTEXIMAGE3DEXTPROC G_glewTexImage3DEXT;

GLEW_FUN_EXPORT PFNGLFRAMEBUFFERTEXTURELAYEREXTPROC G_glewFramebufferTextureLayerEXT;

GLEW_FUN_EXPORT PFNGLTEXBUFFEREXTPROC G_glewTexBufferEXT;

GLEW_FUN_EXPORT PFNGLCLEARCOLORIIEXTPROC G_glewClearColorIiEXT;
GLEW_FUN_EXPORT PFNGLCLEARCOLORIUIEXTPROC G_glewClearColorIuiEXT;
GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERIIVEXTPROC G_glewGetTexParameterIivEXT;
GLEW_FUN_EXPORT PFNGLGETTEXPARAMETERIUIVEXTPROC G_glewGetTexParameterIuivEXT;
GLEW_FUN_EXPORT PFNGLTEXPARAMETERIIVEXTPROC G_glewTexParameterIivEXT;
GLEW_FUN_EXPORT PFNGLTEXPARAMETERIUIVEXTPROC G_glewTexParameterIuivEXT;

GLEW_FUN_EXPORT PFNGLARETEXTURESRESIDENTEXTPROC G_glewAreTexturesResidentEXT;
GLEW_FUN_EXPORT PFNGLBINDTEXTUREEXTPROC G_glewBindTextureEXT;
GLEW_FUN_EXPORT PFNGLDELETETEXTURESEXTPROC G_glewDeleteTexturesEXT;
GLEW_FUN_EXPORT PFNGLGENTEXTURESEXTPROC G_glewGenTexturesEXT;
GLEW_FUN_EXPORT PFNGLISTEXTUREEXTPROC G_glewIsTextureEXT;
GLEW_FUN_EXPORT PFNGLPRIORITIZETEXTURESEXTPROC G_glewPrioritizeTexturesEXT;

GLEW_FUN_EXPORT PFNGLTEXTURENORMALEXTPROC G_glewTextureNormalEXT;

GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTI64VEXTPROC G_glewGetQueryObjecti64vEXT;
GLEW_FUN_EXPORT PFNGLGETQUERYOBJECTUI64VEXTPROC G_glewGetQueryObjectui64vEXT;

GLEW_FUN_EXPORT PFNGLBEGINTRANSFORMFEEDBACKEXTPROC G_glewBeginTransformFeedbackEXT;
GLEW_FUN_EXPORT PFNGLBINDBUFFERBASEEXTPROC G_glewBindBufferBaseEXT;
GLEW_FUN_EXPORT PFNGLBINDBUFFEROFFSETEXTPROC G_glewBindBufferOffsetEXT;
GLEW_FUN_EXPORT PFNGLBINDBUFFERRANGEEXTPROC G_glewBindBufferRangeEXT;
GLEW_FUN_EXPORT PFNGLENDTRANSFORMFEEDBACKEXTPROC G_glewEndTransformFeedbackEXT;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKVARYINGEXTPROC G_glewGetTransformFeedbackVaryingEXT;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKVARYINGSEXTPROC G_glewTransformFeedbackVaryingsEXT;

GLEW_FUN_EXPORT PFNGLARRAYELEMENTEXTPROC G_glewArrayElementEXT;
GLEW_FUN_EXPORT PFNGLCOLORPOINTEREXTPROC G_glewColorPointerEXT;
GLEW_FUN_EXPORT PFNGLDRAWARRAYSEXTPROC G_glewDrawArraysEXT;
GLEW_FUN_EXPORT PFNGLEDGEFLAGPOINTEREXTPROC G_glewEdgeFlagPointerEXT;
GLEW_FUN_EXPORT PFNGLINDEXPOINTEREXTPROC G_glewIndexPointerEXT;
GLEW_FUN_EXPORT PFNGLNORMALPOINTEREXTPROC G_glewNormalPointerEXT;
GLEW_FUN_EXPORT PFNGLTEXCOORDPOINTEREXTPROC G_glewTexCoordPointerEXT;
GLEW_FUN_EXPORT PFNGLVERTEXPOINTEREXTPROC G_glewVertexPointerEXT;

GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBLDVEXTPROC G_glewGetVertexAttribLdvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYVERTEXATTRIBLOFFSETEXTPROC G_glewVertexArrayVertexAttribLOffsetEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1DEXTPROC G_glewVertexAttribL1dEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1DVEXTPROC G_glewVertexAttribL1dvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2DEXTPROC G_glewVertexAttribL2dEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2DVEXTPROC G_glewVertexAttribL2dvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3DEXTPROC G_glewVertexAttribL3dEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3DVEXTPROC G_glewVertexAttribL3dvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4DEXTPROC G_glewVertexAttribL4dEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4DVEXTPROC G_glewVertexAttribL4dvEXT;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBLPOINTEREXTPROC G_glewVertexAttribLPointerEXT;

GLEW_FUN_EXPORT PFNGLBEGINVERTEXSHADEREXTPROC G_glewBeginVertexShaderEXT;
GLEW_FUN_EXPORT PFNGLBINDLIGHTPARAMETEREXTPROC G_glewBindLightParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDMATERIALPARAMETEREXTPROC G_glewBindMaterialParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDPARAMETEREXTPROC G_glewBindParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDTEXGENPARAMETEREXTPROC G_glewBindTexGenParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDTEXTUREUNITPARAMETEREXTPROC G_glewBindTextureUnitParameterEXT;
GLEW_FUN_EXPORT PFNGLBINDVERTEXSHADEREXTPROC G_glewBindVertexShaderEXT;
GLEW_FUN_EXPORT PFNGLDELETEVERTEXSHADEREXTPROC G_glewDeleteVertexShaderEXT;
GLEW_FUN_EXPORT PFNGLDISABLEVARIANTCLIENTSTATEEXTPROC G_glewDisableVariantClientStateEXT;
GLEW_FUN_EXPORT PFNGLENABLEVARIANTCLIENTSTATEEXTPROC G_glewEnableVariantClientStateEXT;
GLEW_FUN_EXPORT PFNGLENDVERTEXSHADEREXTPROC G_glewEndVertexShaderEXT;
GLEW_FUN_EXPORT PFNGLEXTRACTCOMPONENTEXTPROC G_glewExtractComponentEXT;
GLEW_FUN_EXPORT PFNGLGENSYMBOLSEXTPROC G_glewGenSymbolsEXT;
GLEW_FUN_EXPORT PFNGLGENVERTEXSHADERSEXTPROC G_glewGenVertexShadersEXT;
GLEW_FUN_EXPORT PFNGLGETINVARIANTBOOLEANVEXTPROC G_glewGetInvariantBooleanvEXT;
GLEW_FUN_EXPORT PFNGLGETINVARIANTFLOATVEXTPROC G_glewGetInvariantFloatvEXT;
GLEW_FUN_EXPORT PFNGLGETINVARIANTINTEGERVEXTPROC G_glewGetInvariantIntegervEXT;
GLEW_FUN_EXPORT PFNGLGETLOCALCONSTANTBOOLEANVEXTPROC G_glewGetLocalConstantBooleanvEXT;
GLEW_FUN_EXPORT PFNGLGETLOCALCONSTANTFLOATVEXTPROC G_glewGetLocalConstantFloatvEXT;
GLEW_FUN_EXPORT PFNGLGETLOCALCONSTANTINTEGERVEXTPROC G_glewGetLocalConstantIntegervEXT;
GLEW_FUN_EXPORT PFNGLGETVARIANTBOOLEANVEXTPROC G_glewGetVariantBooleanvEXT;
GLEW_FUN_EXPORT PFNGLGETVARIANTFLOATVEXTPROC G_glewGetVariantFloatvEXT;
GLEW_FUN_EXPORT PFNGLGETVARIANTINTEGERVEXTPROC G_glewGetVariantIntegervEXT;
GLEW_FUN_EXPORT PFNGLGETVARIANTPOINTERVEXTPROC G_glewGetVariantPointervEXT;
GLEW_FUN_EXPORT PFNGLINSERTCOMPONENTEXTPROC G_glewInsertComponentEXT;
GLEW_FUN_EXPORT PFNGLISVARIANTENABLEDEXTPROC G_glewIsVariantEnabledEXT;
GLEW_FUN_EXPORT PFNGLSETINVARIANTEXTPROC G_glewSetInvariantEXT;
GLEW_FUN_EXPORT PFNGLSETLOCALCONSTANTEXTPROC G_glewSetLocalConstantEXT;
GLEW_FUN_EXPORT PFNGLSHADEROP1EXTPROC G_glewShaderOp1EXT;
GLEW_FUN_EXPORT PFNGLSHADEROP2EXTPROC G_glewShaderOp2EXT;
GLEW_FUN_EXPORT PFNGLSHADEROP3EXTPROC G_glewShaderOp3EXT;
GLEW_FUN_EXPORT PFNGLSWIZZLEEXTPROC G_glewSwizzleEXT;
GLEW_FUN_EXPORT PFNGLVARIANTPOINTEREXTPROC G_glewVariantPointerEXT;
GLEW_FUN_EXPORT PFNGLVARIANTBVEXTPROC G_glewVariantbvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTDVEXTPROC G_glewVariantdvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTFVEXTPROC G_glewVariantfvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTIVEXTPROC G_glewVariantivEXT;
GLEW_FUN_EXPORT PFNGLVARIANTSVEXTPROC G_glewVariantsvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTUBVEXTPROC G_glewVariantubvEXT;
GLEW_FUN_EXPORT PFNGLVARIANTUIVEXTPROC G_glewVariantuivEXT;
GLEW_FUN_EXPORT PFNGLVARIANTUSVEXTPROC G_glewVariantusvEXT;
GLEW_FUN_EXPORT PFNGLWRITEMASKEXTPROC G_glewWriteMaskEXT;

GLEW_FUN_EXPORT PFNGLVERTEXWEIGHTPOINTEREXTPROC G_glewVertexWeightPointerEXT;
GLEW_FUN_EXPORT PFNGLVERTEXWEIGHTFEXTPROC G_glewVertexWeightfEXT;
GLEW_FUN_EXPORT PFNGLVERTEXWEIGHTFVEXTPROC G_glewVertexWeightfvEXT;

GLEW_FUN_EXPORT PFNGLIMPORTSYNCEXTPROC G_glewImportSyncEXT;

GLEW_FUN_EXPORT PFNGLFRAMETERMINATORGREMEDYPROC G_glewFrameTerminatorGREMEDY;

GLEW_FUN_EXPORT PFNGLSTRINGMARKERGREMEDYPROC G_glewStringMarkerGREMEDY;

GLEW_FUN_EXPORT PFNGLGETIMAGETRANSFORMPARAMETERFVHPPROC G_glewGetImageTransformParameterfvHP;
GLEW_FUN_EXPORT PFNGLGETIMAGETRANSFORMPARAMETERIVHPPROC G_glewGetImageTransformParameterivHP;
GLEW_FUN_EXPORT PFNGLIMAGETRANSFORMPARAMETERFHPPROC G_glewImageTransformParameterfHP;
GLEW_FUN_EXPORT PFNGLIMAGETRANSFORMPARAMETERFVHPPROC G_glewImageTransformParameterfvHP;
GLEW_FUN_EXPORT PFNGLIMAGETRANSFORMPARAMETERIHPPROC G_glewImageTransformParameteriHP;
GLEW_FUN_EXPORT PFNGLIMAGETRANSFORMPARAMETERIVHPPROC G_glewImageTransformParameterivHP;

GLEW_FUN_EXPORT PFNGLMULTIMODEDRAWARRAYSIBMPROC G_glewMultiModeDrawArraysIBM;
GLEW_FUN_EXPORT PFNGLMULTIMODEDRAWELEMENTSIBMPROC G_glewMultiModeDrawElementsIBM;

GLEW_FUN_EXPORT PFNGLCOLORPOINTERLISTIBMPROC G_glewColorPointerListIBM;
GLEW_FUN_EXPORT PFNGLEDGEFLAGPOINTERLISTIBMPROC G_glewEdgeFlagPointerListIBM;
GLEW_FUN_EXPORT PFNGLFOGCOORDPOINTERLISTIBMPROC G_glewFogCoordPointerListIBM;
GLEW_FUN_EXPORT PFNGLINDEXPOINTERLISTIBMPROC G_glewIndexPointerListIBM;
GLEW_FUN_EXPORT PFNGLNORMALPOINTERLISTIBMPROC G_glewNormalPointerListIBM;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORPOINTERLISTIBMPROC G_glewSecondaryColorPointerListIBM;
GLEW_FUN_EXPORT PFNGLTEXCOORDPOINTERLISTIBMPROC G_glewTexCoordPointerListIBM;
GLEW_FUN_EXPORT PFNGLVERTEXPOINTERLISTIBMPROC G_glewVertexPointerListIBM;

GLEW_FUN_EXPORT PFNGLCOLORPOINTERVINTELPROC G_glewColorPointervINTEL;
GLEW_FUN_EXPORT PFNGLNORMALPOINTERVINTELPROC G_glewNormalPointervINTEL;
GLEW_FUN_EXPORT PFNGLTEXCOORDPOINTERVINTELPROC G_glewTexCoordPointervINTEL;
GLEW_FUN_EXPORT PFNGLVERTEXPOINTERVINTELPROC G_glewVertexPointervINTEL;

GLEW_FUN_EXPORT PFNGLTEXSCISSORFUNCINTELPROC G_glewTexScissorFuncINTEL;
GLEW_FUN_EXPORT PFNGLTEXSCISSORINTELPROC G_glewTexScissorINTEL;

GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECALLBACKPROC G_glewDebugMessageCallback;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGECONTROLPROC G_glewDebugMessageControl;
GLEW_FUN_EXPORT PFNGLDEBUGMESSAGEINSERTPROC G_glewDebugMessageInsert;
GLEW_FUN_EXPORT PFNGLGETDEBUGMESSAGELOGPROC G_glewGetDebugMessageLog;
GLEW_FUN_EXPORT PFNGLGETOBJECTLABELPROC G_glewGetObjectLabel;
GLEW_FUN_EXPORT PFNGLGETOBJECTPTRLABELPROC G_glewGetObjectPtrLabel;
GLEW_FUN_EXPORT PFNGLGETPOINTERVPROC G_glewGetPointerv;
GLEW_FUN_EXPORT PFNGLOBJECTLABELPROC G_glewObjectLabel;
GLEW_FUN_EXPORT PFNGLOBJECTPTRLABELPROC G_glewObjectPtrLabel;
GLEW_FUN_EXPORT PFNGLPUSHDEBUGGROUPPROC G_glewPushDebugGroup;

GLEW_FUN_EXPORT PFNGLBUFFERREGIONENABLEDPROC G_glewBufferRegionEnabled;
GLEW_FUN_EXPORT PFNGLDELETEBUFFERREGIONPROC G_glewDeleteBufferRegion;
GLEW_FUN_EXPORT PFNGLDRAWBUFFERREGIONPROC G_glewDrawBufferRegion;
GLEW_FUN_EXPORT PFNGLNEWBUFFERREGIONPROC G_glewNewBufferRegion;
GLEW_FUN_EXPORT PFNGLREADBUFFERREGIONPROC G_glewReadBufferRegion;

GLEW_FUN_EXPORT PFNGLRESIZEBUFFERSMESAPROC G_glewResizeBuffersMESA;

GLEW_FUN_EXPORT PFNGLWINDOWPOS2DMESAPROC G_glewWindowPos2dMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2DVMESAPROC G_glewWindowPos2dvMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FMESAPROC G_glewWindowPos2fMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2FVMESAPROC G_glewWindowPos2fvMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IMESAPROC G_glewWindowPos2iMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2IVMESAPROC G_glewWindowPos2ivMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SMESAPROC G_glewWindowPos2sMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS2SVMESAPROC G_glewWindowPos2svMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DMESAPROC G_glewWindowPos3dMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3DVMESAPROC G_glewWindowPos3dvMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FMESAPROC G_glewWindowPos3fMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3FVMESAPROC G_glewWindowPos3fvMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IMESAPROC G_glewWindowPos3iMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3IVMESAPROC G_glewWindowPos3ivMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SMESAPROC G_glewWindowPos3sMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS3SVMESAPROC G_glewWindowPos3svMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS4DMESAPROC G_glewWindowPos4dMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS4DVMESAPROC G_glewWindowPos4dvMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS4FMESAPROC G_glewWindowPos4fMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS4FVMESAPROC G_glewWindowPos4fvMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS4IMESAPROC G_glewWindowPos4iMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS4IVMESAPROC G_glewWindowPos4ivMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS4SMESAPROC G_glewWindowPos4sMESA;
GLEW_FUN_EXPORT PFNGLWINDOWPOS4SVMESAPROC G_glewWindowPos4svMESA;

GLEW_FUN_EXPORT PFNGLGETIMAGEHANDLENVPROC G_glewGetImageHandleNV;
GLEW_FUN_EXPORT PFNGLGETTEXTUREHANDLENVPROC G_glewGetTextureHandleNV;
GLEW_FUN_EXPORT PFNGLGETTEXTURESAMPLERHANDLENVPROC G_glewGetTextureSamplerHandleNV;
GLEW_FUN_EXPORT PFNGLISIMAGEHANDLERESIDENTNVPROC G_glewIsImageHandleResidentNV;
GLEW_FUN_EXPORT PFNGLISTEXTUREHANDLERESIDENTNVPROC G_glewIsTextureHandleResidentNV;
GLEW_FUN_EXPORT PFNGLMAKEIMAGEHANDLENONRESIDENTNVPROC G_glewMakeImageHandleNonResidentNV;
GLEW_FUN_EXPORT PFNGLMAKEIMAGEHANDLERESIDENTNVPROC G_glewMakeImageHandleResidentNV;
GLEW_FUN_EXPORT PFNGLMAKETEXTUREHANDLENONRESIDENTNVPROC G_glewMakeTextureHandleNonResidentNV;
GLEW_FUN_EXPORT PFNGLMAKETEXTUREHANDLERESIDENTNVPROC G_glewMakeTextureHandleResidentNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMHANDLEUI64NVPROC G_glewProgramUniformHandleui64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMHANDLEUI64VNVPROC G_glewProgramUniformHandleui64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORMHANDLEUI64NVPROC G_glewUniformHandleui64NV;
GLEW_FUN_EXPORT PFNGLUNIFORMHANDLEUI64VNVPROC G_glewUniformHandleui64vNV;

GLEW_FUN_EXPORT PFNGLBEGINCONDITIONALRENDERNVPROC G_glewBeginConditionalRenderNV;
GLEW_FUN_EXPORT PFNGLENDCONDITIONALRENDERNVPROC G_glewEndConditionalRenderNV;

GLEW_FUN_EXPORT PFNGLCOPYIMAGESUBDATANVPROC G_glewCopyImageSubDataNV;

GLEW_FUN_EXPORT PFNGLCLEARDEPTHDNVPROC G_glewClearDepthdNV;
GLEW_FUN_EXPORT PFNGLDEPTHBOUNDSDNVPROC G_glewDepthBoundsdNV;
GLEW_FUN_EXPORT PFNGLDEPTHRANGEDNVPROC G_glewDepthRangedNV;

GLEW_FUN_EXPORT PFNGLEVALMAPSNVPROC G_glewEvalMapsNV;
GLEW_FUN_EXPORT PFNGLGETMAPATTRIBPARAMETERFVNVPROC G_glewGetMapAttribParameterfvNV;
GLEW_FUN_EXPORT PFNGLGETMAPATTRIBPARAMETERIVNVPROC G_glewGetMapAttribParameterivNV;
GLEW_FUN_EXPORT PFNGLGETMAPCONTROLPOINTSNVPROC G_glewGetMapControlPointsNV;
GLEW_FUN_EXPORT PFNGLGETMAPPARAMETERFVNVPROC G_glewGetMapParameterfvNV;
GLEW_FUN_EXPORT PFNGLGETMAPPARAMETERIVNVPROC G_glewGetMapParameterivNV;
GLEW_FUN_EXPORT PFNGLMAPCONTROLPOINTSNVPROC G_glewMapControlPointsNV;
GLEW_FUN_EXPORT PFNGLMAPPARAMETERFVNVPROC G_glewMapParameterfvNV;
GLEW_FUN_EXPORT PFNGLMAPPARAMETERIVNVPROC G_glewMapParameterivNV;

GLEW_FUN_EXPORT PFNGLGETMULTISAMPLEFVNVPROC G_glewGetMultisamplefvNV;
GLEW_FUN_EXPORT PFNGLSAMPLEMASKINDEXEDNVPROC G_glewSampleMaskIndexedNV;
GLEW_FUN_EXPORT PFNGLTEXRENDERBUFFERNVPROC G_glewTexRenderbufferNV;

GLEW_FUN_EXPORT PFNGLDELETEFENCESNVPROC G_glewDeleteFencesNV;
GLEW_FUN_EXPORT PFNGLFINISHFENCENVPROC G_glewFinishFenceNV;
GLEW_FUN_EXPORT PFNGLGENFENCESNVPROC G_glewGenFencesNV;
GLEW_FUN_EXPORT PFNGLGETFENCEIVNVPROC G_glewGetFenceivNV;
GLEW_FUN_EXPORT PFNGLISFENCENVPROC G_glewIsFenceNV;
GLEW_FUN_EXPORT PFNGLSETFENCENVPROC G_glewSetFenceNV;
GLEW_FUN_EXPORT PFNGLTESTFENCENVPROC G_glewTestFenceNV;

GLEW_FUN_EXPORT PFNGLGETPROGRAMNAMEDPARAMETERDVNVPROC G_glewGetProgramNamedParameterdvNV;
GLEW_FUN_EXPORT PFNGLGETPROGRAMNAMEDPARAMETERFVNVPROC G_glewGetProgramNamedParameterfvNV;
GLEW_FUN_EXPORT PFNGLPROGRAMNAMEDPARAMETER4DNVPROC G_glewProgramNamedParameter4dNV;
GLEW_FUN_EXPORT PFNGLPROGRAMNAMEDPARAMETER4DVNVPROC G_glewProgramNamedParameter4dvNV;
GLEW_FUN_EXPORT PFNGLPROGRAMNAMEDPARAMETER4FNVPROC G_glewProgramNamedParameter4fNV;
GLEW_FUN_EXPORT PFNGLPROGRAMNAMEDPARAMETER4FVNVPROC G_glewProgramNamedParameter4fvNV;

GLEW_FUN_EXPORT PFNGLRENDERBUFFERSTORAGEMULTISAMPLECOVERAGENVPROC G_glewRenderbufferStorageMultisampleCoverageNV;

GLEW_FUN_EXPORT PFNGLPROGRAMVERTEXLIMITNVPROC G_glewProgramVertexLimitNV;

GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETERI4INVPROC G_glewProgramEnvParameterI4iNV;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETERI4IVNVPROC G_glewProgramEnvParameterI4ivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETERI4UINVPROC G_glewProgramEnvParameterI4uiNV;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETERI4UIVNVPROC G_glewProgramEnvParameterI4uivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETERSI4IVNVPROC G_glewProgramEnvParametersI4ivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMENVPARAMETERSI4UIVNVPROC G_glewProgramEnvParametersI4uivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETERI4INVPROC G_glewProgramLocalParameterI4iNV;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETERI4IVNVPROC G_glewProgramLocalParameterI4ivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETERI4UINVPROC G_glewProgramLocalParameterI4uiNV;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETERI4UIVNVPROC G_glewProgramLocalParameterI4uivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETERSI4IVNVPROC G_glewProgramLocalParametersI4ivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMLOCALPARAMETERSI4UIVNVPROC G_glewProgramLocalParametersI4uivNV;

GLEW_FUN_EXPORT PFNGLGETUNIFORMI64VNVPROC G_glewGetUniformi64vNV;
GLEW_FUN_EXPORT PFNGLGETUNIFORMUI64VNVPROC G_glewGetUniformui64vNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1I64NVPROC G_glewProgramUniform1i64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1I64VNVPROC G_glewProgramUniform1i64vNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UI64NVPROC G_glewProgramUniform1ui64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM1UI64VNVPROC G_glewProgramUniform1ui64vNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2I64NVPROC G_glewProgramUniform2i64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2I64VNVPROC G_glewProgramUniform2i64vNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UI64NVPROC G_glewProgramUniform2ui64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM2UI64VNVPROC G_glewProgramUniform2ui64vNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3I64NVPROC G_glewProgramUniform3i64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3I64VNVPROC G_glewProgramUniform3i64vNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UI64NVPROC G_glewProgramUniform3ui64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM3UI64VNVPROC G_glewProgramUniform3ui64vNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4I64NVPROC G_glewProgramUniform4i64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4I64VNVPROC G_glewProgramUniform4i64vNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UI64NVPROC G_glewProgramUniform4ui64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORM4UI64VNVPROC G_glewProgramUniform4ui64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORM1I64NVPROC G_glewUniform1i64NV;
GLEW_FUN_EXPORT PFNGLUNIFORM1I64VNVPROC G_glewUniform1i64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORM1UI64NVPROC G_glewUniform1ui64NV;
GLEW_FUN_EXPORT PFNGLUNIFORM1UI64VNVPROC G_glewUniform1ui64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORM2I64NVPROC G_glewUniform2i64NV;
GLEW_FUN_EXPORT PFNGLUNIFORM2I64VNVPROC G_glewUniform2i64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORM2UI64NVPROC G_glewUniform2ui64NV;
GLEW_FUN_EXPORT PFNGLUNIFORM2UI64VNVPROC G_glewUniform2ui64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORM3I64NVPROC G_glewUniform3i64NV;
GLEW_FUN_EXPORT PFNGLUNIFORM3I64VNVPROC G_glewUniform3i64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORM3UI64NVPROC G_glewUniform3ui64NV;
GLEW_FUN_EXPORT PFNGLUNIFORM3UI64VNVPROC G_glewUniform3ui64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORM4I64NVPROC G_glewUniform4i64NV;
GLEW_FUN_EXPORT PFNGLUNIFORM4I64VNVPROC G_glewUniform4i64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORM4UI64NVPROC G_glewUniform4ui64NV;
GLEW_FUN_EXPORT PFNGLUNIFORM4UI64VNVPROC G_glewUniform4ui64vNV;

GLEW_FUN_EXPORT PFNGLCOLOR3HNVPROC G_glewColor3hNV;
GLEW_FUN_EXPORT PFNGLCOLOR3HVNVPROC G_glewColor3hvNV;
GLEW_FUN_EXPORT PFNGLCOLOR4HNVPROC G_glewColor4hNV;
GLEW_FUN_EXPORT PFNGLCOLOR4HVNVPROC G_glewColor4hvNV;
GLEW_FUN_EXPORT PFNGLFOGCOORDHNVPROC G_glewFogCoordhNV;
GLEW_FUN_EXPORT PFNGLFOGCOORDHVNVPROC G_glewFogCoordhvNV;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1HNVPROC G_glewMultiTexCoord1hNV;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD1HVNVPROC G_glewMultiTexCoord1hvNV;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2HNVPROC G_glewMultiTexCoord2hNV;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD2HVNVPROC G_glewMultiTexCoord2hvNV;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3HNVPROC G_glewMultiTexCoord3hNV;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD3HVNVPROC G_glewMultiTexCoord3hvNV;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4HNVPROC G_glewMultiTexCoord4hNV;
GLEW_FUN_EXPORT PFNGLMULTITEXCOORD4HVNVPROC G_glewMultiTexCoord4hvNV;
GLEW_FUN_EXPORT PFNGLNORMAL3HNVPROC G_glewNormal3hNV;
GLEW_FUN_EXPORT PFNGLNORMAL3HVNVPROC G_glewNormal3hvNV;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3HNVPROC G_glewSecondaryColor3hNV;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLOR3HVNVPROC G_glewSecondaryColor3hvNV;
GLEW_FUN_EXPORT PFNGLTEXCOORD1HNVPROC G_glewTexCoord1hNV;
GLEW_FUN_EXPORT PFNGLTEXCOORD1HVNVPROC G_glewTexCoord1hvNV;
GLEW_FUN_EXPORT PFNGLTEXCOORD2HNVPROC G_glewTexCoord2hNV;
GLEW_FUN_EXPORT PFNGLTEXCOORD2HVNVPROC G_glewTexCoord2hvNV;
GLEW_FUN_EXPORT PFNGLTEXCOORD3HNVPROC G_glewTexCoord3hNV;
GLEW_FUN_EXPORT PFNGLTEXCOORD3HVNVPROC G_glewTexCoord3hvNV;
GLEW_FUN_EXPORT PFNGLTEXCOORD4HNVPROC G_glewTexCoord4hNV;
GLEW_FUN_EXPORT PFNGLTEXCOORD4HVNVPROC G_glewTexCoord4hvNV;
GLEW_FUN_EXPORT PFNGLVERTEX2HNVPROC G_glewVertex2hNV;
GLEW_FUN_EXPORT PFNGLVERTEX2HVNVPROC G_glewVertex2hvNV;
GLEW_FUN_EXPORT PFNGLVERTEX3HNVPROC G_glewVertex3hNV;
GLEW_FUN_EXPORT PFNGLVERTEX3HVNVPROC G_glewVertex3hvNV;
GLEW_FUN_EXPORT PFNGLVERTEX4HNVPROC G_glewVertex4hNV;
GLEW_FUN_EXPORT PFNGLVERTEX4HVNVPROC G_glewVertex4hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1HNVPROC G_glewVertexAttrib1hNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1HVNVPROC G_glewVertexAttrib1hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2HNVPROC G_glewVertexAttrib2hNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2HVNVPROC G_glewVertexAttrib2hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3HNVPROC G_glewVertexAttrib3hNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3HVNVPROC G_glewVertexAttrib3hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4HNVPROC G_glewVertexAttrib4hNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4HVNVPROC G_glewVertexAttrib4hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS1HVNVPROC G_glewVertexAttribs1hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS2HVNVPROC G_glewVertexAttribs2hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS3HVNVPROC G_glewVertexAttribs3hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS4HVNVPROC G_glewVertexAttribs4hvNV;
GLEW_FUN_EXPORT PFNGLVERTEXWEIGHTHNVPROC G_glewVertexWeighthNV;
GLEW_FUN_EXPORT PFNGLVERTEXWEIGHTHVNVPROC G_glewVertexWeighthvNV;

GLEW_FUN_EXPORT PFNGLBEGINOCCLUSIONQUERYNVPROC G_glewBeginOcclusionQueryNV;
GLEW_FUN_EXPORT PFNGLDELETEOCCLUSIONQUERIESNVPROC G_glewDeleteOcclusionQueriesNV;
GLEW_FUN_EXPORT PFNGLENDOCCLUSIONQUERYNVPROC G_glewEndOcclusionQueryNV;
GLEW_FUN_EXPORT PFNGLGENOCCLUSIONQUERIESNVPROC G_glewGenOcclusionQueriesNV;
GLEW_FUN_EXPORT PFNGLGETOCCLUSIONQUERYIVNVPROC G_glewGetOcclusionQueryivNV;
GLEW_FUN_EXPORT PFNGLGETOCCLUSIONQUERYUIVNVPROC G_glewGetOcclusionQueryuivNV;
GLEW_FUN_EXPORT PFNGLISOCCLUSIONQUERYNVPROC G_glewIsOcclusionQueryNV;

GLEW_FUN_EXPORT PFNGLPROGRAMBUFFERPARAMETERSIIVNVPROC G_glewProgramBufferParametersIivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMBUFFERPARAMETERSIUIVNVPROC G_glewProgramBufferParametersIuivNV;
GLEW_FUN_EXPORT PFNGLPROGRAMBUFFERPARAMETERSFVNVPROC G_glewProgramBufferParametersfvNV;

GLEW_FUN_EXPORT PFNGLCOPYPATHNVPROC G_glewCopyPathNV;
GLEW_FUN_EXPORT PFNGLCOVERFILLPATHINSTANCEDNVPROC G_glewCoverFillPathInstancedNV;
GLEW_FUN_EXPORT PFNGLCOVERFILLPATHNVPROC G_glewCoverFillPathNV;
GLEW_FUN_EXPORT PFNGLCOVERSTROKEPATHINSTANCEDNVPROC G_glewCoverStrokePathInstancedNV;
GLEW_FUN_EXPORT PFNGLCOVERSTROKEPATHNVPROC G_glewCoverStrokePathNV;
GLEW_FUN_EXPORT PFNGLDELETEPATHSNVPROC G_glewDeletePathsNV;
GLEW_FUN_EXPORT PFNGLGENPATHSNVPROC G_glewGenPathsNV;
GLEW_FUN_EXPORT PFNGLGETPATHCOLORGENFVNVPROC G_glewGetPathColorGenfvNV;
GLEW_FUN_EXPORT PFNGLGETPATHCOLORGENIVNVPROC G_glewGetPathColorGenivNV;
GLEW_FUN_EXPORT PFNGLGETPATHCOMMANDSNVPROC G_glewGetPathCommandsNV;
GLEW_FUN_EXPORT PFNGLGETPATHCOORDSNVPROC G_glewGetPathCoordsNV;
GLEW_FUN_EXPORT PFNGLGETPATHDASHARRAYNVPROC G_glewGetPathDashArrayNV;
GLEW_FUN_EXPORT PFNGLGETPATHLENGTHNVPROC G_glewGetPathLengthNV;
GLEW_FUN_EXPORT PFNGLGETPATHMETRICRANGENVPROC G_glewGetPathMetricRangeNV;
GLEW_FUN_EXPORT PFNGLGETPATHMETRICSNVPROC G_glewGetPathMetricsNV;
GLEW_FUN_EXPORT PFNGLGETPATHPARAMETERFVNVPROC G_glewGetPathParameterfvNV;
GLEW_FUN_EXPORT PFNGLGETPATHPARAMETERIVNVPROC G_glewGetPathParameterivNV;
GLEW_FUN_EXPORT PFNGLGETPATHSPACINGNVPROC G_glewGetPathSpacingNV;
GLEW_FUN_EXPORT PFNGLGETPATHTEXGENFVNVPROC G_glewGetPathTexGenfvNV;
GLEW_FUN_EXPORT PFNGLGETPATHTEXGENIVNVPROC G_glewGetPathTexGenivNV;
GLEW_FUN_EXPORT PFNGLINTERPOLATEPATHSNVPROC G_glewInterpolatePathsNV;
GLEW_FUN_EXPORT PFNGLISPATHNVPROC G_glewIsPathNV;
GLEW_FUN_EXPORT PFNGLISPOINTINFILLPATHNVPROC G_glewIsPointInFillPathNV;
GLEW_FUN_EXPORT PFNGLISPOINTINSTROKEPATHNVPROC G_glewIsPointInStrokePathNV;
GLEW_FUN_EXPORT PFNGLPATHCOLORGENNVPROC G_glewPathColorGenNV;
GLEW_FUN_EXPORT PFNGLPATHCOMMANDSNVPROC G_glewPathCommandsNV;
GLEW_FUN_EXPORT PFNGLPATHCOORDSNVPROC G_glewPathCoordsNV;
GLEW_FUN_EXPORT PFNGLPATHCOVERDEPTHFUNCNVPROC G_glewPathCoverDepthFuncNV;
GLEW_FUN_EXPORT PFNGLPATHDASHARRAYNVPROC G_glewPathDashArrayNV;
GLEW_FUN_EXPORT PFNGLPATHFOGGENNVPROC G_glewPathFogGenNV;
GLEW_FUN_EXPORT PFNGLPATHGLYPHRANGENVPROC G_glewPathGlyphRangeNV;
GLEW_FUN_EXPORT PFNGLPATHGLYPHSNVPROC G_glewPathGlyphsNV;
GLEW_FUN_EXPORT PFNGLPATHPARAMETERFNVPROC G_glewPathParameterfNV;
GLEW_FUN_EXPORT PFNGLPATHPARAMETERFVNVPROC G_glewPathParameterfvNV;
GLEW_FUN_EXPORT PFNGLPATHPARAMETERINVPROC G_glewPathParameteriNV;
GLEW_FUN_EXPORT PFNGLPATHPARAMETERIVNVPROC G_glewPathParameterivNV;
GLEW_FUN_EXPORT PFNGLPATHSTENCILDEPTHOFFSETNVPROC G_glewPathStencilDepthOffsetNV;
GLEW_FUN_EXPORT PFNGLPATHSTENCILFUNCNVPROC G_glewPathStencilFuncNV;
GLEW_FUN_EXPORT PFNGLPATHSTRINGNVPROC G_glewPathStringNV;
GLEW_FUN_EXPORT PFNGLPATHSUBCOMMANDSNVPROC G_glewPathSubCommandsNV;
GLEW_FUN_EXPORT PFNGLPATHSUBCOORDSNVPROC G_glewPathSubCoordsNV;
GLEW_FUN_EXPORT PFNGLPATHTEXGENNVPROC G_glewPathTexGenNV;
GLEW_FUN_EXPORT PFNGLPOINTALONGPATHNVPROC G_glewPointAlongPathNV;
GLEW_FUN_EXPORT PFNGLSTENCILFILLPATHINSTANCEDNVPROC G_glewStencilFillPathInstancedNV;
GLEW_FUN_EXPORT PFNGLSTENCILFILLPATHNVPROC G_glewStencilFillPathNV;
GLEW_FUN_EXPORT PFNGLSTENCILSTROKEPATHINSTANCEDNVPROC G_glewStencilStrokePathInstancedNV;
GLEW_FUN_EXPORT PFNGLSTENCILSTROKEPATHNVPROC G_glewStencilStrokePathNV;
GLEW_FUN_EXPORT PFNGLTRANSFORMPATHNVPROC G_glewTransformPathNV;
GLEW_FUN_EXPORT PFNGLWEIGHTPATHSNVPROC G_glewWeightPathsNV;

GLEW_FUN_EXPORT PFNGLFLUSHPIXELDATARANGENVPROC G_glewFlushPixelDataRangeNV;
GLEW_FUN_EXPORT PFNGLPIXELDATARANGENVPROC G_glewPixelDataRangeNV;

GLEW_FUN_EXPORT PFNGLPOINTPARAMETERINVPROC G_glewPointParameteriNV;
GLEW_FUN_EXPORT PFNGLPOINTPARAMETERIVNVPROC G_glewPointParameterivNV;

GLEW_FUN_EXPORT PFNGLGETVIDEOI64VNVPROC G_glewGetVideoi64vNV;
GLEW_FUN_EXPORT PFNGLGETVIDEOIVNVPROC G_glewGetVideoivNV;
GLEW_FUN_EXPORT PFNGLGETVIDEOUI64VNVPROC G_glewGetVideoui64vNV;
GLEW_FUN_EXPORT PFNGLGETVIDEOUIVNVPROC G_glewGetVideouivNV;
GLEW_FUN_EXPORT PFNGLPRESENTFRAMEDUALFILLNVPROC G_glewPresentFrameDualFillNV;
GLEW_FUN_EXPORT PFNGLPRESENTFRAMEKEYEDNVPROC G_glewPresentFrameKeyedNV;

GLEW_FUN_EXPORT PFNGLPRIMITIVERESTARTINDEXNVPROC G_glewPrimitiveRestartIndexNV;
GLEW_FUN_EXPORT PFNGLPRIMITIVERESTARTNVPROC G_glewPrimitiveRestartNV;

GLEW_FUN_EXPORT PFNGLCOMBINERINPUTNVPROC G_glewCombinerInputNV;
GLEW_FUN_EXPORT PFNGLCOMBINEROUTPUTNVPROC G_glewCombinerOutputNV;
GLEW_FUN_EXPORT PFNGLCOMBINERPARAMETERFNVPROC G_glewCombinerParameterfNV;
GLEW_FUN_EXPORT PFNGLCOMBINERPARAMETERFVNVPROC G_glewCombinerParameterfvNV;
GLEW_FUN_EXPORT PFNGLCOMBINERPARAMETERINVPROC G_glewCombinerParameteriNV;
GLEW_FUN_EXPORT PFNGLCOMBINERPARAMETERIVNVPROC G_glewCombinerParameterivNV;
GLEW_FUN_EXPORT PFNGLFINALCOMBINERINPUTNVPROC G_glewFinalCombinerInputNV;
GLEW_FUN_EXPORT PFNGLGETCOMBINERINPUTPARAMETERFVNVPROC G_glewGetCombinerInputParameterfvNV;
GLEW_FUN_EXPORT PFNGLGETCOMBINERINPUTPARAMETERIVNVPROC G_glewGetCombinerInputParameterivNV;
GLEW_FUN_EXPORT PFNGLGETCOMBINEROUTPUTPARAMETERFVNVPROC G_glewGetCombinerOutputParameterfvNV;
GLEW_FUN_EXPORT PFNGLGETCOMBINEROUTPUTPARAMETERIVNVPROC G_glewGetCombinerOutputParameterivNV;
GLEW_FUN_EXPORT PFNGLGETFINALCOMBINERINPUTPARAMETERFVNVPROC G_glewGetFinalCombinerInputParameterfvNV;
GLEW_FUN_EXPORT PFNGLGETFINALCOMBINERINPUTPARAMETERIVNVPROC G_glewGetFinalCombinerInputParameterivNV;

GLEW_FUN_EXPORT PFNGLCOMBINERSTAGEPARAMETERFVNVPROC G_glewCombinerStageParameterfvNV;
GLEW_FUN_EXPORT PFNGLGETCOMBINERSTAGEPARAMETERFVNVPROC G_glewGetCombinerStageParameterfvNV;

GLEW_FUN_EXPORT PFNGLGETBUFFERPARAMETERUI64VNVPROC G_glewGetBufferParameterui64vNV;
GLEW_FUN_EXPORT PFNGLGETINTEGERUI64VNVPROC G_glewGetIntegerui64vNV;
GLEW_FUN_EXPORT PFNGLGETNAMEDBUFFERPARAMETERUI64VNVPROC G_glewGetNamedBufferParameterui64vNV;
GLEW_FUN_EXPORT PFNGLISBUFFERRESIDENTNVPROC G_glewIsBufferResidentNV;
GLEW_FUN_EXPORT PFNGLISNAMEDBUFFERRESIDENTNVPROC G_glewIsNamedBufferResidentNV;
GLEW_FUN_EXPORT PFNGLMAKEBUFFERNONRESIDENTNVPROC G_glewMakeBufferNonResidentNV;
GLEW_FUN_EXPORT PFNGLMAKEBUFFERRESIDENTNVPROC G_glewMakeBufferResidentNV;
GLEW_FUN_EXPORT PFNGLMAKENAMEDBUFFERNONRESIDENTNVPROC G_glewMakeNamedBufferNonResidentNV;
GLEW_FUN_EXPORT PFNGLMAKENAMEDBUFFERRESIDENTNVPROC G_glewMakeNamedBufferResidentNV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMUI64NVPROC G_glewProgramUniformui64NV;
GLEW_FUN_EXPORT PFNGLPROGRAMUNIFORMUI64VNVPROC G_glewProgramUniformui64vNV;
GLEW_FUN_EXPORT PFNGLUNIFORMUI64NVPROC G_glewUniformui64NV;
GLEW_FUN_EXPORT PFNGLUNIFORMUI64VNVPROC G_glewUniformui64vNV;

GLEW_FUN_EXPORT PFNGLTEXTUREBARRIERNVPROC G_glewTextureBarrierNV;

GLEW_FUN_EXPORT PFNGLTEXIMAGE2DMULTISAMPLECOVERAGENVPROC G_glewTexImage2DMultisampleCoverageNV;
GLEW_FUN_EXPORT PFNGLTEXIMAGE3DMULTISAMPLECOVERAGENVPROC G_glewTexImage3DMultisampleCoverageNV;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE2DMULTISAMPLECOVERAGENVPROC G_glewTextureImage2DMultisampleCoverageNV;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE2DMULTISAMPLENVPROC G_glewTextureImage2DMultisampleNV;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE3DMULTISAMPLECOVERAGENVPROC G_glewTextureImage3DMultisampleCoverageNV;
GLEW_FUN_EXPORT PFNGLTEXTUREIMAGE3DMULTISAMPLENVPROC G_glewTextureImage3DMultisampleNV;

GLEW_FUN_EXPORT PFNGLACTIVEVARYINGNVPROC G_glewActiveVaryingNV;
GLEW_FUN_EXPORT PFNGLBEGINTRANSFORMFEEDBACKNVPROC G_glewBeginTransformFeedbackNV;
GLEW_FUN_EXPORT PFNGLBINDBUFFERBASENVPROC G_glewBindBufferBaseNV;
GLEW_FUN_EXPORT PFNGLBINDBUFFEROFFSETNVPROC G_glewBindBufferOffsetNV;
GLEW_FUN_EXPORT PFNGLBINDBUFFERRANGENVPROC G_glewBindBufferRangeNV;
GLEW_FUN_EXPORT PFNGLENDTRANSFORMFEEDBACKNVPROC G_glewEndTransformFeedbackNV;
GLEW_FUN_EXPORT PFNGLGETACTIVEVARYINGNVPROC G_glewGetActiveVaryingNV;
GLEW_FUN_EXPORT PFNGLGETTRANSFORMFEEDBACKVARYINGNVPROC G_glewGetTransformFeedbackVaryingNV;
GLEW_FUN_EXPORT PFNGLGETVARYINGLOCATIONNVPROC G_glewGetVaryingLocationNV;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKATTRIBSNVPROC G_glewTransformFeedbackAttribsNV;
GLEW_FUN_EXPORT PFNGLTRANSFORMFEEDBACKVARYINGSNVPROC G_glewTransformFeedbackVaryingsNV;

GLEW_FUN_EXPORT PFNGLBINDTRANSFORMFEEDBACKNVPROC G_glewBindTransformFeedbackNV;
GLEW_FUN_EXPORT PFNGLDELETETRANSFORMFEEDBACKSNVPROC G_glewDeleteTransformFeedbacksNV;
GLEW_FUN_EXPORT PFNGLDRAWTRANSFORMFEEDBACKNVPROC G_glewDrawTransformFeedbackNV;
GLEW_FUN_EXPORT PFNGLGENTRANSFORMFEEDBACKSNVPROC G_glewGenTransformFeedbacksNV;
GLEW_FUN_EXPORT PFNGLISTRANSFORMFEEDBACKNVPROC G_glewIsTransformFeedbackNV;
GLEW_FUN_EXPORT PFNGLPAUSETRANSFORMFEEDBACKNVPROC G_glewPauseTransformFeedbackNV;
GLEW_FUN_EXPORT PFNGLRESUMETRANSFORMFEEDBACKNVPROC G_glewResumeTransformFeedbackNV;

GLEW_FUN_EXPORT PFNGLVDPAUFININVPROC G_glewVDPAUFiniNV;
GLEW_FUN_EXPORT PFNGLVDPAUGETSURFACEIVNVPROC G_glewVDPAUGetSurfaceivNV;
GLEW_FUN_EXPORT PFNGLVDPAUINITNVPROC G_glewVDPAUInitNV;
GLEW_FUN_EXPORT PFNGLVDPAUISSURFACENVPROC G_glewVDPAUIsSurfaceNV;
GLEW_FUN_EXPORT PFNGLVDPAUMAPSURFACESNVPROC G_glewVDPAUMapSurfacesNV;
GLEW_FUN_EXPORT PFNGLVDPAUREGISTEROUTPUTSURFACENVPROC G_glewVDPAURegisterOutputSurfaceNV;
GLEW_FUN_EXPORT PFNGLVDPAUREGISTERVIDEOSURFACENVPROC G_glewVDPAURegisterVideoSurfaceNV;
GLEW_FUN_EXPORT PFNGLVDPAUSURFACEACCESSNVPROC G_glewVDPAUSurfaceAccessNV;
GLEW_FUN_EXPORT PFNGLVDPAUUNMAPSURFACESNVPROC G_glewVDPAUUnmapSurfacesNV;
GLEW_FUN_EXPORT PFNGLVDPAUUNREGISTERSURFACENVPROC G_glewVDPAUUnregisterSurfaceNV;

GLEW_FUN_EXPORT PFNGLFLUSHVERTEXARRAYRANGENVPROC G_glewFlushVertexArrayRangeNV;
GLEW_FUN_EXPORT PFNGLVERTEXARRAYRANGENVPROC G_glewVertexArrayRangeNV;

GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBLI64VNVPROC G_glewGetVertexAttribLi64vNV;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBLUI64VNVPROC G_glewGetVertexAttribLui64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1I64NVPROC G_glewVertexAttribL1i64NV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1I64VNVPROC G_glewVertexAttribL1i64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1UI64NVPROC G_glewVertexAttribL1ui64NV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL1UI64VNVPROC G_glewVertexAttribL1ui64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2I64NVPROC G_glewVertexAttribL2i64NV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2I64VNVPROC G_glewVertexAttribL2i64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2UI64NVPROC G_glewVertexAttribL2ui64NV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL2UI64VNVPROC G_glewVertexAttribL2ui64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3I64NVPROC G_glewVertexAttribL3i64NV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3I64VNVPROC G_glewVertexAttribL3i64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3UI64NVPROC G_glewVertexAttribL3ui64NV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL3UI64VNVPROC G_glewVertexAttribL3ui64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4I64NVPROC G_glewVertexAttribL4i64NV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4I64VNVPROC G_glewVertexAttribL4i64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4UI64NVPROC G_glewVertexAttribL4ui64NV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBL4UI64VNVPROC G_glewVertexAttribL4ui64vNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBLFORMATNVPROC G_glewVertexAttribLFormatNV;

GLEW_FUN_EXPORT PFNGLBUFFERADDRESSRANGENVPROC G_glewBufferAddressRangeNV;
GLEW_FUN_EXPORT PFNGLCOLORFORMATNVPROC G_glewColorFormatNV;
GLEW_FUN_EXPORT PFNGLEDGEFLAGFORMATNVPROC G_glewEdgeFlagFormatNV;
GLEW_FUN_EXPORT PFNGLFOGCOORDFORMATNVPROC G_glewFogCoordFormatNV;
GLEW_FUN_EXPORT PFNGLGETINTEGERUI64I_VNVPROC G_glewGetIntegerui64i_vNV;
GLEW_FUN_EXPORT PFNGLINDEXFORMATNVPROC G_glewIndexFormatNV;
GLEW_FUN_EXPORT PFNGLNORMALFORMATNVPROC G_glewNormalFormatNV;
GLEW_FUN_EXPORT PFNGLSECONDARYCOLORFORMATNVPROC G_glewSecondaryColorFormatNV;
GLEW_FUN_EXPORT PFNGLTEXCOORDFORMATNVPROC G_glewTexCoordFormatNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBFORMATNVPROC G_glewVertexAttribFormatNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBIFORMATNVPROC G_glewVertexAttribIFormatNV;
GLEW_FUN_EXPORT PFNGLVERTEXFORMATNVPROC G_glewVertexFormatNV;

GLEW_FUN_EXPORT PFNGLAREPROGRAMSRESIDENTNVPROC G_glewAreProgramsResidentNV;
GLEW_FUN_EXPORT PFNGLBINDPROGRAMNVPROC G_glewBindProgramNV;
GLEW_FUN_EXPORT PFNGLDELETEPROGRAMSNVPROC G_glewDeleteProgramsNV;
GLEW_FUN_EXPORT PFNGLEXECUTEPROGRAMNVPROC G_glewExecuteProgramNV;
GLEW_FUN_EXPORT PFNGLGENPROGRAMSNVPROC G_glewGenProgramsNV;
GLEW_FUN_EXPORT PFNGLGETPROGRAMPARAMETERDVNVPROC G_glewGetProgramParameterdvNV;
GLEW_FUN_EXPORT PFNGLGETPROGRAMPARAMETERFVNVPROC G_glewGetProgramParameterfvNV;
GLEW_FUN_EXPORT PFNGLGETPROGRAMSTRINGNVPROC G_glewGetProgramStringNV;
GLEW_FUN_EXPORT PFNGLGETPROGRAMIVNVPROC G_glewGetProgramivNV;
GLEW_FUN_EXPORT PFNGLGETTRACKMATRIXIVNVPROC G_glewGetTrackMatrixivNV;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBPOINTERVNVPROC G_glewGetVertexAttribPointervNV;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBDVNVPROC G_glewGetVertexAttribdvNV;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBFVNVPROC G_glewGetVertexAttribfvNV;
GLEW_FUN_EXPORT PFNGLGETVERTEXATTRIBIVNVPROC G_glewGetVertexAttribivNV;
GLEW_FUN_EXPORT PFNGLISPROGRAMNVPROC G_glewIsProgramNV;
GLEW_FUN_EXPORT PFNGLLOADPROGRAMNVPROC G_glewLoadProgramNV;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETER4DNVPROC G_glewProgramParameter4dNV;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETER4DVNVPROC G_glewProgramParameter4dvNV;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETER4FNVPROC G_glewProgramParameter4fNV;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETER4FVNVPROC G_glewProgramParameter4fvNV;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERS4DVNVPROC G_glewProgramParameters4dvNV;
GLEW_FUN_EXPORT PFNGLPROGRAMPARAMETERS4FVNVPROC G_glewProgramParameters4fvNV;
GLEW_FUN_EXPORT PFNGLREQUESTRESIDENTPROGRAMSNVPROC G_glewRequestResidentProgramsNV;
GLEW_FUN_EXPORT PFNGLTRACKMATRIXNVPROC G_glewTrackMatrixNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DNVPROC G_glewVertexAttrib1dNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1DVNVPROC G_glewVertexAttrib1dvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FNVPROC G_glewVertexAttrib1fNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1FVNVPROC G_glewVertexAttrib1fvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SNVPROC G_glewVertexAttrib1sNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB1SVNVPROC G_glewVertexAttrib1svNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DNVPROC G_glewVertexAttrib2dNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2DVNVPROC G_glewVertexAttrib2dvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FNVPROC G_glewVertexAttrib2fNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2FVNVPROC G_glewVertexAttrib2fvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SNVPROC G_glewVertexAttrib2sNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB2SVNVPROC G_glewVertexAttrib2svNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DNVPROC G_glewVertexAttrib3dNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3DVNVPROC G_glewVertexAttrib3dvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FNVPROC G_glewVertexAttrib3fNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3FVNVPROC G_glewVertexAttrib3fvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SNVPROC G_glewVertexAttrib3sNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB3SVNVPROC G_glewVertexAttrib3svNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DNVPROC G_glewVertexAttrib4dNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4DVNVPROC G_glewVertexAttrib4dvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FNVPROC G_glewVertexAttrib4fNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4FVNVPROC G_glewVertexAttrib4fvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SNVPROC G_glewVertexAttrib4sNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4SVNVPROC G_glewVertexAttrib4svNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UBNVPROC G_glewVertexAttrib4ubNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIB4UBVNVPROC G_glewVertexAttrib4ubvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBPOINTERNVPROC G_glewVertexAttribPointerNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS1DVNVPROC G_glewVertexAttribs1dvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS1FVNVPROC G_glewVertexAttribs1fvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS1SVNVPROC G_glewVertexAttribs1svNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS2DVNVPROC G_glewVertexAttribs2dvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS2FVNVPROC G_glewVertexAttribs2fvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS2SVNVPROC G_glewVertexAttribs2svNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS3DVNVPROC G_glewVertexAttribs3dvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS3FVNVPROC G_glewVertexAttribs3fvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS3SVNVPROC G_glewVertexAttribs3svNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS4DVNVPROC G_glewVertexAttribs4dvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS4FVNVPROC G_glewVertexAttribs4fvNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS4SVNVPROC G_glewVertexAttribs4svNV;
GLEW_FUN_EXPORT PFNGLVERTEXATTRIBS4UBVNVPROC G_glewVertexAttribs4ubvNV;

GLEW_FUN_EXPORT PFNGLBEGINVIDEOCAPTURENVPROC G_glewBeginVideoCaptureNV;
GLEW_FUN_EXPORT PFNGLBINDVIDEOCAPTURESTREAMBUFFERNVPROC G_glewBindVideoCaptureStreamBufferNV;
GLEW_FUN_EXPORT PFNGLBINDVIDEOCAPTURESTREAMTEXTURENVPROC G_glewBindVideoCaptureStreamTextureNV;
GLEW_FUN_EXPORT PFNGLENDVIDEOCAPTURENVPROC G_glewEndVideoCaptureNV;
GLEW_FUN_EXPORT PFNGLGETVIDEOCAPTURESTREAMDVNVPROC G_glewGetVideoCaptureStreamdvNV;
GLEW_FUN_EXPORT PFNGLGETVIDEOCAPTURESTREAMFVNVPROC G_glewGetVideoCaptureStreamfvNV;
GLEW_FUN_EXPORT PFNGLGETVIDEOCAPTURESTREAMIVNVPROC G_glewGetVideoCaptureStreamivNV;
GLEW_FUN_EXPORT PFNGLGETVIDEOCAPTUREIVNVPROC G_glewGetVideoCaptureivNV;
GLEW_FUN_EXPORT PFNGLVIDEOCAPTURENVPROC G_glewVideoCaptureNV;
GLEW_FUN_EXPORT PFNGLVIDEOCAPTURESTREAMPARAMETERDVNVPROC G_glewVideoCaptureStreamParameterdvNV;
GLEW_FUN_EXPORT PFNGLVIDEOCAPTURESTREAMPARAMETERFVNVPROC G_glewVideoCaptureStreamParameterfvNV;
GLEW_FUN_EXPORT PFNGLVIDEOCAPTURESTREAMPARAMETERIVNVPROC G_glewVideoCaptureStreamParameterivNV;

GLEW_FUN_EXPORT PFNGLCLEARDEPTHFOESPROC G_glewClearDepthfOES;
GLEW_FUN_EXPORT PFNGLCLIPPLANEFOESPROC G_glewClipPlanefOES;
GLEW_FUN_EXPORT PFNGLDEPTHRANGEFOESPROC G_glewDepthRangefOES;
GLEW_FUN_EXPORT PFNGLFRUSTUMFOESPROC G_glewFrustumfOES;
GLEW_FUN_EXPORT PFNGLGETCLIPPLANEFOESPROC G_glewGetClipPlanefOES;
GLEW_FUN_EXPORT PFNGLORTHOFOESPROC G_glewOrthofOES;

GLEW_FUN_EXPORT PFNGLERRORSTRINGREGALPROC G_glewErrorStringREGAL;

GLEW_FUN_EXPORT PFNGLGETEXTENSIONREGALPROC G_glewGetExtensionREGAL;
GLEW_FUN_EXPORT PFNGLISSUPPORTEDREGALPROC G_glewIsSupportedREGAL;

GLEW_FUN_EXPORT PFNGLDETAILTEXFUNCSGISPROC G_glewDetailTexFuncSGIS;
GLEW_FUN_EXPORT PFNGLGETDETAILTEXFUNCSGISPROC G_glewGetDetailTexFuncSGIS;

GLEW_FUN_EXPORT PFNGLFOGFUNCSGISPROC G_glewFogFuncSGIS;
GLEW_FUN_EXPORT PFNGLGETFOGFUNCSGISPROC G_glewGetFogFuncSGIS;

GLEW_FUN_EXPORT PFNGLSAMPLEMASKSGISPROC G_glewSampleMaskSGIS;
GLEW_FUN_EXPORT PFNGLSAMPLEPATTERNSGISPROC G_glewSamplePatternSGIS;

GLEW_FUN_EXPORT PFNGLGETSHARPENTEXFUNCSGISPROC G_glewGetSharpenTexFuncSGIS;
GLEW_FUN_EXPORT PFNGLSHARPENTEXFUNCSGISPROC G_glewSharpenTexFuncSGIS;

GLEW_FUN_EXPORT PFNGLTEXIMAGE4DSGISPROC G_glewTexImage4DSGIS;
GLEW_FUN_EXPORT PFNGLTEXSUBIMAGE4DSGISPROC G_glewTexSubImage4DSGIS;

GLEW_FUN_EXPORT PFNGLGETTEXFILTERFUNCSGISPROC G_glewGetTexFilterFuncSGIS;
GLEW_FUN_EXPORT PFNGLTEXFILTERFUNCSGISPROC G_glewTexFilterFuncSGIS;

GLEW_FUN_EXPORT PFNGLASYNCMARKERSGIXPROC G_glewAsyncMarkerSGIX;
GLEW_FUN_EXPORT PFNGLDELETEASYNCMARKERSSGIXPROC G_glewDeleteAsyncMarkersSGIX;
GLEW_FUN_EXPORT PFNGLFINISHASYNCSGIXPROC G_glewFinishAsyncSGIX;
GLEW_FUN_EXPORT PFNGLGENASYNCMARKERSSGIXPROC G_glewGenAsyncMarkersSGIX;
GLEW_FUN_EXPORT PFNGLISASYNCMARKERSGIXPROC G_glewIsAsyncMarkerSGIX;
GLEW_FUN_EXPORT PFNGLPOLLASYNCSGIXPROC G_glewPollAsyncSGIX;

GLEW_FUN_EXPORT PFNGLFLUSHRASTERSGIXPROC G_glewFlushRasterSGIX;

GLEW_FUN_EXPORT PFNGLTEXTUREFOGSGIXPROC G_glewTextureFogSGIX;

GLEW_FUN_EXPORT PFNGLFRAGMENTCOLORMATERIALSGIXPROC G_glewFragmentColorMaterialSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELFSGIXPROC G_glewFragmentLightModelfSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELFVSGIXPROC G_glewFragmentLightModelfvSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELISGIXPROC G_glewFragmentLightModeliSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTMODELIVSGIXPROC G_glewFragmentLightModelivSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTFSGIXPROC G_glewFragmentLightfSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTFVSGIXPROC G_glewFragmentLightfvSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTISGIXPROC G_glewFragmentLightiSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTLIGHTIVSGIXPROC G_glewFragmentLightivSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALFSGIXPROC G_glewFragmentMaterialfSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALFVSGIXPROC G_glewFragmentMaterialfvSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALISGIXPROC G_glewFragmentMaterialiSGIX;
GLEW_FUN_EXPORT PFNGLFRAGMENTMATERIALIVSGIXPROC G_glewFragmentMaterialivSGIX;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTLIGHTFVSGIXPROC G_glewGetFragmentLightfvSGIX;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTLIGHTIVSGIXPROC G_glewGetFragmentLightivSGIX;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTMATERIALFVSGIXPROC G_glewGetFragmentMaterialfvSGIX;
GLEW_FUN_EXPORT PFNGLGETFRAGMENTMATERIALIVSGIXPROC G_glewGetFragmentMaterialivSGIX;

GLEW_FUN_EXPORT PFNGLFRAMEZOOMSGIXPROC G_glewFrameZoomSGIX;

GLEW_FUN_EXPORT PFNGLPIXELTEXGENSGIXPROC G_glewPixelTexGenSGIX;

GLEW_FUN_EXPORT PFNGLREFERENCEPLANESGIXPROC G_glewReferencePlaneSGIX;

GLEW_FUN_EXPORT PFNGLSPRITEPARAMETERFSGIXPROC G_glewSpriteParameterfSGIX;
GLEW_FUN_EXPORT PFNGLSPRITEPARAMETERFVSGIXPROC G_glewSpriteParameterfvSGIX;
GLEW_FUN_EXPORT PFNGLSPRITEPARAMETERISGIXPROC G_glewSpriteParameteriSGIX;
GLEW_FUN_EXPORT PFNGLSPRITEPARAMETERIVSGIXPROC G_glewSpriteParameterivSGIX;

GLEW_FUN_EXPORT PFNGLTAGSAMPLEBUFFERSGIXPROC G_glewTagSampleBufferSGIX;

GLEW_FUN_EXPORT PFNGLCOLORTABLEPARAMETERFVSGIPROC G_glewColorTableParameterfvSGI;
GLEW_FUN_EXPORT PFNGLCOLORTABLEPARAMETERIVSGIPROC G_glewColorTableParameterivSGI;
GLEW_FUN_EXPORT PFNGLCOLORTABLESGIPROC G_glewColorTableSGI;
GLEW_FUN_EXPORT PFNGLCOPYCOLORTABLESGIPROC G_glewCopyColorTableSGI;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERFVSGIPROC G_glewGetColorTableParameterfvSGI;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLEPARAMETERIVSGIPROC G_glewGetColorTableParameterivSGI;
GLEW_FUN_EXPORT PFNGLGETCOLORTABLESGIPROC G_glewGetColorTableSGI;

GLEW_FUN_EXPORT PFNGLFINISHTEXTURESUNXPROC G_glewFinishTextureSUNX;

GLEW_FUN_EXPORT PFNGLGLOBALALPHAFACTORBSUNPROC G_glewGlobalAlphaFactorbSUN;
GLEW_FUN_EXPORT PFNGLGLOBALALPHAFACTORDSUNPROC G_glewGlobalAlphaFactordSUN;
GLEW_FUN_EXPORT PFNGLGLOBALALPHAFACTORFSUNPROC G_glewGlobalAlphaFactorfSUN;
GLEW_FUN_EXPORT PFNGLGLOBALALPHAFACTORISUNPROC G_glewGlobalAlphaFactoriSUN;
GLEW_FUN_EXPORT PFNGLGLOBALALPHAFACTORSSUNPROC G_glewGlobalAlphaFactorsSUN;
GLEW_FUN_EXPORT PFNGLGLOBALALPHAFACTORUBSUNPROC G_glewGlobalAlphaFactorubSUN;
GLEW_FUN_EXPORT PFNGLGLOBALALPHAFACTORUISUNPROC G_glewGlobalAlphaFactoruiSUN;
GLEW_FUN_EXPORT PFNGLGLOBALALPHAFACTORUSSUNPROC G_glewGlobalAlphaFactorusSUN;

GLEW_FUN_EXPORT PFNGLREADVIDEOPIXELSSUNPROC G_glewReadVideoPixelsSUN;

GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEPOINTERSUNPROC G_glewReplacementCodePointerSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUBSUNPROC G_glewReplacementCodeubSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUBVSUNPROC G_glewReplacementCodeubvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUISUNPROC G_glewReplacementCodeuiSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUIVSUNPROC G_glewReplacementCodeuivSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUSSUNPROC G_glewReplacementCodeusSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUSVSUNPROC G_glewReplacementCodeusvSUN;

GLEW_FUN_EXPORT PFNGLCOLOR3FVERTEX3FSUNPROC G_glewColor3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLCOLOR3FVERTEX3FVSUNPROC G_glewColor3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLCOLOR4FNORMAL3FVERTEX3FSUNPROC G_glewColor4fNormal3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLCOLOR4FNORMAL3FVERTEX3FVSUNPROC G_glewColor4fNormal3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLCOLOR4UBVERTEX2FSUNPROC G_glewColor4ubVertex2fSUN;
GLEW_FUN_EXPORT PFNGLCOLOR4UBVERTEX2FVSUNPROC G_glewColor4ubVertex2fvSUN;
GLEW_FUN_EXPORT PFNGLCOLOR4UBVERTEX3FSUNPROC G_glewColor4ubVertex3fSUN;
GLEW_FUN_EXPORT PFNGLCOLOR4UBVERTEX3FVSUNPROC G_glewColor4ubVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLNORMAL3FVERTEX3FSUNPROC G_glewNormal3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLNORMAL3FVERTEX3FVSUNPROC G_glewNormal3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FSUNPROC G_glewReplacementCodeuiColor3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUICOLOR3FVERTEX3FVSUNPROC G_glewReplacementCodeuiColor3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FSUNPROC G_glewReplacementCodeuiColor4fNormal3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUICOLOR4FNORMAL3FVERTEX3FVSUNPROC G_glewReplacementCodeuiColor4fNormal3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FSUNPROC G_glewReplacementCodeuiColor4ubVertex3fSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUICOLOR4UBVERTEX3FVSUNPROC G_glewReplacementCodeuiColor4ubVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FSUNPROC G_glewReplacementCodeuiNormal3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUINORMAL3FVERTEX3FVSUNPROC G_glewReplacementCodeuiNormal3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC G_glewReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUITEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC G_glewReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FSUNPROC G_glewReplacementCodeuiTexCoord2fNormal3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUITEXCOORD2FNORMAL3FVERTEX3FVSUNPROC G_glewReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FSUNPROC G_glewReplacementCodeuiTexCoord2fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUITEXCOORD2FVERTEX3FVSUNPROC G_glewReplacementCodeuiTexCoord2fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUIVERTEX3FSUNPROC G_glewReplacementCodeuiVertex3fSUN;
GLEW_FUN_EXPORT PFNGLREPLACEMENTCODEUIVERTEX3FVSUNPROC G_glewReplacementCodeuiVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FCOLOR3FVERTEX3FSUNPROC G_glewTexCoord2fColor3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FCOLOR3FVERTEX3FVSUNPROC G_glewTexCoord2fColor3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FSUNPROC G_glewTexCoord2fColor4fNormal3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FCOLOR4FNORMAL3FVERTEX3FVSUNPROC G_glewTexCoord2fColor4fNormal3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FCOLOR4UBVERTEX3FSUNPROC G_glewTexCoord2fColor4ubVertex3fSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FCOLOR4UBVERTEX3FVSUNPROC G_glewTexCoord2fColor4ubVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FNORMAL3FVERTEX3FSUNPROC G_glewTexCoord2fNormal3fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FNORMAL3FVERTEX3FVSUNPROC G_glewTexCoord2fNormal3fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FVERTEX3FSUNPROC G_glewTexCoord2fVertex3fSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD2FVERTEX3FVSUNPROC G_glewTexCoord2fVertex3fvSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FSUNPROC G_glewTexCoord4fColor4fNormal3fVertex4fSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD4FCOLOR4FNORMAL3FVERTEX4FVSUNPROC G_glewTexCoord4fColor4fNormal3fVertex4fvSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD4FVERTEX4FSUNPROC G_glewTexCoord4fVertex4fSUN;
GLEW_FUN_EXPORT PFNGLTEXCOORD4FVERTEX4FVSUNPROC G_glewTexCoord4fVertex4fvSUN;

GLEW_FUN_EXPORT PFNGLADDSWAPHINTRECTWINPROC G_glewAddSwapHintRectWIN;

#if defined(GLEW_MX) && !defined(_WIN32)
struct GLEWContextStruct
{
#endif /* GLEW_MX */

GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_1_1;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_1_2;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_1_2_1;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_1_3;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_1_4;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_1_5;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_2_0;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_2_1;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_3_0;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_3_1;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_3_2;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_3_3;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_4_0;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_4_1;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_4_2;
GLEW_VAR_EXPORT GLboolean G_GLEW_VERSION_4_3;
GLEW_VAR_EXPORT GLboolean G_GLEW_3DFX_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_3DFX_tbuffer;
GLEW_VAR_EXPORT GLboolean G_GLEW_3DFX_texture_compression_FXT1;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_blend_minmax_factor;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_conservative_depth;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_debug_output;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_depth_clamp_separate;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_draw_buffers_blend;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_multi_draw_indirect;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_name_gen_delete;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_performance_monitor;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_pinned_memory;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_query_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_sample_positions;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_seamless_cubemap_per_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_shader_stencil_export;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_stencil_operation_extended;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_texture_texture4;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_transform_feedback3_lines_triangles;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_vertex_shader_layer;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_vertex_shader_tessellator;
GLEW_VAR_EXPORT GLboolean G_GLEW_AMD_vertex_shader_viewport_index;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_aux_depth_stencil;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_client_storage;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_element_array;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_fence;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_float_pixels;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_flush_buffer_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_object_purgeable;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_pixel_buffer;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_rgb_422;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_row_bytes;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_specular_vector;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_texture_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_transform_hint;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_vertex_array_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_vertex_array_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_vertex_program_evaluators;
GLEW_VAR_EXPORT GLboolean G_GLEW_APPLE_ycbcr_422;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_ES2_compatibility;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_ES3_compatibility;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_arrays_of_arrays;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_base_instance;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_blend_func_extended;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_cl_event;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_clear_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_color_buffer_float;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_compatibility;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_compressed_texture_pixel_storage;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_compute_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_conservative_depth;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_copy_buffer;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_copy_image;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_debug_output;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_depth_buffer_float;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_depth_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_depth_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_draw_buffers;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_draw_buffers_blend;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_draw_elements_base_vertex;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_draw_indirect;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_draw_instanced;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_explicit_attrib_location;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_explicit_uniform_location;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_fragment_coord_conventions;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_fragment_layer_viewport;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_fragment_program;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_fragment_program_shadow;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_fragment_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_framebuffer_no_attachments;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_framebuffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_framebuffer_sRGB;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_geometry_shader4;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_get_program_binary;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_gpu_shader5;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_gpu_shader_fp64;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_half_float_pixel;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_half_float_vertex;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_imaging;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_instanced_arrays;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_internalformat_query;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_internalformat_query2;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_invalidate_subdata;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_map_buffer_alignment;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_map_buffer_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_matrix_palette;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_multi_draw_indirect;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_multitexture;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_occlusion_query;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_occlusion_query2;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_pixel_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_point_parameters;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_point_sprite;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_program_interface_query;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_provoking_vertex;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_robust_buffer_access_behavior;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_robustness;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_robustness_application_isolation;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_robustness_share_group_isolation;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_sample_shading;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_sampler_objects;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_seamless_cube_map;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_separate_shader_objects;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_atomic_counters;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_bit_encoding;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_image_load_store;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_image_size;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_objects;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_precision;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_stencil_export;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_storage_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_subroutine;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shader_texture_lod;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shading_language_100;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shading_language_420pack;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shading_language_include;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shading_language_packing;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shadow;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_shadow_ambient;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_stencil_texturing;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_sync;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_tessellation_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_border_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_buffer_object_rgb32;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_buffer_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_compression;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_compression_bptc;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_compression_rgtc;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_cube_map;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_cube_map_array;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_env_add;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_env_combine;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_env_crossbar;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_env_dot3;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_float;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_gather;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_mirrored_repeat;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_non_power_of_two;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_query_levels;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_query_lod;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_rectangle;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_rg;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_rgb10_a2ui;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_storage;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_storage_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_swizzle;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_texture_view;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_timer_query;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_transform_feedback2;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_transform_feedback3;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_transform_feedback_instanced;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_transpose_matrix;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_uniform_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_array_bgra;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_array_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_attrib_64bit;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_attrib_binding;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_blend;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_program;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_vertex_type_2_10_10_10_rev;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_viewport_array;
GLEW_VAR_EXPORT GLboolean G_GLEW_ARB_window_pos;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATIX_point_sprites;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATIX_texture_env_combine3;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATIX_texture_env_route;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATIX_vertex_shader_output_point_size;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_draw_buffers;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_element_array;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_envmap_bumpmap;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_fragment_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_map_object_buffer;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_meminfo;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_pn_triangles;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_separate_stencil;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_shader_texture_lod;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_text_fragment_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_texture_compression_3dc;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_texture_env_combine3;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_texture_float;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_texture_mirror_once;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_vertex_array_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_vertex_attrib_array_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_ATI_vertex_streams;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_422_pixels;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_Cg_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_abgr;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_bgra;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_bindable_uniform;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_blend_color;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_blend_equation_separate;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_blend_func_separate;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_blend_logic_op;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_blend_minmax;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_blend_subtract;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_clip_volume_hint;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_cmyka;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_color_subtable;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_compiled_vertex_array;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_convolution;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_coordinate_frame;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_copy_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_cull_vertex;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_debug_marker;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_depth_bounds_test;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_direct_state_access;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_draw_buffers2;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_draw_instanced;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_draw_range_elements;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_fog_coord;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_fragment_lighting;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_framebuffer_blit;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_framebuffer_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_framebuffer_multisample_blit_scaled;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_framebuffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_framebuffer_sRGB;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_geometry_shader4;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_gpu_program_parameters;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_gpu_shader4;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_histogram;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_index_array_formats;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_index_func;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_index_material;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_index_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_light_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_misc_attribute;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_multi_draw_arrays;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_packed_depth_stencil;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_packed_float;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_packed_pixels;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_paletted_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_pixel_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_pixel_transform;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_pixel_transform_color_table;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_point_parameters;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_polygon_offset;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_provoking_vertex;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_rescale_normal;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_scene_marker;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_secondary_color;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_separate_shader_objects;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_separate_specular_color;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_shader_image_load_store;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_shadow_funcs;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_shared_texture_palette;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_stencil_clear_tag;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_stencil_two_side;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_stencil_wrap;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_subtexture;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture3D;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_array;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_compression_dxt1;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_compression_latc;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_compression_rgtc;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_compression_s3tc;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_cube_map;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_edge_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_env;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_env_add;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_env_combine;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_env_dot3;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_filter_anisotropic;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_integer;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_lod_bias;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_mirror_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_perturb_normal;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_rectangle;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_sRGB;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_sRGB_decode;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_shared_exponent;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_snorm;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_texture_swizzle;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_timer_query;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_transform_feedback;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_vertex_array;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_vertex_array_bgra;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_vertex_attrib_64bit;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_vertex_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_vertex_weighting;
GLEW_VAR_EXPORT GLboolean G_GLEW_EXT_x11_sync_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_GREMEDY_frame_terminator;
GLEW_VAR_EXPORT GLboolean G_GLEW_GREMEDY_string_marker;
GLEW_VAR_EXPORT GLboolean G_GLEW_HP_convolution_border_modes;
GLEW_VAR_EXPORT GLboolean G_GLEW_HP_image_transform;
GLEW_VAR_EXPORT GLboolean G_GLEW_HP_occlusion_test;
GLEW_VAR_EXPORT GLboolean G_GLEW_HP_texture_lighting;
GLEW_VAR_EXPORT GLboolean G_GLEW_IBM_cull_vertex;
GLEW_VAR_EXPORT GLboolean G_GLEW_IBM_multimode_draw_arrays;
GLEW_VAR_EXPORT GLboolean G_GLEW_IBM_rasterpos_clip;
GLEW_VAR_EXPORT GLboolean G_GLEW_IBM_static_data;
GLEW_VAR_EXPORT GLboolean G_GLEW_IBM_texture_mirrored_repeat;
GLEW_VAR_EXPORT GLboolean G_GLEW_IBM_vertex_array_lists;
GLEW_VAR_EXPORT GLboolean G_GLEW_INGR_color_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_INGR_interlace_read;
GLEW_VAR_EXPORT GLboolean G_GLEW_INTEL_parallel_arrays;
GLEW_VAR_EXPORT GLboolean G_GLEW_INTEL_texture_scissor;
GLEW_VAR_EXPORT GLboolean G_GLEW_KHR_debug;
GLEW_VAR_EXPORT GLboolean G_GLEW_KHR_texture_compression_astc_ldr;
GLEW_VAR_EXPORT GLboolean G_GLEW_KTX_buffer_region;
GLEW_VAR_EXPORT GLboolean G_GLEW_MESAX_texture_stack;
GLEW_VAR_EXPORT GLboolean G_GLEW_MESA_pack_invert;
GLEW_VAR_EXPORT GLboolean G_GLEW_MESA_resize_buffers;
GLEW_VAR_EXPORT GLboolean G_GLEW_MESA_window_pos;
GLEW_VAR_EXPORT GLboolean G_GLEW_MESA_ycbcr_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_NVX_gpu_memory_info;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_bindless_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_blend_square;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_conditional_render;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_copy_depth_to_color;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_copy_image;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_depth_buffer_float;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_depth_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_depth_range_unclamped;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_evaluators;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_explicit_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_fence;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_float_buffer;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_fog_distance;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_fragment_program;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_fragment_program2;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_fragment_program4;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_fragment_program_option;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_framebuffer_multisample_coverage;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_geometry_program4;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_geometry_shader4;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_gpu_program4;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_gpu_program5;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_gpu_program_fp64;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_gpu_shader5;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_half_float;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_light_max_exponent;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_multisample_coverage;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_multisample_filter_hint;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_occlusion_query;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_packed_depth_stencil;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_parameter_buffer_object;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_parameter_buffer_object2;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_path_rendering;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_pixel_data_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_point_sprite;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_present_video;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_primitive_restart;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_register_combiners;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_register_combiners2;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_shader_atomic_float;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_shader_buffer_load;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_tessellation_program5;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texgen_emboss;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texgen_reflection;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_barrier;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_compression_vtc;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_env_combine4;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_expand_normal;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_rectangle;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_shader;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_shader2;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_texture_shader3;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_transform_feedback;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_transform_feedback2;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vdpau_interop;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_array_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_array_range2;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_attrib_integer_64bit;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_buffer_unified_memory;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_program;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_program1_1;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_program2;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_program2_option;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_program3;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_vertex_program4;
GLEW_VAR_EXPORT GLboolean G_GLEW_NV_video_capture;
GLEW_VAR_EXPORT GLboolean G_GLEW_OES_byte_coordinates;
GLEW_VAR_EXPORT GLboolean G_GLEW_OES_compressed_paletted_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_OES_read_format;
GLEW_VAR_EXPORT GLboolean G_GLEW_OES_single_precision;
GLEW_VAR_EXPORT GLboolean G_GLEW_OML_interlace;
GLEW_VAR_EXPORT GLboolean G_GLEW_OML_resample;
GLEW_VAR_EXPORT GLboolean G_GLEW_OML_subsample;
GLEW_VAR_EXPORT GLboolean G_GLEW_PGI_misc_hints;
GLEW_VAR_EXPORT GLboolean G_GLEW_PGI_vertex_hints;
GLEW_VAR_EXPORT GLboolean G_GLEW_REGAL_error_string;
GLEW_VAR_EXPORT GLboolean G_GLEW_REGAL_extension_query;
GLEW_VAR_EXPORT GLboolean G_GLEW_REGAL_log;
GLEW_VAR_EXPORT GLboolean G_GLEW_REND_screen_coordinates;
GLEW_VAR_EXPORT GLboolean G_GLEW_S3_s3tc;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_color_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_detail_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_fog_function;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_generate_mipmap;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_multisample;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_pixel_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_point_line_texgen;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_sharpen_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_texture4D;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_texture_border_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_texture_edge_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_texture_filter4;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_texture_lod;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIS_texture_select;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_async;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_async_histogram;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_async_pixel;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_blend_alpha_minmax;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_clipmap;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_convolution_accuracy;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_depth_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_flush_raster;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_fog_offset;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_fog_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_fragment_specular_lighting;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_framezoom;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_interlace;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_ir_instrument1;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_list_priority;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_pixel_texture;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_pixel_texture_bits;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_reference_plane;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_resample;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_shadow;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_shadow_ambient;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_sprite;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_tag_sample_buffer;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_texture_add_env;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_texture_coordinate_clamp;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_texture_lod_bias;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_texture_multi_buffer;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_texture_range;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_texture_scale_bias;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_vertex_preclip;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_vertex_preclip_hint;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGIX_ycrcb;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGI_color_matrix;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGI_color_table;
GLEW_VAR_EXPORT GLboolean G_GLEW_SGI_texture_color_table;
GLEW_VAR_EXPORT GLboolean G_GLEW_SUNX_constant_data;
GLEW_VAR_EXPORT GLboolean G_GLEW_SUN_convolution_border_modes;
GLEW_VAR_EXPORT GLboolean G_GLEW_SUN_global_alpha;
GLEW_VAR_EXPORT GLboolean G_GLEW_SUN_mesh_array;
GLEW_VAR_EXPORT GLboolean G_GLEW_SUN_read_video_pixels;
GLEW_VAR_EXPORT GLboolean G_GLEW_SUN_slice_accum;
GLEW_VAR_EXPORT GLboolean G_GLEW_SUN_triangle_list;
GLEW_VAR_EXPORT GLboolean G_GLEW_SUN_vertex;
GLEW_VAR_EXPORT GLboolean G_GLEW_WIN_phong_shading;
GLEW_VAR_EXPORT GLboolean G_GLEW_WIN_specular_fog;
GLEW_VAR_EXPORT GLboolean G_GLEW_WIN_swap_hint;

#ifdef GLEW_MX
}; /* GLEWContextStruct */
#endif /* GLEW_MX */

/* ------------------------------------------------------------------------- */

/* error codes */
#define GLEW_OK 0
#define GLEW_NO_ERROR 0
#define GLEW_ERROR_NO_GL_VERSION 1  /* missing GL version */
#define GLEW_ERROR_GL_VERSION_10_ONLY 2  /* Need at least OpenGL 1.1 */
#define GLEW_ERROR_GLX_VERSION_11_ONLY 3  /* Need at least GLX 1.2 */

/* string codes */
#define GLEW_VERSION 1
#define GLEW_VERSION_MAJOR 2
#define GLEW_VERSION_MINOR 3
#define GLEW_VERSION_MICRO 4

/* API */
#ifdef GLEW_MX

typedef struct GLEWContextStruct GLEWContext;
GLEWAPI GLenum GLEWAPIENTRY glewContextInit (GLEWContext *ctx);
GLEWAPI GLboolean GLEWAPIENTRY glewContextIsSupported (const GLEWContext *ctx, const char *name);

#define glewInit() glewContextInit(glewGetContext())
#define glewIsSupported(x) glewContextIsSupported(glewGetContext(), x)
#define glewIsExtensionSupported(x) glewIsSupported(x)

#define GLEW_GET_VAR(x) (*(const GLboolean*)&(glewGetContext()->x))
#ifdef _WIN32
#  define GLEW_GET_FUN(x) glewGetContext()->x
#else
#  define GLEW_GET_FUN(x) x
#endif

#else /* GLEW_MX */

GLEWAPI GLenum GLEWAPIENTRY glewInit (void);
GLEWAPI GLboolean GLEWAPIENTRY glewIsSupported (const char *name);
#define glewIsExtensionSupported(x) glewIsSupported(x)

#define GLEW_GET_VAR(x) (*(const GLboolean*)&x)
#define GLEW_GET_FUN(x) x

#endif /* GLEW_MX */

GLEWAPI GLboolean glewExperimental;
GLEWAPI GLboolean GLEWAPIENTRY glewGetExtension (const char *name);
GLEWAPI const GLubyte * GLEWAPIENTRY glewGetErrorString (GLenum error);
GLEWAPI const GLubyte * GLEWAPIENTRY glewGetString (GLenum name);

#ifdef __cplusplus
}
#endif

#ifdef GLEW_APIENTRY_DEFINED
#undef GLEW_APIENTRY_DEFINED
#undef APIENTRY
#undef GLAPIENTRY
#define GLAPIENTRY
#endif

#ifdef GLEW_CALLBACK_DEFINED
#undef GLEW_CALLBACK_DEFINED
#undef CALLBACK
#endif

#ifdef GLEW_WINGDIAPI_DEFINED
#undef GLEW_WINGDIAPI_DEFINED
#undef WINGDIAPI
#endif

#undef GLAPI
/* #undef GLEWAPI */

#endif /* G_glew_h__ */
