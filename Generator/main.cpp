/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#include <sstream>
#include <iostream>
#include <fstream>
#include <random>
#include <limits.h> 

const auto string_size = 1024 * 1024; // 1 Mb 
const int string_double_capacity = string_size / 13; // length("8.19572e+307") = 12 + '\n' 

const double random_lower_bound = 0;
const double random_upper_bound = std::numeric_limits<double>::max();

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

void generate(const char * file_name, int max_file_size)
{
    std::ofstream file(file_name);

    std::uniform_real_distribution<double> unif(random_lower_bound, random_upper_bound);
    std::default_random_engine re;

    std::string str;
    str.reserve(string_size);
    std::stringstream ss(str);
    int counter = 0;

    while (max_file_size > 0)
    {
        double val = unif(re);
        auto str = converter::to_string(val);

        ss << str << std::endl;
        counter++;
        max_file_size -= str.length();
        max_file_size--; // std::endl

        if (counter == string_double_capacity)
        {
            file << ss.rdbuf();
            file.flush();
            str.clear();
            ss.str(str);
            counter = 0;
        }
    }

    file << ss.rdbuf();
    file.flush();
    file.close();
}

void usage(const char * app_name)
{
    std::cout << "usage: " << app_name << " <file name> <max size in bytes>" << std::endl;
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 3)
        {
            usage(argv[0]);
            return 1;
        }

        auto file_name = argv[1];
        auto max_file_size = atoi(argv[2]);

        generate(file_name, max_file_size);
    }
    catch (std::exception & ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
