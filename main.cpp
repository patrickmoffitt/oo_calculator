#include <iostream>
#include <list>
#include "calculator.h"
#include "calculator_testing.h"

int main() {

    // Testing harness.
    try {
        Infix_calculator_testing t;
        t.run();
    } catch (const std::system_error& e) {
        std::cerr << e.what() << std::endl;
        std::exit(e.code().value());
    }
    /*
    std::list<std::string> tests{
            "5/2*3",
            "2.3^2.2",
            "5/2*3^3",
            "5÷(2*(3^3))",
            "1−2−3",
            "1+2+3",
            "(1+2)+3",
            "1+(2+3)",
            "1.1-2.2-3.3",
            "1.1+2.2+3.3",
            "(1.1+2.2)+3.3",
            "1.1+(2.2+3.3)",
    };
    for (auto t : tests) {
        Infix_calculator c{t};
        std::cout << t << "    " << c.format() << std::endl;
    }
    */
    return 0;
}