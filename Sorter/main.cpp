/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#include <iostream>

#include "large_file.h"

void usage(const char * appName)
{
    std::cout << "usage: " << appName << " <unsorted file name> <sorted file name> <max usage memory size in bytes>" << std::endl;
}

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 4)
        {
            usage(argv[0]);
            return 1;
        }

        auto unsorted_file_name = argv[1];
        auto sorted_file_name = argv[2];
        auto max_memory_size = atoi(argv[3]);

        large_file::sort(unsorted_file_name, sorted_file_name, max_memory_size);
    }
    catch (std::exception & ex)
    {
        std::cout << ex.what() << std::endl;
        return 1;
    }
    
    return 0;
}
