#include "Utils.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define WINDOW_TITLE "OpenGL Window"

GLFWwindow* window;

unsigned FrameCount = 0;

void Initialize(void);
//void InitWindow(int, char* []);
void ResizeFunction(GLFWwindow*, int, int);
void RenderFunction(void);
void TimerFunction(int);
void KeyboardFunction(GLFWwindow*);
void IdleFunction(void);
void CreateObj(void);
void DrawObj(void);
void DeleteObj(void);

int main(int argc, char* argv[])
{
	Initialize();

	while (!glfwWindowShouldClose(window))
	{
		KeyboardFunction(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}

void Initialize()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(800, 600, WINDOW_TITLE, NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, ResizeFunction);

	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << "\n";
}

void ResizeFunction(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void RenderFunction(void)
{
	++FrameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.1f, 1.0f);
}

void KeyboardFunction(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}