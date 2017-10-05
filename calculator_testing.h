//
// Created by Patrick Moffitt on 10/5/17.
//

#ifndef INC_9_CALCULATOR_CALCULATOR_TESTING_H
#define INC_9_CALCULATOR_CALCULATOR_TESTING_H

#include <map>

using std::map;
using std::string;

class Infix_calculator_testing {
    static map<string, double> cases;
    // Credit to Michael Goldshteyn on SO.
    bool approximately_equal(double a, double b, double error_factor);
public:
    void run();
};
#endif //INC_9_CALCULATOR_CALCULATOR_TESTING_H
