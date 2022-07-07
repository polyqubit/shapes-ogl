#include "Utils.h"
#include "Shader.h"
#include "Camera.h"
#include "Shape.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define WINDOW_TITLE "OpenGL Window"

GLFWwindow* window;

unsigned frameCount = 0;

float angle = 0;
float movCam = 0;
float cameraSpeed = 0;
bool camMode = false, firstMouse = true;
Camera camera;

float lastX = 400, lastY = 300;

GLuint BufferIds[10] = { 0 };

GLuint VBOIds[10] = { 0 };
GLuint VAOIds[10] = { 0 };

unsigned FrameCount = 0;

float CubeRotation = 0;
clock_t LastTime = 0;
clock_t LastTimeKey = 0;

Shader generalshaders;
Shader lightshaders;

glm::mat4 model = glm::mat4(1.0f); // use this to apply geometric transformations
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection;

glm::vec3 lightPos = glm::vec3(0.0, 20.0, 0.0);

// identity matrix
const glm::mat4 identity = glm::mat4(1.0f);

// initialize random engine
//std::random_device rd;
//std::mt19937 gen(rd());
//
//std::uniform_real_distribution<> posDist(-20, 20);
//std::uniform_real_distribution<> rotDist(-1, 1);
//
//glm::vec3 cPosArr[750];
//glm::vec3 tpPosArr[750];
//
//glm::vec3 cRotArr[750];
//glm::vec3 tpRotArr[750];

void Initialize(void);
//void InitWindow(int, char* []);
void ResizeFunction(GLFWwindow*, int, int);
void RenderFunction(GLFWwindow*);
void TimerFunction(int);
void KeyboardFunction(GLFWwindow*);
void MouseFunction(GLFWwindow*, double, double);
void IdleFunction(void);
void CreateObj(void);
void DrawObj(void);
//void DeleteObj(void);

int main(int argc, char* argv[])
{
	Initialize();

	while (!glfwWindowShouldClose(window))
	{
		KeyboardFunction(window);

		RenderFunction(window);

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

	glewExperimental = GL_TRUE;
	GLenum GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) {
		std::cout << "Failed to initialize glew " << glewGetErrorString(GlewInitResult) << "\n";
		exit(EXIT_FAILURE);
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, ResizeFunction);

	CreateObj();

	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << "\n";

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");*/

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseFunction);

	glfwSetWindowPos(window, 400, 100);

	std::cout << "CONTROLS:\n";
	std::cout << "W         FORWARD\n";
	std::cout << "A         LEFT\n";
	std::cout << "S         BACK\n";
	std::cout << "D         RIGHT\n";
	std::cout << "Q         ORBIT\n";
	std::cout << "E         CONTROL\n";
	std::cout << "UP        UP\n";
	std::cout << "DOWN      DOWN\n";
	std::cout << "LEFT      ROTATE LEFT\n";
	std::cout << "RIGHT     ROTATE RIGHT\n";
	std::cout << "ESCAPE    CLOSE WINDOW\n";
}

void ResizeFunction(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void RenderFunction(GLFWwindow* window)
{
	++FrameCount;

	//glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawObj();
	glfwSwapBuffers(window);

}

void KeyboardFunction(GLFWwindow* window)
{
	clock_t Now = clock();
	if (LastTimeKey == 0)
		LastTimeKey = Now;
	float number = ((float)(Now - LastTimeKey) / CLOCKS_PER_SEC);
	cameraSpeed = 5 * number;
	LastTimeKey = Now;

	if ((glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) && !camMode)
		camMode = true;
	if ((glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) && camMode)
		camMode = false;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (!camMode) {
		glm::vec3 direction;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.Position += cameraSpeed * camera.Front;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.Position -= cameraSpeed * camera.Front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.Position -= glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.Position += glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			camera.Position += camera.Up * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			camera.Position -= camera.Up * cameraSpeed;
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		{
			camera.Yaw -= 60.0f * number;
			direction.x = cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
			direction.y = sin(glm::radians(camera.Pitch));
			direction.z = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
			camera.Front = glm::normalize(direction);
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		{
			camera.Yaw += 60.0f * number;
			direction.x = cos(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
			direction.y = sin(glm::radians(camera.Pitch));
			direction.z = sin(glm::radians(camera.Yaw)) * cos(glm::radians(camera.Pitch));
			camera.Front = glm::normalize(direction);
		}
	}
}

void MouseFunction(GLFWwindow* window, double x, double y)
{
	if (firstMouse) // initially set to true
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed: y ranges bottom to top
	lastX = x;
	lastY = y;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void CreateObj() {
	/*int posAL = sizeof(cPosArr) / sizeof(glm::vec3);
	int rotAL = sizeof(cRotArr) / sizeof(glm::vec3);
	for (unsigned int i = 0; i < posAL; i++) {
		cPosArr[i] = glm::vec3(posDist(gen), posDist(gen), posDist(gen));
	}
	for (unsigned int i = 0; i < rotAL; i++) {
		cRotArr[i] = glm::vec3(rotDist(gen), rotDist(gen), rotDist(gen));
	}
	for (unsigned int i = 0; i < posAL; i++) {
		tpPosArr[i] = glm::vec3(posDist(gen), posDist(gen), posDist(gen));
	}
	for (unsigned int i = 0; i < rotAL; i++) {
		tpRotArr[i] = glm::vec3(rotDist(gen), rotDist(gen), rotDist(gen));
	}*/

	generalshaders = Shader("GeneralVertexShader.glsl", "GeneralFragmentShader.glsl");
	lightshaders = Shader("LightVertexShader.glsl", "LightFragmentShader.glsl");
	ExitOnGLError("ERROR: Could not load shaders");

	// model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// set camera
	view = camera.GetViewMatrix();

	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 50.0f);
	
	generalshaders.use();
	generalshaders.setMat4("view", view);
	generalshaders.setMat4("projection", projection);
	ExitOnGLError("ERROR: Could not set uniforms for general shader");

	lightshaders.use();
	lightshaders.setMat4("view", view);
	lightshaders.setMat4("projection", projection);
	ExitOnGLError("ERROR: Could not set uniforms for light shader");

	/*glGenVertexArrays(1, &VAOIds[0]);
	ExitOnGLError("ERROR: Could not generate the VAOs");
	glBindVertexArray(VAOIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	ExitOnGLError("ERROR: Could not enable vertex attributes");

	glGenBuffers(2, &BufferIds[1]);
	ExitOnGLError("ERROR: Could not generate the buffer objects");

	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(cubeStruct.VERTICES[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(cubeStruct.VERTICES[0]), (GLvoid*)sizeof(cubeStruct.VERTICES[0].Position));
	ExitOnGLError("ERROR: Could not set VAO attributes");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeStruct.INDICES), cubeStruct.INDICES, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the IBO to the VAO");*/


	/*
	starting from the specified location, glGen...() returns a list of integers that are currently available to be bound
	number = number of ints to be generated, stored contiguously
	*/
	glGenBuffers(3, &VBOIds[0]);
	ExitOnGLError("ERROR: Could not generate the buffer objects");

	glGenVertexArrays(2, &VAOIds[0]);
	ExitOnGLError("ERROR: Could not generate the VAOs");

	// VBO for lightable cube
	glBindBuffer(GL_ARRAY_BUFFER, VBOIds[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(clStruct.VERTICES), clStruct.VERTICES, GL_STATIC_DRAW);

	// VAO for lightable cube
	glBindVertexArray(VAOIds[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO");
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	ExitOnGLError("ERROR: Could not enable vertex attributes");
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	ExitOnGLError("ERROR: Could not set VAO attributes");

	//// generate index buffer object for cube VAO
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIds[2]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeStruct.INDICES), cubeStruct.INDICES, GL_STATIC_DRAW);
	//ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

	// VBO for light source
	glBindBuffer(GL_ARRAY_BUFFER, VBOIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeStruct.VERTICES), cubeStruct.VERTICES, GL_STATIC_DRAW);

	// VAO for light source
	glBindVertexArray(VAOIds[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOIds[1]);
	ExitOnGLError("ERROR: Could not bind the VAO");
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	ExitOnGLError("ERROR: Could not enable vertex attributes");
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(cubeStruct.VERTICES[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(cubeStruct.VERTICES[0]), (GLvoid*)sizeof(cubeStruct.VERTICES[0].Position));
	ExitOnGLError("ERROR: Could not set VAO attributes");

	// generate index buffer object for light VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeStruct.INDICES), cubeStruct.INDICES, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

	//// VBO for triprism
	//glBindBuffer(GL_ARRAY_BUFFER, VBOIds[1]);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(tpStruct.VERTICES), tpStruct.VERTICES, GL_STATIC_DRAW);
	//
	//// VAO for triprism
	//glBindVertexArray(VAOIds[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, VBOIds[1]);
	//ExitOnGLError("ERROR: Could not bind the VAO");
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//ExitOnGLError("ERROR: Could not enable vertex attributes");
	//glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(tpStruct.VERTICES[0]), (GLvoid*)0);
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(tpStruct.VERTICES[0]), (GLvoid*)sizeof(tpStruct.VERTICES[0].Position));
	//ExitOnGLError("ERROR: Could not set VAO attributes");

	//// generate index buffer object for VAO2
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOIds[3]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tpStruct.INDICES), tpStruct.INDICES, GL_STATIC_DRAW);
	//ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

	glBindVertexArray(0);
}


void DrawObj(void) {
	clock_t Now = clock();
	if (LastTime == 0)
		LastTime = Now;

	float number = ((float)(Now - LastTime) / CLOCKS_PER_SEC);
	angle += 180.0f * number;
	movCam += 1.0f * number;
	// important: n * ((float)(Now - LastTime) / CLOCKS_PER_SEC
	LastTime = Now;

	if (camMode) {
		const float radius = 20.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0));
	}
	else {
		view = camera.GetViewMatrix();
	}

	//float sinangle = abs(sin(angle / 64.0f)) * 1.5f;
	//model = glm::rotate(model, glm::radians(15.0f)/CLOCKS_PER_SEC, glm::vec3(-1.0, 1.0, 0.5));
	//model = glm::scale(model, glm::vec3(sinangle,sinangle,sinangle));


	generalshaders.use();
	generalshaders.setVec3("light_Pos", lightPos);
	generalshaders.setVec3("view_Pos", camera.Position);
	generalshaders.setVec3("light_Color", glm::vec3(1.0f, 1.0f, 1.0f));
	generalshaders.setVec3("object_Color", glm::vec3(0.8f, 0.5f, 0.1f));
	// draw cube
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
	model = glm::rotate(model, glm::radians(2 * 20.0f + angle), glm::vec3(1.0));
	model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
	generalshaders.setMat4("view", view);
	generalshaders.setMat4("model", model);

	glBindVertexArray(VAOIds[0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// light cube
	int lengthI = sizeof(cubeStruct.INDICES) / sizeof(GLuint);
	lightshaders.use();
	model = glm::mat4(1.0f);
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(2.0f));
	lightshaders.setMat4("view", view);
	lightshaders.setMat4("model", model);

	glBindVertexArray(VAOIds[1]);
	glDrawElements(GL_TRIANGLES, lengthI, GL_UNSIGNED_INT, (GLvoid*)0);
	//lengthA = sizeof(tpPosArr) / sizeof(glm::vec3);
	//lengthI = sizeof(tpStruct.INDICES) / sizeof(GLuint);
	//// draw triprisms
	//for (unsigned int i = 0; i < lengthA; i++) {
	//	model = glm::mat4(1.0f);
	//	model = glm::translate(model, tpPosArr[i]);
	//	model = glm::rotate(model, glm::radians(i * 20.0f + angle), tpRotArr[i]);
	//	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	//	generalshaders.setMat4("view", view);
	//	generalshaders.setMat4("model", model);

	//	glBindVertexArray(VAOIds[1]);
	//	glDrawElements(GL_TRIANGLES, lengthI, GL_UNSIGNED_INT, (GLvoid*)0);
	//}
	glBindVertexArray(0);
}

void DeleteObj(void) {
	glDeleteBuffers(2, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}