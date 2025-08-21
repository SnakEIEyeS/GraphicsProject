#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform sampler2D u_RenderToSampler;


in vec3 o_VertexTexture;

void main()
{
	vec4 sampledDiffuseColor = texture(u_RenderToSampler, vec2(o_VertexTexture));

	//vec4 FinalDiffuseColor = vec4(vec3(sampledDiffuseColor) * o_cosTheta, 1.f);
	vec4 FinalDiffuseColor = sampledDiffuseColor;

	o_ColorFS = sampledDiffuseColor;
	//o_ColorFS = FinalDiffuseColor;

	//o_ColorFS = vec4(1.f, 1.f, 1.f, 1.f);
}


/*References:
http://www.opengl-tutorial.org/beginners-tutorials/tutorial-8-basic-shading/ for Ambient Color 'calculation'
*/

