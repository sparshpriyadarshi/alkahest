#pragma once

#include "../macros.h"
#include "_base.h"

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

    class NOT_EXPORTED InputSystem : public ISubsystem
    {
    public:
        InputSystem() {};
        virtual ~InputSystem() {};
    public:
        virtual void init() override;
        virtual void cleanup() override;
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
        void setWindowPointer(void *window);
    private:
        std::unordered_map<Key, KeyState> m_keys;
        std::unordered_map<MouseButton, ButtonState> m_mouseButtons;
        double m_mouseX, m_mouseY, m_scrollX, m_scrollY;
    };
}
