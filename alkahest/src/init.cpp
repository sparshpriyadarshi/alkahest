#include <alkahest/init.h>
#include <alkahest/systems/window.h>
#include <alkahest/systems/input.h>

namespace Alkahest
{
    namespace
    {
        static bool s_shouldStop = false;

        // Static engine subsystems
        static WindowSystem s_windowSystem;
        InputSystem& s_inputSystem = InputSystem::getInstance();
    }

    void Game::stop()
    {
        s_shouldStop = true;
    }

    void Game::sysInit()
    {
        // Initialize engine subsystems

        // Window System
        logTrace("Initializing Window System...");
        s_windowSystem.init();
        logTrace("Creating window...");
        s_windowSystem.createWindow();
        logTrace("Setting window close callback...");
        s_windowSystem.setWindowCloseCallback(Game::stop);
        
        // Input System
        logTrace("Initializing Input System...");
        s_inputSystem.init();
        logTrace("Setting window pointer for Input System...");
        s_inputSystem.setWindowPointer(s_windowSystem.getWindowPointer());

        // Call application init()
        init();
    }

    void Game::sysUpdate()
    {
        s_windowSystem.update();
    }

    void Game::sysCleanup()
    {
        // Call application cleanup()
        cleanup();

        // Clean up engine subsystems
        s_windowSystem.cleanup();
    }

    void Game::run()
    {
        while(!s_shouldStop)
        {
            sysUpdate();
            update();
        }
    }

    Game& CreateGame()
    {
        static Game g;
        return g;
    };
}

// Sample platform-specific function
extern std::string platform;

/** The entrypoint for the application */
int main(int argc, char** argv)
{
    Alkahest::logInfo("Platform: {}", platform);
    Alkahest::Game& game = Alkahest::CreateGame();
    game.sysInit();
    game.run();
    game.sysCleanup();
    return 0;
}
