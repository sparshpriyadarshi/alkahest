#include <alkahest/util/input.h>

namespace Alkahest
{
    namespace Input
    {
        namespace {
            static InputSystem s_inputSystem;
        }

        void setInputSystem(InputSystem& i)
        {
            s_inputSystem = i;
        }
    }
}
