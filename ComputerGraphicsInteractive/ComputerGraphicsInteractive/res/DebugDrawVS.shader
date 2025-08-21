#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec3 i_VertexTexture;

uniform mat4 u_Projection;
uniform mat4 u_Camera;
uniform mat4 u_Object;

//out vec4 o_VertexPosition;

void main()
{
	gl_Position = u_Projection * u_Camera * u_Object * vec4(i_Pos, 1.f);
}