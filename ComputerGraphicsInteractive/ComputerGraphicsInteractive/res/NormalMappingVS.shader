#version 330 core

layout(location = 0) in vec3 i_VertexPos;
layout(location = 1) in vec3 i_VertexNormal;
layout(location = 2) in vec3 i_VertexTexture;
layout(location = 3) in vec3 i_VertexTangent;
layout(location = 4) in vec3 i_VertexBitangent;

/*uniform mat4 u_Projection;
uniform mat4 u_Camera;
uniform mat4 u_Object;
uniform vec3 u_LightPosition;*/

//out mat3 o_TBNMat;
out vec3 o_VertexPositionVS_ModelSpace;
out vec3 o_VertexNormalVS_ModelSpace;
out vec3 o_VertexTangentVS_ModelSpace;
out vec3 o_VertexBitangentVS_ModelSpace;
out vec3 o_VertexTextureVS_ModelSpace;

void main()
{
	//gl_Position = u_Projection * u_Camera * u_Object * vec4(i_Pos, 1.f);

	//o_VertexPosition_ModelSpace = u_Projection * u_Camera * u_Object * vec4(i_VertexPos, 1.f);
	
	o_VertexPositionVS_ModelSpace = i_VertexPos;
	o_VertexNormalVS_ModelSpace = i_VertexNormal;
	o_VertexTextureVS_ModelSpace = i_VertexTexture;

	o_VertexTangentVS_ModelSpace = i_VertexTangent;
	o_VertexBitangentVS_ModelSpace = i_VertexBitangent;

	/*vec3 Tangent = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(i_VertexTangent, 0.f)));
	vec3 Bitangent = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(i_VertexBitangent, 0.f)));
	o_TBNMat = mat3(Tangent, Bitangent, VertexNormal_CameraSpace);*/
}