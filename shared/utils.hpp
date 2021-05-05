#ifndef _UTILS_H_
#define _UTILS_H_
#include <string>
#include <sstream>
#include <fstream>

inline std::string read_file_into_string(const char *file_name)
{
    std::ifstream t(file_name);
    if (!t)
        throw "cannot open file";
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}

#endif // _UTILS_H_