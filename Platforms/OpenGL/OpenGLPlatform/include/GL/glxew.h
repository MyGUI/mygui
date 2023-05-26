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

#ifndef __glxew_h__
#define __glxew_h__
#define __GLXEW_H__

#ifdef __glxext_h_
#error glxext.h included before glxew.h
#endif

#if defined(GLX_H) || defined(__GLX_glx_h__) || defined(__glx_h__)
#error glx.h included before glxew.h
#endif

#define __glxext_h_

#define GLX_H
#define __GLX_glx_h__
#define __glx_h__

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <GL/glew.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------- GLX_VERSION_1_0 --------------------------- */

#ifndef GLX_VERSION_1_0
#define GLX_VERSION_1_0 1

#define GLX_USE_GL 1
#define GLX_BUFFER_SIZE 2
#define GLX_LEVEL 3
#define GLX_RGBA 4
#define GLX_DOUBLEBUFFER 5
#define GLX_STEREO 6
#define GLX_AUX_BUFFERS 7
#define GLX_RED_SIZE 8
#define GLX_GREEN_SIZE 9
#define GLX_BLUE_SIZE 10
#define GLX_ALPHA_SIZE 11
#define GLX_DEPTH_SIZE 12
#define GLX_STENCIL_SIZE 13
#define GLX_ACCUM_RED_SIZE 14
#define GLX_ACCUM_GREEN_SIZE 15
#define GLX_ACCUM_BLUE_SIZE 16
#define GLX_ACCUM_ALPHA_SIZE 17
#define GLX_BAD_SCREEN 1
#define GLX_BAD_ATTRIBUTE 2
#define GLX_NO_EXTENSION 3
#define GLX_BAD_VISUAL 4
#define GLX_BAD_CONTEXT 5
#define GLX_BAD_VALUE 6
#define GLX_BAD_ENUM 7

typedef XID GLXDrawable;
typedef XID GLXPixmap;
#ifdef __sun
typedef struct __glXContextRec *GLXContext;
#else
typedef struct __GLXcontextRec *GLXContext;
#endif

typedef unsigned int GLXVideoDeviceNV; 

extern Bool glXQueryExtension (Display *dpy, int *errorBase, int *eventBase);
extern Bool glXQueryVersion (Display *dpy, int *major, int *minor);
extern int glXGetConfig (Display *dpy, XVisualInfo *vis, int attrib, int *value);
extern XVisualInfo* glXChooseVisual (Display *dpy, int screen, int *attribList);
extern GLXPixmap glXCreateGLXPixmap (Display *dpy, XVisualInfo *vis, Pixmap pixmap);
extern void glXDestroyGLXPixmap (Display *dpy, GLXPixmap pix);
extern GLXContext glXCreateContext (Display *dpy, XVisualInfo *vis, GLXContext shareList, Bool direct);
extern void glXDestroyContext (Display *dpy, GLXContext ctx);
extern Bool glXIsDirect (Display *dpy, GLXContext ctx);
extern void glXCopyContext (Display *dpy, GLXContext src, GLXContext dst, GLulong mask);
extern Bool glXMakeCurrent (Display *dpy, GLXDrawable drawable, GLXContext ctx);
extern GLXContext glXGetCurrentContext (void);
extern GLXDrawable glXGetCurrentDrawable (void);
extern void glXWaitGL (void);
extern void glXWaitX (void);
extern void glXSwapBuffers (Display *dpy, GLXDrawable drawable);
extern void glXUseXFont (Font font, int first, int count, int listBase);

#define GLXEW_VERSION_1_0 GLXEW_GET_VAR(__GLXEW_VERSION_1_0)

#endif /* GLX_VERSION_1_0 */

/* ---------------------------- GLX_VERSION_1_1 --------------------------- */

#ifndef GLX_VERSION_1_1
#define GLX_VERSION_1_1

#define GLX_VENDOR 0x1
#define GLX_VERSION 0x2
#define GLX_EXTENSIONS 0x3

extern const char* glXQueryExtensionsString (Display *dpy, int screen);
extern const char* glXGetClientString (Display *dpy, int name);
extern const char* glXQueryServerString (Display *dpy, int screen, int name);

#define GLXEW_VERSION_1_1 GLXEW_GET_VAR(__GLXEW_VERSION_1_1)

#endif /* GLX_VERSION_1_1 */

/* ---------------------------- GLX_VERSION_1_2 ---------------------------- */

#ifndef GLX_VERSION_1_2
#define GLX_VERSION_1_2 1

typedef Display* ( * PFNGLXGETCURRENTDISPLAYPROC) (void);

#define glXGetCurrentDisplay GLXEW_GET_FUN(G_glewXGetCurrentDisplay)

#define GLXEW_VERSION_1_2 GLXEW_GET_VAR(__GLXEW_VERSION_1_2)

#endif /* GLX_VERSION_1_2 */

/* ---------------------------- GLX_VERSION_1_3 ---------------------------- */

#ifndef GLX_VERSION_1_3
#define GLX_VERSION_1_3 1

#define GLX_RGBA_BIT 0x00000001
#define GLX_FRONT_LEFT_BUFFER_BIT 0x00000001
#define GLX_WINDOW_BIT 0x00000001
#define GLX_COLOR_INDEX_BIT 0x00000002
#define GLX_PIXMAP_BIT 0x00000002
#define GLX_FRONT_RIGHT_BUFFER_BIT 0x00000002
#define GLX_BACK_LEFT_BUFFER_BIT 0x00000004
#define GLX_PBUFFER_BIT 0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT 0x00000008
#define GLX_AUX_BUFFERS_BIT 0x00000010
#define GLX_CONFIG_CAVEAT 0x20
#define GLX_DEPTH_BUFFER_BIT 0x00000020
#define GLX_X_VISUAL_TYPE 0x22
#define GLX_TRANSPARENT_TYPE 0x23
#define GLX_TRANSPARENT_INDEX_VALUE 0x24
#define GLX_TRANSPARENT_RED_VALUE 0x25
#define GLX_TRANSPARENT_GREEN_VALUE 0x26
#define GLX_TRANSPARENT_BLUE_VALUE 0x27
#define GLX_TRANSPARENT_ALPHA_VALUE 0x28
#define GLX_STENCIL_BUFFER_BIT 0x00000040
#define GLX_ACCUM_BUFFER_BIT 0x00000080
#define GLX_NONE 0x8000
#define GLX_SLOW_CONFIG 0x8001
#define GLX_TRUE_COLOR 0x8002
#define GLX_DIRECT_COLOR 0x8003
#define GLX_PSEUDO_COLOR 0x8004
#define GLX_STATIC_COLOR 0x8005
#define GLX_GRAY_SCALE 0x8006
#define GLX_STATIC_GRAY 0x8007
#define GLX_TRANSPARENT_RGB 0x8008
#define GLX_TRANSPARENT_INDEX 0x8009
#define GLX_VISUAL_ID 0x800B
#define GLX_SCREEN 0x800C
#define GLX_NON_CONFORMANT_CONFIG 0x800D
#define GLX_DRAWABLE_TYPE 0x8010
#define GLX_RENDER_TYPE 0x8011
#define GLX_X_RENDERABLE 0x8012
#define GLX_FBCONFIG_ID 0x8013
#define GLX_RGBA_TYPE 0x8014
#define GLX_COLOR_INDEX_TYPE 0x8015
#define GLX_MAX_PBUFFER_WIDTH 0x8016
#define GLX_MAX_PBUFFER_HEIGHT 0x8017
#define GLX_MAX_PBUFFER_PIXELS 0x8018
#define GLX_PRESERVED_CONTENTS 0x801B
#define GLX_LARGEST_PBUFFER 0x801C
#define GLX_WIDTH 0x801D
#define GLX_HEIGHT 0x801E
#define GLX_EVENT_MASK 0x801F
#define GLX_DAMAGED 0x8020
#define GLX_SAVED 0x8021
#define GLX_WINDOW 0x8022
#define GLX_PBUFFER 0x8023
#define GLX_PBUFFER_HEIGHT 0x8040
#define GLX_PBUFFER_WIDTH 0x8041
#define GLX_PBUFFER_CLOBBER_MASK 0x08000000
#define GLX_DONT_CARE 0xFFFFFFFF

typedef XID GLXFBConfigID;
typedef XID GLXPbuffer;
typedef XID GLXWindow;
typedef struct __GLXFBConfigRec *GLXFBConfig;

typedef struct {
  int event_type; 
  int draw_type; 
  unsigned long serial; 
  Bool send_event; 
  Display *display; 
  GLXDrawable drawable; 
  unsigned int buffer_mask; 
  unsigned int aux_buffer; 
  int x, y; 
  int width, height; 
  int count; 
} GLXPbufferClobberEvent;
typedef union __GLXEvent {
  GLXPbufferClobberEvent glxpbufferclobber; 
  long pad[24]; 
} GLXEvent;

typedef GLXFBConfig* ( * PFNGLXCHOOSEFBCONFIGPROC) (Display *dpy, int screen, const int *attrib_list, int *nelements);
typedef GLXContext ( * PFNGLXCREATENEWCONTEXTPROC) (Display *dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
typedef GLXPbuffer ( * PFNGLXCREATEPBUFFERPROC) (Display *dpy, GLXFBConfig config, const int *attrib_list);
typedef GLXPixmap ( * PFNGLXCREATEPIXMAPPROC) (Display *dpy, GLXFBConfig config, Pixmap pixmap, const int *attrib_list);
typedef GLXWindow ( * PFNGLXCREATEWINDOWPROC) (Display *dpy, GLXFBConfig config, Window win, const int *attrib_list);
typedef void ( * PFNGLXDESTROYPBUFFERPROC) (Display *dpy, GLXPbuffer pbuf);
typedef void ( * PFNGLXDESTROYPIXMAPPROC) (Display *dpy, GLXPixmap pixmap);
typedef void ( * PFNGLXDESTROYWINDOWPROC) (Display *dpy, GLXWindow win);
typedef GLXDrawable ( * PFNGLXGETCURRENTREADDRAWABLEPROC) (void);
typedef int ( * PFNGLXGETFBCONFIGATTRIBPROC) (Display *dpy, GLXFBConfig config, int attribute, int *value);
typedef GLXFBConfig* ( * PFNGLXGETFBCONFIGSPROC) (Display *dpy, int screen, int *nelements);
typedef void ( * PFNGLXGETSELECTEDEVENTPROC) (Display *dpy, GLXDrawable draw, unsigned long *event_mask);
typedef XVisualInfo* ( * PFNGLXGETVISUALFROMFBCONFIGPROC) (Display *dpy, GLXFBConfig config);
typedef Bool ( * PFNGLXMAKECONTEXTCURRENTPROC) (Display *display, GLXDrawable draw, GLXDrawable read, GLXContext ctx);
typedef int ( * PFNGLXQUERYCONTEXTPROC) (Display *dpy, GLXContext ctx, int attribute, int *value);
typedef void ( * PFNGLXQUERYDRAWABLEPROC) (Display *dpy, GLXDrawable draw, int attribute, unsigned int *value);
typedef void ( * PFNGLXSELECTEVENTPROC) (Display *dpy, GLXDrawable draw, unsigned long event_mask);

#define glXChooseFBConfig GLXEW_GET_FUN(G_glewXChooseFBConfig)
#define glXCreateNewContext GLXEW_GET_FUN(G_glewXCreateNewContext)
#define glXCreatePbuffer GLXEW_GET_FUN(G_glewXCreatePbuffer)
#define glXCreatePixmap GLXEW_GET_FUN(G_glewXCreatePixmap)
#define glXCreateWindow GLXEW_GET_FUN(G_glewXCreateWindow)
#define glXDestroyPbuffer GLXEW_GET_FUN(G_glewXDestroyPbuffer)
#define glXDestroyPixmap GLXEW_GET_FUN(G_glewXDestroyPixmap)
#define glXDestroyWindow GLXEW_GET_FUN(G_glewXDestroyWindow)
#define glXGetCurrentReadDrawable GLXEW_GET_FUN(G_glewXGetCurrentReadDrawable)
#define glXGetFBConfigAttrib GLXEW_GET_FUN(G_glewXGetFBConfigAttrib)
#define glXGetFBConfigs GLXEW_GET_FUN(G_glewXGetFBConfigs)
#define glXGetSelectedEvent GLXEW_GET_FUN(G_glewXGetSelectedEvent)
#define glXGetVisualFromFBConfig GLXEW_GET_FUN(G_glewXGetVisualFromFBConfig)
#define glXMakeContextCurrent GLXEW_GET_FUN(G_glewXMakeContextCurrent)
#define glXQueryContext GLXEW_GET_FUN(G_glewXQueryContext)
#define glXQueryDrawable GLXEW_GET_FUN(G_glewXQueryDrawable)
#define glXSelectEvent GLXEW_GET_FUN(G_glewXSelectEvent)

#define GLXEW_VERSION_1_3 GLXEW_GET_VAR(__GLXEW_VERSION_1_3)

#endif /* GLX_VERSION_1_3 */

/* ---------------------------- GLX_VERSION_1_4 ---------------------------- */

#ifndef GLX_VERSION_1_4
#define GLX_VERSION_1_4 1

#define GLX_SAMPLE_BUFFERS 100000
#define GLX_SAMPLES 100001

extern void ( * glXGetProcAddress (const GLubyte *procName)) (void);

#define GLXEW_VERSION_1_4 GLXEW_GET_VAR(__GLXEW_VERSION_1_4)

#endif /* GLX_VERSION_1_4 */

/* -------------------------- GLX_3DFX_multisample ------------------------- */

#ifndef GLX_3DFX_multisample
#define GLX_3DFX_multisample 1

#define GLX_SAMPLE_BUFFERS_3DFX 0x8050
#define GLX_SAMPLES_3DFX 0x8051

#define GLXEW_3DFX_multisample GLXEW_GET_VAR(__GLXEW_3DFX_multisample)

#endif /* GLX_3DFX_multisample */

/* ------------------------ GLX_AMD_gpu_association ------------------------ */

#ifndef GLX_AMD_gpu_association
#define GLX_AMD_gpu_association 1

#define GLX_GPU_VENDOR_AMD 0x1F00
#define GLX_GPU_RENDERER_STRING_AMD 0x1F01
#define GLX_GPU_OPENGL_VERSION_STRING_AMD 0x1F02
#define GLX_GPU_FASTEST_TARGET_GPUS_AMD 0x21A2
#define GLX_GPU_RAM_AMD 0x21A3
#define GLX_GPU_CLOCK_AMD 0x21A4
#define GLX_GPU_NUM_PIPES_AMD 0x21A5
#define GLX_GPU_NUM_SIMD_AMD 0x21A6
#define GLX_GPU_NUM_RB_AMD 0x21A7
#define GLX_GPU_NUM_SPI_AMD 0x21A8

#define GLXEW_AMD_gpu_association GLXEW_GET_VAR(__GLXEW_AMD_gpu_association)

#endif /* GLX_AMD_gpu_association */

/* ------------------------- GLX_ARB_create_context ------------------------ */

#ifndef GLX_ARB_create_context
#define GLX_ARB_create_context 1

#define GLX_CONTEXT_DEBUG_BIT_ARB 0x0001
#define GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x0002
#define GLX_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define GLX_CONTEXT_MINOR_VERSION_ARB 0x2092
#define GLX_CONTEXT_FLAGS_ARB 0x2094

typedef GLXContext ( * PFNGLXCREATECONTEXTATTRIBSARBPROC) (Display* dpy, GLXFBConfig config, GLXContext share_context, Bool direct, const int *attrib_list);

#define glXCreateContextAttribsARB GLXEW_GET_FUN(G_glewXCreateContextAttribsARB)

#define GLXEW_ARB_create_context GLXEW_GET_VAR(__GLXEW_ARB_create_context)

#endif /* GLX_ARB_create_context */

/* --------------------- GLX_ARB_create_context_profile -------------------- */

#ifndef GLX_ARB_create_context_profile
#define GLX_ARB_create_context_profile 1

#define GLX_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define GLX_CONTEXT_PROFILE_MASK_ARB 0x9126

#define GLXEW_ARB_create_context_profile GLXEW_GET_VAR(__GLXEW_ARB_create_context_profile)

#endif /* GLX_ARB_create_context_profile */

/* ------------------- GLX_ARB_create_context_robustness ------------------- */

#ifndef GLX_ARB_create_context_robustness
#define GLX_ARB_create_context_robustness 1

#define GLX_CONTEXT_ROBUST_ACCESS_BIT_ARB 0x00000004
#define GLX_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define GLX_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define GLX_NO_RESET_NOTIFICATION_ARB 0x8261

#define GLXEW_ARB_create_context_robustness GLXEW_GET_VAR(__GLXEW_ARB_create_context_robustness)

#endif /* GLX_ARB_create_context_robustness */

/* ------------------------- GLX_ARB_fbconfig_float ------------------------ */

#ifndef GLX_ARB_fbconfig_float
#define GLX_ARB_fbconfig_float 1

#define GLX_RGBA_FLOAT_BIT 0x00000004
#define GLX_RGBA_FLOAT_TYPE 0x20B9

#define GLXEW_ARB_fbconfig_float GLXEW_GET_VAR(__GLXEW_ARB_fbconfig_float)

#endif /* GLX_ARB_fbconfig_float */

/* ------------------------ GLX_ARB_framebuffer_sRGB ----------------------- */

#ifndef GLX_ARB_framebuffer_sRGB
#define GLX_ARB_framebuffer_sRGB 1

#define GLX_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20B2

#define GLXEW_ARB_framebuffer_sRGB GLXEW_GET_VAR(__GLXEW_ARB_framebuffer_sRGB)

#endif /* GLX_ARB_framebuffer_sRGB */

/* ------------------------ GLX_ARB_get_proc_address ----------------------- */

#ifndef GLX_ARB_get_proc_address
#define GLX_ARB_get_proc_address 1

extern void ( * glXGetProcAddressARB (const GLubyte *procName)) (void);

#define GLXEW_ARB_get_proc_address GLXEW_GET_VAR(__GLXEW_ARB_get_proc_address)

#endif /* GLX_ARB_get_proc_address */

/* -------------------------- GLX_ARB_multisample -------------------------- */

#ifndef GLX_ARB_multisample
#define GLX_ARB_multisample 1

#define GLX_SAMPLE_BUFFERS_ARB 100000
#define GLX_SAMPLES_ARB 100001

#define GLXEW_ARB_multisample GLXEW_GET_VAR(__GLXEW_ARB_multisample)

#endif /* GLX_ARB_multisample */

/* ---------------- GLX_ARB_robustness_application_isolation --------------- */

#ifndef GLX_ARB_robustness_application_isolation
#define GLX_ARB_robustness_application_isolation 1

#define GLX_CONTEXT_RESET_ISOLATION_BIT_ARB 0x00000008

#define GLXEW_ARB_robustness_application_isolation GLXEW_GET_VAR(__GLXEW_ARB_robustness_application_isolation)

#endif /* GLX_ARB_robustness_application_isolation */

/* ---------------- GLX_ARB_robustness_share_group_isolation --------------- */

#ifndef GLX_ARB_robustness_share_group_isolation
#define GLX_ARB_robustness_share_group_isolation 1

#define GLX_CONTEXT_RESET_ISOLATION_BIT_ARB 0x00000008

#define GLXEW_ARB_robustness_share_group_isolation GLXEW_GET_VAR(__GLXEW_ARB_robustness_share_group_isolation)

#endif /* GLX_ARB_robustness_share_group_isolation */

/* ---------------------- GLX_ARB_vertex_buffer_object --------------------- */

#ifndef GLX_ARB_vertex_buffer_object
#define GLX_ARB_vertex_buffer_object 1

#define GLX_CONTEXT_ALLOW_BUFFER_BYTE_ORDER_MISMATCH_ARB 0x2095

#define GLXEW_ARB_vertex_buffer_object GLXEW_GET_VAR(__GLXEW_ARB_vertex_buffer_object)

#endif /* GLX_ARB_vertex_buffer_object */

/* ----------------------- GLX_ATI_pixel_format_float ---------------------- */

#ifndef GLX_ATI_pixel_format_float
#define GLX_ATI_pixel_format_float 1

#define GLX_RGBA_FLOAT_ATI_BIT 0x00000100

#define GLXEW_ATI_pixel_format_float GLXEW_GET_VAR(__GLXEW_ATI_pixel_format_float)

#endif /* GLX_ATI_pixel_format_float */

/* ------------------------- GLX_ATI_render_texture ------------------------ */

#ifndef GLX_ATI_render_texture
#define GLX_ATI_render_texture 1

#define GLX_BIND_TO_TEXTURE_RGB_ATI 0x9800
#define GLX_BIND_TO_TEXTURE_RGBA_ATI 0x9801
#define GLX_TEXTURE_FORMAT_ATI 0x9802
#define GLX_TEXTURE_TARGET_ATI 0x9803
#define GLX_MIPMAP_TEXTURE_ATI 0x9804
#define GLX_TEXTURE_RGB_ATI 0x9805
#define GLX_TEXTURE_RGBA_ATI 0x9806
#define GLX_NO_TEXTURE_ATI 0x9807
#define GLX_TEXTURE_CUBE_MAP_ATI 0x9808
#define GLX_TEXTURE_1D_ATI 0x9809
#define GLX_TEXTURE_2D_ATI 0x980A
#define GLX_MIPMAP_LEVEL_ATI 0x980B
#define GLX_CUBE_MAP_FACE_ATI 0x980C
#define GLX_TEXTURE_CUBE_MAP_POSITIVE_X_ATI 0x980D
#define GLX_TEXTURE_CUBE_MAP_NEGATIVE_X_ATI 0x980E
#define GLX_TEXTURE_CUBE_MAP_POSITIVE_Y_ATI 0x980F
#define GLX_TEXTURE_CUBE_MAP_NEGATIVE_Y_ATI 0x9810
#define GLX_TEXTURE_CUBE_MAP_POSITIVE_Z_ATI 0x9811
#define GLX_TEXTURE_CUBE_MAP_NEGATIVE_Z_ATI 0x9812
#define GLX_FRONT_LEFT_ATI 0x9813
#define GLX_FRONT_RIGHT_ATI 0x9814
#define GLX_BACK_LEFT_ATI 0x9815
#define GLX_BACK_RIGHT_ATI 0x9816
#define GLX_AUX0_ATI 0x9817
#define GLX_AUX1_ATI 0x9818
#define GLX_AUX2_ATI 0x9819
#define GLX_AUX3_ATI 0x981A
#define GLX_AUX4_ATI 0x981B
#define GLX_AUX5_ATI 0x981C
#define GLX_AUX6_ATI 0x981D
#define GLX_AUX7_ATI 0x981E
#define GLX_AUX8_ATI 0x981F
#define GLX_AUX9_ATI 0x9820
#define GLX_BIND_TO_TEXTURE_LUMINANCE_ATI 0x9821
#define GLX_BIND_TO_TEXTURE_INTENSITY_ATI 0x9822

typedef void ( * PFNGLXBINDTEXIMAGEATIPROC) (Display *dpy, GLXPbuffer pbuf, int buffer);
typedef void ( * PFNGLXDRAWABLEATTRIBATIPROC) (Display *dpy, GLXDrawable draw, const int *attrib_list);
typedef void ( * PFNGLXRELEASETEXIMAGEATIPROC) (Display *dpy, GLXPbuffer pbuf, int buffer);

#define glXBindTexImageATI GLXEW_GET_FUN(G_glewXBindTexImageATI)
#define glXDrawableAttribATI GLXEW_GET_FUN(G_glewXDrawableAttribATI)
#define glXReleaseTexImageATI GLXEW_GET_FUN(G_glewXReleaseTexImageATI)

#define GLXEW_ATI_render_texture GLXEW_GET_VAR(__GLXEW_ATI_render_texture)

#endif /* GLX_ATI_render_texture */

/* ------------------- GLX_EXT_create_context_es2_profile ------------------ */

#ifndef GLX_EXT_create_context_es2_profile
#define GLX_EXT_create_context_es2_profile 1

#define GLX_CONTEXT_ES2_PROFILE_BIT_EXT 0x00000004

#define GLXEW_EXT_create_context_es2_profile GLXEW_GET_VAR(__GLXEW_EXT_create_context_es2_profile)

#endif /* GLX_EXT_create_context_es2_profile */

/* ------------------- GLX_EXT_create_context_es_profile ------------------- */

#ifndef GLX_EXT_create_context_es_profile
#define GLX_EXT_create_context_es_profile 1

#define GLX_CONTEXT_ES_PROFILE_BIT_EXT 0x00000004

#define GLXEW_EXT_create_context_es_profile GLXEW_GET_VAR(__GLXEW_EXT_create_context_es_profile)

#endif /* GLX_EXT_create_context_es_profile */

/* --------------------- GLX_EXT_fbconfig_packed_float --------------------- */

#ifndef GLX_EXT_fbconfig_packed_float
#define GLX_EXT_fbconfig_packed_float 1

#define GLX_RGBA_UNSIGNED_FLOAT_BIT_EXT 0x00000008
#define GLX_RGBA_UNSIGNED_FLOAT_TYPE_EXT 0x20B1

#define GLXEW_EXT_fbconfig_packed_float GLXEW_GET_VAR(__GLXEW_EXT_fbconfig_packed_float)

#endif /* GLX_EXT_fbconfig_packed_float */

/* ------------------------ GLX_EXT_framebuffer_sRGB ----------------------- */

#ifndef GLX_EXT_framebuffer_sRGB
#define GLX_EXT_framebuffer_sRGB 1

#define GLX_FRAMEBUFFER_SRGB_CAPABLE_EXT 0x20B2

#define GLXEW_EXT_framebuffer_sRGB GLXEW_GET_VAR(__GLXEW_EXT_framebuffer_sRGB)

#endif /* GLX_EXT_framebuffer_sRGB */

/* ------------------------- GLX_EXT_import_context ------------------------ */

#ifndef GLX_EXT_import_context
#define GLX_EXT_import_context 1

#define GLX_SHARE_CONTEXT_EXT 0x800A
#define GLX_VISUAL_ID_EXT 0x800B
#define GLX_SCREEN_EXT 0x800C

typedef XID GLXContextID;

typedef void ( * PFNGLXFREECONTEXTEXTPROC) (Display* dpy, GLXContext context);
typedef GLXContextID ( * PFNGLXGETCONTEXTIDEXTPROC) (const GLXContext context);
typedef GLXContext ( * PFNGLXIMPORTCONTEXTEXTPROC) (Display* dpy, GLXContextID contextID);
typedef int ( * PFNGLXQUERYCONTEXTINFOEXTPROC) (Display* dpy, GLXContext context, int attribute,int *value);

#define glXFreeContextEXT GLXEW_GET_FUN(G_glewXFreeContextEXT)
#define glXGetContextIDEXT GLXEW_GET_FUN(G_glewXGetContextIDEXT)
#define glXImportContextEXT GLXEW_GET_FUN(G_glewXImportContextEXT)
#define glXQueryContextInfoEXT GLXEW_GET_FUN(G_glewXQueryContextInfoEXT)

#define GLXEW_EXT_import_context GLXEW_GET_VAR(__GLXEW_EXT_import_context)

#endif /* GLX_EXT_import_context */

/* -------------------------- GLX_EXT_scene_marker ------------------------- */

#ifndef GLX_EXT_scene_marker
#define GLX_EXT_scene_marker 1

#define GLXEW_EXT_scene_marker GLXEW_GET_VAR(__GLXEW_EXT_scene_marker)

#endif /* GLX_EXT_scene_marker */

/* -------------------------- GLX_EXT_swap_control ------------------------- */

#ifndef GLX_EXT_swap_control
#define GLX_EXT_swap_control 1

#define GLX_SWAP_INTERVAL_EXT 0x20F1
#define GLX_MAX_SWAP_INTERVAL_EXT 0x20F2

typedef void ( * PFNGLXSWAPINTERVALEXTPROC) (Display* dpy, GLXDrawable drawable, int interval);

#define glXSwapIntervalEXT GLXEW_GET_FUN(G_glewXSwapIntervalEXT)

#define GLXEW_EXT_swap_control GLXEW_GET_VAR(__GLXEW_EXT_swap_control)

#endif /* GLX_EXT_swap_control */

/* ----------------------- GLX_EXT_swap_control_tear ----------------------- */

#ifndef GLX_EXT_swap_control_tear
#define GLX_EXT_swap_control_tear 1

#define GLX_LATE_SWAPS_TEAR_EXT 0x20F3

#define GLXEW_EXT_swap_control_tear GLXEW_GET_VAR(__GLXEW_EXT_swap_control_tear)

#endif /* GLX_EXT_swap_control_tear */

/* ---------------------- GLX_EXT_texture_from_pixmap ---------------------- */

#ifndef GLX_EXT_texture_from_pixmap
#define GLX_EXT_texture_from_pixmap 1

#define GLX_TEXTURE_1D_BIT_EXT 0x00000001
#define GLX_TEXTURE_2D_BIT_EXT 0x00000002
#define GLX_TEXTURE_RECTANGLE_BIT_EXT 0x00000004
#define GLX_BIND_TO_TEXTURE_RGB_EXT 0x20D0
#define GLX_BIND_TO_TEXTURE_RGBA_EXT 0x20D1
#define GLX_BIND_TO_MIPMAP_TEXTURE_EXT 0x20D2
#define GLX_BIND_TO_TEXTURE_TARGETS_EXT 0x20D3
#define GLX_Y_INVERTED_EXT 0x20D4
#define GLX_TEXTURE_FORMAT_EXT 0x20D5
#define GLX_TEXTURE_TARGET_EXT 0x20D6
#define GLX_MIPMAP_TEXTURE_EXT 0x20D7
#define GLX_TEXTURE_FORMAT_NONE_EXT 0x20D8
#define GLX_TEXTURE_FORMAT_RGB_EXT 0x20D9
#define GLX_TEXTURE_FORMAT_RGBA_EXT 0x20DA
#define GLX_TEXTURE_1D_EXT 0x20DB
#define GLX_TEXTURE_2D_EXT 0x20DC
#define GLX_TEXTURE_RECTANGLE_EXT 0x20DD
#define GLX_FRONT_LEFT_EXT 0x20DE
#define GLX_FRONT_RIGHT_EXT 0x20DF
#define GLX_BACK_LEFT_EXT 0x20E0
#define GLX_BACK_RIGHT_EXT 0x20E1
#define GLX_AUX0_EXT 0x20E2
#define GLX_AUX1_EXT 0x20E3
#define GLX_AUX2_EXT 0x20E4
#define GLX_AUX3_EXT 0x20E5
#define GLX_AUX4_EXT 0x20E6
#define GLX_AUX5_EXT 0x20E7
#define GLX_AUX6_EXT 0x20E8
#define GLX_AUX7_EXT 0x20E9
#define GLX_AUX8_EXT 0x20EA
#define GLX_AUX9_EXT 0x20EB

typedef void ( * PFNGLXBINDTEXIMAGEEXTPROC) (Display* display, GLXDrawable drawable, int buffer, const int *attrib_list);
typedef void ( * PFNGLXRELEASETEXIMAGEEXTPROC) (Display* display, GLXDrawable drawable, int buffer);

#define glXBindTexImageEXT GLXEW_GET_FUN(G_glewXBindTexImageEXT)
#define glXReleaseTexImageEXT GLXEW_GET_FUN(G_glewXReleaseTexImageEXT)

#define GLXEW_EXT_texture_from_pixmap GLXEW_GET_VAR(__GLXEW_EXT_texture_from_pixmap)

#endif /* GLX_EXT_texture_from_pixmap */

/* -------------------------- GLX_EXT_visual_info -------------------------- */

#ifndef GLX_EXT_visual_info
#define GLX_EXT_visual_info 1

#define GLX_X_VISUAL_TYPE_EXT 0x22
#define GLX_TRANSPARENT_TYPE_EXT 0x23
#define GLX_TRANSPARENT_INDEX_VALUE_EXT 0x24
#define GLX_TRANSPARENT_RED_VALUE_EXT 0x25
#define GLX_TRANSPARENT_GREEN_VALUE_EXT 0x26
#define GLX_TRANSPARENT_BLUE_VALUE_EXT 0x27
#define GLX_TRANSPARENT_ALPHA_VALUE_EXT 0x28
#define GLX_NONE_EXT 0x8000
#define GLX_TRUE_COLOR_EXT 0x8002
#define GLX_DIRECT_COLOR_EXT 0x8003
#define GLX_PSEUDO_COLOR_EXT 0x8004
#define GLX_STATIC_COLOR_EXT 0x8005
#define GLX_GRAY_SCALE_EXT 0x8006
#define GLX_STATIC_GRAY_EXT 0x8007
#define GLX_TRANSPARENT_RGB_EXT 0x8008
#define GLX_TRANSPARENT_INDEX_EXT 0x8009

#define GLXEW_EXT_visual_info GLXEW_GET_VAR(__GLXEW_EXT_visual_info)

#endif /* GLX_EXT_visual_info */

/* ------------------------- GLX_EXT_visual_rating ------------------------- */

#ifndef GLX_EXT_visual_rating
#define GLX_EXT_visual_rating 1

#define GLX_VISUAL_CAVEAT_EXT 0x20
#define GLX_SLOW_VISUAL_EXT 0x8001
#define GLX_NON_CONFORMANT_VISUAL_EXT 0x800D

#define GLXEW_EXT_visual_rating GLXEW_GET_VAR(__GLXEW_EXT_visual_rating)

#endif /* GLX_EXT_visual_rating */

/* -------------------------- GLX_INTEL_swap_event ------------------------- */

#ifndef GLX_INTEL_swap_event
#define GLX_INTEL_swap_event 1

#define GLX_EXCHANGE_COMPLETE_INTEL 0x8180
#define GLX_COPY_COMPLETE_INTEL 0x8181
#define GLX_FLIP_COMPLETE_INTEL 0x8182
#define GLX_BUFFER_SWAP_COMPLETE_INTEL_MASK 0x04000000

#define GLXEW_INTEL_swap_event GLXEW_GET_VAR(__GLXEW_INTEL_swap_event)

#endif /* GLX_INTEL_swap_event */

/* -------------------------- GLX_MESA_agp_offset -------------------------- */

#ifndef GLX_MESA_agp_offset
#define GLX_MESA_agp_offset 1

typedef unsigned int ( * PFNGLXGETAGPOFFSETMESAPROC) (const void* pointer);

#define glXGetAGPOffsetMESA GLXEW_GET_FUN(G_glewXGetAGPOffsetMESA)

#define GLXEW_MESA_agp_offset GLXEW_GET_VAR(__GLXEW_MESA_agp_offset)

#endif /* GLX_MESA_agp_offset */

/* ------------------------ GLX_MESA_copy_sub_buffer ----------------------- */

#ifndef GLX_MESA_copy_sub_buffer
#define GLX_MESA_copy_sub_buffer 1

typedef void ( * PFNGLXCOPYSUBBUFFERMESAPROC) (Display* dpy, GLXDrawable drawable, int x, int y, int width, int height);

#define glXCopySubBufferMESA GLXEW_GET_FUN(G_glewXCopySubBufferMESA)

#define GLXEW_MESA_copy_sub_buffer GLXEW_GET_VAR(__GLXEW_MESA_copy_sub_buffer)

#endif /* GLX_MESA_copy_sub_buffer */

/* ------------------------ GLX_MESA_pixmap_colormap ----------------------- */

#ifndef GLX_MESA_pixmap_colormap
#define GLX_MESA_pixmap_colormap 1

typedef GLXPixmap ( * PFNGLXCREATEGLXPIXMAPMESAPROC) (Display* dpy, XVisualInfo *visual, Pixmap pixmap, Colormap cmap);

#define glXCreateGLXPixmapMESA GLXEW_GET_FUN(G_glewXCreateGLXPixmapMESA)

#define GLXEW_MESA_pixmap_colormap GLXEW_GET_VAR(__GLXEW_MESA_pixmap_colormap)

#endif /* GLX_MESA_pixmap_colormap */

/* ------------------------ GLX_MESA_release_buffers ----------------------- */

#ifndef GLX_MESA_release_buffers
#define GLX_MESA_release_buffers 1

typedef Bool ( * PFNGLXRELEASEBUFFERSMESAPROC) (Display* dpy, GLXDrawable d);

#define glXReleaseBuffersMESA GLXEW_GET_FUN(G_glewXReleaseBuffersMESA)

#define GLXEW_MESA_release_buffers GLXEW_GET_VAR(__GLXEW_MESA_release_buffers)

#endif /* GLX_MESA_release_buffers */

/* ------------------------- GLX_MESA_set_3dfx_mode ------------------------ */

#ifndef GLX_MESA_set_3dfx_mode
#define GLX_MESA_set_3dfx_mode 1

#define GLX_3DFX_WINDOW_MODE_MESA 0x1
#define GLX_3DFX_FULLSCREEN_MODE_MESA 0x2

typedef GLboolean ( * PFNGLXSET3DFXMODEMESAPROC) (GLint mode);

#define glXSet3DfxModeMESA GLXEW_GET_FUN(G_glewXSet3DfxModeMESA)

#define GLXEW_MESA_set_3dfx_mode GLXEW_GET_VAR(__GLXEW_MESA_set_3dfx_mode)

#endif /* GLX_MESA_set_3dfx_mode */

/* ------------------------- GLX_MESA_swap_control ------------------------- */

#ifndef GLX_MESA_swap_control
#define GLX_MESA_swap_control 1

typedef int ( * PFNGLXGETSWAPINTERVALMESAPROC) (void);
typedef int ( * PFNGLXSWAPINTERVALMESAPROC) (unsigned int interval);

#define glXGetSwapIntervalMESA GLXEW_GET_FUN(G_glewXGetSwapIntervalMESA)
#define glXSwapIntervalMESA GLXEW_GET_FUN(G_glewXSwapIntervalMESA)

#define GLXEW_MESA_swap_control GLXEW_GET_VAR(__GLXEW_MESA_swap_control)

#endif /* GLX_MESA_swap_control */

/* --------------------------- GLX_NV_copy_image --------------------------- */

#ifndef GLX_NV_copy_image
#define GLX_NV_copy_image 1

typedef void ( * PFNGLXCOPYIMAGESUBDATANVPROC) (Display *dpy, GLXContext srcCtx, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLXContext dstCtx, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth);

#define glXCopyImageSubDataNV GLXEW_GET_FUN(G_glewXCopyImageSubDataNV)

#define GLXEW_NV_copy_image GLXEW_GET_VAR(__GLXEW_NV_copy_image)

#endif /* GLX_NV_copy_image */

/* -------------------------- GLX_NV_float_buffer -------------------------- */

#ifndef GLX_NV_float_buffer
#define GLX_NV_float_buffer 1

#define GLX_FLOAT_COMPONENTS_NV 0x20B0

#define GLXEW_NV_float_buffer GLXEW_GET_VAR(__GLXEW_NV_float_buffer)

#endif /* GLX_NV_float_buffer */

/* ---------------------- GLX_NV_multisample_coverage ---------------------- */

#ifndef GLX_NV_multisample_coverage
#define GLX_NV_multisample_coverage 1

#define GLX_COLOR_SAMPLES_NV 0x20B3
#define GLX_COVERAGE_SAMPLES_NV 100001

#define GLXEW_NV_multisample_coverage GLXEW_GET_VAR(__GLXEW_NV_multisample_coverage)

#endif /* GLX_NV_multisample_coverage */

/* -------------------------- GLX_NV_present_video ------------------------- */

#ifndef GLX_NV_present_video
#define GLX_NV_present_video 1

#define GLX_NUM_VIDEO_SLOTS_NV 0x20F0

typedef int ( * PFNGLXBINDVIDEODEVICENVPROC) (Display* dpy, unsigned int video_slot, unsigned int video_device, const int *attrib_list);
typedef unsigned int* ( * PFNGLXENUMERATEVIDEODEVICESNVPROC) (Display *dpy, int screen, int *nelements);

#define glXBindVideoDeviceNV GLXEW_GET_FUN(G_glewXBindVideoDeviceNV)
#define glXEnumerateVideoDevicesNV GLXEW_GET_FUN(G_glewXEnumerateVideoDevicesNV)

#define GLXEW_NV_present_video GLXEW_GET_VAR(__GLXEW_NV_present_video)

#endif /* GLX_NV_present_video */

/* --------------------------- GLX_NV_swap_group --------------------------- */

#ifndef GLX_NV_swap_group
#define GLX_NV_swap_group 1

typedef Bool ( * PFNGLXBINDSWAPBARRIERNVPROC) (Display* dpy, GLuint group, GLuint barrier);
typedef Bool ( * PFNGLXJOINSWAPGROUPNVPROC) (Display* dpy, GLXDrawable drawable, GLuint group);
typedef Bool ( * PFNGLXQUERYFRAMECOUNTNVPROC) (Display* dpy, int screen, GLuint *count);
typedef Bool ( * PFNGLXQUERYMAXSWAPGROUPSNVPROC) (Display* dpy, int screen, GLuint *maxGroups, GLuint *maxBarriers);
typedef Bool ( * PFNGLXQUERYSWAPGROUPNVPROC) (Display* dpy, GLXDrawable drawable, GLuint *group, GLuint *barrier);
typedef Bool ( * PFNGLXRESETFRAMECOUNTNVPROC) (Display* dpy, int screen);

#define glXBindSwapBarrierNV GLXEW_GET_FUN(G_glewXBindSwapBarrierNV)
#define glXJoinSwapGroupNV GLXEW_GET_FUN(G_glewXJoinSwapGroupNV)
#define glXQueryFrameCountNV GLXEW_GET_FUN(G_glewXQueryFrameCountNV)
#define glXQueryMaxSwapGroupsNV GLXEW_GET_FUN(G_glewXQueryMaxSwapGroupsNV)
#define glXQuerySwapGroupNV GLXEW_GET_FUN(G_glewXQuerySwapGroupNV)
#define glXResetFrameCountNV GLXEW_GET_FUN(G_glewXResetFrameCountNV)

#define GLXEW_NV_swap_group GLXEW_GET_VAR(__GLXEW_NV_swap_group)

#endif /* GLX_NV_swap_group */

/* ----------------------- GLX_NV_vertex_array_range ----------------------- */

#ifndef GLX_NV_vertex_array_range
#define GLX_NV_vertex_array_range 1

typedef void * ( * PFNGLXALLOCATEMEMORYNVPROC) (GLsizei size, GLfloat readFrequency, GLfloat writeFrequency, GLfloat priority);
typedef void ( * PFNGLXFREEMEMORYNVPROC) (void *pointer);

#define glXAllocateMemoryNV GLXEW_GET_FUN(G_glewXAllocateMemoryNV)
#define glXFreeMemoryNV GLXEW_GET_FUN(G_glewXFreeMemoryNV)

#define GLXEW_NV_vertex_array_range GLXEW_GET_VAR(__GLXEW_NV_vertex_array_range)

#endif /* GLX_NV_vertex_array_range */

/* -------------------------- GLX_NV_video_capture ------------------------- */

#ifndef GLX_NV_video_capture
#define GLX_NV_video_capture 1

#define GLX_DEVICE_ID_NV 0x20CD
#define GLX_UNIQUE_ID_NV 0x20CE
#define GLX_NUM_VIDEO_CAPTURE_SLOTS_NV 0x20CF

typedef XID GLXVideoCaptureDeviceNV;

typedef int ( * PFNGLXBINDVIDEOCAPTUREDEVICENVPROC) (Display* dpy, unsigned int video_capture_slot, GLXVideoCaptureDeviceNV device);
typedef GLXVideoCaptureDeviceNV * ( * PFNGLXENUMERATEVIDEOCAPTUREDEVICESNVPROC) (Display* dpy, int screen, int *nelements);
typedef void ( * PFNGLXLOCKVIDEOCAPTUREDEVICENVPROC) (Display* dpy, GLXVideoCaptureDeviceNV device);
typedef int ( * PFNGLXQUERYVIDEOCAPTUREDEVICENVPROC) (Display* dpy, GLXVideoCaptureDeviceNV device, int attribute, int *value);
typedef void ( * PFNGLXRELEASEVIDEOCAPTUREDEVICENVPROC) (Display* dpy, GLXVideoCaptureDeviceNV device);

#define glXBindVideoCaptureDeviceNV GLXEW_GET_FUN(G_glewXBindVideoCaptureDeviceNV)
#define glXEnumerateVideoCaptureDevicesNV GLXEW_GET_FUN(G_glewXEnumerateVideoCaptureDevicesNV)
#define glXLockVideoCaptureDeviceNV GLXEW_GET_FUN(G_glewXLockVideoCaptureDeviceNV)
#define glXQueryVideoCaptureDeviceNV GLXEW_GET_FUN(G_glewXQueryVideoCaptureDeviceNV)
#define glXReleaseVideoCaptureDeviceNV GLXEW_GET_FUN(G_glewXReleaseVideoCaptureDeviceNV)

#define GLXEW_NV_video_capture GLXEW_GET_VAR(__GLXEW_NV_video_capture)

#endif /* GLX_NV_video_capture */

/* ---------------------------- GLX_NV_video_out --------------------------- */

#ifndef GLX_NV_video_out
#define GLX_NV_video_out 1

#define GLX_VIDEO_OUT_COLOR_NV 0x20C3
#define GLX_VIDEO_OUT_ALPHA_NV 0x20C4
#define GLX_VIDEO_OUT_DEPTH_NV 0x20C5
#define GLX_VIDEO_OUT_COLOR_AND_ALPHA_NV 0x20C6
#define GLX_VIDEO_OUT_COLOR_AND_DEPTH_NV 0x20C7
#define GLX_VIDEO_OUT_FRAME_NV 0x20C8
#define GLX_VIDEO_OUT_FIELD_1_NV 0x20C9
#define GLX_VIDEO_OUT_FIELD_2_NV 0x20CA
#define GLX_VIDEO_OUT_STACKED_FIELDS_1_2_NV 0x20CB
#define GLX_VIDEO_OUT_STACKED_FIELDS_2_1_NV 0x20CC

typedef int ( * PFNGLXBINDVIDEOIMAGENVPROC) (Display* dpy, GLXVideoDeviceNV VideoDevice, GLXPbuffer pbuf, int iVideoBuffer);
typedef int ( * PFNGLXGETVIDEODEVICENVPROC) (Display* dpy, int screen, int numVideoDevices, GLXVideoDeviceNV *pVideoDevice);
typedef int ( * PFNGLXGETVIDEOINFONVPROC) (Display* dpy, int screen, GLXVideoDeviceNV VideoDevice, unsigned long *pulCounterOutputPbuffer, unsigned long *pulCounterOutputVideo);
typedef int ( * PFNGLXRELEASEVIDEODEVICENVPROC) (Display* dpy, int screen, GLXVideoDeviceNV VideoDevice);
typedef int ( * PFNGLXRELEASEVIDEOIMAGENVPROC) (Display* dpy, GLXPbuffer pbuf);
typedef int ( * PFNGLXSENDPBUFFERTOVIDEONVPROC) (Display* dpy, GLXPbuffer pbuf, int iBufferType, unsigned long *pulCounterPbuffer, GLboolean bBlock);

#define glXBindVideoImageNV GLXEW_GET_FUN(G_glewXBindVideoImageNV)
#define glXGetVideoDeviceNV GLXEW_GET_FUN(G_glewXGetVideoDeviceNV)
#define glXGetVideoInfoNV GLXEW_GET_FUN(G_glewXGetVideoInfoNV)
#define glXReleaseVideoDeviceNV GLXEW_GET_FUN(G_glewXReleaseVideoDeviceNV)
#define glXReleaseVideoImageNV GLXEW_GET_FUN(G_glewXReleaseVideoImageNV)
#define glXSendPbufferToVideoNV GLXEW_GET_FUN(G_glewXSendPbufferToVideoNV)

#define GLXEW_NV_video_out GLXEW_GET_VAR(__GLXEW_NV_video_out)

#endif /* GLX_NV_video_out */

/* -------------------------- GLX_OML_swap_method -------------------------- */

#ifndef GLX_OML_swap_method
#define GLX_OML_swap_method 1

#define GLX_SWAP_METHOD_OML 0x8060
#define GLX_SWAP_EXCHANGE_OML 0x8061
#define GLX_SWAP_COPY_OML 0x8062
#define GLX_SWAP_UNDEFINED_OML 0x8063

#define GLXEW_OML_swap_method GLXEW_GET_VAR(__GLXEW_OML_swap_method)

#endif /* GLX_OML_swap_method */

/* -------------------------- GLX_OML_sync_control ------------------------- */

#ifndef GLX_OML_sync_control
#define GLX_OML_sync_control 1

typedef Bool ( * PFNGLXGETMSCRATEOMLPROC) (Display* dpy, GLXDrawable drawable, int32_t* numerator, int32_t* denominator);
typedef Bool ( * PFNGLXGETSYNCVALUESOMLPROC) (Display* dpy, GLXDrawable drawable, int64_t* ust, int64_t* msc, int64_t* sbc);
typedef int64_t ( * PFNGLXSWAPBUFFERSMSCOMLPROC) (Display* dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder);
typedef Bool ( * PFNGLXWAITFORMSCOMLPROC) (Display* dpy, GLXDrawable drawable, int64_t target_msc, int64_t divisor, int64_t remainder, int64_t* ust, int64_t* msc, int64_t* sbc);
typedef Bool ( * PFNGLXWAITFORSBCOMLPROC) (Display* dpy, GLXDrawable drawable, int64_t target_sbc, int64_t* ust, int64_t* msc, int64_t* sbc);

#define glXGetMscRateOML GLXEW_GET_FUN(G_glewXGetMscRateOML)
#define glXGetSyncValuesOML GLXEW_GET_FUN(G_glewXGetSyncValuesOML)
#define glXSwapBuffersMscOML GLXEW_GET_FUN(G_glewXSwapBuffersMscOML)
#define glXWaitForMscOML GLXEW_GET_FUN(G_glewXWaitForMscOML)
#define glXWaitForSbcOML GLXEW_GET_FUN(G_glewXWaitForSbcOML)

#define GLXEW_OML_sync_control GLXEW_GET_VAR(__GLXEW_OML_sync_control)

#endif /* GLX_OML_sync_control */

/* ------------------------ GLX_SGIS_blended_overlay ----------------------- */

#ifndef GLX_SGIS_blended_overlay
#define GLX_SGIS_blended_overlay 1

#define GLX_BLENDED_RGBA_SGIS 0x8025

#define GLXEW_SGIS_blended_overlay GLXEW_GET_VAR(__GLXEW_SGIS_blended_overlay)

#endif /* GLX_SGIS_blended_overlay */

/* -------------------------- GLX_SGIS_color_range ------------------------- */

#ifndef GLX_SGIS_color_range
#define GLX_SGIS_color_range 1

#define GLX_MIN_RED_SGIS 0
#define GLX_MAX_GREEN_SGIS 0
#define GLX_MIN_BLUE_SGIS 0
#define GLX_MAX_ALPHA_SGIS 0
#define GLX_MIN_GREEN_SGIS 0
#define GLX_MIN_ALPHA_SGIS 0
#define GLX_MAX_RED_SGIS 0
#define GLX_EXTENDED_RANGE_SGIS 0
#define GLX_MAX_BLUE_SGIS 0

#define GLXEW_SGIS_color_range GLXEW_GET_VAR(__GLXEW_SGIS_color_range)

#endif /* GLX_SGIS_color_range */

/* -------------------------- GLX_SGIS_multisample ------------------------- */

#ifndef GLX_SGIS_multisample
#define GLX_SGIS_multisample 1

#define GLX_SAMPLE_BUFFERS_SGIS 100000
#define GLX_SAMPLES_SGIS 100001

#define GLXEW_SGIS_multisample GLXEW_GET_VAR(__GLXEW_SGIS_multisample)

#endif /* GLX_SGIS_multisample */

/* ---------------------- GLX_SGIS_shared_multisample ---------------------- */

#ifndef GLX_SGIS_shared_multisample
#define GLX_SGIS_shared_multisample 1

#define GLX_MULTISAMPLE_SUB_RECT_WIDTH_SGIS 0x8026
#define GLX_MULTISAMPLE_SUB_RECT_HEIGHT_SGIS 0x8027

#define GLXEW_SGIS_shared_multisample GLXEW_GET_VAR(__GLXEW_SGIS_shared_multisample)

#endif /* GLX_SGIS_shared_multisample */

/* --------------------------- GLX_SGIX_fbconfig --------------------------- */

#ifndef GLX_SGIX_fbconfig
#define GLX_SGIX_fbconfig 1

#define GLX_WINDOW_BIT_SGIX 0x00000001
#define GLX_RGBA_BIT_SGIX 0x00000001
#define GLX_PIXMAP_BIT_SGIX 0x00000002
#define GLX_COLOR_INDEX_BIT_SGIX 0x00000002
#define GLX_SCREEN_EXT 0x800C
#define GLX_DRAWABLE_TYPE_SGIX 0x8010
#define GLX_RENDER_TYPE_SGIX 0x8011
#define GLX_X_RENDERABLE_SGIX 0x8012
#define GLX_FBCONFIG_ID_SGIX 0x8013
#define GLX_RGBA_TYPE_SGIX 0x8014
#define GLX_COLOR_INDEX_TYPE_SGIX 0x8015

typedef XID GLXFBConfigIDSGIX;
typedef struct __GLXFBConfigRec *GLXFBConfigSGIX;

typedef GLXFBConfigSGIX* ( * PFNGLXCHOOSEFBCONFIGSGIXPROC) (Display *dpy, int screen, const int *attrib_list, int *nelements);
typedef GLXContext ( * PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC) (Display* dpy, GLXFBConfig config, int render_type, GLXContext share_list, Bool direct);
typedef GLXPixmap ( * PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC) (Display* dpy, GLXFBConfig config, Pixmap pixmap);
typedef int ( * PFNGLXGETFBCONFIGATTRIBSGIXPROC) (Display* dpy, GLXFBConfigSGIX config, int attribute, int *value);
typedef GLXFBConfigSGIX ( * PFNGLXGETFBCONFIGFROMVISUALSGIXPROC) (Display* dpy, XVisualInfo *vis);
typedef XVisualInfo* ( * PFNGLXGETVISUALFROMFBCONFIGSGIXPROC) (Display *dpy, GLXFBConfig config);

#define glXChooseFBConfigSGIX GLXEW_GET_FUN(G_glewXChooseFBConfigSGIX)
#define glXCreateContextWithConfigSGIX GLXEW_GET_FUN(G_glewXCreateContextWithConfigSGIX)
#define glXCreateGLXPixmapWithConfigSGIX GLXEW_GET_FUN(G_glewXCreateGLXPixmapWithConfigSGIX)
#define glXGetFBConfigAttribSGIX GLXEW_GET_FUN(G_glewXGetFBConfigAttribSGIX)
#define glXGetFBConfigFromVisualSGIX GLXEW_GET_FUN(G_glewXGetFBConfigFromVisualSGIX)
#define glXGetVisualFromFBConfigSGIX GLXEW_GET_FUN(G_glewXGetVisualFromFBConfigSGIX)

#define GLXEW_SGIX_fbconfig GLXEW_GET_VAR(__GLXEW_SGIX_fbconfig)

#endif /* GLX_SGIX_fbconfig */

/* --------------------------- GLX_SGIX_hyperpipe -------------------------- */

#ifndef GLX_SGIX_hyperpipe
#define GLX_SGIX_hyperpipe 1

#define GLX_HYPERPIPE_DISPLAY_PIPE_SGIX 0x00000001
#define GLX_PIPE_RECT_SGIX 0x00000001
#define GLX_PIPE_RECT_LIMITS_SGIX 0x00000002
#define GLX_HYPERPIPE_RENDER_PIPE_SGIX 0x00000002
#define GLX_HYPERPIPE_STEREO_SGIX 0x00000003
#define GLX_HYPERPIPE_PIXEL_AVERAGE_SGIX 0x00000004
#define GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX 80
#define GLX_BAD_HYPERPIPE_CONFIG_SGIX 91
#define GLX_BAD_HYPERPIPE_SGIX 92
#define GLX_HYPERPIPE_ID_SGIX 0x8030

typedef struct {
  char pipeName[GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX]; 
  int  networkId; 
} GLXHyperpipeNetworkSGIX;
typedef struct {
  char pipeName[GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX]; 
  int XOrigin; 
  int YOrigin; 
  int maxHeight; 
  int maxWidth; 
} GLXPipeRectLimits;
typedef struct {
  char pipeName[GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX]; 
  int channel; 
  unsigned int participationType; 
  int timeSlice; 
} GLXHyperpipeConfigSGIX;
typedef struct {
  char pipeName[GLX_HYPERPIPE_PIPE_NAME_LENGTH_SGIX]; 
  int srcXOrigin; 
  int srcYOrigin; 
  int srcWidth; 
  int srcHeight; 
  int destXOrigin; 
  int destYOrigin; 
  int destWidth; 
  int destHeight; 
} GLXPipeRect;

typedef int ( * PFNGLXBINDHYPERPIPESGIXPROC) (Display *dpy, int hpId);
typedef int ( * PFNGLXDESTROYHYPERPIPECONFIGSGIXPROC) (Display *dpy, int hpId);
typedef int ( * PFNGLXHYPERPIPEATTRIBSGIXPROC) (Display *dpy, int timeSlice, int attrib, int size, void *attribList);
typedef int ( * PFNGLXHYPERPIPECONFIGSGIXPROC) (Display *dpy, int networkId, int npipes, GLXHyperpipeConfigSGIX *cfg, int *hpId);
typedef int ( * PFNGLXQUERYHYPERPIPEATTRIBSGIXPROC) (Display *dpy, int timeSlice, int attrib, int size, void *returnAttribList);
typedef int ( * PFNGLXQUERYHYPERPIPEBESTATTRIBSGIXPROC) (Display *dpy, int timeSlice, int attrib, int size, void *attribList, void *returnAttribList);
typedef GLXHyperpipeConfigSGIX * ( * PFNGLXQUERYHYPERPIPECONFIGSGIXPROC) (Display *dpy, int hpId, int *npipes);
typedef GLXHyperpipeNetworkSGIX * ( * PFNGLXQUERYHYPERPIPENETWORKSGIXPROC) (Display *dpy, int *npipes);

#define glXBindHyperpipeSGIX GLXEW_GET_FUN(G_glewXBindHyperpipeSGIX)
#define glXDestroyHyperpipeConfigSGIX GLXEW_GET_FUN(G_glewXDestroyHyperpipeConfigSGIX)
#define glXHyperpipeAttribSGIX GLXEW_GET_FUN(G_glewXHyperpipeAttribSGIX)
#define glXHyperpipeConfigSGIX GLXEW_GET_FUN(G_glewXHyperpipeConfigSGIX)
#define glXQueryHyperpipeAttribSGIX GLXEW_GET_FUN(G_glewXQueryHyperpipeAttribSGIX)
#define glXQueryHyperpipeBestAttribSGIX GLXEW_GET_FUN(G_glewXQueryHyperpipeBestAttribSGIX)
#define glXQueryHyperpipeConfigSGIX GLXEW_GET_FUN(G_glewXQueryHyperpipeConfigSGIX)
#define glXQueryHyperpipeNetworkSGIX GLXEW_GET_FUN(G_glewXQueryHyperpipeNetworkSGIX)

#define GLXEW_SGIX_hyperpipe GLXEW_GET_VAR(__GLXEW_SGIX_hyperpipe)

#endif /* GLX_SGIX_hyperpipe */

/* ---------------------------- GLX_SGIX_pbuffer --------------------------- */

#ifndef GLX_SGIX_pbuffer
#define GLX_SGIX_pbuffer 1

#define GLX_FRONT_LEFT_BUFFER_BIT_SGIX 0x00000001
#define GLX_FRONT_RIGHT_BUFFER_BIT_SGIX 0x00000002
#define GLX_PBUFFER_BIT_SGIX 0x00000004
#define GLX_BACK_LEFT_BUFFER_BIT_SGIX 0x00000004
#define GLX_BACK_RIGHT_BUFFER_BIT_SGIX 0x00000008
#define GLX_AUX_BUFFERS_BIT_SGIX 0x00000010
#define GLX_DEPTH_BUFFER_BIT_SGIX 0x00000020
#define GLX_STENCIL_BUFFER_BIT_SGIX 0x00000040
#define GLX_ACCUM_BUFFER_BIT_SGIX 0x00000080
#define GLX_SAMPLE_BUFFERS_BIT_SGIX 0x00000100
#define GLX_MAX_PBUFFER_WIDTH_SGIX 0x8016
#define GLX_MAX_PBUFFER_HEIGHT_SGIX 0x8017
#define GLX_MAX_PBUFFER_PIXELS_SGIX 0x8018
#define GLX_OPTIMAL_PBUFFER_WIDTH_SGIX 0x8019
#define GLX_OPTIMAL_PBUFFER_HEIGHT_SGIX 0x801A
#define GLX_PRESERVED_CONTENTS_SGIX 0x801B
#define GLX_LARGEST_PBUFFER_SGIX 0x801C
#define GLX_WIDTH_SGIX 0x801D
#define GLX_HEIGHT_SGIX 0x801E
#define GLX_EVENT_MASK_SGIX 0x801F
#define GLX_DAMAGED_SGIX 0x8020
#define GLX_SAVED_SGIX 0x8021
#define GLX_WINDOW_SGIX 0x8022
#define GLX_PBUFFER_SGIX 0x8023
#define GLX_BUFFER_CLOBBER_MASK_SGIX 0x08000000

typedef XID GLXPbufferSGIX;
typedef struct { int type; unsigned long serial; Bool send_event; Display *display; GLXDrawable drawable; int event_type; int draw_type; unsigned int mask; int x, y; int width, height; int count; } GLXBufferClobberEventSGIX;

typedef GLXPbuffer ( * PFNGLXCREATEGLXPBUFFERSGIXPROC) (Display* dpy, GLXFBConfig config, unsigned int width, unsigned int height, int *attrib_list);
typedef void ( * PFNGLXDESTROYGLXPBUFFERSGIXPROC) (Display* dpy, GLXPbuffer pbuf);
typedef void ( * PFNGLXGETSELECTEDEVENTSGIXPROC) (Display* dpy, GLXDrawable drawable, unsigned long *mask);
typedef void ( * PFNGLXQUERYGLXPBUFFERSGIXPROC) (Display* dpy, GLXPbuffer pbuf, int attribute, unsigned int *value);
typedef void ( * PFNGLXSELECTEVENTSGIXPROC) (Display* dpy, GLXDrawable drawable, unsigned long mask);

#define glXCreateGLXPbufferSGIX GLXEW_GET_FUN(G_glewXCreateGLXPbufferSGIX)
#define glXDestroyGLXPbufferSGIX GLXEW_GET_FUN(G_glewXDestroyGLXPbufferSGIX)
#define glXGetSelectedEventSGIX GLXEW_GET_FUN(G_glewXGetSelectedEventSGIX)
#define glXQueryGLXPbufferSGIX GLXEW_GET_FUN(G_glewXQueryGLXPbufferSGIX)
#define glXSelectEventSGIX GLXEW_GET_FUN(G_glewXSelectEventSGIX)

#define GLXEW_SGIX_pbuffer GLXEW_GET_VAR(__GLXEW_SGIX_pbuffer)

#endif /* GLX_SGIX_pbuffer */

/* ------------------------- GLX_SGIX_swap_barrier ------------------------- */

#ifndef GLX_SGIX_swap_barrier
#define GLX_SGIX_swap_barrier 1

typedef void ( * PFNGLXBINDSWAPBARRIERSGIXPROC) (Display *dpy, GLXDrawable drawable, int barrier);
typedef Bool ( * PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC) (Display *dpy, int screen, int *max);

#define glXBindSwapBarrierSGIX GLXEW_GET_FUN(G_glewXBindSwapBarrierSGIX)
#define glXQueryMaxSwapBarriersSGIX GLXEW_GET_FUN(G_glewXQueryMaxSwapBarriersSGIX)

#define GLXEW_SGIX_swap_barrier GLXEW_GET_VAR(__GLXEW_SGIX_swap_barrier)

#endif /* GLX_SGIX_swap_barrier */

/* -------------------------- GLX_SGIX_swap_group -------------------------- */

#ifndef GLX_SGIX_swap_group
#define GLX_SGIX_swap_group 1

typedef void ( * PFNGLXJOINSWAPGROUPSGIXPROC) (Display *dpy, GLXDrawable drawable, GLXDrawable member);

#define glXJoinSwapGroupSGIX GLXEW_GET_FUN(G_glewXJoinSwapGroupSGIX)

#define GLXEW_SGIX_swap_group GLXEW_GET_VAR(__GLXEW_SGIX_swap_group)

#endif /* GLX_SGIX_swap_group */

/* ------------------------- GLX_SGIX_video_resize ------------------------- */

#ifndef GLX_SGIX_video_resize
#define GLX_SGIX_video_resize 1

#define GLX_SYNC_FRAME_SGIX 0x00000000
#define GLX_SYNC_SWAP_SGIX 0x00000001

typedef int ( * PFNGLXBINDCHANNELTOWINDOWSGIXPROC) (Display* display, int screen, int channel, Window window);
typedef int ( * PFNGLXCHANNELRECTSGIXPROC) (Display* display, int screen, int channel, int x, int y, int w, int h);
typedef int ( * PFNGLXCHANNELRECTSYNCSGIXPROC) (Display* display, int screen, int channel, GLenum synctype);
typedef int ( * PFNGLXQUERYCHANNELDELTASSGIXPROC) (Display* display, int screen, int channel, int *x, int *y, int *w, int *h);
typedef int ( * PFNGLXQUERYCHANNELRECTSGIXPROC) (Display* display, int screen, int channel, int *dx, int *dy, int *dw, int *dh);

#define glXBindChannelToWindowSGIX GLXEW_GET_FUN(G_glewXBindChannelToWindowSGIX)
#define glXChannelRectSGIX GLXEW_GET_FUN(G_glewXChannelRectSGIX)
#define glXChannelRectSyncSGIX GLXEW_GET_FUN(G_glewXChannelRectSyncSGIX)
#define glXQueryChannelDeltasSGIX GLXEW_GET_FUN(G_glewXQueryChannelDeltasSGIX)
#define glXQueryChannelRectSGIX GLXEW_GET_FUN(G_glewXQueryChannelRectSGIX)

#define GLXEW_SGIX_video_resize GLXEW_GET_VAR(__GLXEW_SGIX_video_resize)

#endif /* GLX_SGIX_video_resize */

/* ---------------------- GLX_SGIX_visual_select_group --------------------- */

#ifndef GLX_SGIX_visual_select_group
#define GLX_SGIX_visual_select_group 1

#define GLX_VISUAL_SELECT_GROUP_SGIX 0x8028

#define GLXEW_SGIX_visual_select_group GLXEW_GET_VAR(__GLXEW_SGIX_visual_select_group)

#endif /* GLX_SGIX_visual_select_group */

/* ---------------------------- GLX_SGI_cushion ---------------------------- */

#ifndef GLX_SGI_cushion
#define GLX_SGI_cushion 1

typedef void ( * PFNGLXCUSHIONSGIPROC) (Display* dpy, Window window, float cushion);

#define glXCushionSGI GLXEW_GET_FUN(G_glewXCushionSGI)

#define GLXEW_SGI_cushion GLXEW_GET_VAR(__GLXEW_SGI_cushion)

#endif /* GLX_SGI_cushion */

/* ----------------------- GLX_SGI_make_current_read ----------------------- */

#ifndef GLX_SGI_make_current_read
#define GLX_SGI_make_current_read 1

typedef GLXDrawable ( * PFNGLXGETCURRENTREADDRAWABLESGIPROC) (void);
typedef Bool ( * PFNGLXMAKECURRENTREADSGIPROC) (Display* dpy, GLXDrawable draw, GLXDrawable read, GLXContext ctx);

#define glXGetCurrentReadDrawableSGI GLXEW_GET_FUN(G_glewXGetCurrentReadDrawableSGI)
#define glXMakeCurrentReadSGI GLXEW_GET_FUN(G_glewXMakeCurrentReadSGI)

#define GLXEW_SGI_make_current_read GLXEW_GET_VAR(__GLXEW_SGI_make_current_read)

#endif /* GLX_SGI_make_current_read */

/* -------------------------- GLX_SGI_swap_control ------------------------- */

#ifndef GLX_SGI_swap_control
#define GLX_SGI_swap_control 1

typedef int ( * PFNGLXSWAPINTERVALSGIPROC) (int interval);

#define glXSwapIntervalSGI GLXEW_GET_FUN(G_glewXSwapIntervalSGI)

#define GLXEW_SGI_swap_control GLXEW_GET_VAR(__GLXEW_SGI_swap_control)

#endif /* GLX_SGI_swap_control */

/* --------------------------- GLX_SGI_video_sync -------------------------- */

#ifndef GLX_SGI_video_sync
#define GLX_SGI_video_sync 1

typedef int ( * PFNGLXGETVIDEOSYNCSGIPROC) (unsigned int* count);
typedef int ( * PFNGLXWAITVIDEOSYNCSGIPROC) (int divisor, int remainder, unsigned int* count);

#define glXGetVideoSyncSGI GLXEW_GET_FUN(G_glewXGetVideoSyncSGI)
#define glXWaitVideoSyncSGI GLXEW_GET_FUN(G_glewXWaitVideoSyncSGI)

#define GLXEW_SGI_video_sync GLXEW_GET_VAR(__GLXEW_SGI_video_sync)

#endif /* GLX_SGI_video_sync */

/* --------------------- GLX_SUN_get_transparent_index --------------------- */

#ifndef GLX_SUN_get_transparent_index
#define GLX_SUN_get_transparent_index 1

typedef Status ( * PFNGLXGETTRANSPARENTINDEXSUNPROC) (Display* dpy, Window overlay, Window underlay, unsigned long *pTransparentIndex);

#define glXGetTransparentIndexSUN GLXEW_GET_FUN(G_glewXGetTransparentIndexSUN)

#define GLXEW_SUN_get_transparent_index GLXEW_GET_VAR(__GLXEW_SUN_get_transparent_index)

#endif /* GLX_SUN_get_transparent_index */

/* -------------------------- GLX_SUN_video_resize ------------------------- */

#ifndef GLX_SUN_video_resize
#define GLX_SUN_video_resize 1

#define GLX_VIDEO_RESIZE_SUN 0x8171
#define GL_VIDEO_RESIZE_COMPENSATION_SUN 0x85CD

typedef int ( * PFNGLXGETVIDEORESIZESUNPROC) (Display* display, GLXDrawable window, float* factor);
typedef int ( * PFNGLXVIDEORESIZESUNPROC) (Display* display, GLXDrawable window, float factor);

#define glXGetVideoResizeSUN GLXEW_GET_FUN(G_glewXGetVideoResizeSUN)
#define glXVideoResizeSUN GLXEW_GET_FUN(G_glewXVideoResizeSUN)

#define GLXEW_SUN_video_resize GLXEW_GET_VAR(__GLXEW_SUN_video_resize)

#endif /* GLX_SUN_video_resize */

/* ------------------------------------------------------------------------- */

#ifdef GLEW_MX
#define GLXEW_FUN_EXPORT
#define GLXEW_VAR_EXPORT
#else
#define GLXEW_FUN_EXPORT GLEW_FUN_EXPORT
#define GLXEW_VAR_EXPORT GLEW_VAR_EXPORT
#endif /* GLEW_MX */

GLXEW_FUN_EXPORT PFNGLXGETCURRENTDISPLAYPROC G_glewXGetCurrentDisplay;

GLXEW_FUN_EXPORT PFNGLXCHOOSEFBCONFIGPROC G_glewXChooseFBConfig;
GLXEW_FUN_EXPORT PFNGLXCREATENEWCONTEXTPROC G_glewXCreateNewContext;
GLXEW_FUN_EXPORT PFNGLXCREATEPBUFFERPROC G_glewXCreatePbuffer;
GLXEW_FUN_EXPORT PFNGLXCREATEPIXMAPPROC G_glewXCreatePixmap;
GLXEW_FUN_EXPORT PFNGLXCREATEWINDOWPROC G_glewXCreateWindow;
GLXEW_FUN_EXPORT PFNGLXDESTROYPBUFFERPROC G_glewXDestroyPbuffer;
GLXEW_FUN_EXPORT PFNGLXDESTROYPIXMAPPROC G_glewXDestroyPixmap;
GLXEW_FUN_EXPORT PFNGLXDESTROYWINDOWPROC G_glewXDestroyWindow;
GLXEW_FUN_EXPORT PFNGLXGETCURRENTREADDRAWABLEPROC G_glewXGetCurrentReadDrawable;
GLXEW_FUN_EXPORT PFNGLXGETFBCONFIGATTRIBPROC G_glewXGetFBConfigAttrib;
GLXEW_FUN_EXPORT PFNGLXGETFBCONFIGSPROC G_glewXGetFBConfigs;
GLXEW_FUN_EXPORT PFNGLXGETSELECTEDEVENTPROC G_glewXGetSelectedEvent;
GLXEW_FUN_EXPORT PFNGLXGETVISUALFROMFBCONFIGPROC G_glewXGetVisualFromFBConfig;
GLXEW_FUN_EXPORT PFNGLXMAKECONTEXTCURRENTPROC G_glewXMakeContextCurrent;
GLXEW_FUN_EXPORT PFNGLXQUERYCONTEXTPROC G_glewXQueryContext;
GLXEW_FUN_EXPORT PFNGLXQUERYDRAWABLEPROC G_glewXQueryDrawable;
GLXEW_FUN_EXPORT PFNGLXSELECTEVENTPROC G_glewXSelectEvent;

GLXEW_FUN_EXPORT PFNGLXCREATECONTEXTATTRIBSARBPROC G_glewXCreateContextAttribsARB;

GLXEW_FUN_EXPORT PFNGLXBINDTEXIMAGEATIPROC G_glewXBindTexImageATI;
GLXEW_FUN_EXPORT PFNGLXDRAWABLEATTRIBATIPROC G_glewXDrawableAttribATI;
GLXEW_FUN_EXPORT PFNGLXRELEASETEXIMAGEATIPROC G_glewXReleaseTexImageATI;

GLXEW_FUN_EXPORT PFNGLXFREECONTEXTEXTPROC G_glewXFreeContextEXT;
GLXEW_FUN_EXPORT PFNGLXGETCONTEXTIDEXTPROC G_glewXGetContextIDEXT;
GLXEW_FUN_EXPORT PFNGLXIMPORTCONTEXTEXTPROC G_glewXImportContextEXT;
GLXEW_FUN_EXPORT PFNGLXQUERYCONTEXTINFOEXTPROC G_glewXQueryContextInfoEXT;

GLXEW_FUN_EXPORT PFNGLXSWAPINTERVALEXTPROC G_glewXSwapIntervalEXT;

GLXEW_FUN_EXPORT PFNGLXBINDTEXIMAGEEXTPROC G_glewXBindTexImageEXT;
GLXEW_FUN_EXPORT PFNGLXRELEASETEXIMAGEEXTPROC G_glewXReleaseTexImageEXT;

GLXEW_FUN_EXPORT PFNGLXGETAGPOFFSETMESAPROC G_glewXGetAGPOffsetMESA;

GLXEW_FUN_EXPORT PFNGLXCOPYSUBBUFFERMESAPROC G_glewXCopySubBufferMESA;

GLXEW_FUN_EXPORT PFNGLXCREATEGLXPIXMAPMESAPROC G_glewXCreateGLXPixmapMESA;

GLXEW_FUN_EXPORT PFNGLXRELEASEBUFFERSMESAPROC G_glewXReleaseBuffersMESA;

GLXEW_FUN_EXPORT PFNGLXSET3DFXMODEMESAPROC G_glewXSet3DfxModeMESA;

GLXEW_FUN_EXPORT PFNGLXGETSWAPINTERVALMESAPROC G_glewXGetSwapIntervalMESA;
GLXEW_FUN_EXPORT PFNGLXSWAPINTERVALMESAPROC G_glewXSwapIntervalMESA;

GLXEW_FUN_EXPORT PFNGLXCOPYIMAGESUBDATANVPROC G_glewXCopyImageSubDataNV;

GLXEW_FUN_EXPORT PFNGLXBINDVIDEODEVICENVPROC G_glewXBindVideoDeviceNV;
GLXEW_FUN_EXPORT PFNGLXENUMERATEVIDEODEVICESNVPROC G_glewXEnumerateVideoDevicesNV;

GLXEW_FUN_EXPORT PFNGLXBINDSWAPBARRIERNVPROC G_glewXBindSwapBarrierNV;
GLXEW_FUN_EXPORT PFNGLXJOINSWAPGROUPNVPROC G_glewXJoinSwapGroupNV;
GLXEW_FUN_EXPORT PFNGLXQUERYFRAMECOUNTNVPROC G_glewXQueryFrameCountNV;
GLXEW_FUN_EXPORT PFNGLXQUERYMAXSWAPGROUPSNVPROC G_glewXQueryMaxSwapGroupsNV;
GLXEW_FUN_EXPORT PFNGLXQUERYSWAPGROUPNVPROC G_glewXQuerySwapGroupNV;
GLXEW_FUN_EXPORT PFNGLXRESETFRAMECOUNTNVPROC G_glewXResetFrameCountNV;

GLXEW_FUN_EXPORT PFNGLXALLOCATEMEMORYNVPROC G_glewXAllocateMemoryNV;
GLXEW_FUN_EXPORT PFNGLXFREEMEMORYNVPROC G_glewXFreeMemoryNV;

GLXEW_FUN_EXPORT PFNGLXBINDVIDEOCAPTUREDEVICENVPROC G_glewXBindVideoCaptureDeviceNV;
GLXEW_FUN_EXPORT PFNGLXENUMERATEVIDEOCAPTUREDEVICESNVPROC G_glewXEnumerateVideoCaptureDevicesNV;
GLXEW_FUN_EXPORT PFNGLXLOCKVIDEOCAPTUREDEVICENVPROC G_glewXLockVideoCaptureDeviceNV;
GLXEW_FUN_EXPORT PFNGLXQUERYVIDEOCAPTUREDEVICENVPROC G_glewXQueryVideoCaptureDeviceNV;
GLXEW_FUN_EXPORT PFNGLXRELEASEVIDEOCAPTUREDEVICENVPROC G_glewXReleaseVideoCaptureDeviceNV;

GLXEW_FUN_EXPORT PFNGLXBINDVIDEOIMAGENVPROC G_glewXBindVideoImageNV;
GLXEW_FUN_EXPORT PFNGLXGETVIDEODEVICENVPROC G_glewXGetVideoDeviceNV;
GLXEW_FUN_EXPORT PFNGLXGETVIDEOINFONVPROC G_glewXGetVideoInfoNV;
GLXEW_FUN_EXPORT PFNGLXRELEASEVIDEODEVICENVPROC G_glewXReleaseVideoDeviceNV;
GLXEW_FUN_EXPORT PFNGLXRELEASEVIDEOIMAGENVPROC G_glewXReleaseVideoImageNV;
GLXEW_FUN_EXPORT PFNGLXSENDPBUFFERTOVIDEONVPROC G_glewXSendPbufferToVideoNV;

GLXEW_FUN_EXPORT PFNGLXGETMSCRATEOMLPROC G_glewXGetMscRateOML;
GLXEW_FUN_EXPORT PFNGLXGETSYNCVALUESOMLPROC G_glewXGetSyncValuesOML;
GLXEW_FUN_EXPORT PFNGLXSWAPBUFFERSMSCOMLPROC G_glewXSwapBuffersMscOML;
GLXEW_FUN_EXPORT PFNGLXWAITFORMSCOMLPROC G_glewXWaitForMscOML;
GLXEW_FUN_EXPORT PFNGLXWAITFORSBCOMLPROC G_glewXWaitForSbcOML;

GLXEW_FUN_EXPORT PFNGLXCHOOSEFBCONFIGSGIXPROC G_glewXChooseFBConfigSGIX;
GLXEW_FUN_EXPORT PFNGLXCREATECONTEXTWITHCONFIGSGIXPROC G_glewXCreateContextWithConfigSGIX;
GLXEW_FUN_EXPORT PFNGLXCREATEGLXPIXMAPWITHCONFIGSGIXPROC G_glewXCreateGLXPixmapWithConfigSGIX;
GLXEW_FUN_EXPORT PFNGLXGETFBCONFIGATTRIBSGIXPROC G_glewXGetFBConfigAttribSGIX;
GLXEW_FUN_EXPORT PFNGLXGETFBCONFIGFROMVISUALSGIXPROC G_glewXGetFBConfigFromVisualSGIX;
GLXEW_FUN_EXPORT PFNGLXGETVISUALFROMFBCONFIGSGIXPROC G_glewXGetVisualFromFBConfigSGIX;

GLXEW_FUN_EXPORT PFNGLXBINDHYPERPIPESGIXPROC G_glewXBindHyperpipeSGIX;
GLXEW_FUN_EXPORT PFNGLXDESTROYHYPERPIPECONFIGSGIXPROC G_glewXDestroyHyperpipeConfigSGIX;
GLXEW_FUN_EXPORT PFNGLXHYPERPIPEATTRIBSGIXPROC G_glewXHyperpipeAttribSGIX;
GLXEW_FUN_EXPORT PFNGLXHYPERPIPECONFIGSGIXPROC G_glewXHyperpipeConfigSGIX;
GLXEW_FUN_EXPORT PFNGLXQUERYHYPERPIPEATTRIBSGIXPROC G_glewXQueryHyperpipeAttribSGIX;
GLXEW_FUN_EXPORT PFNGLXQUERYHYPERPIPEBESTATTRIBSGIXPROC G_glewXQueryHyperpipeBestAttribSGIX;
GLXEW_FUN_EXPORT PFNGLXQUERYHYPERPIPECONFIGSGIXPROC G_glewXQueryHyperpipeConfigSGIX;
GLXEW_FUN_EXPORT PFNGLXQUERYHYPERPIPENETWORKSGIXPROC G_glewXQueryHyperpipeNetworkSGIX;

GLXEW_FUN_EXPORT PFNGLXCREATEGLXPBUFFERSGIXPROC G_glewXCreateGLXPbufferSGIX;
GLXEW_FUN_EXPORT PFNGLXDESTROYGLXPBUFFERSGIXPROC G_glewXDestroyGLXPbufferSGIX;
GLXEW_FUN_EXPORT PFNGLXGETSELECTEDEVENTSGIXPROC G_glewXGetSelectedEventSGIX;
GLXEW_FUN_EXPORT PFNGLXQUERYGLXPBUFFERSGIXPROC G_glewXQueryGLXPbufferSGIX;
GLXEW_FUN_EXPORT PFNGLXSELECTEVENTSGIXPROC G_glewXSelectEventSGIX;

GLXEW_FUN_EXPORT PFNGLXBINDSWAPBARRIERSGIXPROC G_glewXBindSwapBarrierSGIX;
GLXEW_FUN_EXPORT PFNGLXQUERYMAXSWAPBARRIERSSGIXPROC G_glewXQueryMaxSwapBarriersSGIX;

GLXEW_FUN_EXPORT PFNGLXJOINSWAPGROUPSGIXPROC G_glewXJoinSwapGroupSGIX;

GLXEW_FUN_EXPORT PFNGLXBINDCHANNELTOWINDOWSGIXPROC G_glewXBindChannelToWindowSGIX;
GLXEW_FUN_EXPORT PFNGLXCHANNELRECTSGIXPROC G_glewXChannelRectSGIX;
GLXEW_FUN_EXPORT PFNGLXCHANNELRECTSYNCSGIXPROC G_glewXChannelRectSyncSGIX;
GLXEW_FUN_EXPORT PFNGLXQUERYCHANNELDELTASSGIXPROC G_glewXQueryChannelDeltasSGIX;
GLXEW_FUN_EXPORT PFNGLXQUERYCHANNELRECTSGIXPROC G_glewXQueryChannelRectSGIX;

GLXEW_FUN_EXPORT PFNGLXCUSHIONSGIPROC G_glewXCushionSGI;

GLXEW_FUN_EXPORT PFNGLXGETCURRENTREADDRAWABLESGIPROC G_glewXGetCurrentReadDrawableSGI;
GLXEW_FUN_EXPORT PFNGLXMAKECURRENTREADSGIPROC G_glewXMakeCurrentReadSGI;

GLXEW_FUN_EXPORT PFNGLXSWAPINTERVALSGIPROC G_glewXSwapIntervalSGI;

GLXEW_FUN_EXPORT PFNGLXGETVIDEOSYNCSGIPROC G_glewXGetVideoSyncSGI;
GLXEW_FUN_EXPORT PFNGLXWAITVIDEOSYNCSGIPROC G_glewXWaitVideoSyncSGI;

GLXEW_FUN_EXPORT PFNGLXGETTRANSPARENTINDEXSUNPROC G_glewXGetTransparentIndexSUN;

GLXEW_FUN_EXPORT PFNGLXGETVIDEORESIZESUNPROC G_glewXGetVideoResizeSUN;
GLXEW_FUN_EXPORT PFNGLXVIDEORESIZESUNPROC G_glewXVideoResizeSUN;

#if defined(GLEW_MX)
struct GLXEWContextStruct
{
#endif /* GLEW_MX */

GLXEW_VAR_EXPORT GLboolean __GLXEW_VERSION_1_0;
GLXEW_VAR_EXPORT GLboolean __GLXEW_VERSION_1_1;
GLXEW_VAR_EXPORT GLboolean __GLXEW_VERSION_1_2;
GLXEW_VAR_EXPORT GLboolean __GLXEW_VERSION_1_3;
GLXEW_VAR_EXPORT GLboolean __GLXEW_VERSION_1_4;
GLXEW_VAR_EXPORT GLboolean __GLXEW_3DFX_multisample;
GLXEW_VAR_EXPORT GLboolean __GLXEW_AMD_gpu_association;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_create_context;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_create_context_profile;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_create_context_robustness;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_fbconfig_float;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_framebuffer_sRGB;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_get_proc_address;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_multisample;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_robustness_application_isolation;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_robustness_share_group_isolation;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ARB_vertex_buffer_object;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ATI_pixel_format_float;
GLXEW_VAR_EXPORT GLboolean __GLXEW_ATI_render_texture;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_create_context_es2_profile;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_create_context_es_profile;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_fbconfig_packed_float;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_framebuffer_sRGB;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_import_context;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_scene_marker;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_swap_control;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_swap_control_tear;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_texture_from_pixmap;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_visual_info;
GLXEW_VAR_EXPORT GLboolean __GLXEW_EXT_visual_rating;
GLXEW_VAR_EXPORT GLboolean __GLXEW_INTEL_swap_event;
GLXEW_VAR_EXPORT GLboolean __GLXEW_MESA_agp_offset;
GLXEW_VAR_EXPORT GLboolean __GLXEW_MESA_copy_sub_buffer;
GLXEW_VAR_EXPORT GLboolean __GLXEW_MESA_pixmap_colormap;
GLXEW_VAR_EXPORT GLboolean __GLXEW_MESA_release_buffers;
GLXEW_VAR_EXPORT GLboolean __GLXEW_MESA_set_3dfx_mode;
GLXEW_VAR_EXPORT GLboolean __GLXEW_MESA_swap_control;
GLXEW_VAR_EXPORT GLboolean __GLXEW_NV_copy_image;
GLXEW_VAR_EXPORT GLboolean __GLXEW_NV_float_buffer;
GLXEW_VAR_EXPORT GLboolean __GLXEW_NV_multisample_coverage;
GLXEW_VAR_EXPORT GLboolean __GLXEW_NV_present_video;
GLXEW_VAR_EXPORT GLboolean __GLXEW_NV_swap_group;
GLXEW_VAR_EXPORT GLboolean __GLXEW_NV_vertex_array_range;
GLXEW_VAR_EXPORT GLboolean __GLXEW_NV_video_capture;
GLXEW_VAR_EXPORT GLboolean __GLXEW_NV_video_out;
GLXEW_VAR_EXPORT GLboolean __GLXEW_OML_swap_method;
GLXEW_VAR_EXPORT GLboolean __GLXEW_OML_sync_control;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIS_blended_overlay;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIS_color_range;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIS_multisample;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIS_shared_multisample;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIX_fbconfig;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIX_hyperpipe;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIX_pbuffer;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIX_swap_barrier;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIX_swap_group;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIX_video_resize;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGIX_visual_select_group;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGI_cushion;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGI_make_current_read;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGI_swap_control;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SGI_video_sync;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SUN_get_transparent_index;
GLXEW_VAR_EXPORT GLboolean __GLXEW_SUN_video_resize;

#ifdef GLEW_MX
}; /* GLXEWContextStruct */
#endif /* GLEW_MX */

/* ------------------------------------------------------------------------ */

#ifdef GLEW_MX

typedef struct GLXEWContextStruct GLXEWContext;
GLEWAPI GLenum GLEWAPIENTRY glxewContextInit (GLXEWContext *ctx);
GLEWAPI GLboolean GLEWAPIENTRY glxewContextIsSupported (const GLXEWContext *ctx, const char *name);

#define glxewInit() glxewContextInit(glxewGetContext())
#define glxewIsSupported(x) glxewContextIsSupported(glxewGetContext(), x)

#define GLXEW_GET_VAR(x) (*(const GLboolean*)&(glxewGetContext()->x))
#define GLXEW_GET_FUN(x) x

#else /* GLEW_MX */

#define GLXEW_GET_VAR(x) (*(const GLboolean*)&x)
#define GLXEW_GET_FUN(x) x

GLEWAPI GLboolean GLEWAPIENTRY glxewIsSupported (const char *name);

#endif /* GLEW_MX */

GLEWAPI GLboolean GLEWAPIENTRY glxewGetExtension (const char *name);

#ifdef __cplusplus
}
#endif

#endif /* __glxew_h__ */
