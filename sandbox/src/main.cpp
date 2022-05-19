#include <alkahest.h>

void Alkahest::Game::init()
{
}

void Alkahest::Game::update()
{
    Alkahest::logInfo("In function: {}", "update()");
    AKST_ASSERT(1 == 2);  // Should fail
}

void Alkahest::Game::cleanup()
{
}
