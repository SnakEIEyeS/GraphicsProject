#version 330 core

layout(location = 0) out vec4 o_ColorFS;

uniform sampler2D u_EdgesTexSampler;
uniform sampler2D u_AreaTexSampler;


in vec3 o_VertexTexture;

const int MaxSearchSteps = 8;
const vec2 StepSizeX = vec2(2.f, 0.f);
const vec2 StepSizeY = vec2(0.f, 2.f);
const float StartSearchOffset = 1.5f;
const float EdgelExistenceRequisite = 0.9f;

const int NumDistances = 129;
const float AreaSize = NumDistances * 5.f;

float SearchXLeft(vec2 i_VertexTexture);
float SearchXRight(vec2 i_VertexTexture);
float SearchYUp(vec2 i_VertexTexture);
float SearchYDown(vec2 i_VertexTexture);
vec2 CalculateArea(vec2 i_DistanceToEdges, float i_CrossEdgeVal1, float i_CrossEdgeVal2);

void main()
{
	/*vec4 sampledDiffuseColor = texture(u_EdgesTexSampler, vec2(o_VertexTexture));

	vec4 FinalDiffuseColor = sampledDiffuseColor;

	o_ColorFS = sampledDiffuseColor;*/

	vec4 BlendingWeights = vec4(0.f, 0.f, 0.f, 0.f);
	vec2 FragEdgeData = texture(u_EdgesTexSampler, vec2(o_VertexTexture)).rg;

	if (FragEdgeData.y > 0.f)
	{
		float DistanceToEdgeLeft = SearchXLeft(vec2(o_VertexTexture));
		float DistanceToEdgeRight = SearchXRight(vec2(o_VertexTexture));
		vec2 EdgeCoordsLeft = vec2(o_VertexTexture) + vec2(DistanceToEdgeLeft, -0.25f);
		vec2 EdgeCoordsRight = vec2(o_VertexTexture) + vec2(DistanceToEdgeRight + 1.f, -0.25f);

		float EdgeLeftData = texture(u_EdgesTexSampler, EdgeCoordsLeft).r;
		float EdgeRightData = texture(u_EdgesTexSampler, EdgeCoordsRight).r;

		vec2 SampledArea = CalculateArea(vec2(abs(DistanceToEdgeLeft), abs(DistanceToEdgeRight)), EdgeLeftData, EdgeRightData);
		BlendingWeights.x = SampledArea.x;
		BlendingWeights.y = SampledArea.y;
	}

	if (FragEdgeData.x > 0.f)
	{
		float DistanceToEdgeUp = SearchYUp(vec2(o_VertexTexture));
		float DistanceToEdgeDown = SearchYDown(vec2(o_VertexTexture));
		vec2 EdgeCoordsUp = vec2(o_VertexTexture) + vec2(-0.25f, DistanceToEdgeUp);
		vec2 EdgeCoordsDown = vec2(o_VertexTexture) + vec2(-0.25f, DistanceToEdgeDown + 1.f);

		float EdgeUpData = texture(u_EdgesTexSampler, EdgeCoordsUp).g;
		float EdgeDownData = texture(u_EdgesTexSampler, EdgeCoordsDown).g;

		vec2 SampledArea = CalculateArea(vec2(abs(DistanceToEdgeUp), abs(DistanceToEdgeDown)), EdgeUpData, EdgeDownData);
		BlendingWeights.z = SampledArea.x;
		BlendingWeights.w = SampledArea.y;
	}

	o_ColorFS = BlendingWeights;
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

vec2 CalculateArea(vec2 i_DistanceToEdges, float i_CrossEdgeVal1, float i_CrossEdgeVal2)
{
	vec2 pixCoord = NumDistances * round(4.f * vec2(i_CrossEdgeVal1, i_CrossEdgeVal2)) + i_DistanceToEdges;

	vec2 texCoord = pixCoord / (AreaSize - 1.f);

	vec2 sampledArea = texture(u_AreaTexSampler, texCoord).rg;

	return sampledArea;
}


/*

*********** References ****************

GPU Pro 2 - For the Blending Weight Area calculation function - vec2 CalculateArea(vec2 i_DistanceToEdges, float i_CrossEdgeVal1, float i_CrossEdgeVal2)

*/
