#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

using namespace std;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 800, "Openglwindow", NULL, NULL); //wskaznik do okienka
	// Error check if the window fails to create
	if (window == NULL) //jesli nie uda sie zrobic okienka
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate(); //zakoncz z bledem
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);
	//Load GLAD so it configures OpenGL
	gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	glViewport(0, 0, 800, 800);
	// Specify the color of the background
	glClearColor(0.0, 0.0, 0.0, 1.0f); //kolor w rgba
	// Clean the back buffer and assign the new color to it
	glClear(GL_COLOR_BUFFER_BIT);
	// Swap the back buffer with the front buffer
	glfwSwapBuffers(window);
	while (!glfwWindowShouldClose(window))
	{
		// Take care of all GLFW events
		glfwPollEvents();
	}
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();

}