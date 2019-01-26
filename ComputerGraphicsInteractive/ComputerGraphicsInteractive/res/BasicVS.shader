#version 330 core

layout(location = 0) in vec3 i_Pos;

uniform mat4 u_Transformation;
out vec4 o_ColorVS;

void main()
{
	glPosition = u_Transformation * vec4(i_Pos, 1);
	o_ColorVS = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}