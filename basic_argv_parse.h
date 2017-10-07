#ifndef BASIC_ARGV_PARSE_LIBRARY_H
#define BASIC_ARGV_PARSE_LIBRARY_H

#include <string>

using std::string;

class Basic_argv_parse {
public:
    static char *get_option(char **begin, char **end, const std::string &option);

    static bool option_exists(char **begin, char **end, const std::string &option);
};

#endif