#version 330 core

layout(location = 0) in vec3 i_Pos;
layout(location = 1) in vec3 i_Normal;
layout(location = 2) in vec3 i_VertexTexture;
layout(location = 3) in vec3 i_VertexTangent;
layout(location = 4) in vec3 i_VertexBitangent;

uniform mat4 u_Projection;
uniform mat4 u_Camera;
uniform mat4 u_Object;
uniform vec3 u_LightPosition;

out mat3 o_TBNMat;
out vec4 o_ColorVS;
out vec4 o_VertexPosition;
out vec4 o_VertexNormal;
out vec3 o_VertexTexture;

void main()
{
	gl_Position = u_Projection * u_Camera * u_Object * vec4(i_Pos, 1.f);

	vec4 VertexPosition_CameraSpace = u_Camera * u_Object * vec4(i_Pos, 1.f);
	vec4 LightPosition_CameraSpace = u_Camera * vec4(u_LightPosition, 1.f);
	vec4 LightDirection_CameraSpace = normalize(vec4(vec3(LightPosition_CameraSpace - VertexPosition_CameraSpace), 0.f));
	//vec4 LightDirection_CameraSpace = -normalize(u_Camera * vec4(u_LightPosition, 0.f));
	vec4 ViewDirection_CameraSpace = -normalize(u_Camera * u_Object * vec4(i_Pos, 1.f));
	vec4 VertexNormal_CameraSpace = transpose(inverse(u_Camera)) * u_Object * vec4(i_Normal, 0.f);

	//o_ColorVS = vec4(1.f, 0.f, 0.f, 1.0f);
	o_ColorVS = transpose(inverse(u_Camera)) * u_Object * vec4(i_Normal, 1.f);

	o_VertexPosition = u_Projection * u_Camera * u_Object * vec4(i_Pos, 1.f);
	o_VertexNormal = vec4(i_Normal, 0.f);


	o_VertexTexture = i_VertexTexture;

	vec3 Tangent = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(i_VertexTangent, 0.f)));
	vec3 Bitangent = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(i_VertexBitangent, 0.f)));
	o_TBNMat = mat3(Tangent, Bitangent, VertexNormal_CameraSpace);
}