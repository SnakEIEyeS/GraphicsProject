#include <glad/glad.h>
#include <GL/glfw3.h>

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "Engine/Engine.h"
#include "Timer/FrameTime.h"
#include "Input/Input.h"
#include "LightSource/PointLight.h"
#include "Rendering/Rendering.h"

#include "cyCodeBase/cyGL.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase//cyPoint.h"
#include "cyCodeBase/cyTriMesh.h"

const float PI = 3.14f;

enum VertexInfo
{
	VertexPosition = 0,
	VertexNormal = 1,
	VertexTexture = 2
};


int main(void)
{
	Engine::Startup();
	Engine::Input::Startup();

	GLFWwindow* window;
	const char* WindowName = "Shantanu's Window: Hello World!";
	const int WindowWidth = 800;
	const int WindowHeight = 600;

	/* Initialize the library */
	if (!glfwInit())
	{
		return -1;
	}	

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(WindowWidth, WindowHeight, WindowName, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    std::cout << "Failed to initialize GLAD" << std::endl;
	    return -1;
	}

	//Check for Anisotropic Filtering
	if (glfwExtensionSupported("GL_EXT_texture_filter_anisotropic"))
	{
		std::cout << "Anisotropic filtering is supported\n";
		float AnisoLevel;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &AnisoLevel);
		std::cout << "Anisotropic filtering max level: " << AnisoLevel << "\n";
	}
	else
	{
		std::cout << "No Anisotropic Filtering support\n";
	}

	Engine::Rendering::Startup();
	Engine::Timing::CalcCPUFrequency();
	float dt;

	//VAO
	unsigned int vertexArrayID;
	glGenVertexArrays(1, &vertexArrayID);
	glBindVertexArray(vertexArrayID);

	/*
	//User input filename functionality
	char inFilename[256];
	std::cout << "Enter mesh filename: ";
	std::cin >> inFilename;
	cyTriMesh* TriMeshObj = new cyTriMesh();
	TriMeshObj->LoadFromFileObj(inFilename, true);
	*/
	
	//Loading obj
	cyTriMesh* TriMeshObj = new cyTriMesh();
	TriMeshObj->LoadFromFileObj("../Resources/teapot.obj", true);
	//TriMeshObj->LoadFromFileObj(inFilename, true);
	Engine::Rendering::SetMaterialDetails(TriMeshObj, 0);
	std::cout << "Number of Vertices: " << TriMeshObj->NV() << "\n";
	std::cout << "Number of VertNormals: " << TriMeshObj->NVN() << "\n";
	std::cout << "Number of Faces: " << TriMeshObj->NF() << "\n";
	std::cout << "Number of TexCoords: " << TriMeshObj->NVT() << "\n";
	std::cout << "Number of Materials: " << TriMeshObj->NM() << "\n";
	std::cout << "Material Details at index 0 -\n";
	std::cout << "\tMaterial name: " << TriMeshObj->M(0).name << "\n";
	std::cout << "\tAmbient color map: " << TriMeshObj->M(0).map_Ka << "\n";
	std::cout << "\tDiffuse color map: " << TriMeshObj->M(0).map_Kd << "\n";
	std::cout << "\tSpecular color map: " << TriMeshObj->M(0).map_Ks << "\n";
	std::cout << "\tSpecular exponent: " << TriMeshObj->M(0).Ns << "\n";

	

	/*
	int LastFaceIndex = TriMeshObj->NF();
	std::cout <<  "Test face n " << TriMeshObj->F(LastFaceIndex).v[0] << " " << TriMeshObj->F(LastFaceIndex).v[1] << " " << TriMeshObj->F(LastFaceIndex).v[2] << "\n";
	std::cout << "Test face n verts :\n";
	std::cout << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[0]).x << " " << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[0]).y << " " << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[0]).z << "\n";
	std::cout << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[1]).x << " " << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[1]).y << " " << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[1]).z << "\n";
	std::cout << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[2]).x << " " << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[2]).y << " " << TriMeshObj->V(TriMeshObj->F(LastFaceIndex).v[2]).z << "\n";
	*/


	//Vertex Positions
	unsigned int vertexPosBufferID;
	glGenBuffers(1, &vertexPosBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosBufferID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->V(0)) * TriMeshObj->NV(), const_cast<void*>(reinterpret_cast<void*>(&TriMeshObj->V(0))), GL_STATIC_DRAW);

	assert(sizeof(TriMeshObj->F(0).v) / sizeof(TriMeshObj->F(0).v[0]) == 3);
	const int TriVertsToDraw = TriMeshObj->NF() * sizeof(TriMeshObj->F(0).v) / sizeof(TriMeshObj->F(0).v[0]);
	std::cout << "Calculated Verts to draw = " << TriVertsToDraw << "\n";
	std::vector<cyPoint3f> AVertexPositions;
	AVertexPositions.reserve(TriVertsToDraw);
	for (unsigned int i = 0; i < TriMeshObj->NF(); i++)
	{
		AVertexPositions.push_back(TriMeshObj->V(TriMeshObj->F(i).v[0]));
		AVertexPositions.push_back(TriMeshObj->V(TriMeshObj->F(i).v[1]));
		AVertexPositions.push_back(TriMeshObj->V(TriMeshObj->F(i).v[2]));
		//std::cout << i << " " << AVertexPositions[i].x << " " << AVertexPositions[i].y << " " << AVertexPositions[i].z << "\n";
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->V(0)) * TriVertsToDraw, const_cast<void*>(reinterpret_cast<void*>(&AVertexPositions[0])), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(VertexInfo::VertexPosition);
	glVertexAttribPointer(VertexInfo::VertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(TriMeshObj->V(0)), (const void*)0);
    

	//Vertex Normals
	unsigned int vertexNormalBufferID;
	glGenBuffers(1, &vertexNormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBufferID);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->VN(0))*TriMeshObj->NVN(), const_cast<void*>(reinterpret_cast<void*>(&TriMeshObj->VN(0))), GL_STATIC_DRAW);

	assert(sizeof(TriMeshObj->F(0).v) / sizeof(TriMeshObj->F(0).v[0]) == 3);
	std::vector<cyPoint3f> AVertexNormals;
	AVertexNormals.reserve(TriVertsToDraw);
	for (unsigned int i = 0; i < TriMeshObj->NF(); i++)
	{
		AVertexNormals.push_back(TriMeshObj->VN(TriMeshObj->F(i).v[0]));
		AVertexNormals.push_back(TriMeshObj->VN(TriMeshObj->F(i).v[1]));
		AVertexNormals.push_back(TriMeshObj->VN(TriMeshObj->F(i).v[2]));
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->VN(0)) * TriVertsToDraw, const_cast<void*>(reinterpret_cast<void*>(&AVertexNormals[0])), GL_STATIC_DRAW);

	glEnableVertexAttribArray(VertexInfo::VertexNormal);
	glVertexAttribPointer(VertexInfo::VertexNormal, sizeof(TriMeshObj->VN(0)) / sizeof(TriMeshObj->VN(0).x), GL_FLOAT, GL_FALSE, sizeof(TriMeshObj->VN(0)), (const void*)0);


	//Vertex Texture Coordinates
	unsigned int vertexTextureBufferID;
	glGenBuffers(1, &vertexTextureBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexTextureBufferID);

	assert(sizeof(TriMeshObj->F(0).v) / sizeof(TriMeshObj->F(0).v[0]) == 3);
	std::vector<cyPoint3f> AVertexTextures;
	AVertexTextures.reserve(TriVertsToDraw);
	const int NumVertexTextures = TriMeshObj->NVT();
	for (unsigned int i = 0; i < TriMeshObj->NF(); i++)
	{
		AVertexTextures.push_back(TriMeshObj->VT(TriMeshObj->F(i).v[0] % (NumVertexTextures - 1)));
		AVertexTextures.push_back(TriMeshObj->VT(TriMeshObj->F(i).v[1] % (NumVertexTextures - 1)));
		AVertexTextures.push_back(TriMeshObj->VT(TriMeshObj->F(i).v[2] % (NumVertexTextures - 1)));
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->VT(0))*TriVertsToDraw, const_cast<void*>(reinterpret_cast<void*>(&AVertexTextures[0])), GL_STATIC_DRAW);

	glEnableVertexAttribArray(VertexInfo::VertexTexture);
	glVertexAttribPointer(VertexInfo::VertexTexture, sizeof(TriMeshObj->VT(0)) / sizeof(TriMeshObj->VT(0).x), GL_FLOAT, GL_FALSE, sizeof(TriMeshObj->VT(0)), (const void*)0);

	//Texture Loading and Setup
	//Ambient Texture
	std::string AmbientTextureFilename = "../Resources/";
	AmbientTextureFilename += TriMeshObj->M(0).map_Ka;
	std::vector<unsigned char> AmbientTextureImageData;
	unsigned int AmbientTextureWidth;
	unsigned int AmbientTextureHeight;
	Engine::Rendering::DecodeTexturePNG(AmbientTextureFilename, AmbientTextureImageData, AmbientTextureWidth, AmbientTextureHeight);
	std::cout << "\nAmbient Texture details:\n";
	std::cout << "\tImage Width: " << AmbientTextureWidth << "\n";
	std::cout << "\tImage Height: " << AmbientTextureHeight << "\n";

	glActiveTexture(GL_TEXTURE0);
	unsigned int ambientTextureID;
	glGenTextures(1, &ambientTextureID);
	glBindTexture(GL_TEXTURE_2D, ambientTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, AmbientTextureWidth, AmbientTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)&AmbientTextureImageData[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(ambientTextureID, GL_TEXTURE_MAX_ANISOTROPY, Engine::Rendering::GetMaxAnisotropicLevel());

	//Diffuse Texture
	std::string DiffuseTextureFilename = "../Resources/";
	DiffuseTextureFilename += TriMeshObj->M(0).map_Kd;
	std::vector<unsigned char> DiffuseTextureImageData;
	unsigned int DiffuseTextureWidth;
	unsigned int DiffuseTextureHeight;
	Engine::Rendering::DecodeTexturePNG(DiffuseTextureFilename, DiffuseTextureImageData, DiffuseTextureWidth, DiffuseTextureHeight);
	std::cout << "\nDiffuse Texture details:\n";
	std::cout << "\tImage Width: " << DiffuseTextureWidth << "\n";
	std::cout << "\tImage Height: " << DiffuseTextureHeight << "\n";

	glActiveTexture(GL_TEXTURE1);
	unsigned int diffuseTextureID;
	glGenTextures(1, &diffuseTextureID);
	glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, DiffuseTextureWidth, DiffuseTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)&DiffuseTextureImageData[0]);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(diffuseTextureID, GL_TEXTURE_MAX_ANISOTROPY, Engine::Rendering::GetMaxAnisotropicLevel());

	//Specular Texture
	std::string SpecularTextureFilename = "../Resources/";
	SpecularTextureFilename += TriMeshObj->M(0).map_Ks;
	std::vector<unsigned char> SpecularTextureImageData;
	unsigned int SpecularTextureWidth;
	unsigned int SpecularTextureHeight;
	Engine::Rendering::DecodeTexturePNG(SpecularTextureFilename, SpecularTextureImageData, SpecularTextureWidth, SpecularTextureHeight);
	std::cout << "\nSpecular Texture details:\n";
	std::cout << "\tImage Width: " << SpecularTextureWidth << "\n";
	std::cout << "\tImage Height: " << SpecularTextureHeight << "\n";

	glActiveTexture(GL_TEXTURE2);
	unsigned int specularTextureID;
	glGenTextures(1, &specularTextureID);
	glBindTexture(GL_TEXTURE_2D, specularTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SpecularTextureWidth, SpecularTextureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const void*)&SpecularTextureImageData[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(specularTextureID, GL_TEXTURE_MAX_ANISOTROPY, Engine::Rendering::GetMaxAnisotropicLevel());


	Engine::Rendering::BuildAndUseProgram();

	float fovy = 45.f;
	float FOV = 1/tan(fovy);
	//float aspect = 2.f; //2:1
	float aspect = ((float)WindowWidth) / WindowHeight;
	float zNear = 0.1f;
	float zFar = 100.f;

	cyMatrix4f CameraMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, -27.0f,
		0.0f, 0.f, 0.f, 1.0f
	);
	cyMatrix4f const ProjectionMatrix(
		FOV/aspect, 0.0f,                          0.0f,                                0.0f,
		0.0f,       FOV,                           0.0f,                                0.0f,
		0.0f,       0.0f, (zFar + zNear)/(zNear - zFar), (2.f * zFar * zNear)/(zNear - zFar),
		0.0f,       0.0f,                         -1.0f,                                 0.0f
	);

	cyMatrix4f FinalTransformMatrix;
	FinalTransformMatrix = ProjectionMatrix * CameraMatrix;

	Engine::Rendering::GetGLProgram()->SetUniformMatrix4("u_Transformation", FinalTransformMatrix.data);

	cyMatrix4f const RotateMatrix(
		cos(0.005f), 0.0f, sin(0.005f), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(0.005f), 0.0f, cos(0.005f), 0.0f,
		0.0f, 0.f, 0.f, 1.0f
	);

	cyMatrix4f const XRotateMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(-PI / 4), -sin(-PI / 4), 0.0f,
		0.0f, sin(-PI / 4), cos(-PI / 4), 0.0f,
		0.0f, 0.f, 0.f, 1.0f
	);

	cyMatrix4f const YRotateMatrix(
		cos(PI / 8), 0.0f, sin(PI / 8), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-sin(PI / 8), 0.0f, cos(PI / 8), 0.0f,
		0.0f, 0.f, 0.f, 1.0f
	);

	cyMatrix4f const ZRotateMatrix(
		cos(PI / 4), -sin(PI / 4), 0.0f, 0.0f,
		sin(PI / 4), cos(PI / 4), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.f, 0.f, 1.0f
	);

	//FinalTransformMatrix = ProjectionMatrix * CameraMatrix * XRotateMatrix;
	//Engine::Rendering::GetGLProgram()->SetUniformMatrix4("u_Transformation", FinalTransformMatrix.data);
	CameraMatrix *= XRotateMatrix;
	Engine::Rendering::GetGLProgram()->SetUniformMatrix4("u_Projection", ProjectionMatrix.data);
	Engine::Rendering::GetGLProgram()->SetUniformMatrix4("u_Camera", CameraMatrix.data);

	

	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		 dt = Engine::Timing::GetLastFrameTime_ms();

		/* Render here */
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//CameraMatrix *= RotateMatrix;
		//Engine::Rendering::GetGLProgram()->SetUniformMatrix4("u_Camera", CameraMatrix.data);
		
		
		Engine::Input::Update(window, dt);
		Engine::Rendering::Update(window, dt);

		const cyPoint3f PointLightPos3 = Engine::Rendering::GetRenderPointLight().GetPosition().GetNonHomogeneous();
		Engine::Rendering::GetGLProgram()->SetUniform("u_LightPosition", PointLightPos3);
		//Engine::Rendering::GetGLProgram()->SetUniform("u_DiffuseColor", cyPoint4f(1.f, 0.f, 0.f, 1.f));
		//Engine::Rendering::GetGLProgram()->SetUniform("u_SpecularColor", cyPoint4f(1.f, 1.f, 1.f, 1.f));
		Engine::Rendering::GetGLProgram()->SetUniform("u_AmbientConstant", Engine::Rendering::AmbientConstant);
		//Engine::Rendering::GetGLProgram()->SetUniform("u_SpecularExponent", Engine::Rendering::MaterialSpecularExponent);
		Engine::Rendering::GetGLProgram()->SetUniform("u_SpecularExponent", Engine::Rendering::SpecularAlpha);

		//Set TextureSampler uniforms
		Engine::Rendering::GetGLProgram()->SetUniform("u_AmbientTextureSampler", GL_TEXTURE0);
		Engine::Rendering::GetGLProgram()->SetUniform("u_DiffuseTextureSampler", GL_TEXTURE1);
		Engine::Rendering::GetGLProgram()->SetUniform("u_SpecularTextureSampler", GL_TEXTURE2);

		//Drawing code
		glBindVertexArray(vertexArrayID);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexPosBufferID);
		glDrawArrays(GL_TRIANGLES, 0, AVertexPositions.size());
		//glDrawElements(GL_TRIANGLES, TriMeshObj->NF() * sizeof(TriMeshObj->F(0))/sizeof(TriMeshObj->F(0).v[0]), GL_UNSIGNED_INT, &TriMeshObj->F(0));
		//Drawing code end

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	Engine::Rendering::Shutdown();
	glfwTerminate();

	Engine::Input::Shutdown();
	Engine::Shutdown();
    
	return 0;
}


/*References:
https://www.glfw.org/documentation.html for GLFW startup, main loop, termination and window creation
https://learnopengl.com/Getting-started/Hello-Window for glad initialization
*/
