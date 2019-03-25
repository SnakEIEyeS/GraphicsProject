#version 410 core

layout(quads) in;

uniform sampler2D u_DisplacementMap;

uniform mat4 u_Projection;
uniform mat4 u_Camera;
uniform mat4 u_Object;
uniform float u_DisplacementFactor_ModelSpace;

in vec3 o_VertexPositionTCS_ModelSpace[];
in vec3 o_VertexNormalTCS_ModelSpace[];
in vec3 o_VertexTangentTCS_ModelSpace[];
in vec3 o_VertexBitangentTCS_ModelSpace[];
in vec3 o_VertexTextureTCS_ModelSpace[];

out mat3 o_TBNMat_CameraSpace;
out vec4 o_VertexPositionToFS_ModelSpace;
out vec4 o_VertexNormalToFS_ModelSpace;
out vec3 o_VertexTextureToFS_ModelSpace;

void main()
{
	

	//Vertex Normal Interpolation & Calculation
	vec3 VertexNormal1 = mix(o_VertexNormalTCS_ModelSpace[1], o_VertexNormalTCS_ModelSpace[0], gl_TessCoord.x);
	vec3 VertexNormal2 = mix(o_VertexNormalTCS_ModelSpace[2], o_VertexNormalTCS_ModelSpace[3], gl_TessCoord.x);
	vec3 VertexNormalFinal_ModelSpace = normalize(mix(VertexNormal1, VertexNormal2, gl_TessCoord.y));
	o_VertexNormalToFS_ModelSpace = vec4(VertexNormalFinal_ModelSpace, 0.f);

	vec3 VertexNormalFinal_CameraSpace = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(VertexNormalFinal_ModelSpace, 0.f)));

	//Vertex Tangent Interpolation & Calculation
	vec3 VertexTangent1 = mix(o_VertexTangentTCS_ModelSpace[1], o_VertexTangentTCS_ModelSpace[0], gl_TessCoord.x);
	vec3 VertexTangent2 = mix(o_VertexTangentTCS_ModelSpace[2], o_VertexTangentTCS_ModelSpace[3], gl_TessCoord.x);
	vec3 VertexTangentFinal_ModelSpace = normalize(mix(VertexTangent1, VertexTangent2, gl_TessCoord.y));
	vec3 VertexTangentFinal_CameraSpace = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(VertexTangentFinal_ModelSpace, 0.f)));

	//Vertex Bitangent Interpolation & Calculation
	vec3 VertexBitangent1 = mix(o_VertexBitangentTCS_ModelSpace[1], o_VertexBitangentTCS_ModelSpace[0], gl_TessCoord.x);
	vec3 VertexBitangent2 = mix(o_VertexBitangentTCS_ModelSpace[2], o_VertexBitangentTCS_ModelSpace[3], gl_TessCoord.x);
	vec3 VertexBitangentFinal_ModelSpace = normalize(mix(VertexBitangent1, VertexBitangent2, gl_TessCoord.y));
	vec3 VertexBitangentFinal_CameraSpace = normalize(vec3(transpose(inverse(u_Camera)) * u_Object * vec4(VertexBitangentFinal_ModelSpace, 0.f)));
	
	o_TBNMat_CameraSpace = mat3(VertexTangentFinal_CameraSpace, VertexBitangentFinal_CameraSpace, VertexNormalFinal_CameraSpace);

	//Vertex Texture Interpolation & Calculation
	vec3 VertexTexture1 = mix(o_VertexTextureTCS_ModelSpace[1], o_VertexTextureTCS_ModelSpace[0], gl_TessCoord.x);
	vec3 VertexTexture2 = mix(o_VertexTextureTCS_ModelSpace[2], o_VertexTextureTCS_ModelSpace[3], gl_TessCoord.x);
	vec3 VertexTextureFinal_ModelSpace = mix(VertexTexture1, VertexTexture2, gl_TessCoord.y);
	o_VertexTextureToFS_ModelSpace = VertexTextureFinal_ModelSpace;

	//Vertex Position Interpolation & Calculation
	vec3 VertexPos1 = mix(o_VertexPositionTCS_ModelSpace[1], o_VertexPositionTCS_ModelSpace[0], gl_TessCoord.x);
	vec3 VertexPos2 = mix(o_VertexPositionTCS_ModelSpace[2], o_VertexPositionTCS_ModelSpace[3], gl_TessCoord.x);
	vec3 VertexPositionInterp_ModelSpace = mix(VertexPos1, VertexPos2, gl_TessCoord.y);
	
	//Add Vertex Displacement from Displacement Map
	vec4 sampledDisplacement = texture(u_DisplacementMap, vec2(VertexTextureFinal_ModelSpace));
	float DisplacementValue = sampledDisplacement.x;
	vec4 VertexPositionFinal_ModelSpace = vec4((VertexPositionInterp_ModelSpace + VertexNormalFinal_ModelSpace * DisplacementValue * u_DisplacementFactor_ModelSpace), 1.f);
	//vec4 VertexPositionFinal_ModelSpace = vec4(VertexPositionInterp_ModelSpace + vec3(0.f, 0.f, DisplacementValue * 10.f), 1.f);
	//vec4 VertexPositionFinal_ModelSpace = vec4(VertexPositionInterp_ModelSpace, 1.f);

	o_VertexPositionToFS_ModelSpace = VertexPositionFinal_ModelSpace;
	vec4 VertexPositionFinal_ClipSpace = u_Projection * u_Camera * u_Object * VertexPositionFinal_ModelSpace;
	gl_Position = VertexPositionFinal_ClipSpace;
}
