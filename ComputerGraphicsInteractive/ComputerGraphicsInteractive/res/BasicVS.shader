#version 330 core

layout(location = 0) in vec3 i_Pos;

uniform mat4 u_Projection;
uniform mat4 u_Camera;
out vec4 o_ColorVS;

void main()
{
	gl_Position = u_Projection * u_Camera * vec4(i_Pos, 1.f);
	//gl_Position = vec4(i_Pos.x, i_Pos.y, i_Pos.z, 1.f);
	//vec3 temp = 0.05f * i_Pos;
	//gl_Position = vec4(temp, 1.f);
	o_ColorVS = vec4(1.f, 0.f, 0.f, 1.0f);
}