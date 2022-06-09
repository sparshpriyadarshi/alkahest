#include <alkahest/systems/input.h>
#include <alkahest/platform/linux/keys.h>

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

    InputSystem& InputSystem::getInstance()
    {
        static InputSystem i;
        return i;
    }

    void InputSystem::init(void *window)
    {
        m_mouseX = 0.0;
        m_mouseY = 0.0;
        m_scrollX = 0.0;
        m_scrollY = 0.0;

        s_init = true;

        s_window = window;

        glfwSetKeyCallback(static_cast<GLFWwindow*>(s_window), \
            [](GLFWwindow *window, int key, int scancode, int action, int mods)
            {
                Input::setKeyState(static_cast<Key>(key), \
                    static_cast<KeyState>(action));
            });

        glfwSetMouseButtonCallback(static_cast<GLFWwindow*>(s_window), \
            [](GLFWwindow* window, int button, int action, int mods)
            {
                Input::setMouseButtonState(static_cast<MouseButton>(button), \
                    static_cast<ButtonState>(action));
            });

        glfwSetCursorPosCallback(static_cast<GLFWwindow*>(s_window), \
            [](GLFWwindow* window, double x, double y)
            {
                Input::setMousePos(x, y);
            });

        glfwSetScrollCallback(static_cast<GLFWwindow*>(s_window), \
            [](GLFWwindow* window, double x, double y)
            {
                Input::setMouseScroll(x, y);
            });
    }

    void InputSystem::cleanup()
    {
        s_init = false;
    }

    bool InputSystem::isKeyDown(Key keycode)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        return getKeyState(keycode) == KeyState::KeyDown;
    }

    bool InputSystem::isKeyUp(Key keycode)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        return getKeyState(keycode) == KeyState::KeyUp;
    }

    bool InputSystem::isKeyHeld(Key keycode)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        KeyState state = getKeyState(keycode);
        return state == KeyState::KeyHeld || state == KeyState::KeyDown;
    }

    KeyState InputSystem::getKeyState(Key keycode)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        std::unordered_map<Key, KeyState>::iterator state = m_keys.find(keycode);
        if (state == m_keys.end())
        {
            return KeyState::KeyUp;
        }
        return state->second;
    }

    bool InputSystem::isMouseButtonDown(MouseButton button)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        return getMouseButtonState(button) == ButtonState::ButtonDown;
    }

    bool InputSystem::isMouseButtonUp(MouseButton button)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        return getMouseButtonState(button) == ButtonState::ButtonUp;
    }

    bool InputSystem::isMouseButtonHeld(MouseButton button)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        ButtonState state = getMouseButtonState(button);
        return state == ButtonState::ButtonHeld || state == ButtonState::ButtonDown;
    }

    ButtonState InputSystem::getMouseButtonState(MouseButton button)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        std::unordered_map<MouseButton, ButtonState>::iterator state = m_mouseButtons.find(button);
        if (state == m_mouseButtons.end())
        {
            return ButtonState::ButtonUp;
        }
        return state->second;
    }

    glm::vec2 InputSystem::getMousePos()
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        return { m_mouseX, m_mouseY };
    }

    glm::vec2 InputSystem::getMouseScroll()
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        return { m_scrollX, m_scrollY };
    }

    float InputSystem::getAxis(Axis axis)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        //TODO: axis polling
        return 0.0f;
    }

    void InputSystem::setKeyState(Key keycode, KeyState state)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        logTrace("Setting key [{}] to state [{}]", keycode, state);
        m_keys.insert_or_assign(keycode, state);
    }

    void InputSystem::setMouseButtonState(MouseButton button, ButtonState state)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        logTrace("Setting mouse button [{}] to state [{}]", button, state);
        m_mouseButtons.insert_or_assign(button, state);
    }

    void InputSystem::setMousePos(double x, double y, bool propagate)
    {
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
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
        AKST_ENG_ASSERT(ready(), "InputSystem accessed before it was ready!");
        logTrace("Setting mouse scroll to ({}, {})", x, y);
        m_scrollX = x;
        m_scrollY = y;
    }

    bool Input::isKeyDown(Key keycode)
    {
        return InputSystem::getInstance().isKeyDown(keycode);
    }

    bool Input::isKeyUp(Key keycode)
    {
        return InputSystem::getInstance().isKeyUp(keycode);
    }

    bool Input::isKeyHeld(Key keycode)
    {
        return InputSystem::getInstance().isKeyHeld(keycode);
    }

    bool Input::isMouseButtonDown(MouseButton button)
    {
        return InputSystem::getInstance().isMouseButtonDown(button);
    }

    bool Input::isMouseButtonUp(MouseButton button)
    {
        return InputSystem::getInstance().isMouseButtonUp(button);
    }
    
    bool Input::isMouseButtonHeld(MouseButton button)
    {
        return InputSystem::getInstance().isMouseButtonHeld(button);
    }

    glm::vec2 Input::getMousePos()
    {
        return InputSystem::getInstance().getMousePos();
    }

    glm::vec2 Input::getMouseScroll()
    {
        return InputSystem::getInstance().getMouseScroll();
    }

    float Input::getAxis(Axis axis)
    {
        return InputSystem::getInstance().getAxis(axis);
    }

    void Input::setKeyState(Key keycode, KeyState state)
    {
        InputSystem::getInstance().setKeyState(keycode, state);
    }

    void Input::setMouseButtonState(MouseButton button, ButtonState state)
    {
        InputSystem::getInstance().setMouseButtonState(button, state);
    }

    void Input::setMousePos(double x, double y, bool propagate)
    {
        InputSystem::getInstance().setMousePos(x, y, propagate);
    }

    void Input::setMouseScroll(double x, double y)
    {
        InputSystem::getInstance().setMouseScroll(x, y);
    }
}
