//
// Created by Patrick Moffitt on 10/5/17.
//

#include <iostream>
#include <string>
#include <map>
#include <cmath>
#include <iomanip>
#include "calculator.h"
#include "calculator_testing.h"

using std::map;
using std::string;
using std::fabs;

map<string, double> Infix_calculator_testing::cases = {
        {"8--7   +8++7",                              30.0},
        {"(8−−7)+(9×+1) −−3.4 − 4 ÷ 7",               26.8285714285714},
        {"(8−−7)+(9×+1) −−3.4 − 4 ÷ 7+2^2",           30.8285714285714},
        {"(8−−7)+(9×+1) −−3.4 − 4 ÷ 7*2^2",           25.1142857142857},
        {"(8−−7)+(9×+1) −−3.4 − 4 ÷ 7/2^2",           27.2571428571429},
        {"(8−−7)+(9×+1)−−3.4−4÷7",                    26.8285714285714},
        {"(8−−7)+(9×+1)−−3.4−4÷7+2^2",                30.8285714285714},
        {"(8−−7)+(9×+1)−−3.4−4÷7*2^2",                25.1142857142857},
        {"(8−−7)+(9×+1)−−3.4−4÷7/2^2",                27.2571428571429},
        {"(8.4 −+7.1)-(9.9× +1.0) −+3.4 + 4.6 ÷ 7.3", -11.3698630136986},
        {"(8+−7) + (9×-1) +−3.4 − 4 ÷ 7+2^2",         -7.97142857142857},
        {"(8−−7) *(9×+1) * −3.4 − 4 ÷ 7*2^2",         -461.285714285714},
        {"(8−−7)/ (9×+1) /−3.4 − 4 ÷ 7/2^2",          -0.633053221288515},
        {"-1+-1",                                     -2.0},
        {"-1++1",                                     0.0},
        {"5+2*3",                                     11.0},
        {"5^2*3",                                     75.0},
        {"5/2*3",                                     7.5},
        {"2.3^2.2",                                   6.24886639474804},
        {"5÷2×3^3",                                   67.5},
        {"5÷(2×(3^3))",                               0.0925925925925926},
        {"1-2-3",                                     -4.0},
        {"1+2+3",                                     6.0},
        {"(1+2)+3",                                   6.0},
        {"1+(2+3)",                                   6.0},
        {"1.1-2.2-3.3",                               -4.4},
        {"1.1+2.2+3.3",                               6.6},
        {"(1.1+2.2)+3.3",                             6.6},
        {"1.1+(2.2+3.3)",                             6.6},
};


// Credit to Michael Goldshteyn on SO.
bool Infix_calculator_testing::approximately_equal(double a, double b, double error_factor = 12.0) {
    return a == b || fabs(a - b) < fabs(std::min(a, b)) * std::numeric_limits<double>::epsilon() * error_factor;
}

void Infix_calculator_testing::run() {
    string equation;
    double sample;
    for (auto &test : cases) {
        Infix_calculator c{test.first, true};
        sample = c.compute();
        //sample = c.solve();
        if (not approximately_equal(test.second, sample)) {
            std::cerr << std::setprecision(std::numeric_limits<long double>::digits10 + 1)
                      << "Testing " << equation << " failed. Test result " << sample
                      << " not equal to expected result " << test.second << ". "
                      << "Difference is " << fabs(sample - test.second) << ". " << std::endl;
            auto err = std::make_error_condition(std::errc::operation_canceled);
            throw std::runtime_error(err.message());
        }
    }
}
