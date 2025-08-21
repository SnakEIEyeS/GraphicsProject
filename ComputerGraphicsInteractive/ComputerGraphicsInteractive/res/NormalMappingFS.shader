#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform samplerCube u_CubeMapSampler;
uniform sampler2D u_NormalMapSampler;
uniform sampler2D u_DiffuseTextureSampler;
uniform sampler2D u_SpecularTextureSampler;

uniform mat4 u_Projection;
uniform mat4 u_Camera;
uniform mat4 u_Object;
uniform vec3 u_LightPosition;
uniform vec3 u_CameraPosition;
uniform float u_SpecularExponent;

in mat3 o_TBNMat_CameraSpace;
in vec4 o_VertexPositionToFS_ModelSpace;
in vec4 o_VertexNormalToFS_ModelSpace;
in vec3 o_VertexTextureToFS_ModelSpace;


void main()
{
	vec4 VertexPosition_CameraSpace = u_Camera * u_Object * o_VertexPositionToFS_ModelSpace;
	vec4 LightPosition_CameraSpace = u_Camera * vec4(u_LightPosition, 1.f);
	vec4 LightDirection_CameraSpace = normalize(vec4(vec3(LightPosition_CameraSpace - VertexPosition_CameraSpace), 0.f));
	//vec4 LightDirection_CameraSpace = -normalize(u_Camera * vec4 (u_LightPosition, 0.f));
	vec4 ViewDirection_CameraSpace = -normalize(u_Camera * u_Object * o_VertexPositionToFS_ModelSpace);
	//vec4 VertexNormal_CameraSpace = transpose(inverse(u_Camera)) * u_Object * vec4(i_Normal, 0.f);
	
	vec3 mappedNormal = texture(u_NormalMapSampler, vec2(o_VertexTextureToFS_ModelSpace)).rgb;
	mappedNormal = normalize(mappedNormal * 2.f - 1.f);
	mappedNormal = normalize(o_TBNMat_CameraSpace * mappedNormal);

	float cosTheta = clamp(dot(LightDirection_CameraSpace, vec4(mappedNormal, 0.f)), 0.f, 1.f);
	vec4 HalfVector = normalize(vec4(vec3(ViewDirection_CameraSpace + LightDirection_CameraSpace), 0.f));
	float SpecularComponent = dot(HalfVector, vec4(mappedNormal, 0.f));
	

	/*vec3 ViewDirection = normalize(u_CameraPosition - vec3(o_VertexPositionToFS_ModelSpace));
	//vec4 ViewDirection_CameraSpace = -normalize(u_Camera * u_Object * o_VertexPositionToFS_ModelSpace);
	vec3 ReflectionVector = vec3(ViewDirection) + 2.f * dot(vec3(ViewDirection), Normal) * Normal;
	vec4 ReflectionColor = vec4(texture(u_CubeMapSampler, -ReflectionVector).rgb, 1.f);*/

	vec4 sampledDiffuseColor = texture(u_DiffuseTextureSampler, vec2(o_VertexTextureToFS_ModelSpace));
	vec4 FinalDiffuseColor = vec4(vec3(sampledDiffuseColor) * cosTheta, 1.f);

	vec4 sampledSpecularColor = texture(u_SpecularTextureSampler, vec2(o_VertexTextureToFS_ModelSpace));
	vec4 FinalSpecularColor = vec4(vec3(sampledSpecularColor) * pow(SpecularComponent, u_SpecularExponent), 1.f);

	//o_ColorFS = ReflectionColor + FinalSpecularColor;
	//o_ColorFS = ReflectionColor;
	o_ColorFS = FinalDiffuseColor + FinalSpecularColor;
}