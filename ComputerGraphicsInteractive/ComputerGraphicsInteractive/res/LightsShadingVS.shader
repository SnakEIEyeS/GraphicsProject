#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;

uniform mat4 u_Projection;
uniform mat4 u_Camera;
out vec4 o_ColorVS;

void main()
{
	gl_Position = u_Projection * u_Camera * vec4(i_Pos, 1.f);
	
	//o_ColorVS = vec4(1.f, 0.f, 0.f, 1.0f);
	o_ColorVS = transpose(inverse(u_Camera)) * vec4(i_Normal, 1.f);
}