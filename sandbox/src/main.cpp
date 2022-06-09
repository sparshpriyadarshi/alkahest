#include <alkahest.h>

extern Alkahest::HeapAllocator g_PrimaryHeapAllocator;

class Test
{
public:
    Test() = default;
    void *operator new(std::size_t size)
    {
        void *mem = g_PrimaryHeapAllocator.alloc(size);
        return ::new(mem) Test;
    }
};

void Alkahest::Game::init()
{
    Alkahest::logDebug("In Game::init()");
    Test *t = new Test();
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
