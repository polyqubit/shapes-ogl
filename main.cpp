#include "Utils.h"
#include "Shader.h"
#include "Camera.h"
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

GLuint BufferIds[3] = { 0 };

unsigned FrameCount = 0;

float CubeRotation = 0;
clock_t LastTime = 0;
clock_t LastTimeKey = 0;

Shader shaders;

glm::mat4 model = glm::mat4(1.0f); // use this to apply geometric transformations
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection;

// identity matrix
const glm::mat4 identity = glm::mat4(1.0f);

// initialize random engine
std::random_device rd;
std::mt19937 gen(rd());

std::uniform_real_distribution<> posDist(-20, 20);
std::uniform_real_distribution<> rotDist(-1, 1);

glm::vec3 posArr[1500];

glm::vec3 rotArr[1500];

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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");

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

	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
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

	if (!camMode) {
		glm::vec3 direction;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
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
	const Vertex VERTICES[6] =
	{
		// front part (both are clockwise)
		{ { 0, 1, -.75,          1 }, { 1, 0.5, 0.5,    1 } },
		{ { .866f, -.5f, -.75,   1 }, { 0.5, 1, 0.5,    1 } },
		{ { -.866f, -.5f, -.75,  1 }, { 0.5, 0.5, 1,    1 } },
		// back part
		{ { 0, 1, .75,           1 }, { 1, 0.5, 0.5,    1 } },
		{ { .866f, -.5f, .75,    1 }, { 0.5, 1, 0.5,    1 } },
		{ { -.866f, -.5f, .75 ,  1 }, { 0.5, 0.5, 1,    1 } }
	};

	const GLuint INDICES[24] =
	{
	  0,1,2,
	  0,3,4,
	  0,4,1,
	  0,5,3,
	  0,2,5,
	  1,5,2,
	  1,4,5,
	  3,5,4
	};


	int posAL = sizeof(posArr) / sizeof(glm::vec3);
	int rotAL = sizeof(rotArr) / sizeof(glm::vec3);
	for (unsigned int i = 0; i < posAL; i++) {
		posArr[i] = glm::vec3(posDist(gen), posDist(gen), posDist(gen));
	}
	for (unsigned int i = 0; i < rotAL; i++) {
		rotArr[i] = glm::vec3(rotDist(gen), rotDist(gen), rotDist(gen));
	}

	shaders = Shader("VertexShader.glsl", "FragmentShader.glsl");
	shaders.use();

	// model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	// set camera
	view = camera.GetViewMatrix();

	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 50.0f);

	shaders.setMat4("view", view);
	shaders.setMat4("projection", projection);

	glGenVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not generate the VAO");
	glBindVertexArray(BufferIds[0]);
	ExitOnGLError("ERROR: Could not bind the VAO");

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	ExitOnGLError("ERROR: Could not enable vertex attributes");

	glGenBuffers(2, &BufferIds[1]);
	ExitOnGLError("ERROR: Could not generate the buffer objects");

	glBindBuffer(GL_ARRAY_BUFFER, BufferIds[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTICES), VERTICES, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the VBO to the VAO");

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(VERTICES[0]), (GLvoid*)sizeof(VERTICES[0].Position));
	ExitOnGLError("ERROR: Could not set VAO attributes");

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIds[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(INDICES), INDICES, GL_STATIC_DRAW);
	ExitOnGLError("ERROR: Could not bind the IBO to the VAO");

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

	shaders.use();
	//float sinangle = abs(sin(angle / 64.0f)) * 1.5f;
	//model = glm::rotate(model, glm::radians(15.0f)/CLOCKS_PER_SEC, glm::vec3(-1.0, 1.0, 0.5));
	//model = glm::scale(model, glm::vec3(sinangle,sinangle,sinangle));

	int lengthA = sizeof(posArr) / sizeof(glm::vec3);
	glBindVertexArray(BufferIds[0]);
	for (unsigned int i = 0; i < lengthA; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, posArr[i]);
		model = glm::rotate(model, glm::radians(i * 20.0f + angle), rotArr[i]);
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		shaders.setMat4("view", view);
		shaders.setMat4("model", model);
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (GLvoid*)0);
	}
	glBindVertexArray(0);
}

void DeleteObj(void) {
	glDeleteBuffers(2, &BufferIds[1]);
	glDeleteVertexArrays(1, &BufferIds[0]);
	ExitOnGLError("ERROR: Could not destroy the buffer objects");
}