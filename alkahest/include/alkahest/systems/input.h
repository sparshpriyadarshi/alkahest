#pragma once

#include "../macros.h"

#include <glm/glm.hpp>

namespace Alkahest
{
    // Forward declarations
    enum InputMode : uint32_t;
    enum KeyState : uint8_t;
    enum ButtonState : uint8_t;
    enum Axis : uint8_t;
    enum MouseButton : uint8_t;
    enum Key : int16_t;

    class NOT_EXPORTED InputSystem
    {
    public:
        InputSystem() {};
        virtual ~InputSystem() {};
    public:
        void init(void *window);
        void cleanup();
    public:
        bool isKeyDown(Key keycode);
        bool isKeyUp(Key keycode);
        bool isKeyHeld(Key keycode);
        KeyState getKeyState(Key keycode);

        bool isMouseButtonDown(MouseButton button);
        bool isMouseButtonUp(MouseButton button);
        bool isMouseButtonHeld(MouseButton button);
        ButtonState getMouseButtonState(MouseButton button);

        glm::vec2 getMousePos();
        glm::vec2 getMouseScroll();

        float getAxis(Axis axis);
    public:
        void setKeyState(Key keycode, KeyState state);
        void setMouseButtonState(MouseButton button, ButtonState state);
        void setMousePos(double x, double y, bool propagate = false);
        void setMouseScroll(double x, double y);
    public:
        static InputSystem& getInstance();
    private:
        std::unordered_map<Key, KeyState> m_keys;
        std::unordered_map<MouseButton, ButtonState> m_mouseButtons;
        double m_mouseX, m_mouseY, m_scrollX, m_scrollY;
    };

    class API Input
    {
    public:
        static bool isKeyDown(Key keycode);
        static bool isKeyUp(Key keycode);
        static bool isKeyHeld(Key keycode);

        static bool isMouseButtonDown(MouseButton button);
        static bool isMouseButtonUp(MouseButton button);
        static bool isMouseButtonHeld(MouseButton button);

        static glm::vec2 getMousePos();
        static glm::vec2 getMouseScroll();

        static float getAxis(Axis axis);
    public:
        static void setKeyState(Key keycode, KeyState state);
        static void setMouseButtonState(MouseButton button, ButtonState state);
        static void setMousePos(double x, double y, bool propagate = false);
        static void setMouseScroll(double x, double y);
    private:
        Input() = delete;
        ~Input() = delete;
    };
}
