#include <glad/glad.h>
#include <GL/glfw3.h>

#include <iostream>
#include <math.h>

#include "Engine/Engine.h"
#include "Timer/FrameTime.h"
#include "Input/Input.h"
#include "Rendering/Rendering.h"

#include "cyCodeBase/cyGL.h"
#include "cyCodeBase/cyMatrix.h"
#include "cyCodeBase/cyTriMesh.h"


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

	//VBO
	unsigned int vertexBufferID;
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TriMeshObj->V(0)) * TriMeshObj->NV(), const_cast<void*>(reinterpret_cast<void*>(&TriMeshObj->V(0))), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(*TriMeshObj), const_cast<void*>(reinterpret_cast<void*>(sizeof(TriMeshObj->V(0)) * TriMeshObj->NV())));

	/*cyGLSLProgram* TeapotProgram = new cyGLSLProgram();
	bool built = TeapotProgram->BuildFiles("res/BasicVS.shader", "res/BasicFS.shader", nullptr, nullptr, nullptr);
	assert(built);
	TeapotProgram->Bind();*/
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
		0.0f, 0.0f, 1.0f, 30.0f,
		0.0f, 0.f, 0.f, 1.0f
	);
	cyMatrix4f ProjectionMatrix(
		FOV/aspect, 0.0f,                          0.0f,                                0.0f,
		0.0f,       FOV,                           0.0f,                                0.0f,
		0.0f,       0.0f, (zFar + zNear)/(zNear - zFar), (2.f * zFar * zNear)/(zNear - zFar),
		0.0f,       0.0f,                         -1.0f,                                 0.0f
	);
	ProjectionMatrix = ProjectionMatrix * CameraMatrix;
	float UniformMatrix[16];
	ProjectionMatrix.Get(UniformMatrix);

	Engine::Rendering::GetGLProgram()->SetUniformMatrix4("u_Transformation", UniformMatrix);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		 dt = Engine::Timing::GetLastFrameTime_ms();

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
		
		Engine::Input::Update(window, dt);
		Engine::Rendering::Update(window, dt);

		//Drawing code
		glBindVertexArray(vertexArrayID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glDrawArrays(GL_TRIANGLES, 0, TriMeshObj->NV());
		//glDrawArrays(GL_TRIANGLES, 2, 50);
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
