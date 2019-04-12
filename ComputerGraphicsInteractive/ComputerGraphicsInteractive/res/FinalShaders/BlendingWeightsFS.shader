#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform sampler2D u_EdgesTexSampler;


in vec3 o_VertexTexture;

const int MaxSearchSteps = 4;
const vec2 StepSizeX = vec2(2.f, 0.f);
const vec2 StepSizeY = vec2(0.f, 2.f);
const float StartSearchOffset = 1.5f;
const float EdgelExistenceRequisite = 0.9f;

float SearchXLeft(vec2 i_VertexTexture);
float SearchXRight(vec2 i_VertexTexture);
float SearchYUp(vec2 i_VertexTexture);
float SearchYDown(vec2 i_VertexTexture);

void main()
{
	vec4 sampledDiffuseColor = texture(u_EdgesTexSampler, vec2(o_VertexTexture));

	vec4 FinalDiffuseColor = sampledDiffuseColor;

	o_ColorFS = sampledDiffuseColor;

	vec4 BlendingWeights = vec4(0.f, 0.f, 0.f, 0.f);
	vec2 FragEdgeData = texture(u_EdgesTexSampler, vec2(o_VertexTexture)).rg;

	if (FragEdgeData.g > 0.f)
	{
		float DistanceToEdgeLeft = SearchXLeft(vec2(o_VertexTexture));
		float DistanceToEdgeRight = SearchXRight(vec2(o_VertexTexture));
		vec2 EdgeCoordsLeft = vec2(o_VertexTexture) + vec2(DistanceToEdgeLeft, -0.25f);
		vec2 EdgeCoordsRight = vec2(o_VertexTexture) + vec2(DistanceToEdgeRight + 1.f, -0.25f);

		vec2 EdgeLeftData = texture(u_EdgesTexSampler, EdgeCoordsLeft).rg;
		vec2 EdgeRightData = texture(u_EdgesTexSampler, EdgeCoordsRight).rg;
	}

	if (FragEdgeData.r > 0.f)
	{
		float DistanceToEdgeUp = SearchYUp(vec2(o_VertexTexture));
		float DistanceToEdgeDown = SearchYDown(vec2(o_VertexTexture));
		vec2 EdgeCoordsUp = vec2(o_VertexTexture) + vec2(-0.25f, DistanceToEdgeUp);
		vec2 EdgeCoordsDown = vec2(o_VertexTexture) + vec2(-0.25f, DistanceToEdgeDown + 1.f);

		vec2 EdgeUpData = texture(u_EdgesTexSampler, EdgeCoordsUp).rg;
		vec2 EdgeDownData = texture(u_EdgesTexSampler, EdgeCoordsDown).rg;
	}

}

float SearchXLeft(vec2 i_VertexTexture)
{
	vec2 VertexTextureToCheck = i_VertexTexture - vec2(StartSearchOffset, 0.f);

	float e = 0.f;
	int i;
	for (i = 0; i < MaxSearchSteps; i++)
	{
		e = texture(u_EdgesTexSampler, vec2(VertexTextureToCheck)).g;

		if (e < EdgelExistenceRequisite)
		{
			break;
		}
		else
		{
			VertexTextureToCheck -= StepSizeX;
		}
	}

	return max(-2.f * i -2.f * e, -2.f * MaxSearchSteps);
}

float SearchXRight(vec2 i_VertexTexture)
{
	vec2 VertexTextureToCheck = i_VertexTexture + vec2(StartSearchOffset, 0.f);

	float e = 0.f;
	int i;
	for (i = 0; i < MaxSearchSteps; i++)
	{
		e = texture(u_EdgesTexSampler, vec2(VertexTextureToCheck)).g;

		if (e < EdgelExistenceRequisite)
		{
			break;
		}
		else
		{
			VertexTextureToCheck += StepSizeX;
		}
	}

	return min(2.f * i + 2.f * e, 2.f * MaxSearchSteps);
}

float SearchYUp(vec2 i_VertexTexture)
{
	vec2 VertexTextureToCheck = i_VertexTexture + vec2(0.f, StartSearchOffset);

	float e = 0.f;
	int i;
	for (i = 0; i < MaxSearchSteps; i++)
	{
		e = texture(u_EdgesTexSampler, vec2(VertexTextureToCheck)).r;

		if (e < EdgelExistenceRequisite)
		{
			break;
		}
		else
		{
			VertexTextureToCheck += StepSizeY;
		}
	}

	return min(2.f * i + 2.f * e, 2.f * MaxSearchSteps);
}

float SearchYDown(vec2 i_VertexTexture)
{
	vec2 VertexTextureToCheck = i_VertexTexture - vec2(0.f, StartSearchOffset);

	float e = 0.f;
	int i;
	for (i = 0; i < MaxSearchSteps; i++)
	{
		e = texture(u_EdgesTexSampler, vec2(VertexTextureToCheck)).r;

		if (e < EdgelExistenceRequisite)
		{
			break;
		}
		else
		{
			VertexTextureToCheck -= StepSizeY;
		}
	}

	return max(-2.f * i - 2.f * e, -2.f * MaxSearchSteps);
}
