#version 450

layout(location = 0) in vec3 VertexPosition;
layout(location = 1) in vec4 VertexColor;
layout(location = 2) in vec2 VertexTexCoord;

layout(location = 0) out vec4 vColor;
layout(location = 1) out vec2 vTexCoord;

void main()
{
	vTexCoord = VertexTexCoord;
	vColor = VertexColor;

	// MyGUI provides vertices in NDC with Y pointing up, while Vulkan NDC has Y pointing down.
	gl_Position = vec4(VertexPosition.x, -VertexPosition.y, VertexPosition.z, 1.0);
}
