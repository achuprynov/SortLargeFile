/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#pragma once

#include <fstream>
#include <vector>
#include <memory>

namespace large_file
{
    void sort(
        const char * unsorted_file_name,
        const char * sorted_file_name,
        int max_memory_size
    );

    class file
    {
    private:
        std::string name;
        std::ifstream ifile;
        std::ofstream ofile;
        double val;

        bool is_temp_file;

    public:
        file(const char * _name);
        file(const char * _name, int _index);
        virtual ~file();

        bool read(std::vector<double> & buffer);
        void write(const std::vector<double> & buffer, std::string & str);

        bool next();
        double get();

        std::vector<std::shared_ptr<file>> split(
            const char * temp_file_name,
            std::vector<double> & buffer,
            std::string & str);

        void merge(
            std::vector<std::shared_ptr<file>> & temp_files,
            std::vector<double> & buffer,
            std::string & str);
    };
}
