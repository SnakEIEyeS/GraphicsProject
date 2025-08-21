#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec3 i_VertexTexture;

/*uniform mat4 u_PlaneProjection;
uniform mat4 u_PlaneCamera;
uniform mat4 u_PlaneObject;*/

out vec3 o_VertexTexture;

void main()
{
	gl_Position = vec4(i_Pos, 1.f);

	o_VertexTexture = i_VertexTexture;
}
