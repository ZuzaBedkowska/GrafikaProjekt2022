#include<iostream>
#include <cmath>
#include <math.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;

constexpr auto PI = 3.14159265358979323846;

void makeCircle(int n, GLfloat vertices[], GLuint indices[], double x, double y, double r) //ile trojatow na kolo, wierzcholki, polaczenia, wsp srodka i promien
{
	double angle = 2 * PI / n; //kat miedzy ramionami trojkata
	double angle1 = 0.0;
	for (int i = 0; i < 3 * 3 * n; ++i)
	{
		vertices[i] = x; //pierwszy wierzcholek kazdego trojkata to srodek kola
		i++;
		vertices[i] = y;
		i++;
		vertices[i] = 0.0;
		i++;
		vertices[i] = r * cos(angle1); //drugi wierzcholek z f trygonometrycznych
		i++;
		vertices[i] = r * sin(angle1);
		i++;
		vertices[i] = 0.0;
		i++;
		angle1 += angle;
		vertices[i] = r * cos(angle1); //trzeci wierzcholek tak samo jak 2, ale przesuniety o kat
		i++;
		vertices[i] = r * sin(angle1);
		i++;
		vertices[i] = 0.0;
	}
	for (int i = 0; i < 3 * n; ++i)
	{
		indices[i] = i; //pierwszy wierzcholek kazdego trojkata to srodek kola
	}
}

int main()
{
	double n = 100; //ile trojatow tworzy kolo
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Openglwindow", NULL, NULL); //wskaznik do okienka
	if (window == NULL) //jesli nie uda sie zrobic okienka
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate(); //zakoncz z bledem
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, 800, 800);
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	//trojkat
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		" FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
		"}\n\0";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	GLfloat vertices[3 * 3 * 100]{}; //100 trojkatow po 3 punkty po 3 wsp
	// Indices for vertices order
	GLuint indices[3 * 100]{};
	makeCircle(n, vertices, indices, 0.0, 0.0, 1.0);
	GLuint VAO, VBO, EBO; //nowa wersja trojkata
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//koniec trojkata

	//petla wyswietlajaca
	GLfloat red = 0.0;
	GLfloat blue = 255.0;
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 3 * n * n, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}