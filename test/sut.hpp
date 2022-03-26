#pragma once

// Replace this with the bimap version you want to use
constexpr int BIMAP_VERSION = 0;

#if (BIMAP_VERSION == 0)
#include <bimap0.hpp>
#endif

#if (BIMAP_VERSION == 1)
#include <bimap1.hpp>
#endif

#include <catch.hpp>
