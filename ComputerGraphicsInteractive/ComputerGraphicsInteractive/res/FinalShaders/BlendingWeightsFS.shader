#version 410 core

layout(location = 0) out vec4 o_ColorFS;

uniform sampler2D u_EdgesTexSampler;
uniform sampler2D u_AreaTexSampler;

/*Change to uniform*/
const int u_TextureWidth = 800;
const int u_TextureHeight = 600;
const vec2 TexelSize = vec2(1.f / u_TextureWidth, 1.f / u_TextureHeight);

in vec3 o_VertexTexture;

const int MaxSearchSteps = 8;
const vec2 StepSizeX = vec2(2.f, 0.f) * TexelSize;
const vec2 StepSizeY = vec2(0.f, 2.f) * TexelSize;
const float StartSearchOffset = 1.5f;
const float EdgelExistenceRequisite = 0.9f;

const int NumDistances = 33;
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
		vec2 EdgeCoordsLeft = vec2(o_VertexTexture) + vec2(DistanceToEdgeLeft, -0.25f) * TexelSize;
		vec2 EdgeCoordsRight = vec2(o_VertexTexture) + vec2(DistanceToEdgeRight + 1.f, -0.25f) * TexelSize;

		vec4 EdgeLeftGatherRed = textureGather(u_EdgesTexSampler, EdgeCoordsLeft, 0);
		float EdgeLeftData = (EdgeLeftGatherRed.x + EdgeLeftGatherRed.y + EdgeLeftGatherRed.z + EdgeLeftGatherRed.w) / 4.f;
		//float EdgeLeftData = texture(u_EdgesTexSampler, EdgeCoordsLeft).r;

		vec4 EdgeRightGatherRed = textureGather(u_EdgesTexSampler, EdgeCoordsRight, 0);
		float EdgeRightData = (EdgeRightGatherRed.x + EdgeRightGatherRed.y + EdgeRightGatherRed.z + EdgeRightGatherRed.w) / 4.f;
		//float EdgeRightData = texture(u_EdgesTexSampler, EdgeCoordsRight).r;

		vec2 SampledArea = CalculateArea(vec2(abs(DistanceToEdgeLeft), abs(DistanceToEdgeRight)), EdgeLeftData, EdgeRightData);
		BlendingWeights.x = SampledArea.x;
		BlendingWeights.y = SampledArea.y;
	}

	if (FragEdgeData.x > 0.f)
	{
		float DistanceToEdgeUp = SearchYUp(vec2(o_VertexTexture));
		float DistanceToEdgeDown = SearchYDown(vec2(o_VertexTexture));
		vec2 EdgeCoordsUp = vec2(o_VertexTexture) + vec2(-0.25f , DistanceToEdgeUp) * TexelSize;
		vec2 EdgeCoordsDown = vec2(o_VertexTexture) + vec2(-0.25f, DistanceToEdgeDown + 1.f) * TexelSize;

		vec4 EdgeUpGatherGreen = textureGather(u_EdgesTexSampler, EdgeCoordsUp, 1);
		float EdgeUpData = (EdgeUpGatherGreen.x + EdgeUpGatherGreen.y + EdgeUpGatherGreen.z + EdgeUpGatherGreen.w) / 4.f;
		//float EdgeUpData = texture(u_EdgesTexSampler, EdgeCoordsUp).g;

		vec4 EdgeDownGatherGreen = textureGather(u_EdgesTexSampler, EdgeCoordsDown, 1);
		float EdgeDownData = (EdgeDownGatherGreen.x + EdgeDownGatherGreen.y + EdgeDownGatherGreen.z + EdgeDownGatherGreen.w) / 4.f;
		//float EdgeDownData = texture(u_EdgesTexSampler, EdgeCoordsDown).g;
		
		
		vec2 SampledArea = CalculateArea(vec2(abs(DistanceToEdgeUp), abs(DistanceToEdgeDown)), EdgeUpData, EdgeDownData);
		BlendingWeights.z = SampledArea.x;
		BlendingWeights.w = SampledArea.y;
	}

	o_ColorFS = BlendingWeights;
}

float SearchXLeft(vec2 i_VertexTexture)
{
	vec2 VertexTextureToCheck = i_VertexTexture - vec2(StartSearchOffset, 0.f) * TexelSize;

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
	vec2 VertexTextureToCheck = i_VertexTexture + vec2(StartSearchOffset, 0.f) * TexelSize;

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
	vec2 VertexTextureToCheck = i_VertexTexture + vec2(0.f, StartSearchOffset) * TexelSize;

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
	vec2 VertexTextureToCheck = i_VertexTexture - vec2(0.f, StartSearchOffset) * TexelSize;

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
	vec2 PixCoord = NumDistances * round(4.f * vec2(i_CrossEdgeVal1, i_CrossEdgeVal2)) + i_DistanceToEdges;

	vec2 TexCoord = PixCoord / (AreaSize - 1.f);

	vec2 SampledArea = texture(u_AreaTexSampler, TexCoord).rg;

	return SampledArea;
}


/*

*********** References ****************

GPU Pro 2 - For the Blending Weight Area calculation function - vec2 CalculateArea(vec2 i_DistanceToEdges, float i_CrossEdgeVal1, float i_CrossEdgeVal2)

*/
