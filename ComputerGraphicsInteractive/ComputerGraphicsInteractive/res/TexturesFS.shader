#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform vec4 u_DiffuseColor;
uniform vec4 u_SpecularColor;
uniform float u_AmbientConstant;
uniform float u_SpecularExponent;

in vec4 o_ColorVS;
in float o_cosTheta;
in float o_SpecularComponent;

void main()
{
	//o_ColorFS = o_ColorVS;
	vec4 AmbientColor = vec4(u_AmbientConstant * vec3(u_DiffuseColor), 1.f);
	vec4 FinalDiffuseColor = vec4(vec3(u_DiffuseColor) * o_cosTheta, 1.f);
	vec4 FinalSpecularColor = vec4(vec3(u_SpecularColor) * pow(o_SpecularComponent, u_SpecularExponent), 1.f);

	o_ColorFS = AmbientColor + FinalDiffuseColor + FinalSpecularColor;
	//o_ColorFS = FinalSpecularColor;
	//o_ColorFS = AmbientColor + FinalDiffuseColor;
	//o_ColorFS = FinalDiffuseColor;
}
