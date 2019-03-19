#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform samplerCube u_CubeMapSampler;
uniform sampler2D u_SpecularTextureSampler;

uniform mat4 u_Camera;
uniform mat4 u_Object;
uniform vec3 u_CameraPosition;
uniform float u_SpecularExponent;

in vec4 o_ColorVS;
in vec4 o_VertexPosition;
in vec4 o_VertexNormal;
in vec3 o_VertexTexture;
in float o_cosTheta;
in float o_SpecularComponent;

void main()
{
	vec3 ViewDirection = normalize(u_CameraPosition - vec3(o_VertexPosition));
	//vec4 ViewDirection_CameraSpace = -normalize(u_Camera * u_Object * o_VertexPosition);
	vec3 Normal = normalize(vec3(o_VertexNormal));
	vec3 ReflectionVector = vec3(ViewDirection) + 2.f * dot(vec3(ViewDirection), Normal) * Normal;
	vec4 ReflectionColor = vec4(texture(u_CubeMapSampler, -ReflectionVector).rgb, 1.f);

	vec4 sampledSpecularColor = texture(u_SpecularTextureSampler, vec2(o_VertexTexture));
	vec4 FinalSpecularColor = vec4(vec3(sampledSpecularColor) * pow(o_SpecularComponent, u_SpecularExponent), 1.f);
	
	//o_ColorFS = ReflectionColor + FinalSpecularColor;
	o_ColorFS = ReflectionColor;
}