#pragma once

#include "alkahest/init.h"

#include "alkahest/util/allocator.h"

#include "alkahest/systems/input.h"

#if defined AKST_WIN32

#elif defined AKST_APPLE

#else
#include "alkahest/platform/linux/keys.h"
#endif
