﻿#include "hellpch.h"
#include "CoreGL.h"
#include "Input.h"
#include <windows.h>

namespace HellEngine
{
	GLFWwindow* CoreGL::s_window;
	GLFWmonitor* CoreGL::s_monitor;
	int CoreGL::windowedWidth;
	int CoreGL::windowedHeight;
	int CoreGL::s_windowWidth;
	int CoreGL::s_windowHeight;
	int CoreGL::windowedPosX;
	int CoreGL::windowedPosY;

	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void processInput(GLFWwindow* window);
//	bool IsCurrentAnimationFinished();
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);


	void CoreGL::InitGL(int windowWidth, int windowHeight)
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

		s_window = glfwCreateWindow(windowWidth, windowHeight, "Window", NULL, NULL);
		if (s_window == NULL) {
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(s_window);
		glfwSetFramebufferSizeCallback(s_window, framebuffer_size_callback);
		glfwSetCursorPosCallback(s_window, mouse_callback);
		glfwSetCursorPos(s_window, windowWidth / 2, windowHeight / 2);
		glfwSetKeyCallback(s_window, key_callback);
		glfwSetMouseButtonCallback(s_window, mouse_button_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			std::cout << "Failed to initialize GLAD" << std::endl;

		s_monitor = glfwGetPrimaryMonitor();
		glfwGetWindowSize(s_window, &windowedWidth, &windowedHeight);
		glfwGetWindowPos(s_window, &windowedPosX, &windowedPosX);
		glfwGetWindowSize(s_window, &s_windowWidth, &s_windowHeight);

		SetVSync(false);
		glEnable(GL_CULL_FACE);

		// Check debug output works
		GLint flags; 
		glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
		if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
		{
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
			glDebugMessageCallback(glDebugOutput, nullptr);
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			std::cout << " Debug output initated successfully\n";
			std::cout << " Debug flag: " << flags << "\n";
		}	

		GLint major, minor;
		glGetIntegerv(GL_MAJOR_VERSION, &major);
		glGetIntegerv(GL_MINOR_VERSION, &minor);

		std::cout << " GL_MAJOR_VERSION: " << major << "\n";
		std::cout << " GL_MINOR_VERSION: " << minor << "\n";

		const GLubyte* vendor = glGetString(GL_VENDOR); // Returns the vendor
		const GLubyte* renderer = glGetString(GL_RENDERER); // Returns a hint to the model
		std::cout << "VENDOR: " << vendor << "\n";
		std::cout << "RENDERER: " << renderer << "\n";
	}

		void CoreGL::ProcessInput()
	{
		processInput(s_window);
	}

	double CoreGL::GetGLTime()
	{
		return glfwGetTime();
	}

	bool CoreGL::IsRunning()
	{
		return !glfwWindowShouldClose(s_window);
	}

	void CoreGL::SwapBuffersAndPollEvents()
	{
		glfwSwapBuffers(s_window);
		glfwPollEvents();
	}

	void CoreGL::Terminate()
	{
		glfwTerminate();
	}

	void CoreGL::OnUpdate()
	{
		// Show/hide cursor
		if (Input::s_showCursor)
			glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(s_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	void CoreGL::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
		
		if (state == GLFW_PRESS)
			Input::s_leftMouseDown = true;
		else if (state == GLFW_RELEASE)
			Input::s_leftMouseDown = false;
	}

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}


	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		Input::MouseUpdate(xpos, ypos);
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
/*		// Reload
		//if (key == GLFW_KEY_R && action == GLFW_PRESS && gunState == GunState::IDLE) {
		//	gunState = GunState::RELOADING;
		//}

		// Edit times
		if (key == GLFW_KEY_P && action == GLFW_PRESS) {
			std::string time = currentModel->m_animations.at(currentAnimation).m_endTime.GetTimeString();
			currentModel->m_animations.at(currentAnimation).m_endTime.SetTime(0, 0, 0, std::stoi(time) + 1);
		}
		if (key == GLFW_KEY_O && action == GLFW_PRESS) {
			std::string time = currentModel->m_animations.at(currentAnimation).m_endTime.GetTimeString();
			currentModel->m_animations.at(currentAnimation).m_endTime.SetTime(0, 0, 0, std::stoi(time) - 1);
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS) {
			std::string time = currentModel->m_animations.at(currentAnimation).m_startTime.GetTimeString();
			currentModel->m_animations.at(currentAnimation).m_startTime.SetTime(0, 0, 0, std::stoi(time) + 1);
		}
		if (key == GLFW_KEY_K && action == GLFW_PRESS) {
			std::string time = currentModel->m_animations.at(currentAnimation).m_startTime.GetTimeString();
			currentModel->m_animations.at(currentAnimation).m_startTime.SetTime(0, 0, 0, std::stoi(time) - 1);
		}*/
	}

	void processInput(GLFWwindow* window)
	{
		// Mouse pressed
		if (Input::s_leftMouseDown && !Input::s_leftMouseDownLastFrame)
			Input::s_leftMousePressed = true;
		else
			Input::s_leftMousePressed = false;
		Input::s_leftMouseDownLastFrame = Input::s_leftMouseDown;

		for (int i = 30; i < 350; i++) {
			// Key down
			if (glfwGetKey(window, i) == GLFW_PRESS)
				Input::s_keyDown[i] = true;
			else
				Input::s_keyDown[i] = false;

			// Key press
			if (Input::s_keyDown[i] && !Input::s_keyDownLastFrame[i])
				Input::s_keyPressed[i] = true;
			else
				Input::s_keyPressed[i] = false;
			Input::s_keyDownLastFrame[i] = Input::s_keyDown[i];
		}

		/*if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		glm::vec3 Front = camera.m_Front;
		Front.y = 0;
		Front = glm::normalize(Front);

		// Stopped

		if ((glfwGetKey(window, GLFW_KEY_W) != GLFW_PRESS) &&
			(glfwGetKey(window, GLFW_KEY_S) != GLFW_PRESS) &&
			(glfwGetKey(window, GLFW_KEY_A) != GLFW_PRESS) &&
			(glfwGetKey(window, GLFW_KEY_D) != GLFW_PRESS))
			playerMovementState = PlayerMovementState::STOPPED;

		// Walking Movement
		float cameraSpeed = 2.5 * deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			playerMovementState = PlayerMovementState::WALKING;
			camera.m_transform.position -= cameraSpeed * Front;
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			playerMovementState = PlayerMovementState::WALKING;
			camera.m_transform.position += cameraSpeed * Front;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			playerMovementState = PlayerMovementState::WALKING;
			camera.m_transform.position += glm::normalize(glm::cross(camera.m_Front, camera.m_Up)) * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			playerMovementState = PlayerMovementState::WALKING;
			camera.m_transform.position -= glm::normalize(glm::cross(camera.m_Front, camera.m_Up)) * cameraSpeed;
		}
		// Running
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS) {
			if (playerMovementState = PlayerMovementState::WALKING)
				playerMovementState = PlayerMovementState::RUNNING;
		}*/
	}

	bool CoreGL::IsFullscreen(void)
	{
		return glfwGetWindowMonitor(s_window) != nullptr;
	}

	void CoreGL::SetFullscreen(bool fullscreen)
	{
		if (IsFullscreen() == fullscreen)
			return;

		if (fullscreen)
		{
			// backup window position and window size
			glfwGetWindowPos(s_window, &windowedPosX, &windowedPosY);
			glfwGetWindowSize(s_window, &windowedWidth, &windowedHeight);

			// get resolution of monitor
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

			// switch to full screen
			glfwSetWindowMonitor(s_window, s_monitor, 0, 0, mode->width, mode->height, 0);
		}
		else
		{
			// restore last window size and position
			glfwSetWindowMonitor(s_window, nullptr, windowedPosX, windowedPosY, windowedWidth, windowedHeight, 0);
		}
		// store size
		glfwGetWindowSize(s_window, &s_windowWidth, &s_windowHeight);
	}

	void CoreGL::CheckGLError_(const char* file, int line)
	{
		HANDLE  hConsole;
		int k;
	
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		// you can loop k higher to see more color choices
		SetConsoleTextAttribute(hConsole, 4);
	

		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}

		SetConsoleTextAttribute(hConsole, 7);
	}


	void APIENTRY glDebugOutput(GLenum source, GLenum type,	GLuint id, GLenum severity,	GLsizei length,	const GLchar* message, const void* userParam)
	{
		if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)	return;
		if (severity == GL_DEBUG_SEVERITY_LOW)	return;

		HANDLE  hConsole;
		int k;
		hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, 4);
	//	std::cout << "GL ERROR\n";

		// ignore non-significant error/warning codes
	//	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

		std::cout << "---------------" << std::endl;
		std::cout << "Debug message (" << id << "): " << message << std::endl;

		switch (source)
		{
		case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
		} std::cout << std::endl;

		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
		} std::cout << std::endl;

		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
		} std::cout << std::endl;
		std::cout << std::endl;

		if (severity == GL_DEBUG_SEVERITY_HIGH)
		{
			std::cout << "SEVERE!!!\n";
			std::cout << "SEVERE!!!\n";
		}
		/*if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
		{
			std::cout << "NOTIFICATION!!!\n";
			std::cout << "NOTIFICATION!!!\n";
		}*/
	}
}	