#include "input_handler.hpp"

using glm::vec2;

MouseState InputHandler::mouseState_ = MouseState({vec2(0.f, 0.f), vec2(0.f, 0.f), HOVERING});

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
            InputHandler::mouseState_.clickPos2f = InputHandler::mouseState_.curPos2f;
            InputHandler::mouseState_.state = LEFT_DOWN;
            InputHandler::mouseState_.released = false;
        } else {
            InputHandler::mouseState_.state = HOVERING;
            InputHandler::mouseState_.released = true;

        }
    }
}

void InputHandler::keyCallback(GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GL_TRUE);
}

const MouseState& InputHandler::getMouseState() const
{
    return mouseState_;
}

void InputHandler::resetMouseReleased()
{
    mouseState_.released = false;
}
