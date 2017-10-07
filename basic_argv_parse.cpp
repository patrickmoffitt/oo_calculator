#include "basic_argv_parse.h"


using std::string;
using std::find;

char *Basic_argv_parse::get_option(char **begin, char **end, const std::string &option) {
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool Basic_argv_parse::option_exists(char **begin, char **end, const std::string &option) {
    return std::find(begin, end, option) != end;
}