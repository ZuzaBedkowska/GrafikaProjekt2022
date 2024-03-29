#include <iostream>
#include <cmath>
#include <math.h>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

using namespace std;

constexpr auto PI = 3.14159265358979323846;

void makeCircle(int n, int p, GLfloat vertices[], GLuint indices[], double x, double y, double red, double green, double blue, double r) //ile trojatow na kolo, wierzcholki, polaczenia, wsp srodka i promien
{
	double angle = 2 * PI / n; //kat miedzy ramionami trojkata
	double angle1 = 0.0;
	for (int i = p * 3 * 5 * n + 10 * 2 * 5 * n; i < (p + 1) * 3 * 5 * n + 10 * 2 * 5 * n; ++i) //punkty orbit ktore juz wypelniaja tablica = 9 * 2 * 5 * n
	{
		vertices[i] = x; //pierwszy wierzcholek kazdego trojkata to srodek kola
		i++;
		vertices[i] = y;
		i++;
		vertices[i] = red;
		i++;
		vertices[i] = green;
		i++;
		vertices[i] = blue;
		i++;
		vertices[i] = x + r * cos(angle1); //drugi wierzcholek z f trygonometrycznych
		i++;
		vertices[i] = y + r * sin(angle1);
		i++;
		vertices[i] = red;
		i++;
		vertices[i] = green;
		i++;
		vertices[i] = blue;
		i++;
		angle1 += angle;
		vertices[i] = x + r * cos(angle1); //trzeci wierzcholek tak samo jak 2, ale przesuniety o kat
		i++;
		vertices[i] = y + r * sin(angle1);
		i++;
		vertices[i] = red;
		i++;
		vertices[i] = green;
		i++;
		vertices[i] = blue;
	}
	for (int i = p * 3 * n + 10 * 2 * n; i < (p + 1) * 3 * n + 10 * 2 * n; ++i)
	{
		indices[i] = i; //pierwszy wierzcholek kazdego trojkata to srodek kola
	}
}

void makeOrbit(int n, int p, GLfloat vertices[], GLuint indices[], double x, double y, double red, double green, double blue, double r)
{
	double angle = 2 * PI / n; //kat miedzy ramionami trojkata
	double angle1 = 0.0;
	for (int i = p * 2 * 5 * n; i < (p + 1) * 2 * 5 * n; ++i)
	{
		vertices[i] = x + r * cos(angle1); //pierwszy wierzcholek z f trygonometrycznych
		i++;
		vertices[i] = y + r * sin(angle1);
		i++;
		vertices[i] = red;
		i++;
		vertices[i] = green;
		i++;
		vertices[i] = blue;
		i++;
		angle1 += angle;
		vertices[i] = x + r * cos(angle1); //2 wierzcholek tak samo jak 1, ale przesuniety o kat
		i++;
		vertices[i] = y + r * sin(angle1);
		i++;
		vertices[i] = red;
		i++;
		vertices[i] = green;
		i++;
		vertices[i] = blue;
	}
	for (int i = p * 2 * n; i < (p + 1) * 2 * n; ++i)
	{
		indices[i] = i; //pierwszy wierzcholek kazdego trojkata to srodek kola
	}
}

void calculateCirclePosition(double & x, double & y, double x_s, double y_s, double r, double angle) //nowe wsp srodka okregu, wsp srodka orbity, odleglosc od 0.0, kat odchylenia od 0.0
{
	x = x_s + r * cos(angle);
	y = y_s + r * sin(angle);
}

int main()
{
	double n = 100; //ile trojatow tworzy kolo
	int p = 2 * 10 * n;
	vector <double> distances(10, 0.0);
	for (int i = 0; i < 9; ++i)
	{
		distances[i] = i * 0.1;
	}
	vector <double> x = distances;
	vector <double> y(10, 0.0);
	vector <double> angle1(10, 0.0);
	vector <double> angle2(10, 0.0);
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);

	//trojkat
	const char* vertexShaderSource = "#version 330 core\n"
		"in vec2 position;\n"
		"in vec3 color;\n"
		"out vec3 Color;\n"
		"void main()\n"
		"{\n"
		" Color = color;\n"
		" gl_Position = vec4(position, 0.0, 1.0);\n"
		"}\0";
	//Fragment Shader source code
	const char* fragmentShaderSource = "#version 330 core\n"
		"in vec3 Color;\n"
		"out vec4 outColor;\n"
		"void main()\n"
		"{\n"
		" outColor = vec4(Color, 1.0);\n"
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
	//koniec trojkata

	//petla wyswietlajaca
	angle1[3] = 2 * PI / 1000; //obrot ziemi
	angle1[1] = angle1[3] / 0.24; //proporcjonalne przeszktalcenie czasu obrotu ziemi w programie na czas obrotu merkurego
	angle1[2] = angle1[3] / 0.615;
	angle1[4] = angle1[3] / 1.88;
	angle1[5] = angle1[3] / 11.86;
	angle1[6] = angle1[3] / 29.5;
	angle1[7] = angle1[3] / 84;
	angle1[8] = angle1[3] / 164;
	angle1[9] = angle1[3] * 12;
	double x_s = 0.0, y_s = 0.0; //wspolrzedne srodka ukladu slonecznego
	while (!glfwWindowShouldClose(window))
	{
		
		GLfloat vertices[10 * 2 * 5 * 100+ 10 * 3 * 5 * 100]{}; //100 trojkatow po 3 punkty po 3 wsp
		// Indices for vertices order
		GLuint indices[10 * 3 * 100 + 10 * 2 * 100]{};
		makeOrbit(n, 0, vertices, indices, x_s, y_s, 1, 1, 1, distances[1]); //m orbit
		makeOrbit(n, 1, vertices, indices, x_s, y_s, 1, 1, 1, distances[2]); //v orbit
		makeOrbit(n, 2, vertices, indices, x_s, y_s, 1, 1, 1, distances[3]); //e orbit
		makeOrbit(n, 3, vertices, indices, x_s, y_s, 1, 1, 1, distances[4]); //m orbit
		makeOrbit(n, 4, vertices, indices, x_s, y_s, 1, 1, 1, distances[5]); //j orbit
		makeOrbit(n, 5, vertices, indices, x_s, y_s, 1, 1, 1, distances[6]); //s orbit
		makeOrbit(n, 6, vertices, indices, x_s, y_s, 1, 1, 1, distances[7]); //u orbit
		makeOrbit(n, 7, vertices, indices, x_s, y_s, 1, 1, 1, distances[8]); //n orbit
		calculateCirclePosition(x[6], y[6], x_s, y_s, distances[6], angle2[6]);
		angle2[6] += angle1[6];
		makeOrbit(n, 8, vertices, indices, x[6], y[6], 1, 1, 1, 0.045);  //s ring
		calculateCirclePosition(x[3], y[3], x_s, y_s, distances[3], angle2[3]);
		angle2[3] += angle1[3];
		makeOrbit(n, 9, vertices, indices, x[3], y[3], 1, 1, 1, 0.05);  //moon orbit


		makeCircle(n, 0, vertices, indices, x[0], y[0], 1.0, 0.98, 0.2219, 0.08); //s, argumenty od y[0] do 0.08 to kolorki w formacie red, green, blue
		calculateCirclePosition(x[1], y[1], x_s, y_s, distances[1], angle2[1]);
		angle2[1] += angle1[1];
		makeCircle(n, 1, vertices, indices, x[1], y[1], 0.7412, 0.4314, 0.1176, 0.01); //m
		calculateCirclePosition(x[2], y[2], x_s, y_s, distances[2], angle2[2]);
		angle2[2] += angle1[2];
		makeCircle(n, 2, vertices, indices, x[2], y[2], 0.6235, 0.5412, 0.4196, 0.025); //v
		makeCircle(n, 3, vertices, indices, x[3], y[3], 0.1451, 0.5725, 0.7725, 0.028); //e
		calculateCirclePosition(x[4], y[4], x_s, y_s, distances[4], angle2[4]);
		angle2[4] += angle1[4];
		makeCircle(n, 4, vertices, indices, x[4], y[4], 0.7569, 0.2392, 0.0588, 0.015); //m
		calculateCirclePosition(x[5], y[5], x_s, y_s, distances[5], angle2[5]);
		angle2[5] += angle1[5];
		makeCircle(n, 5, vertices, indices, x[5], y[5], 0.4941, 0.2863, 0.1176, 0.05); //j
		makeCircle(n, 6, vertices, indices,  x[6], y[6], 0.8509, 0.7333, 0.5608, 0.035); //s
		calculateCirclePosition(x[7], y[7], x_s, y_s, distances[7], angle2[7]);
		angle2[7] += angle1[7];
		makeCircle(n, 7, vertices, indices, x[7], y[7], 0.0353, 0.3216, 0.8706, 0.025); //u
		calculateCirclePosition(x[8], y[8], x_s, y_s, distances[8], angle2[8]);
		angle2[8] += angle1[8];
		makeCircle(n, 8, vertices, indices, x[8], y[8], 0.2706, 0.8941, 0.9922, 0.025); //n
		calculateCirclePosition(x[9], y[9], x[3], y[3], 0.05, angle2[9]);
		angle2[9] += angle1[9];
		makeCircle(n, 9, vertices, indices, x[9], y[9], 0.78, 0.78, 0.78, 0.01); //moon
		GLuint VAO, VBO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
		GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
			(void*)(2 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, 2 * 10 * n, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, (3 * 10 * n), GL_UNSIGNED_INT, (void*) (p * sizeof(GL_UNSIGNED_INT)));
		glfwSwapBuffers(window);
		glfwPollEvents();
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);
		glDeleteProgram(shaderProgram);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteProgram(shaderProgram);
		Sleep(1);
	}

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
