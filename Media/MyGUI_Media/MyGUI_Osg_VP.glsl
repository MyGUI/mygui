#version 130

// Vertex program for the OSG render system. The vertex data is provided by the
// fixed-function pipeline (glVertexPointer/glColorPointer/glTexCoordPointer in
// osgDrawBatches), so the shader consumes the conventional built-in attributes
// and matrices. gl_ModelViewProjectionMatrix is the identity for the GUI camera
// and flips Y for the render-to-texture camera, matching the fixed-function path.
// GLSL 1.30 is used (instead of the 1.50 used by the OpenGL3 backend) so that the
// fixed-function built-ins are available regardless of the context profile.

varying vec4 Color;
varying vec2 TexCoord;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	Color = gl_Color;
	TexCoord = gl_MultiTexCoord0.xy;
}
