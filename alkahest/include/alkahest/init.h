#pragma once

#include "macros.h"

namespace Alkahest
{
    class Game
    {
    public:
        Game()
        {
            sysInit();
            init();
        }

        ~Game()
        {
            cleanup();
            sysCleanup();
        };
    public:
        void init();
        void update();
        void cleanup();
    private:
        void sysInit() {};
        void sysUpdate() {};
        void sysCleanup() {};
    public:
        void run()
        {
            while (true)
            {
                sysUpdate();
                update();
            }
        };
    };

    Game& CreateGame()
    {
        static Game g;
        return g;
    };
}

int main(int argc, char** argv)
{
    Alkahest::Game& game = Alkahest::CreateGame();
    game.run();
    return 0;
}
