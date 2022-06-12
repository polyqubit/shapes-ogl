#include "Utils.h"
#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define WINDOW_TITLE_PREFIX "OpenGL Window"

int
CurrentWidth = 800,
CurrentHeight = 600,
WindowHandle = 0;

float angle = 0;

GLuint BufferIds[3] = { 0 };

unsigned FrameCount = 0;

clock_t LastTime = 0;

Shader shaders;

void Initialize(int, char* []);
void InitWindow(int, char* []);
void ResizeFunction(int, int);
void RenderFunction(void);
void TimerFunction(int);
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

    glutReshapeFunc(ResizeFunction);
    glutDisplayFunc(RenderFunction);
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

    DrawObj();

    glutSwapBuffers();
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
    const Vertex VERTICES[4] =
    {
      { {  .5f,  .5f, 0, 1 }, { 1, 0.5, 1, 1 } },
      { { -.5f,  .5f, 0, 1 }, { 0.5, 0.5, 1, 1 } },
      { { -.5f, -.5f, 0, 1 }, { 0, 1, 0.5, 1 } },
      { {  .5f, -.5f, 0, 1 }, { 1, 1, 0.25, 1 } }
    };

    // const Vertex CORNERS[4];

    const GLuint INDICES[6] =
    {
      0,1,2,
      2,3,0
    };

    shaders = Shader("VertexShader.glsl","FragmentShader.glsl");

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
    shaders.use();
    angle += 1.0f;
    float sinangle = abs(sin(angle/64.0f))*1.5f;
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(angle), glm::vec3(0.0, 0.0, 1.0));
    trans = glm::scale(trans, glm::vec3(sinangle,sinangle,sinangle));

    glBindVertexArray(BufferIds[0]);
    shaders.setMat4("transform", trans);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void DeleteObj(void) {
    glDeleteBuffers(2, &BufferIds[1]);
    glDeleteVertexArrays(1, &BufferIds[0]);
    ExitOnGLError("ERROR: Could not destroy the buffer objects");
}