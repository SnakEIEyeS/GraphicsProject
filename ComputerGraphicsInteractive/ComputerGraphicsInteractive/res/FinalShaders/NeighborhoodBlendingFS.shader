#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform sampler2D u_ColorTexSampler;
uniform sampler2D u_BlendWeightTexSampler;


in vec3 o_VertexTexture;

void main()
{
	/*vec4 sampledDiffuseColor = texture(u_ColorTexSampler, vec2(o_VertexTexture));

	vec4 FinalDiffuseColor = sampledDiffuseColor;

	o_ColorFS = sampledDiffuseColor;*/

	vec4 FragBlendWeight = texture(u_BlendWeightTexSampler, vec2(o_VertexTexture));

	const ivec2 BottomOffset = ivec2(0, -1);
	float BottomFragBlendWeight = textureOffset(u_BlendWeightTexSampler, vec2(o_VertexTexture), BottomOffset).g;

	const ivec2 RightOffset = ivec2(1, 0);
	float RightFragBlendWeight = textureOffset(u_BlendWeightTexSampler, vec2(o_VertexTexture), RightOffset).a;

	vec4 FourNeighborSampledArea = vec4(FragBlendWeight.x, BottomFragBlendWeight, FragBlendWeight.z, RightFragBlendWeight);
	vec4 WeightedFourNeighborBlendValues = FourNeighborSampledArea * FourNeighborSampledArea * FourNeighborSampledArea;

	float SumBlendValues = dot(WeightedFourNeighborBlendValues, vec4(1.f));

	if (SumBlendValues > 0.f)
	{
		o_ColorFS = vec4(0.f, 0.f, 0.f, 0.f);
		//Add Top color
		o_ColorFS += texture(u_ColorTexSampler, vec2(o_VertexTexture) + vec2(0.f, FourNeighborSampledArea.x)) * WeightedFourNeighborBlendValues.x;
		//Add Bottom color
		o_ColorFS += texture(u_ColorTexSampler, vec2(o_VertexTexture) + vec2(0.f, -FourNeighborSampledArea.y)) * WeightedFourNeighborBlendValues.y;
		//Add Left color
		o_ColorFS += texture(u_ColorTexSampler, vec2(o_VertexTexture) + vec2(-FourNeighborSampledArea.z, 0.f)) * WeightedFourNeighborBlendValues.z;
		//Add Right color
		o_ColorFS += texture(u_ColorTexSampler, vec2(o_VertexTexture) + vec2(FourNeighborSampledArea.w, 0.f)) * WeightedFourNeighborBlendValues.w;

		o_ColorFS /= SumBlendValues;
	}
	else
	{
		o_ColorFS = texture(u_ColorTexSampler, vec2(o_VertexTexture));
	}

}