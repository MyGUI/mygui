#version 150
in vec4 Color;
in vec2 TexCoord;
uniform sampler2D Texture;
out vec4 fragColor;
void main(void)
{
	fragColor = texture(Texture, TexCoord) * Color;
}
