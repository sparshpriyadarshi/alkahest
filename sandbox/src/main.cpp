#include <alkahest.h>

void Alkahest::Game::init()
{
}

void Alkahest::Game::update()
{
    if (Alkahest::Input::isKeyDown(Alkahest::Key::Space))
    {
        Alkahest::logDebug("Space pressed!");
    }
}

void Alkahest::Game::cleanup()
{
}
