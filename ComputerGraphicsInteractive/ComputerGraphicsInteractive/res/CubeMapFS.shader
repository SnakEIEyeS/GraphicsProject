#version 330 core

layout(location = 0) out vec4 o_ColorFS;
//out vec4 FragColor;

uniform samplerCube u_CubeMapSampler;


in vec3 o_VertexTexture;

void main()
{
	vec4 sampledCubeColor = texture(u_CubeMapSampler, o_VertexTexture);

	//FragColor = sampledCubeColor;
	o_ColorFS = sampledCubeColor;

	o_ColorFS = vec4(1.f, 0.f, 0.f, 1.f);
}

