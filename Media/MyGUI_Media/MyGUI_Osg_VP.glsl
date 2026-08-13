#version 150

precision highp int;
precision highp float;

in vec4 osg_Vertex;
in vec4 osg_Color;
in vec2 osg_MultiTexCoord0;

uniform mat4 osg_ModelViewProjectionMatrix;

out vec4 Color;
out vec2 TexCoord;

void main()
{
	gl_Position = osg_ModelViewProjectionMatrix * osg_Vertex;
	Color = osg_Color;
	TexCoord = osg_MultiTexCoord0.xy;
}
