#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>


// Prot?tipo da fun??o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot?tipos das fun??es
GLuint setupShader();

int setupGeometryJanela0();
int setupGeometryJanela1();

// Dimens?es da janela (pode ser alterado em tempo de execu??o)
const GLuint WIDTH = 800, HEIGHT = 600;

// C?digo fonte do Vertex Shader (em GLSL): ainda hardcoded
const GLchar* vertexShaderSource = "#version 430\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

//C?difo fonte do Fragment Shader (em GLSL): ainda hardcoded
const GLchar* fragmentShaderSource = "#version 430\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";

// Fun??o MAIN
int main()
{
	// Inicializa??o da GLFW
	glfwInit();

	//Necess?rio em alguns casos dependendo da configura??o do hardware
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Cria??o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! - Ruana Soares", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun??o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun??es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}


	// Definindo as dimens?es da viewport com as mesmas dimens?es da janela da aplica??o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	GLuint VAO_Janela0 = setupGeometryJanela0();
	GLuint VAO_Janela1 = setupGeometryJanela1();


	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	assert(colorLoc > -1);
	glUseProgram(shaderID);

	// Loop da aplica??o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun??es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);


		glUniform4f(colorLoc, 0.0f, 1.0f, 1.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Janela0);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Janela0);
		glDrawArrays(GL_LINE_LOOP, 0, 4);

		glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
		glUseProgram(shaderID);
		glBindVertexArray(VAO_Janela1);
		glDrawArrays(GL_LINE_STRIP, 0, 5);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	
	glDeleteVertexArrays(1, &VAO_Janela0);
	glDeleteVertexArrays(1, &VAO_Janela1);
	// Finaliza a execu??o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


GLuint setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compila??o (exibi??o via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compila??o (exibi??o via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}







int setupGeometryJanela0()
{

	GLfloat vertices[] = {
		-0.4f, -0.4f, 0.0f,   
		-0.4f,  -0.0f, 0.0f, 
		 0.0f,  -0.0f, 0.0f , 
		 0.0f,  -0.4f, 0.0f,
	};

	GLuint VBO, VAO_Janela0;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO_Janela0);

	glBindVertexArray(VAO_Janela0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);


	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	return VAO_Janela0;
}

int setupGeometryJanela1()
{

	GLfloat vertices[] = {
		-0.4f, -0.2f, 0.0f,    
		 0.0f,  -0.2f, 0.0f, 
		 0.0f,  0.0f, 0.0f , 
		-0.2f,  0.0f, 0.0f,
		-0.2f,  -0.4f, 0.0f
	};

	GLuint VBO, VAO_Janela1;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	glGenVertexArrays(1, &VAO_Janela1);

	glBindVertexArray(VAO_Janela1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	return VAO_Janela1;
}