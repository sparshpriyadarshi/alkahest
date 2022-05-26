#pragma once

#include "../macros.h"

namespace Alkahest
{
    // Markers represent a point within memory
    typedef std::uint32_t Marker;

    class NOT_EXPORTED BaseAllocator
    {
    public:
        BaseAllocator() {};
        virtual ~BaseAllocator() {};
        virtual void clear() = 0;
    };

    class API StackAllocator : public BaseAllocator
    {
    public:
        StackAllocator(std::uint32_t size);
        virtual ~StackAllocator();
        virtual void clear() override;
    public:
        void *alloc(std::uint32_t size);
        Marker getMarker();
        void freeToMarker(Marker m);
    private:
        Marker m_topMarker;
        void *m_stack;
        std::uint32_t m_size;
    };
}
