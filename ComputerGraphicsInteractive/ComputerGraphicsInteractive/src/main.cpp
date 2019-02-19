#include <glad/glad.h>
#include <GL/glfw3.h>

#include <iostream>
#include <math.h>
#include <string>
#include <vector>

#include "Camera/Camera.h"
#include "Engine/Engine.h"
#include "GameObject/GameObject.h"
#include "Timer/FrameTime.h"
#include "Input/Input.h"
#include "LightSource/PointLight.h"
#include "ModelHandler/ModelHandler.h"
#include "Rendering/Rendering.h"

#include "cyCodeBase/cyGL.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyPoint.h"
#include "cyCodeBase/cyTriMesh.h"

const float PI = 3.14f;

//TODO delete from here
/*enum VertexInfo
{
	VertexPosition = 0,
	VertexNormal = 1,
	VertexTexture = 2
};*/


int main(void)
{
	Engine::Startup();
	Engine::Input::Startup();

	GLFWwindow* window;
	const char* WindowName = "Shantanu's Window: Hello World!";
	const unsigned int WindowWidth = 800;
	const unsigned int WindowHeight = 600;

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
	unsigned int teapotVertexArrayID;
	glGenVertexArrays(1, &teapotVertexArrayID);
	glBindVertexArray(teapotVertexArrayID);

	/*
	//User input filename functionality
	char inFilename[256];
	std::cout << "Enter mesh filename: ";
	std::cin >> inFilename;
	cyTriMesh* TeapotTriMesh = new cyTriMesh();
	TeapotTriMesh->LoadFromFileObj(inFilename, true);
	*/
	
	//Loading obj
	cyTriMesh* TeapotTriMesh = new cyTriMesh();
	TeapotTriMesh->LoadFromFileObj("../Resources/teapot.obj", true);
	//TeapotTriMesh->LoadFromFileObj(inFilename, true);
	Engine::Rendering::SetMaterialDetails(TeapotTriMesh, 0);
	std::cout << "Number of Vertices: " << TeapotTriMesh->NV() << "\n";
	std::cout << "Number of VertNormals: " << TeapotTriMesh->NVN() << "\n";
	std::cout << "Number of Faces: " << TeapotTriMesh->NF() << "\n";
	std::cout << "Number of VertexTexCoords: " << TeapotTriMesh->NVT() << "\n";
	std::cout << "Number of Materials: " << TeapotTriMesh->NM() << "\n";
	std::cout << "Material Details at index 0 -\n";
	std::cout << "\tMaterial name: " << TeapotTriMesh->M(0).name << "\n";
	std::cout << "\tAmbient color map: " << TeapotTriMesh->M(0).map_Ka << "\n";
	std::cout << "\tDiffuse color map: " << TeapotTriMesh->M(0).map_Kd << "\n";
	std::cout << "\tSpecular color map: " << TeapotTriMesh->M(0).map_Ks << "\n";
	std::cout << "\tSpecular exponent: " << TeapotTriMesh->M(0).Ns << "\n";


	Engine::ModelHandling::GetModelHandler()->AddVertexPositions(TeapotTriMesh, teapotVertexArrayID);
	Engine::ModelHandling::GetModelHandler()->AddVertexNormals(TeapotTriMesh, teapotVertexArrayID);
	Engine::ModelHandling::GetModelHandler()->AddVertexTextureCoordinates(TeapotTriMesh, teapotVertexArrayID);
	

	//Render Plane
	//Plane VAO
	unsigned int planeVertexArrayID;
	glGenVertexArrays(1, &planeVertexArrayID);
	glBindVertexArray(planeVertexArrayID);

	//Plane VertexPositions
	std::vector<cyPoint3f> APlaneVertPos;
	APlaneVertPos.reserve(6);

	//const float PlaneXExtent = 25.f;
	//const float PlaneYExtent = 15.f;
	const float PlaneXExtent = 40.f;
	const float PlaneYExtent = 30.f;
	cyPoint3f PlaneTopLeft(-PlaneXExtent, PlaneYExtent, 0.f);
	cyPoint3f PlaneTopRight(PlaneXExtent, PlaneYExtent, 0.f);
	cyPoint3f PlaneBottomLeft(-PlaneXExtent, -PlaneYExtent, 0.f);
	cyPoint3f PlaneBottomRight(PlaneXExtent, -PlaneYExtent, 0.f);
	APlaneVertPos.push_back(PlaneTopLeft);
	APlaneVertPos.push_back(PlaneBottomLeft);
	APlaneVertPos.push_back(PlaneBottomRight);
	APlaneVertPos.push_back(PlaneBottomRight);
	APlaneVertPos.push_back(PlaneTopRight);
	APlaneVertPos.push_back(PlaneTopLeft);

	unsigned int planeVertexPosBufferID;
	glGenBuffers(1, &planeVertexPosBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexPosBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneVertPos[0]) * APlaneVertPos.size(), const_cast<void*>(reinterpret_cast<void*>(&APlaneVertPos[0])), GL_STATIC_DRAW);

	glEnableVertexAttribArray(Engine::ModelHandling::VertexInfo::VertexPosition);
	glVertexAttribPointer(Engine::ModelHandling::VertexInfo::VertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(TeapotTriMesh->V(0)), (const void*)0);

	//Plane VertNormals
	std::vector<cyPoint3f> APlaneVertNormals;
	APlaneVertNormals.reserve(6);
	cyPoint3f UpVector(0.f, 0.f, 1.f);
	for (int i = 0; i < 6; i++)
	{
		APlaneVertNormals.push_back(UpVector);
	}

	unsigned int planeVertexNormalBufferID;
	glGenBuffers(1, &planeVertexNormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexNormalBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneVertNormals[0]) * APlaneVertNormals.size(), const_cast<void*>(reinterpret_cast<void*>(&APlaneVertNormals[0])), GL_STATIC_DRAW);
	glEnableVertexAttribArray(Engine::ModelHandling::VertexInfo::VertexNormal);
	glVertexAttribPointer(Engine::ModelHandling::VertexInfo::VertexNormal, sizeof(APlaneVertNormals[0]) / sizeof(APlaneVertNormals[0].x), GL_FLOAT, GL_FALSE, sizeof(APlaneVertNormals[0]), (const void*)0);

	//Plane TextureCoordinates
	std::vector<cyPoint3f> APlaneVertTextures;
	APlaneVertTextures.reserve(6);
	cyPoint3f PlaneUVTopLeft(0.f, 1.f, 0.f);
	cyPoint3f PlaneUVTopRight(1.f, 1.f, 0.f);
	cyPoint3f PlaneUVBottomRight(1.f, 0.f, 0.f);
	cyPoint3f PlaneUVBottomLeft(0.f, 0.f, 0.f);
	APlaneVertTextures.push_back(PlaneUVTopLeft);
	APlaneVertTextures.push_back(PlaneUVBottomLeft);
	APlaneVertTextures.push_back(PlaneUVBottomRight);
	APlaneVertTextures.push_back(PlaneUVBottomRight);
	APlaneVertTextures.push_back(PlaneUVTopRight);
	APlaneVertTextures.push_back(PlaneUVTopLeft);

	unsigned int planeVertexTextureBufferID;
	glGenBuffers(1, &planeVertexTextureBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexTextureBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneVertTextures[0])*APlaneVertTextures.size(), const_cast<void*>(reinterpret_cast<void*>(&APlaneVertTextures[0])), GL_STATIC_DRAW);
	glEnableVertexAttribArray(Engine::ModelHandling::VertexInfo::VertexTexture);
	glVertexAttribPointer(Engine::ModelHandling::VertexInfo::VertexTexture, sizeof(APlaneVertTextures[0]) / sizeof(APlaneVertTextures[0].x), GL_FLOAT, GL_FALSE, sizeof(APlaneVertTextures[0]), (const void*)0);


	//Texture Loading and Setup
	//Ambient Texture
	std::string AmbientTextureFilename = "../Resources/";
	AmbientTextureFilename += TeapotTriMesh->M(0).map_Ka;
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
	DiffuseTextureFilename += TeapotTriMesh->M(0).map_Kd;
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
	SpecularTextureFilename += TeapotTriMesh->M(0).map_Ks;
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


	//Engine::Rendering::BuildAndUseProgram();
	cyGLSLProgram* MainSceneProgram = Engine::Rendering::BuildProgram(Engine::Rendering::SceneVertexShaderFile, Engine::Rendering::SceneFragmentShaderFile);
	cyGLSLProgram* RenderTextureProgram = Engine::Rendering::BuildProgram(Engine::Rendering::RenderTextureVertexShaderFile, Engine::Rendering::RenderTextureFragmentShaderFile);

	
	float fovy = 45.f;
	float FOV = 1/tan(fovy);
	//float aspect = 2.f; //2:1
	float aspect = ((float)WindowWidth) / WindowHeight;
	float zNear = 0.1f;
	float zFar = 100.f;

	Engine::Entity::Camera* MainSceneCamera = 
		new Engine::Entity::Camera(new Engine::Entity::GameObject(), 
		fovy, WindowWidth, WindowHeight, zNear, zFar);

	MainSceneCamera->GetGameObject()->SetPosition(cyPoint3f(0.f, 0.f, -60.f));
	MainSceneCamera->GetGameObject()->SetRotation(cyPoint3f(0.f, 0.f, 0.f));

	float* PerspectiveTest = MainSceneCamera->GetPerspectiveProjection().data;
	float* CameraTransformTest = MainSceneCamera->GetGameObject()->GetTransform().data;

	//Engine::Input::CameraGameObject = MainSceneCamera->GetGameObject();
	Engine::Input::SetCameraGameObject(MainSceneCamera->GetGameObject());

	
	//Create RenderTexture for teapot's RenderToTexture operation
	glActiveTexture(GL_TEXTURE3);
	cyGLRenderTexture2D* SceneRenderTexture = new cyGLRenderTexture2D();
	bool bRenderTextureReady = SceneRenderTexture->Initialize(true, 4, WindowWidth, WindowHeight, cy::GL::TYPE_UBYTE);
	assert(bRenderTextureReady);

	//Set texture settings for texture that will be used by Plane
	//SceneRenderTexture->BindTexture(3);
	//glActiveTexture(GL_TEXTURE3);
	SceneRenderTexture->BindTexture();
	SceneRenderTexture->SetTextureFilteringMode(GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
	SceneRenderTexture->BuildTextureMipmaps();
	glTexParameterf(SceneRenderTexture->GetTextureID(), GL_TEXTURE_MAX_ANISOTROPY, Engine::Rendering::GetMaxAnisotropicLevel());


	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		 dt = Engine::Timing::GetLastFrameTime_ms();

		/* Render here */
		 glClearColor(1.f, 0.f, 0.f, 1.f);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//CameraMatrix *= RotateMatrix;
		//MainSceneProgram->SetUniformMatrix4("u_Camera", CameraMatrix.data);
		

		//TODO handle order of code if Rendering Update does all the rendering
		Engine::Input::Update(window, dt);
		Engine::Rendering::Update(window, dt);
		
		//Bind our own RenderTexture
		/*glActiveTexture(GL_TEXTURE3);
		SceneRenderTexture->Bind();
		
		//SceneRenderTexture->BindTexture();
		glClearColor(0.f, 0.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
		
		//Render the scene to our RenderTexture
		MainSceneProgram->Bind();

		//MainSceneProgram->SetUniformMatrix4("u_Projection", ProjectionMatrix.data);
		//MainSceneProgram->SetUniformMatrix4("u_Camera", CameraMatrix.data);
		MainSceneProgram->SetUniformMatrix4("u_Projection", MainSceneCamera->GetPerspectiveProjection().data);
		MainSceneProgram->SetUniformMatrix4("u_Camera", MainSceneCamera->GetGameObject()->GetTransform().data);

		const cyPoint3f PointLightPos3 = Engine::Rendering::GetRenderPointLight().GetPosition().GetNonHomogeneous();
		MainSceneProgram->SetUniform("u_LightPosition", PointLightPos3);
		MainSceneProgram->SetUniform("u_AmbientConstant", Engine::Rendering::AmbientConstant);
		MainSceneProgram->SetUniform("u_SpecularExponent", Engine::Rendering::MaterialSpecularExponent);
		MainSceneProgram->SetUniform("u_SpecularExponent", Engine::Rendering::SpecularAlpha);

		//Set TextureSampler uniforms
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ambientTextureID);
		MainSceneProgram->SetUniform("u_AmbientTextureSampler", GL_TEXTURE0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, diffuseTextureID);
		MainSceneProgram->SetUniform("u_DiffuseTextureSampler", GL_TEXTURE1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, specularTextureID);
		MainSceneProgram->SetUniform("u_SpecularTextureSampler", GL_TEXTURE2);

		//Drawing code
		glBindVertexArray(teapotVertexArrayID);

		//TODO put the vertices and all data into GameObject or something
		glDrawArrays(GL_TRIANGLES, 0, TeapotTriMesh->NF() * sizeof(TeapotTriMesh->F(0).v) / sizeof(TeapotTriMesh->F(0).v[0]));
		//glDrawElements(GL_TRIANGLES, TeapotTriMesh->NF() * sizeof(TeapotTriMesh->F(0))/sizeof(TeapotTriMesh->F(0).v[0]), GL_UNSIGNED_INT, &TeapotTriMesh->F(0));

		
		//Unbind our RenderTexture so normal rendering buffers are brought back
		/*SceneRenderTexture->Unbind();
		
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderTextureProgram->Bind();

		RenderTextureProgram->SetUniformMatrix4("u_PlaneProjection", ProjectionMatrix.data);
		RenderTextureProgram->SetUniformMatrix4("u_PlaneCamera", CameraMatrix.data);

		//Bind the texture that the teapot scene was rendered to as the texture for the plane
		//SceneRenderTexture->BindTexture(3);
		SceneRenderTexture->BuildTextureMipmaps();
		SceneRenderTexture->BindTexture(3);
		RenderTextureProgram->SetUniform("u_RenderToSampler", GL_TEXTURE3);

		//Draw plane on usual rendering buffers
		
		
		//Render the plane
		glBindVertexArray(planeVertexArrayID);
		glDrawArrays(GL_TRIANGLES, 0, APlaneVertPos.size());*/
		

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
