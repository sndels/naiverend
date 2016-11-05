#include "gl_core_4_1.h"
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>
#include <sstream>

#include "scene.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::cin;

namespace {
    const static GLsizei XRES = 1280;
    const static GLsizei YRES = 720;
    const static char* WINDOW_TITLE = "skunkwork";
}

static void errorCallback(int error, const char* description)
{
    cerr << description << endl;
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    // Init GLFW-context
    glfwSetErrorCallback(errorCallback);
    if (!glfwInit()) {
        cerr << "Error initializing GLFW!" << endl;
        cin.get();
        exit(EXIT_FAILURE);
    }

#ifdef __APPLE__
    // Set desired context hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    // Create the window
    GLFWwindow* windowPtr;
    windowPtr = glfwCreateWindow(XRES, YRES, WINDOW_TITLE, NULL, NULL);
    if (!windowPtr) {
        glfwTerminate();
        cerr << "Error creating OpenGL window!" << endl;
        cin.get();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(windowPtr);

    // Load gl-functions (glLoadGen-header)
    if (ogl_LoadFunctions() == ogl_LOAD_FAILED) {
        cerr << "Error loading GL functions!" << endl;
        cin.get();
        glfwDestroyWindow(windowPtr);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        cerr << "GLError opening glfw context!" << endl;
        cerr << "Error code: " << error << endl;
        cin.get();
        glfwDestroyWindow(windowPtr);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    // Set input callback
    glfwSetKeyCallback(windowPtr, keyCallback);
	// Set vsync on
	glfwSwapInterval(1);	

    // Init GL settings
    glViewport(0, 0, XRES, YRES);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    error = glGetError();
    if(error != GL_NO_ERROR) {
        cerr << "Error initializing OpenGL!" << endl;
        cerr << "Error code: " << error << endl;
        cin.get();
        glfwDestroyWindow(windowPtr);
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    cout << glGetString(GL_VERSION) << endl;

    Scene scene;
    if (!scene.init()) exit(EXIT_FAILURE);
    // Run the main loop
    while (!glfwWindowShouldClose(windowPtr)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        scene.render();
        glfwSwapBuffers(windowPtr);
    }

    // Release resources
    glfwDestroyWindow(windowPtr);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
