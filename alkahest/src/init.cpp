#include <alkahest/init.h>

namespace Alkahest
{
    namespace
    {
        // Static engine subsystems
        static WindowSystem s_windowSystem;
    }

    void Game::sysInit()
    {
        // Initialize engine subsystems
        s_windowSystem.init();
        s_windowSystem.createWindow();

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
        while(true)
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
