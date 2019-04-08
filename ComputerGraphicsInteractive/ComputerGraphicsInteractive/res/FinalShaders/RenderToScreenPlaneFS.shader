#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform sampler2D u_RenderToSampler;


in vec3 o_VertexTexture;

void main()
{
	vec4 sampledDiffuseColor = texture(u_RenderToSampler, vec2(o_VertexTexture));

	vec4 FinalDiffuseColor = sampledDiffuseColor;

	o_ColorFS = sampledDiffuseColor;

}

