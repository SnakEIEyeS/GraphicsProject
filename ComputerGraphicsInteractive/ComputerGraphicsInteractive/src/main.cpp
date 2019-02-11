#include <glad/glad.h>
#include <GL/glfw3.h>

#include <iostream>
#include <math.h>

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
	VertexNormal = 1
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
	
	//Loading obj
	cyTriMesh* TriMeshObj = new cyTriMesh();
	TriMeshObj->LoadFromFileObj("../Resources/teapot.obj", false);
	std::cout << "Number of Vertices: " << TriMeshObj->NV() << "\n";
	std::cout << "Number of VertNormals: " << TriMeshObj->NVN() << "\n";
	std::cout << "Number of Faces: " << TriMeshObj->NF() << "\n";
	std::cout << "Number of TexCoords: " << TriMeshObj->NVT() << "\n";


	//VBO
	unsigned int vertexPosBufferID;
	glGenBuffers(1, &vertexPosBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPosBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->V(0)) * TriMeshObj->NV(), const_cast<void*>(reinterpret_cast<void*>(&TriMeshObj->V(0))), GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(VertexInfo::VertexPosition);
	glVertexAttribPointer(VertexInfo::VertexPosition, 3, GL_FLOAT, GL_FALSE, sizeof(TriMeshObj->V(0)), (const void*)0);
    

	//Vertex Normals
	unsigned int vertexNormalBufferID;
	glGenBuffers(1, &vertexNormalBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexNormalBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->VN(0))*TriMeshObj->NVN(), const_cast<void*>(reinterpret_cast<void*>(&TriMeshObj->VN(0))), GL_STATIC_DRAW);

	glEnableVertexAttribArray(VertexInfo::VertexNormal);
	glVertexAttribPointer(VertexInfo::VertexNormal, sizeof(TriMeshObj->VN(0)) / sizeof(TriMeshObj->VN(0).x), GL_FLOAT, GL_FALSE, sizeof(TriMeshObj->VN(0)), (const void*)0);
	

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
		//float APointLightUniform[3];
		//PointLightPos3.Get(APointLightUniform);
		Engine::Rendering::GetGLProgram()->SetUniform("u_LightPosition", PointLightPos3);
		Engine::Rendering::GetGLProgram()->SetUniform("u_DiffuseColor", cyPoint4f(1.f, 0.f, 0.f, 1.f));
		Engine::Rendering::GetGLProgram()->SetUniform("u_SpecularColor", cyPoint4f(1.f, 1.f, 1.f, 1.f));
		Engine::Rendering::GetGLProgram()->SetUniform("u_AmbientConstant", Engine::Rendering::AmbientConstant);
		Engine::Rendering::GetGLProgram()->SetUniform("u_SpecularAlpha", Engine::Rendering::SpecularAlpha);

		//Drawing code
		glBindVertexArray(vertexArrayID);
		//glBindBuffer(GL_ARRAY_BUFFER, vertexPosBufferID);
		//glDrawArrays(GL_TRIANGLES, 0, TriMeshObj->NV());
		glDrawElements(GL_TRIANGLES, TriMeshObj->NF() * sizeof(TriMeshObj->F(0))/sizeof(TriMeshObj->F(0).v[0]), GL_UNSIGNED_INT, &TriMeshObj->F(0));
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
