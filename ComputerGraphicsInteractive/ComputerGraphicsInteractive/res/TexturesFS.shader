#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform sampler2D u_AmbientTextureSampler;
uniform sampler2D u_DiffuseTextureSampler;
uniform sampler2D u_SpecularTextureSampler;

//uniform vec4 u_DiffuseColor;
//uniform vec4 u_SpecularColor;
uniform float u_AmbientConstant;
uniform float u_SpecularExponent;

in vec4 o_ColorVS;
in vec3 o_VertexTexture;
in float o_cosTheta;
in float o_SpecularComponent;

void main()
{
	//o_ColorFS = o_ColorVS;
	vec4 sampledAmbientColor = texture(u_AmbientTextureSampler, vec2(o_VertexTexture));
	vec4 sampledDiffuseColor = texture(u_DiffuseTextureSampler, vec2(o_VertexTexture));
	vec4 sampledSpecularColor = texture(u_SpecularTextureSampler, vec2(o_VertexTexture));

	vec4 FinalAmbientColor = vec4(u_AmbientConstant * vec3(sampledAmbientColor), 1.f);
	vec4 FinalDiffuseColor = vec4(vec3(sampledDiffuseColor) * o_cosTheta, 1.f);
	vec4 FinalSpecularColor = vec4(vec3(sampledSpecularColor) * pow(o_SpecularComponent, u_SpecularExponent), 1.f);

	o_ColorFS = FinalAmbientColor + FinalDiffuseColor + FinalSpecularColor;
	//o_ColorFS = FinalSpecularColor;
	//o_ColorFS = FinalAmbientColor + FinalDiffuseColor;
	//o_ColorFS = FinalDiffuseColor;
	//o_ColorFS = FinalAmbientColor;
}


/*References:
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/ for Ambient Color 'calculation'
*/
