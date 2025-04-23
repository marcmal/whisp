#pragma once

#include "util/types.hpp"
#include <string>
#include <vector>

namespace whisp
{

struct CoderData
{
    std::vector<whisp::Byte> content;
    std::string filename;
};

}