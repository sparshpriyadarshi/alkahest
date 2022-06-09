#pragma once

#include "../macros.h"

namespace Alkahest
{
    class NOT_EXPORTED WindowSystem
    {
    public:
        WindowSystem() {};
        virtual ~WindowSystem() {};
    public:
        struct WindowProps
        {
            std::string title;
            unsigned int width;
            unsigned int height;

            WindowProps(std::string t = "Alkahest Game", unsigned int w = 1280,\
                unsigned int h = 768): title(t), width(w), height(h) {};
        };
        void init(const WindowProps& props = WindowProps());
        void cleanup();
        void update();
        inline unsigned int getWidth() const { return m_width; };
        inline unsigned int getHeight() const { return m_height; };
        void setVSync(bool vsync);
        bool isVSync() const;
        inline void *getWindowPointer() const { return m_window; };
        void setWindowCloseCallback(std::function<void()> callback);
    private:
        struct WindowData
        {
            std::string title;
            unsigned int width;
            unsigned int height;
            bool vsync;
            std::function<void()> closeCallback;
        };
        WindowData m_data;
        void *m_window;
        unsigned int m_width, m_height;
    };
}
