#include "Utils.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define WINDOW_TITLE_PREFIX "OpenGL Window"

int
CurrentWidth = 1200,
CurrentHeight = 900,
WindowHandle = 0;

const float cameraSpeed = 0.1f;

float angle = 0;
float movCam = 0;

GLuint BufferIds[3] = { 0 };

unsigned FrameCount = 0;

float CubeRotation = 0;
clock_t LastTime = 0;

Shader shaders;

glm::mat4 model = glm::mat4(1.0f); // use this to apply geometric transformations
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection;

// identity matrix
const glm::mat4 identity = glm::mat4(1.0f);

// initialize random engine
std::random_device rd;
std::mt19937 gen(rd());

std::uniform_real_distribution<> posDist(-5, 5);
std::uniform_real_distribution<> rotDist(-1, 1);

glm::vec3 posArr[100];

glm::vec3 rotArr[100];

// camera's coordinate system
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

void Initialize(int, char* []);
void InitWindow(int, char* []);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
void KeyboardFunction(unsigned char, int, int);
void PassiveMovementFunction(int, int);
void IdleFunction(void);
void CreateObj(void);
void DrawObj(void);
void DeleteObj(void);

int main(int argc, char* argv[])
{
	Initialize(argc, argv);

	glutMainLoop();

	DeleteObj();
	exit(EXIT_SUCCESS);
}

void Initialize(int argc, char* argv[])
{
	GLenum GlewInitResult;

	InitWindow(argc, argv);

	glewExperimental = GL_TRUE;
	GlewInitResult = glewInit();

	if (GLEW_OK != GlewInitResult) {
		fprintf(
			stderr,
			"ERROR: %s\n",
			glewGetErrorString(GlewInitResult)
		);
		exit(EXIT_FAILURE);
	}

	fprintf(
		stdout,
		"INFO: OpenGL Version: %s\n",
		glGetString(GL_VERSION)
	);

	glGetError();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	ExitOnGLError("ERROR: Could not set OpenGL depth testing options");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	ExitOnGLError("ERROR: Could not set OpenGL culling options");

	CreateObj();
}

void InitWindow(int argc, char* argv[])
{
	glutInit(&argc, argv);

	glutInitContextVersion(4, 0);
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS
	);

	glutInitWindowSize(CurrentWidth, CurrentHeight);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

	if (WindowHandle < 1) {
		fprintf(
			stderr,
			"ERROR: Could not create a new rendering window.\n"
		);
		exit(EXIT_FAILURE);
	}

	glutSetCursor(GLUT_CURSOR_NONE);

	glutReshapeFunc(ResizeFunction);
	glutDisplayFunc(RenderFunction);
	glutKeyboardFunc(KeyboardFunction);
	glutPassiveMotionFunc(PassiveMovementFunction);
	glutIdleFunc(IdleFunction);
	glutTimerFunc(0, TimerFunction, 0);
}

void ResizeFunction(int Width, int Height)
{
	CurrentWidth = Width;
	CurrentHeight = Height;
	glViewport(0, 0, CurrentWidth, CurrentHeight);
	glUseProgram(0);
}

void RenderFunction(void)
{
	++FrameCount;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::cout << glutLayerGet(GLUT_LAYER_IN_USE);

	glutWarpPointer(600, 450);

	DrawObj();

	glutSwapBuffers();
}

void KeyboardFunction(unsigned char Key, int X, int Y)
{
	switch (Key) 
	{
		case 27: {
			glutDestroyWindow(glutGetWindow());
			break;
		}
		case 'W': case 'w': {
			cameraPos += cameraSpeed * cameraFront;
			break;
		}
		case 'A': case 'a': {
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;
		}
		case 'S': case 's': {
			cameraPos -= cameraSpeed * cameraFront;
			break;
		}
		case 'D': case 'd': {
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
			break;
		}
		case 'Q': case 'q': {
			break;
		}
	}
}

void PassiveMovementFunction(int X, int Y)
{
	std::cout << glutLayerGet(GLUT_HAS_OVERLAY);
	if (X != 600 || Y != 450) glutWarpPointer(600, 450);
}

void IdleFunction(void)
{
	glutPostRedisplay();
}

void TimerFunction(int Value)
{
	if (0 != Value) {
		char* TempString = new char[512];

		sprintf(
			TempString,
			"%s: %d Frames Per Second @ %d x %d",
			WINDOW_TITLE_PREFIX,
			FrameCount * 4,
			CurrentWidth,
			CurrentHeight
		);

		glutSetWindowTitle(TempString);
		delete[] TempString;
	}

	FrameCount = 0;
	glutTimerFunc(250, TimerFunction, 1);
}

void CreateObj(void) {
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
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

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
	angle += 90.0f * number;
	movCam += 1.0f * number;
	// important: n * ((float)(Now - LastTime) / CLOCKS_PER_SEC
	LastTime = Now;

	/*const float radius = 20.0f;
	float camX = sin(movCam) * radius;
	float camZ = cos(movCam) * radius;
	view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0),glm::vec3(1.0, 1.0, 1.0));*/
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	shaders.use();
	//float sinangle = abs(sin(angle / 64.0f)) * 1.5f;
	//model = glm::rotate(model, glm::radians(15.0f)/CLOCKS_PER_SEC, glm::vec3(-1.0, 1.0, 0.5));
	//model = glm::scale(model, glm::vec3(sinangle,sinangle,sinangle));

	int lengthA = sizeof(posArr) / sizeof(glm::vec3);
	glBindVertexArray(BufferIds[0]);
	for (unsigned int i = 0; i < lengthA; i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, posArr[i]);
		//model = glm::rotate(model, glm::radians(i * 20.0f + angle), rotArr[i]);
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