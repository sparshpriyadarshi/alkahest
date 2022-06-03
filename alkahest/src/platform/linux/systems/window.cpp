#include <alkahest/systems/window.h>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Alkahest
{
    static bool s_glfwInit = false;

    static void handleGLFWError(int e, const char *msg)
    {
        logError("GLFW Error (Code {}): {}", e, msg);
    }

    void WindowSystem::init()
    {
        if (!s_glfwInit)
        {
            if (!glfwInit())
            {
                logError("GLFW failed to initialize!");
                const char *message;
                glfwGetError(&message);
                logError(message);
                exit(1);
            }

            if (!gl3wInit())
            {
                logError("GL3W failed to initialize1");
                exit(1);
            }

            glfwSetErrorCallback(handleGLFWError);
            s_glfwInit = true;
        }
    }

    void WindowSystem::cleanup()
    {
        glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(m_window));
        glfwTerminate();
    }

    void WindowSystem::update()
    {
        glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(m_window));
        glfwPollEvents();
    }

    bool WindowSystem::createWindow(const WindowProps& props)
    {
        if (!s_glfwInit)
        {
            return false;
        }

        m_data.title = props.title;
        m_data.width = props.width;
        m_data.height = props.height;

        logTrace("Creating window {}: ({},{})", m_data.title, m_data.width, m_data.height);

        glfwWindowHintString(GLFW_X11_CLASS_NAME, "alkahest");
        glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "alkahest");

        m_window = glfwCreateWindow(static_cast<int>(m_data.width), static_cast<int>(m_data.height), m_data.title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(m_window));
        glfwSetWindowUserPointer(reinterpret_cast<GLFWwindow*>(m_window), &m_data);
        setVSync(true);

        int width, height;
        glfwGetFramebufferSize(reinterpret_cast<GLFWwindow*>(m_window), &width, &height);
        m_width = static_cast<unsigned int>(width);
        m_height = static_cast<unsigned int>(height);
        glViewport(0, 0, m_width, m_height);

        glfwSetFramebufferSizeCallback(reinterpret_cast<GLFWwindow*>(m_window), [](GLFWwindow* window, int width, int height){
            glViewport(0, 0, width, height);
        });

        glfwSetWindowSizeCallback(reinterpret_cast<GLFWwindow*>(m_window), [](GLFWwindow* window, int width, int height){
            WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
            data.width = width;
            data.height = height;
        });

        return true;
    }

    void WindowSystem::setVSync(bool vsync)
    {
        if (vsync)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
        m_data.vsync = vsync;
    }

    bool WindowSystem::isVSync() const
    {
        return m_data.vsync;
    }

    void WindowSystem::setWindowCloseCallback(std::function<void()> callback)
    {
        m_data.closeCallback = callback;

        glfwSetWindowCloseCallback(static_cast<GLFWwindow*>(m_window), \
            [](GLFWwindow *window)
            {
                WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
                data.closeCallback();
            });
    }
}
