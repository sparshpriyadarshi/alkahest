#pragma once

#include "../macros.h"

namespace Alkahest
{
    class NOT_EXPORTED ISubsystem
    {
    public:
        ISubsystem() {};
        virtual ~ISubsystem() {};
    public:
        virtual void init() = 0;
        virtual void cleanup() = 0;
    };
}
