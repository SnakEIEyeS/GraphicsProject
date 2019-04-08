#version 330 core

layout(location = 0) in vec3 i_VertexPos;
layout(location = 1) in vec3 i_VertexNormal;
layout(location = 2) in vec3 i_VertexTexture;
layout(location = 3) in vec3 i_VertexTangent;
layout(location = 4) in vec3 i_VertexBitangent;

uniform mat4 u_Projection;
/*uniform vec3 u_LightPosition;*/
uniform mat4 u_Camera;
uniform mat4 u_Object;

out mat3 o_TBNMat_CameraSpace;
out vec4 o_VertexPositionToFS_ModelSpace;
out vec4 o_VertexNormalToFS_ModelSpace;
//out vec3 o_VertexTangentToFS_ModelSpace;
//out vec3 o_VertexBitangentToFS_ModelSpace;
out vec3 o_VertexTextureToFS_ModelSpace;

void main()
{
	gl_Position = u_Projection * u_Camera * u_Object * vec4(i_VertexPos, 1.f);

	//o_VertexPosition_ModelSpace = u_Projection * u_Camera * u_Object * vec4(i_VertexPos, 1.f);

	o_VertexPositionToFS_ModelSpace = vec4(i_VertexPos, 1.f);
	o_VertexNormalToFS_ModelSpace = vec4(i_VertexNormal, 0.f);
	o_VertexTextureToFS_ModelSpace = i_VertexTexture;

	//o_VertexTangentToFS_ModelSpace = i_VertexTangent;
	//o_VertexBitangentToFS_ModelSpace = i_VertexBitangent;

	vec3 VertexNormal_CameraSpace = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(i_VertexNormal, 0.f)));
	vec3 Tangent = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(i_VertexTangent, 0.f)));
	vec3 Bitangent = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(i_VertexBitangent, 0.f)));
	o_TBNMat_CameraSpace = mat3(Tangent, Bitangent, VertexNormal_CameraSpace);
}