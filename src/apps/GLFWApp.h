#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

class GLFWApp
{
public:
	virtual void init();
	virtual void update(float dt) = 0;
	virtual void draw() = 0;

	GLFWApp();
	void mainLoop();

	virtual ~GLFWApp();

	static void framebufferResizeCallback(GLFWwindow * window, int width, int height);
protected:
	static int WIDTH;
	static int HEIGHT;

	GLFWwindow* m_window;

	bool b_framebufferResized = false;
	double currentElapsedTime = 0;
	double lastCurrentElapsedTime = 0;

private:
	void initWindow();
};

