#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

enum MButtonState
{
    LEFT_DOWN,
    HOVERING
};

struct MouseState
{
    glm::vec2 clickPos2f;
    glm::vec2 curPos2f;
    MButtonState state;
    bool released;
};

class InputHandler
{
public:
    static InputHandler& getIH();
    InputHandler(const InputHandler&) = delete;
    void operator=(const InputHandler&) = delete;

    static void cursorPosCallback(GLFWwindow* w, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* w, int32_t button, int32_t action, int32_t mods);
    static void keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods);

    const MouseState& getMouseState() const;
    void resetMouseReleased();

private:
    InputHandler() {}

    static MouseState mouseState_;
};

#endif
