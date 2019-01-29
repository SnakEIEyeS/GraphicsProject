#version 330 core

layout(location = 0) in vec3 i_Pos;

uniform mat4 u_Transformation;
out vec4 o_ColorVS;

void main()
{
	gl_Position = u_Transformation * vec4(i_Pos, 1);
	//vec3 temp = 0.05f * i_Pos;
	//gl_Position = vec4(temp, 1.f);
	o_ColorVS = vec4(0.5f, 0.5f, 0.5f, 1.0f);
}