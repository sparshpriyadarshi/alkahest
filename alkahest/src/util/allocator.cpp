#include <alkahest/util/allocator.h>

namespace Alkahest
{
    StackAllocator::StackAllocator(std::uint32_t size)
        : m_size(size)
    {
        AKST_ENG_ASSERT(size > 0, "Allocated size must be greater than zero!");

        m_stack = malloc(size);
        m_topMarker = 0;
    }

    StackAllocator::~StackAllocator()
    {
        free(m_stack);
    }

    void StackAllocator::clear()
    {
        m_topMarker = 0;
    }

    void *StackAllocator::alloc(std::uint32_t size)
    {
        AKST_ENG_ASSERT(m_topMarker + size < m_size, \
            "Insufficient space to allocate bytes!");

        void *chunk = static_cast<void*>(static_cast<char*>(m_stack) \
                + m_topMarker);
        m_topMarker += size;
        return chunk;
    }

    Marker StackAllocator::getMarker()
    {
        return m_topMarker;
    }

    void StackAllocator::freeToMarker(Marker m)
    {
        AKST_ENG_ASSERT(m >= 0, "Cannot free memory to a negative address!");
        AKST_ENG_ASSERT(m < m_topMarker, "m must be below the current top of the stack!");

        m_topMarker = m;
    }
}
