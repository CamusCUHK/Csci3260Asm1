/*
Type your name and student ID here
	- Name:	Ng Chi Hon
	- Student ID:	1155116317
*/

#include "Dependencies/glew/glew.h"
#include "Dependencies/GLFW/glfw3.h"

#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <fstream>

using glm::vec3;
using glm::mat4;
GLint programID;

GLuint vaoGrd;
GLuint vboGrd;
GLuint vboGrdCol;

GLuint vaoCube;
GLuint vboCube;
GLuint vboCubeCol;

GLuint vaoPyd;
GLuint vboPyd;
GLuint vboPydCol;

GLuint vaoStar;
GLuint vboStar;
GLuint vboStarCol;

float x_delta = 0.1f;
float y_delta = 0.1f;
int x_press_num = 0;
int y_press_num = 0;
float rotate_cube = 0.0f;
float rotate_pyramid = 0.0f;
int scale = 1;
int scale1 = 1;

void get_OpenGL_info() {
	// OpenGL information
	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* glversion = glGetString(GL_VERSION);
	std::cout << "OpenGL company: " << name << std::endl;
	std::cout << "Renderer name: " << renderer << std::endl;
	std::cout << "OpenGL version: " << glversion << std::endl;
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		std::cout << buffer << std::endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID) {
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID) {
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

std::string readShaderCode(const char* fileName) {
	std::ifstream meInput(fileName);
	if (!meInput.good()) {
		std::cout << "File failed to load ... " << fileName << std::endl;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders() {
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);		//keep tracking the vertexShader 
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];		//set a source of shader
	//adapter[0] = vertexShaderCode;
	std::string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void sendDataToOpenGL() {
	// TODO:

	/*
		1.0 0.0 0.0 red, duh
		0.0 1.0 0.0 green, duh
		0.0 0.0 1.0 blue, duh
		1.0 1.0 0.0 yellow
		1.0 0.0 1.0 purple
		0.0 1.0 1.0 cyan
		1.0 1.0 1.0 white
		1.0 0.5 0.0 orange
		0.5 1.0 0.0 greenish yellow
		0.5 1.0 0.5 light green
		0.0 0.5 0.0 dark green;
	*/
	const GLfloat ground[] =
	{
		-5.0f, -5.0f, -1.0f,
		5.0f, -5.0f, -1.0f,
		5.0f, 5.0f, -1.0f,
		-5.0f, 5.0f, -1.0f,
	};
	const GLfloat ground_col[] =
	{
		+0.3f, +0.3f, +0.3f,
		+0.3f, +0.3f, +0.3f,
		+0.3f, +0.3f, +0.3f,
		+0.3f, +0.3f, +0.3f,
	};

	GLushort ground_ind[] = { 0,1,3,1,2,3 };

	const GLfloat Cube[] =
	{

		-1.0f,-1.0f,-1.0f, //0 1 3 0 2 3
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,

		//frontside
		1.0f,-1.0f, 1.0f, //4 5 7 4 6 7/
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,

		1.0f, 1.0f, 1.0f, // 8 9 11 8 10 11
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		1.0f, 1.0f,-1.0f, //12 13 15 12 14 15
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,

		-1.0f, 1.0f, 1.0f, //16 17 19 16 18 19
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, //20 21 23 20 22 23
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
	};
	const GLfloat Cube_col[] =
	{
		//Left
		1.0f, 0.0f, 0.0f, 
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		//Front
		0.0f, 1.0f, 1.0f, 
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		//Right
		0.19f, 0.64f, 0.84f, 
		0.19f, 0.64f, 0.84f,
		0.19f, 0.64f, 0.84f,
		0.19f, 0.64f, 0.84f,
		0.19f, 0.64f, 0.84f,
		0.19f, 0.64f, 0.84f,

		//Base
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,

		//Top
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,

		//Back
		0.0f, 0.5f, 0.0f, 
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	/*GLushort Cube_ind[] =
	{
		0, 1, 3,
		1, 2, 3,

		4, 5, 7,
		5, 6, 7,

		8, 9, 11,
		9, 10, 11,

		12, 13, 15,
		13, 14, 15,

		16, 17, 19,
		17, 18, 19,

		20, 21, 23,
		21, 22, 23,
	};*/
	/*
	triangle base1 = { PYR_LEFT_FRONT,   PYR_RIGHT_FRONT,  PYR_LEFT_BACK };
	triangle base2 = { PYR_RIGHT_BACK,   PYR_RIGHT_FRONT,  PYR_LEFT_BACK };
	triangle leftside = { PYR_LEFT_FRONT,  PYR_LEFT_BACK,   PYR_TOP };
	triangle frontside = { PYR_LEFT_FRONT,  PYR_RIGHT_FRONT, PYR_TOP };
	triangle rightside = { PYR_RIGHT_BACK ,PYR_RIGHT_FRONT,  PYR_TOP };
	triangle backside = { PYR_RIGHT_BACK,  PYR_LEFT_BACK,   PYR_TOP };
	*/

	const GLfloat pyramid[] =
	{

		-3.0f, -1.5f, 0.0f,	//leftFront
		0.0f, -1.5f, 0.0f,	//RightFront
		-3.0f, 1.5f, 0.0f,	//leftBack

		0.0f, 1.5f, 0.0f,	//RightBack
		0.0f, -1.5f, 0.0f,	//RightFront
		-3.0f, 1.5f, 0.0f,	//leftBack

		-3.0f, -1.5f, 0.0f,	//leftFront
		-3.0f, 1.5f, 0.0f,	//leftBack+
		-1.5f, 0.0f, 2.0f,	//top

		-3.0f, -1.5f, 0.0f,	//leftFront
		0.0f, -1.5f, 0.0f,	//RightFront
		-1.5f, 0.0f, 2.0f,	//top

		0.0f, 1.5f, 0.0f,	//RightBack
		0.0f, -1.5f, 0.0f,	//RightFront
		-1.5f, 0.0f, 2.0f,	//top

		0.0f, 1.5f, 0.0f,	//RightBack
		-3.0f, 1.5f, 0.0f,	//leftBack
		-1.5f, 0.0f, 2.0f,	//top
	};

	const GLfloat pyramidCol[] =
	{
		//base Tri 1
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		//base Tri 2
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,

		//leftside		RED
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		//frontside		Cyan
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,

		//Rightside		Blue
		0.19f, 0.64f, 0.84f,
		0.19f, 0.64f, 0.84f,
		0.19f, 0.64f, 0.84f,

		//Backside		Green
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,

	};


	//	Vao and Vbo for Grd
	glGenVertexArrays(1, &vaoGrd);
	glBindVertexArray(vaoGrd);

	glGenBuffers(1, &vboGrd);
	glBindBuffer(GL_ARRAY_BUFFER, vboGrd);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &vboGrdCol);
	glBindBuffer(GL_ARRAY_BUFFER, vboGrdCol);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_col), ground_col, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	GLuint vboIndexGrd;
	glGenBuffers(1, &vboIndexGrd);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndexGrd);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ground_ind), ground_ind, GL_STATIC_DRAW);

	//	Vao and Vbo for Cube
	glGenVertexArrays(1, &vaoCube);
	glBindVertexArray(vaoCube);

	glGenBuffers(1, &vboCube);
	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube), Cube, GL_STATIC_DRAW);

	glGenBuffers(1, &vboCubeCol);
	glBindBuffer(GL_ARRAY_BUFFER, vboCubeCol);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Cube_col), Cube_col, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vboCube);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vboCubeCol);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	//	Vao and Vbo for Pyd
	glGenVertexArrays(1, &vaoPyd);
	glBindVertexArray(vaoPyd);

	glGenBuffers(1, &vboPyd);
	glBindBuffer(GL_ARRAY_BUFFER, vboPyd);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glGenBuffers(1, &vboPydCol);
	glBindBuffer(GL_ARRAY_BUFFER, vboPydCol);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidCol), pyramidCol, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

}

void generateMVPMatrix(void)
{
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 1.3f, 0.5f, 100.0f);
	glm::mat4 ViewMatrix = glm::lookAt(
		glm::vec3(0, -11, 5),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 0, 1)
	);
	glm::mat4 ModelTransformMatrix = glm::mat4(1.0f);
	glm::mat4 viewMatrix = ProjectionMatrix * ViewMatrix * ModelTransformMatrix;

	GLuint vertexLocation = glGetUniformLocation(programID, "viewMatrix");
	glUniformMatrix4fv(vertexLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}

void paintGL(void) {
	// always run
	// TODO:
	glClearColor(0.5f, 0.5f, 0.5f, 0.5f); //specify the background color
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 modelTransformMatrix;
	glm::mat4 modelTransformMatrix_rotate;
	glm::mat4 modelTransformMatrix_move;
	glm::mat4 modelTransformMatrix_scale;
	glm::mat4 modelTransformMatrix_translate;
	glm::mat4 modelTransformMatrix_translate_1;
	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");


	glBindVertexArray(vaoCube);
	modelTransformMatrix_move = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0.0f));
	modelTransformMatrix_rotate = glm::rotate(glm::mat4(1.0f), rotate_cube, glm::vec3(0, 0, 1));
	modelTransformMatrix_scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
	modelTransformMatrix = modelTransformMatrix_move * modelTransformMatrix_scale * modelTransformMatrix_rotate ;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 12 * 3);

	glBindVertexArray(vaoPyd);
	modelTransformMatrix_translate = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -1));
	modelTransformMatrix_translate_1 = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5, 0, 0));
	modelTransformMatrix_rotate = glm::rotate(glm::mat4(1.0f), rotate_pyramid, glm::vec3(0, 0, 1));
	modelTransformMatrix_move = glm::translate(glm::mat4(1.0f), glm::vec3(x_delta * x_press_num, y_delta * y_press_num, 0.0f));
	modelTransformMatrix = modelTransformMatrix_move * modelTransformMatrix_rotate * modelTransformMatrix_translate_1 *  modelTransformMatrix_translate;
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 6 * 3);

	glBindVertexArray(vaoGrd);
	modelTransformMatrix = glm::mat4(1.0f);
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	


}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	// TODO:
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		x_press_num -= 1;
	}
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		x_press_num += 1;
	}
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		y_press_num += 1;
	}
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		y_press_num -= 1;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		rotate_cube += glm::radians(45.0f);
		rotate_pyramid += glm::radians(45.0f);
		if (scale1 < 5)
		scale1 += 1;
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		rotate_cube -= glm::radians(45.0f);
		rotate_pyramid -= glm::radians(45.0f);
		if (scale1 > 0)
		scale1 -= 1;
	}
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		rotate_cube += glm::radians(45.0f);
		if (scale1 < 5)
		scale1 += 1;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		rotate_cube -= glm::radians(45.0f);
		if (scale1 > 0)
		scale1 -= 1;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		rotate_pyramid += glm::radians(45.0f);
		if (scale1 < 5)
		scale1 += 1;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS)
	{
		rotate_pyramid -= glm::radians(45.0f);
		if (scale1 > 0)
		scale1 -= 1;
	}

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		if (scale < 5)
		scale += 1;
	}
	if (key == GLFW_KEY_Z && scale > 1 && action == GLFW_PRESS)
	{
		if (scale > 0)
		scale -= 1;
	}

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void initializedGL(void) {
	// run only once
	// TODO:
	sendDataToOpenGL();
	installShaders();
	generateMVPMatrix();
}

int main(int argc, char* argv[]) {
	GLFWwindow* window;

	/* Initialize the glfw */
	if (!glfwInit()) {
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}
	/* glfw: configure; necessary for MAC */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 600, "Assignment 1", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);

	/* Initialize the glew */
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	get_OpenGL_info();
	initializedGL();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window)) {
		/* Render here */
		paintGL();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
