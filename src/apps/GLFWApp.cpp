#include "GLFWApp.h"
#include "..\common\MemoryPool.h"

int GLFWApp::WIDTH = 800;
int GLFWApp::HEIGHT = 600;



GLFWApp::GLFWApp()
{
}

void GLFWApp::init()
{
	initWindow();
}

void GLFWApp::initWindow()
{
	glfwInit();
	//glfwSetTime(45);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	m_window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this);
	glfwSetFramebufferSizeCallback(m_window, GLFWApp::framebufferResizeCallback);

}

void GLFWApp::framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = static_cast<GLFWApp*>(glfwGetWindowUserPointer(window));
	app->b_framebufferResized = true;
	app->WIDTH = width;
	app->HEIGHT = height;
}

void GLFWApp::mainLoop()
{
	while (!glfwWindowShouldClose(m_window)) {
		currentElapsedTime = glfwGetTime();
		auto deltaTime = currentElapsedTime - lastCurrentElapsedTime;
		lastCurrentElapsedTime = currentElapsedTime;

		glfwPollEvents();
		update(static_cast<float>(deltaTime));
		draw();
	}
}

GLFWApp::~GLFWApp()
{
	dodf::MemoryPool::Destroy();
	glfwDestroyWindow(m_window);
	glfwTerminate();
}
