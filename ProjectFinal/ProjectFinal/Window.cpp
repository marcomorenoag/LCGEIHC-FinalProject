#include "Window.h"

float Window::x=0.0f;
float Window::y=0.0f;
float Window::z=0.0f;
float Window::x1 = 0.0f;
float Window::y1 = 0.0f;
float Window::z1 = 0.0f;

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	// Asigna variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	
	// Emplea core profile de OpenGL sin retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "LCGEIHC, PROYECTO FINAL", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	
	// Obtiene tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Asigna el contexto a la ventana actual creada
	glfwMakeContextCurrent(mainWindow);

	// Captura del Mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Manejo de eventos de teclado y mouse
	createCallbacks();


	// Permite uso de nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); // Habilita Buffer de profundidad
	glViewport(0, 0, bufferWidth, bufferHeight); // Asigna vista Viewport
	glfwSetWindowUserPointer(mainWindow, this); 
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_D, 0);
	}

	if (key == GLFW_KEY_J &&  (action == GLFW_PRESS || action == GLFW_REPEAT))

	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_J, 0);
		x += 1.0f;
	}

	if (key == GLFW_KEY_L && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_L, 0);
		x -= 1.0f;
	}

	if (key == GLFW_KEY_I && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_I, 0);
		y += 1.0f;
	}

	if (key == GLFW_KEY_K && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_K, 0);
		y -= 1.0f;
	}

	if (key == GLFW_KEY_U && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_U, 0);
		z += 1.0f;
	}

	if (key == GLFW_KEY_O && (action == GLFW_PRESS || action == GLFW_REPEAT))
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_O, 0);
		z -= 1.0f;
	}


	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_G, 0);
		y1 += 1.0f;
	}

	if (key == GLFW_KEY_B && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_B, 0);
		y1 -= 1.0f;
	}

	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_F, 0);
		z1 += 1.0f;
	}

	if (key == GLFW_KEY_H && action == GLFW_PRESS)
	{
		const char* key_name = glfwGetKeyName(GLFW_KEY_H, 0);
		z1-= 1.0f;
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
