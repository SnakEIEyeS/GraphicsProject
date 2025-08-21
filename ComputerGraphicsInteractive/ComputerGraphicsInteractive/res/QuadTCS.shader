#version 410 core

layout(vertices = 4) out;

uniform float TessellationControlLevel;

in vec3 o_VertexPositionVS_ModelSpace[];
in vec3 o_VertexNormalVS_ModelSpace[];
in vec3 o_VertexTangentVS_ModelSpace[];
in vec3 o_VertexBitangentVS_ModelSpace[];
in vec3 o_VertexTextureVS_ModelSpace[];

out vec3 o_VertexPositionTCS_ModelSpace[];
out vec3 o_VertexNormalTCS_ModelSpace[];
out vec3 o_VertexTangentTCS_ModelSpace[];
out vec3 o_VertexBitangentTCS_ModelSpace[];
out vec3 o_VertexTextureTCS_ModelSpace[];

void main()
{
	o_VertexPositionTCS_ModelSpace[gl_InvocationID] = o_VertexPositionVS_ModelSpace[gl_InvocationID];
	o_VertexNormalTCS_ModelSpace[gl_InvocationID] = o_VertexNormalVS_ModelSpace[gl_InvocationID];
	o_VertexTangentTCS_ModelSpace[gl_InvocationID] = o_VertexTangentVS_ModelSpace[gl_InvocationID];
	o_VertexBitangentTCS_ModelSpace[gl_InvocationID] = o_VertexBitangentVS_ModelSpace[gl_InvocationID];
	o_VertexTextureTCS_ModelSpace[gl_InvocationID] = o_VertexTextureVS_ModelSpace[gl_InvocationID];

	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = TessellationControlLevel;
		gl_TessLevelInner[1] = TessellationControlLevel;

		gl_TessLevelOuter[0] = TessellationControlLevel;
		gl_TessLevelOuter[1] = TessellationControlLevel;
		gl_TessLevelOuter[2] = TessellationControlLevel;
		gl_TessLevelOuter[3] = TessellationControlLevel;
	}
}
