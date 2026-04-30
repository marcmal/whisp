#pragma once

// Wrapper as hack to silent GCC 'error: global module fragment contents must be from preprocessor inclusion'
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <CImg.h>
#pragma GCC diagnostic pop