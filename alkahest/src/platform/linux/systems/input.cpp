#include <alkahest/systems/input.h>
#include "keys.h"

#include <GLFW/glfw3.h>

namespace Alkahest
{
    static void *s_window = nullptr;
    static bool s_init = false;

    namespace {
        bool ready()
        {
            return s_window != nullptr && s_init;
        }
    }

    void InputSystem::setWindowPointer(void *window)
    {
        s_window = window;
    }

    void InputSystem::init()
    {
        m_mouseX = 0.0;
        m_mouseY = 0.0;
        m_scrollX = 0.0;
        m_scrollY = 0.0;

        s_init = true;
    }

    void InputSystem::cleanup()
    {
        s_init = false;
    }

    bool InputSystem::isKeyDown(Key keycode)
    {
        return getKeyState(keycode) == KeyState::KeyDown;
    }

    bool InputSystem::isKeyUp(Key keycode)
    {
        return getKeyState(keycode) == KeyState::KeyUp;
    }

    bool InputSystem::isKeyHeld(Key keycode)
    {
        KeyState state = getKeyState(keycode);
        return state == KeyState::KeyHeld || state == KeyState::KeyDown;
    }

    KeyState InputSystem::getKeyState(Key keycode)
    {
        std::unordered_map<Key, KeyState>::iterator state = m_keys.find(keycode);
        if (state == m_keys.end())
        {
            return KeyState::KeyUp;
        }
        return state->second;
    }

    bool InputSystem::isMouseButtonDown(MouseButton button)
    {
        return getMouseButtonState(button) == ButtonState::ButtonDown;
    }

    bool InputSystem::isMouseButtonUp(MouseButton button)
    {
        return getMouseButtonState(button) == ButtonState::ButtonUp;
    }

    bool InputSystem::isMouseButtonHeld(MouseButton button)
    {
        ButtonState state = getMouseButtonState(button);
        return state == ButtonState::ButtonHeld || state == ButtonState::ButtonDown;
    }

    ButtonState InputSystem::getMouseButtonState(MouseButton button)
    {
        std::unordered_map<MouseButton, ButtonState>::iterator state = m_mouseButtons.find(button);
        if (state == m_mouseButtons.end())
        {
            return ButtonState::ButtonUp;
        }
        return state->second;
    }

    glm::vec2 InputSystem::getMousePos()
    {
        return { m_mouseX, m_mouseY };
    }

    glm::vec2 InputSystem::getMouseScroll()
    {
        return { m_scrollX, m_scrollY };
    }

    float InputSystem::getAxis(Axis axis)
    {
        //TODO: axis polling
        return 0.0f;
    }

    void InputSystem::setKeyState(Key keycode, KeyState state)
    {
        logTrace("Setting key [{}] to state [{}]", keycode, state);
        m_keys.insert_or_assign(keycode, state);
    }

    void InputSystem::setMouseButtonState(MouseButton button, ButtonState state)
    {
        logTrace("Setting mouse button [{}] to state [{}]", button, state);
        m_mouseButtons.insert_or_assign(button, state);
    }

    void InputSystem::setMousePos(double x, double y, bool propagate)
    {
        logTrace("Setting mouse position to ({}, {})", x, y);
        m_mouseX = x;
        m_mouseY = y;

        if (propagate)
        {
            glfwSetCursorPos(reinterpret_cast<GLFWwindow*>(s_window), x, y);
        }
    }

    void InputSystem::setMouseScroll(double x, double y)
    {

        logTrace("Setting mouse scroll to ({}, {})", x, y);
        m_scrollX = x;
        m_scrollY = y;
    }
}
