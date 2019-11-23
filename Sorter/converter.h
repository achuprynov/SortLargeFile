/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#pragma once

#include <sstream>

namespace converter
{
    template<class T>
    std::string to_string(T val)
    {
        std::ostringstream ss;
        ss << val;
        return std::move(ss.str());
    }
}
