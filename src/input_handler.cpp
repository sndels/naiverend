#include "input_handler.hpp"

using glm::vec2;

MouseState InputHandler::mouseState_ = MouseState({vec2(0.f, 0.f), vec2(0.f, 0.f), HOVERING, 0.f, 0.f});
KeyboardState InputHandler::keyboardState_ = KeyboardState({false,false,false,false,false});

InputHandler& InputHandler::getIH()
{
    static InputHandler ih;
    return ih;
}

void InputHandler::cursorPosCallback(GLFWwindow* w, double xpos, double ypos)
{
    double x, y;
    glfwGetCursorPos(w, &x, &y);
    InputHandler::mouseState_.curPos2f = vec2(static_cast<float>(x), static_cast<float>(y));
}

void InputHandler::mouseButtonCallback(GLFWwindow* w, int32_t button, int32_t action, int32_t mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            InputHandler::mouseState_.state = LEFT_DOWN;
        } else {
            InputHandler::mouseState_.state = HOVERING;
        }
    }
}

void InputHandler::scrollCallback(GLFWwindow* w, double xoffset, double yoffset) {
    mouseState_.scrollX = static_cast<float>(xoffset);
    mouseState_.scrollY = static_cast<float>(yoffset);
}

void InputHandler::keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
    else if (key == GLFW_KEY_UP) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) keyboardState_.up = true;
        else keyboardState_.up = false;
    }
    else if (key == GLFW_KEY_DOWN) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) keyboardState_.down = true;
        else keyboardState_.down = false;
    }
    else if (key == GLFW_KEY_LEFT) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) keyboardState_.left = true;
        else keyboardState_.left = false;
    }
    else if (key == GLFW_KEY_RIGHT) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) keyboardState_.right = true;
        else keyboardState_.right = false;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) {
        if (action == GLFW_PRESS || action == GLFW_REPEAT) keyboardState_.shift = true;
        else keyboardState_.shift = false;
    }
}

const MouseState& InputHandler::getMouseState() const
{
    return mouseState_;
}

const KeyboardState& InputHandler::getKeyboardState() const
{
    return keyboardState_;
}

void InputHandler::reset() {
    mouseState_.lastPos2f = mouseState_.curPos2f;
    mouseState_.scrollX = 0.f;
    mouseState_.scrollY = 0.f;
}
