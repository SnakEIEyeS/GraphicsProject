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
#include "LightSource/SpotLight.h"
#include "ModelHandler/ModelHandler.h"
#include "Rendering/Rendering.h"
#include "StaticMesh/StaticMesh.h"

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

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
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

	GLuint err;

	//Check for Anisotropic Filtering
	if (glfwExtensionSupported("GL_EXT_texture_filter_anisotropic"))
	{
		std::cout << "Anisotropic filtering is supported\n";
		float AnisoLevel;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &AnisoLevel);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}
		std::cout << "Anisotropic filtering max level: " << AnisoLevel << "\n";
	}
	else
	{
		std::cout << "No Anisotropic Filtering support\n";
	}

	GLint MaxTess;
	glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &MaxTess);
	std::cout << "Max Tessellation Level: " << MaxTess << "\n";
	Engine::Input::SetupMaxTessellationLevel();

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

	Engine::Entity::StaticMesh* TeapotStaticMesh = new Engine::Entity::StaticMesh(new Engine::Entity::GameObject(), teapotVertexArrayID);
	const_cast<Engine::Entity::GameObject*>(TeapotStaticMesh->GetGameObject())->SetPosition(cyPoint3f(0.f, 0.f, 0.f));
	const_cast<Engine::Entity::GameObject*>(TeapotStaticMesh->GetGameObject())->SetRotation(cyPoint3f(0.f, 0.f, 0.f));
	//Bind Teapot movement, rotation to Ctrl
	Engine::Input::SetCtrlBoundGameObject(TeapotStaticMesh->GetGameObject());
	

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
	const float PlaneXExtent = 30.f;
	const float PlaneYExtent = 30.f;

	cyPoint3f ScreenPlaneTopLeft(-1.f, 1.f, 0.f);
	cyPoint3f ScreenPlaneTopRight(1.f, 1.f, 0.f);
	cyPoint3f ScreenPlaneBottomRight(1.f, -1.f, 0.f);
	cyPoint3f ScreenPlaneBottomLeft(-1.f, -1.f, 0.f);

	/*cyPoint3f PlaneTopLeft(-PlaneXExtent, PlaneYExtent, 0.f);
	cyPoint3f PlaneTopRight(PlaneXExtent, PlaneYExtent, 0.f);
	cyPoint3f PlaneBottomLeft(-PlaneXExtent, -PlaneYExtent, 0.f);
	cyPoint3f PlaneBottomRight(PlaneXExtent, -PlaneYExtent, 0.f);
	APlaneVertPos.push_back(PlaneTopLeft);
	APlaneVertPos.push_back(PlaneBottomLeft);
	APlaneVertPos.push_back(PlaneBottomRight);
	APlaneVertPos.push_back(PlaneBottomRight);
	APlaneVertPos.push_back(PlaneTopRight);
	APlaneVertPos.push_back(PlaneTopLeft);*/

	APlaneVertPos.push_back(ScreenPlaneTopLeft);
	APlaneVertPos.push_back(ScreenPlaneBottomLeft);
	APlaneVertPos.push_back(ScreenPlaneBottomRight);
	APlaneVertPos.push_back(ScreenPlaneBottomRight);
	APlaneVertPos.push_back(ScreenPlaneTopRight);
	APlaneVertPos.push_back(ScreenPlaneTopLeft);

	unsigned int planeVertexPosBufferID;
	glGenBuffers(1, &planeVertexPosBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, planeVertexPosBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(APlaneVertPos[0]) * APlaneVertPos.size(), const_cast<void*>(reinterpret_cast<void*>(&APlaneVertPos[0])), GL_STATIC_DRAW);

	glEnableVertexAttribArray(Engine::ModelHandling::VertexInfo::VertexPosition);
	glVertexAttribPointer(Engine::ModelHandling::VertexInfo::VertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(APlaneVertPos[0]), (const void*)0);

	//Plane VertNormals
	std::vector<cyPoint3f> APlaneVertNormals;
	APlaneVertNormals.reserve(6);
	cyPoint3f UpVector(0.f, 0.f, 1.f);
	const unsigned int PlaneVertices = 6;
	for (int i = 0; i < PlaneVertices; i++)
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

	unsigned int PlaneTangentsBufferID;
	unsigned int PlaneBitangentsBufferID;
	Engine::ModelHandling::GetModelHandler()->CalculatePlaneTriTangentsBitangents(APlaneVertPos, APlaneVertTextures, planeVertexArrayID, PlaneTangentsBufferID, PlaneBitangentsBufferID);
	//Engine::ModelHandling::GetModelHandler()->CalculatePlaneQuadTangentsBitangents(APlaneVertPos, APlaneVertTextures, planeVertexArrayID, PlaneTangentsBufferID, PlaneBitangentsBufferID);

	Engine::Entity::StaticMesh* PlaneStaticMesh = new Engine::Entity::StaticMesh(new Engine::Entity::GameObject(), planeVertexArrayID);
	const_cast<Engine::Entity::GameObject*>(PlaneStaticMesh->GetGameObject())->SetPosition(cyPoint3f(0.f, 0.f, 0.f));
	const_cast<Engine::Entity::GameObject*>(PlaneStaticMesh->GetGameObject())->SetRotation(cyPoint3f(0.f, 0.f, 0.f));
	//Bind Plane to be transformed by holding down Alt
	Engine::Input::SetAltBoundGameObject(const_cast<Engine::Entity::GameObject*>(PlaneStaticMesh->GetGameObject()));
	

	//Teapot Texture Loading and Setup
	//Ambient Texture
	std::string AmbientTextureFilename = "../Resources/";
	AmbientTextureFilename += TeapotTriMesh->M(0).map_Ka;
	//std::string AmbientTextureFilename = "../Resources/CubeMap/cubemap_posx.png";
	TeapotStaticMesh->m_AmbientTextureID = Engine::ModelHandling::GetModelHandler()->CreateTexture2D(AmbientTextureFilename.c_str(), 0);
	err = glGetError();
	if (err != 0)
	{
		printf("Error code: %d\n", err);
	}

	//Diffuse Texture
	std::string DiffuseTextureFilename = "../Resources/";
	DiffuseTextureFilename += TeapotTriMesh->M(0).map_Kd;
	//std::string DiffuseTextureFilename = "../Resources/CubeMap/cubemap_posx.png";
	TeapotStaticMesh->m_DiffuseTextureID = Engine::ModelHandling::GetModelHandler()->CreateTexture2D(DiffuseTextureFilename.c_str(), 1);
	err = glGetError();
	if (err != 0)
	{
		printf("Error code: %d\n", err);
	}

	//Specular Texture
	std::string SpecularTextureFilename = "../Resources/";
	SpecularTextureFilename += TeapotTriMesh->M(0).map_Ks;
	//std::string SpecularTextureFilename = "../Resources/CubeMap/cubemap_posx.png";
	TeapotStaticMesh->m_SpecularTextureID = Engine::ModelHandling::GetModelHandler()->CreateTexture2D(SpecularTextureFilename.c_str(), 2);
	err = glGetError();
	if (err != 0)
	{
		printf("Error code: %d\n", err);
	}

	//Normal Map for Plane
	std::string PlaneNormalMapFilename = "../Resources/teapot_normal.png";
	PlaneStaticMesh->m_NormalMapID = Engine::ModelHandling::GetModelHandler()->CreateTexture2D(PlaneNormalMapFilename.c_str(), 5);

	//Displacement Map for Plane
	std::string PlaneDisplacementMapFilename = "../Resources/teapot_disp.png";
	PlaneStaticMesh->m_DisplacementMapID = Engine::ModelHandling::GetModelHandler()->CreateTexture2D(PlaneDisplacementMapFilename.c_str(), 6);
	if (err != 0)
	{
		printf("Error code: %d\n", err);
	}


	//Engine::Rendering::BuildAndUseProgram();
	/*cyGLSLProgram* MainSceneProgram = Engine::Rendering::BuildProgram(Engine::Rendering::SceneVertexShaderFile, Engine::Rendering::SceneFragmentShaderFile, 
		nullptr, Engine::Rendering::QuadTessControlShaderFile, Engine::Rendering::QuadTessEvalShaderFile);*/
	cyGLSLProgram* MainSceneProgram = Engine::Rendering::BuildProgram(Engine::Rendering::SceneVertexShaderFile, Engine::Rendering::SceneFragmentShaderFile);
	cyGLSLProgram* EdgeDetectionProgram = Engine::Rendering::BuildProgram(Engine::Rendering::EdgeDetectionVertexShaderFile, Engine::Rendering::EdgeDetectionFragmentShaderFile);
	cyGLSLProgram* BlendingWeightsProgram = Engine::Rendering::BuildProgram(Engine::Rendering::BlendingWeightsVertexShaderFile, Engine::Rendering::BlendingWeightsFragmentShaderFile);
	cyGLSLProgram* NeighborhoodBlendingProgram = Engine::Rendering::BuildProgram(Engine::Rendering::NeighborhoodBlendingVertexShaderFile, Engine::Rendering::NeighborhoodBlendingFragmentShaderFile);
	cyGLSLProgram* RenderTextureProgram = Engine::Rendering::BuildProgram(Engine::Rendering::RenderTextureVertexShaderFile, Engine::Rendering::RenderTextureFragmentShaderFile);
	cyGLSLProgram* CubeMapTextureProgram = Engine::Rendering::BuildProgram(Engine::Rendering::CubeMapTextureVertexShaderFile, Engine::Rendering::CubeMapTextureFragmentShaderFile);
	/*cyGLSLProgram* DebugDrawProgram = Engine::Rendering::BuildProgram(Engine::Rendering::SceneVertexShaderFile, Engine::Rendering::DebugDrawFragmentShaderFile, 
		Engine::Rendering::DebugDrawGeometryShaderFile, Engine::Rendering::QuadTessControlShaderFile, Engine::Rendering::QuadTessEvalShaderFile);*/
	err = glGetError();
	if (err != 0)
	{
		printf("Error code: %d\n", err);
	}

	

	float fovy = 45.f;
	float FOV = 1/tan(fovy);
	//float aspect = 2.f; //2:1
	float aspect = ((float)WindowWidth) / WindowHeight;
	float zNear = 0.1f;
	float zFar = 10000.f;

	Engine::Entity::Camera* MainSceneCamera = 
		new Engine::Entity::Camera(new Engine::Entity::GameObject(), 
		fovy, WindowWidth, WindowHeight, zNear, zFar);

	MainSceneCamera->GetGameObject()->SetPosition(cyPoint3f(0.f, 0.f, -60.f));
	MainSceneCamera->GetGameObject()->SetRotation(cyPoint3f(0.f, 0.f, 0.f));

	float* PerspectiveTest = MainSceneCamera->GetPerspectiveProjection().data;
	float* CameraTransformTest = MainSceneCamera->GetGameObject()->GetTransform().data;

	//Engine::Input::CameraGameObject = MainSceneCamera->GetGameObject();
	Engine::Input::SetCameraGameObject(MainSceneCamera->GetGameObject());


	/********************** Create RenderBuffers *************************/

	//Create RenderTexture for teapot's RenderToTexture operation
	

	cyGLRenderTexture2D* SceneRenderTexture = new cyGLRenderTexture2D();
	Engine::Rendering::CreateRenderBuffer(SceneRenderTexture, true, 4, WindowWidth, WindowHeight, GL_TEXTURE3);

	cyGLRenderTexture2D* EdgesTexture = new cyGLRenderTexture2D();
	Engine::Rendering::CreateRenderBuffer(EdgesTexture, true, 4, WindowWidth, WindowHeight, GL_TEXTURE3);


	/************************* Create CubeMap ***************************/
	//CubeMap
	unsigned int cubeMapVertexArrayID;
	glGenVertexArrays(1, &cubeMapVertexArrayID);
	glBindVertexArray(cubeMapVertexArrayID);

	//Loading Cube
	cyTriMesh* CubeTriMesh = new cyTriMesh();
	CubeTriMesh->LoadFromFileObj("../Resources/CubeMap/cube.obj", false);
	std::cout << "\nCubeTriMesh details:\n";
	std::cout << "Number of Vertices: " << CubeTriMesh->NV() << "\n";
	std::cout << "Number of VertNormals: " << CubeTriMesh->NVN() << "\n";
	std::cout << "Number of Faces: " << CubeTriMesh->NF() << "\n";
	std::cout << "Number of indices in Faces: " << sizeof(CubeTriMesh->F(0).v) / sizeof(CubeTriMesh->F(0).v[0]) << "\n";
	std::cout << "Number of VertexTexCoords: " << CubeTriMesh->NVT() << "\n";
	std::cout << "Number of Materials: " << CubeTriMesh->NM() << "\n";


	Engine::ModelHandling::GetModelHandler()->AddVertexPositions(CubeTriMesh, cubeMapVertexArrayID);

	Engine::Entity::StaticMesh* CubeMapStaticMesh = new Engine::Entity::StaticMesh(new Engine::Entity::GameObject(), cubeMapVertexArrayID);
	const_cast<Engine::Entity::GameObject*>(CubeMapStaticMesh->GetGameObject())->SetPosition(MainSceneCamera->GetGameObject()->GetPosition());
	const_cast<Engine::Entity::GameObject*>(CubeMapStaticMesh->GetGameObject())->SetRotation(cyPoint3f(0.f, 0.f, 0.f));

	//CubeMap Texture
	const unsigned int numCubeMapTextureFaces = 6; 
	std::string CubeMapTextureFaces[numCubeMapTextureFaces] = { "../Resources/CubeMap/cubemap_posx.png", "../Resources/CubeMap/cubemap_negx.png",
											"../Resources/CubeMap/cubemap_posy.png", "../Resources/CubeMap/cubemap_negy.png",
											"../Resources/CubeMap/cubemap_posz.png", "../Resources/CubeMap/cubemap_negz.png" 
										};
	
	CubeMapStaticMesh->m_DiffuseTextureID = Engine::ModelHandling::GetModelHandler()->CreateTextureCubeMap(CubeMapTextureFaces, numCubeMapTextureFaces, 4);


	Engine::Rendering::GetRenderSpotLight().GetGameObject()->SetRotation(
		cyPoint3f(0.f, 0.f, 0.f) - Engine::Rendering::GetRenderSpotLight().GetGameObject()->GetPosition()
	);
	Engine::Input::SetShiftBoundGameObject(Engine::Rendering::GetRenderSpotLight().GetGameObject());

	err = glGetError();
	if (err != 0)
	{
		printf("Error code: %d\n", err);
	}

	glEnable(GL_DEPTH_TEST);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		 dt = Engine::Timing::GetLastFrameTime_ms();

		/* Render here */
		 glClearColor(0.f, 0.f, 0.f, 1.f);
		 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//TODO handle order of code if Rendering Update does all the rendering
		Engine::Input::Update(window, dt);
		Engine::Rendering::Update(window, dt);

/* ****************** Bind Render Texture ****************** */
		//Bind our own RenderTexture
		//glActiveTexture(GL_TEXTURE3);
		SceneRenderTexture->Bind();

		//SceneRenderTexture->BindTexture(3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, SceneRenderTexture->GetTextureID());
		glClearColor(0.2f, 0.2f, 0.2f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


/********************* Render CubeMap **********************/
		//Render CubeMap
		glDisable(GL_DEPTH_TEST);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}
		cyPoint3f NewPos(-MainSceneCamera->GetGameObject()->GetPosition().x, -MainSceneCamera->GetGameObject()->GetPosition().y, -MainSceneCamera->GetGameObject()->GetPosition().z);
		CubeMapStaticMesh->GetGameObject()->SetPosition(NewPos);
		CubeMapTextureProgram->Bind();
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapStaticMesh->m_DiffuseTextureID);
		CubeMapTextureProgram->SetUniform("u_CubeMapSampler", 4);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}
		CubeMapTextureProgram->SetUniformMatrix4("u_Projection", MainSceneCamera->GetPerspectiveProjection().data);
		CubeMapTextureProgram->SetUniformMatrix4("u_Camera", MainSceneCamera->GetGameObject()->GetTransform().data);
		CubeMapTextureProgram->SetUniformMatrix4("u_CubeObject", CubeMapStaticMesh->GetGameObject()->GetTransform().data);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		glBindVertexArray(CubeMapStaticMesh->GetVertexArrayID());
		glDrawArrays(GL_TRIANGLES, 0, CubeTriMesh->NF() * sizeof(CubeTriMesh->F(0).v) / sizeof(CubeTriMesh->F(0).v[0]));
		glEnable(GL_DEPTH_TEST);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		
/****************** Render Main Scene ********************/
		//Render the scene to our RenderTexture
		MainSceneProgram->Bind();
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		MainSceneProgram->SetUniformMatrix4("u_Projection", MainSceneCamera->GetPerspectiveProjection().data);
		MainSceneProgram->SetUniformMatrix4("u_Camera", MainSceneCamera->GetGameObject()->GetTransform().data);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		//TODO make PointLight have a GameObject
		const cyPoint3f PointLightPos3 = Engine::Rendering::GetRenderPointLight().GetPosition().GetNonHomogeneous();

		const cyPoint3f SpotLightPos3 = Engine::Rendering::GetRenderSpotLight().GetGameObject()->GetPosition();
		
		MainSceneProgram->SetUniform("u_LightPosition", SpotLightPos3);
		MainSceneProgram->SetUniform("u_CameraPosition", MainSceneCamera->GetGameObject()->GetPosition());
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapStaticMesh->m_DiffuseTextureID);
		MainSceneProgram->SetUniform("u_CubeMapSampler", 4);
		MainSceneProgram->SetUniform("u_AmbientConstant", Engine::Rendering::AmbientConstant);
		/*MainSceneProgram->SetUniform("u_SpecularExponent", Engine::Rendering::MaterialSpecularExponent);*/
		MainSceneProgram->SetUniform("u_SpecularExponent", Engine::Rendering::SpecularAlpha);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		//Set TextureSampler uniforms
		/*glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TeapotStaticMesh->m_AmbientTextureID);
		//MainSceneProgram->SetUniform("u_AmbientTextureSampler", GL_TEXTURE0);
		MainSceneProgram->SetUniform("u_AmbientTextureSampler", 0);
		//glUniform1i(glGetUniformLocation(MainSceneProgram->GetID(), "u_AmbientTextureSampler"), 0)*/
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, TeapotStaticMesh->m_DiffuseTextureID);
		MainSceneProgram->SetUniform("u_DiffuseTextureSampler", 1);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, TeapotStaticMesh->m_SpecularTextureID);
		MainSceneProgram->SetUniform("u_SpecularTextureSampler", 2);


		

		//Drawing code
		MainSceneProgram->SetUniformMatrix4("u_Object", TeapotStaticMesh->GetGameObject()->GetTransform().data); 
		glBindVertexArray(TeapotStaticMesh->GetVertexArrayID());

		//TODO put the vertices and all data into GameObject or something
		glDrawArrays(GL_TRIANGLES, 0, TeapotTriMesh->NF() * sizeof(TeapotTriMesh->F(0).v) / sizeof(TeapotTriMesh->F(0).v[0]));
		//glDrawElements(GL_TRIANGLES, TeapotTriMesh->NF() * sizeof(TeapotTriMesh->F(0))/sizeof(TeapotTriMesh->F(0).v[0]), GL_UNSIGNED_INT, &TeapotTriMesh->F(0));
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		
		//Unbind our RenderTexture so normal rendering buffers are brought back
		SceneRenderTexture->Unbind();
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}
		

		/********************** Edge Detection *************************/
		
		EdgesTexture->Bind();
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, EdgesTexture->GetTextureID());
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		EdgeDetectionProgram->Bind();
		bool ProgramBuilt = !EdgeDetectionProgram->IsNull();
		assert(ProgramBuilt);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}


		//Bind the texture that the teapot scene was rendered to as the texture for the plane
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, SceneRenderTexture->GetTextureID());
		glGenerateMipmap(GL_TEXTURE_2D);
		EdgeDetectionProgram->SetUniform("u_RenderToSampler", 3);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}
		

		//Draw plane on usual rendering buffers
		
		
		//Render the plane
		glBindVertexArray(PlaneStaticMesh->GetVertexArrayID());
		glDrawArrays(GL_TRIANGLES, 0, APlaneVertPos.size());
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		EdgesTexture->Unbind();
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		/********************* Blending Weights Pass ***********************/
		
		BlendingWeightsProgram->Bind();
		ProgramBuilt = !BlendingWeightsProgram->IsNull();
		assert(ProgramBuilt);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Bind the Edges Texture
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, EdgesTexture->GetTextureID());
		glGenerateMipmap(GL_TEXTURE_2D);
		BlendingWeightsProgram->SetUniform("u_EdgesTexSampler", 3);
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}


		//Draw plane on usual rendering buffers


		//Render the plane
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}

		glBindVertexArray(PlaneStaticMesh->GetVertexArrayID());
		glDrawArrays(GL_TRIANGLES, 0, APlaneVertPos.size());
		err = glGetError();
		if (err != 0)
		{
			printf("Error code: %d\n", err);
		}


/***********************      *** Debug Draw ***       ***********************************/
		/*if (Engine::Input::IsDebugDrawRequested())
		{
			DebugDrawProgram->Bind();
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}

			DebugDrawProgram->SetUniformMatrix4("u_Projection", MainSceneCamera->GetPerspectiveProjection().data);
			DebugDrawProgram->SetUniformMatrix4("u_Camera", MainSceneCamera->GetGameObject()->GetTransform().data);
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}

			DebugDrawProgram->SetUniformMatrix4("u_Object", PlaneStaticMesh->GetGameObject()->GetTransform().data);

			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_2D, PlaneStaticMesh->m_DisplacementMapID);
			DebugDrawProgram->SetUniform("u_DisplacementMap", 6);
			DebugDrawProgram->SetUniform("u_DisplacementFactor_ModelSpace", PlaneStaticMesh->m_DisplacementFactor);
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}

			//Set Tessellation uniforms
			DebugDrawProgram->SetUniform("TessellationControlLevel", Engine::Input::GetTessellationLevel());
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}


			glBindVertexArray(PlaneStaticMesh->GetVertexArrayID());
			//glDrawArrays(GL_TRIANGLES, 0, APlaneVertPos.size());
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			glPatchParameteri(GL_PATCH_VERTICES, Engine::ModelHandling::GetModelHandler()->NumPatchVertices);
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
			glDrawArrays(GL_PATCHES, 0, APlaneVertPos.size());
			err = glGetError();
			if (err != 0)
			{
				printf("Error code: %d\n", err);
			}
		}*/
		

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
