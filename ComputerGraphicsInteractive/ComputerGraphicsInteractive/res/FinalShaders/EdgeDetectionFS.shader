#version 410 core

layout(location = 0) out vec4 o_ColorFS;

uniform sampler2D u_RenderToSampler;


in vec3 o_VertexTexture;

void main()
{
	vec4 FragSampledColor = texture(u_RenderToSampler, vec2(o_VertexTexture));
	float FragLuminance = 0.2126 * FragSampledColor.x + 0.7152 * FragSampledColor.y + 0.0722 * FragSampledColor.z;

	const ivec2 LeftOffset = ivec2(-1, 0);
	vec4 FragLeftSampledColor = textureOffset(u_RenderToSampler, vec2(o_VertexTexture), LeftOffset);
	float FragLeftLuminance = 0.2126 * FragLeftSampledColor.x + 0.7152 * FragLeftSampledColor.y + 0.0722 * FragLeftSampledColor.z;

	const ivec2 TopOffset = ivec2(0, 1);
	vec4 FragTopSampledColor = textureOffset(u_RenderToSampler, vec2(o_VertexTexture), TopOffset);
	float FragTopLuminance = 0.2126 * FragTopSampledColor.x + 0.7152 * FragTopSampledColor.y + 0.0722 * FragTopSampledColor.z;

	vec2 Delta = abs(vec2(FragLuminance, FragLuminance) - vec2(FragLeftLuminance, FragTopLuminance));
	const float EdgeThreshold = 0.1f;
	vec2 Edges = step(EdgeThreshold, Delta);

	if (dot(Edges, vec2(1.f)) == 0.f)
	{
		discard;
	}

	o_ColorFS = vec4(Edges, 0.f, 1.f);
}


/* 

********* References ********

GPU Pro 2 - for Luminance formula and dot product to filter out presence or absence of edges

*/


