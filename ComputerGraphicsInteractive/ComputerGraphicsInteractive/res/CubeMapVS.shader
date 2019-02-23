#version 330 core

layout(location = 0) in vec3 i_Pos;
//layout(location = 1) in vec3 i_VertexTexture;

uniform mat4 u_Projection;
uniform mat4 u_Camera;
uniform mat4 u_CubeObject;

out vec3 o_VertexTexture;

void main()
{
	gl_Position = u_Projection * u_Camera * u_CubeObject * vec4(i_Pos, 1.f);

	o_VertexTexture = vec3(i_Pos.x / abs(i_Pos.x), i_Pos.y / abs(i_Pos.y), i_Pos.z / abs(i_Pos.z));
}
