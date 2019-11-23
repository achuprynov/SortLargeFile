/* 
 * Copyright (C) Alexander Chuprynov <achuprynov@gmail.com>
 * This file is part of solution of test task described in readme.txt.
 */
#include <algorithm>
#include <sstream>
#include <iostream>

#include "large_file.h"
#include "converter.h"

namespace large_file
{
    const auto string_size = 1024 * 1024; // 1 Mb 
    const int string_double_capacity = string_size / 13; // length("8.19572e+307") = 12 + '\n' 

    void sort(
        const char * unsorted_file_name,
        const char * sorted_file_name,
        int max_memory_size)
    {
        std::string str;
        str.reserve(string_size);

        const auto buffer_size = max_memory_size/sizeof(double);
        std::vector<double> buffer;
        buffer.reserve(buffer_size);

        large_file::file unsorted_file(unsorted_file_name);
        large_file::file sorted_file(sorted_file_name);

        auto temp_files = unsorted_file.split(sorted_file_name, buffer, str);

        sorted_file.merge(temp_files, buffer, str);
    }

    file::file(const char * _name)
    {
        name = _name;
        is_temp_file = false;
    }

    file::file(const char * _name, int _index)
    {
        name = _name + converter::to_string(_index);
        is_temp_file = true;
    }

    file::~file()
    {
        ifile.close();
        ofile.close();
        
        if (is_temp_file)
            std::remove(name.c_str());
    }

    bool file::read(std::vector<double> & buffer)
    {
        if (ifile.is_open() == false)
            ifile.open(name);

        if (ifile.eof())
        {
            ifile.close();
            return false;
        }
        
        while (next())
        {
            buffer.push_back(get());
            if (buffer.size() == buffer.capacity())
            {
                break;
            }
        }

        return true;
    }

    void file::write(const std::vector<double> & buffer, std::string & str)
    {
        if (ofile.is_open() == false)
            ofile.open(name);

        str.clear();
        std::stringstream ss(str);
        int counter = 0;

        for (auto & val : buffer)
        {
            ss << val << std::endl;
            counter++;

            if (counter == string_double_capacity)
            {
                ofile << ss.rdbuf();
                ofile.flush();
                str.clear();
                ss.str(str);
                counter = 0;
            }
        }
        
        ofile << ss.rdbuf();
        ofile.flush();

        if (is_temp_file)
            ofile.close();
    }

    bool file::next()
    {
        if (ifile.is_open() == false)
            ifile.open(name);

        if (ifile >> val)
            return true;
        return false;
    }

    double file::get()
    {
        return val;
    }

    std::vector<std::shared_ptr<file>> file::split(
        const char * temp_file_name,
        std::vector<double> & buffer,
        std::string & str)
    {
        std::vector<std::shared_ptr<file>> temp_files;

        while (read(buffer))
        {
            std::sort(buffer.begin(), buffer.end());

            auto temp_file = std::make_shared<file>(temp_file_name, temp_files.size());
            temp_files.push_back(temp_file);
            temp_file->write(buffer, str);

            buffer.clear();
        }

        return std::move(temp_files);
    }

    void file::merge(
        std::vector<std::shared_ptr<file>> & temp_files,
        std::vector<double> & buffer,
        std::string & str)
    {
        buffer.clear();

        // read first value from file
        for (auto & file : temp_files)
        {
            file->next();
        }

        while (!temp_files.empty())
        {
            std::sort(temp_files.begin(), temp_files.end(),
                [](const std::shared_ptr<file> & a, const std::shared_ptr<file> & b) -> bool
                { 
                    return a->get() < b->get(); 
                }
            );

            buffer.push_back(temp_files.front()->get());
            
            if (buffer.size() == buffer.capacity())
            {
                write(buffer, str);
                buffer.clear();
            }

            if (temp_files.front()->next() == false)
            {
                temp_files.erase(temp_files.begin());
            }
        }

        write(buffer, str);
    }
}
