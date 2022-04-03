#pragma once

// Comment this out if you want to check compile time failures
#define NO_FAIL

// Replace this with the bimap version you want to use
#define BIMAP_VERSION 1

#if (BIMAP_VERSION == 0)
#include <bimap0.hpp>
#endif

#if (BIMAP_VERSION == 1)
#include <bimap1.hpp>
#endif

#include <catch.hpp>
