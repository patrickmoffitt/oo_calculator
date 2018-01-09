#include <iostream>
#include <list>
#include "boost/format.hpp"
#include "calculator.h"
#include "calculator_testing.h"
#include "basic_argv_parse.h"


using std::getline;
using std::cin;
using std::cout;
using std::cerr;
using std::string;
using std::endl;
using std::system_error;
using std::exit;

int main(int argc, char * argv[]) {

    bool infix_calculator_debug{false};

    if (Basic_argv_parse::option_exists(argv, argv + argc, "-v")
        or Basic_argv_parse::option_exists(argv, argv + argc, "-d")) {
        // Enable debugging messages.
        infix_calculator_debug = true;
    }

    if (Basic_argv_parse::option_exists(argv, argv + argc, "-t")) {
        // Testing mode.
        try {
            Infix_calculator_testing t;
            t.run();
        } catch (const system_error& e) {
            cerr << e.what() << endl;
            exit(e.code().value());
        }

    } else {
        // Interactive mode.
        string equation;              // The infix expression to solve.
        double result{0.0};           // The solution to the equation.
        cout << endl << "Press Enter to quit." << endl;
        cout << "Enter an arithmetic equation in infix notation: ";
        while (getline(cin, equation) and equation.length() > 0) {
            Infix_calculator c{equation, infix_calculator_debug};
            result = c.compute();
            cout << endl << c.format() << " = " << boost::format("%.4g") % result << endl;
            cout << endl << "Enter an arithmetic equation in infix notation: ";
            // Start over.
            result = 0.0;
        }
    }

    return 0;
}