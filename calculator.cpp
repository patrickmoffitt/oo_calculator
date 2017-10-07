//
// Created by Patrick Moffitt on 10/4/17.
//

#include <iostream>
#include <string>
#include <list>
#include <regex>
#include <map>
#include <stdexcept>
#include <system_error>
#include <utility>
#include "boost/lexical_cast.hpp"
#include "calculator.h"

using std::cerr;
using std::cout;
using std::endl;
using std::function;
using std::string;
using std::regex;
using std::regex_replace;
using std::regex_search;
using std::sregex_iterator;
using std::distance;
using std::move;
using std::make_tuple;

/*
 * The intent of the format_* functions is to transform the input equation into a proper white-space
 * delimited string that can be trivially tokenized into the operator and value stacks. Pasting
 * from the Mac OS Numbers application or Microsoft Excel is supported.
 *
 * The intent of the validate_* functions is to check for invalid equations.
 */

// Format
// The Mac OS Numbers application uses a look-alike minus sign. This function replaces it with '-'.
regex Infix_calculator::rgx_looks_like_minus(R"(−)");

Infix_calculator &Infix_calculator::format_fix_minus() {
    equation = regex_replace(equation, rgx_looks_like_minus, "-");
    return *this;
}

/*
 * The Mac OS Numbers application uses an alternate division character. This function replaces it
 * with '/'.
 */
regex Infix_calculator::rgx_div_char(R"(÷)");

Infix_calculator &Infix_calculator::format_fix_divide() {
    equation = regex_replace(equation, rgx_div_char, "/");
    return *this;
}

/*
 * TThe Mac OS Numbers application uses an alternate multiplication character. This function
 * replaces it with '*'.
 */
regex Infix_calculator::rgx_times_char(R"(×)");

Infix_calculator &Infix_calculator::format_fix_multiply() {
    equation = regex_replace(equation, rgx_times_char, "*");
    return *this;
}

// Make operators distinct from signs. -1+-1 becomes -1 + -1.
regex Infix_calculator::rgx_operator_sign("([+\\-*\\/^])([+-]\\d+\\.?\\d*)");
regex Infix_calculator::rgx_digit_operator("([\\d()])([+\\-*\\/^])");

Infix_calculator &Infix_calculator::format_pad_operators() {
    equation = regex_replace(equation, rgx_operator_sign, "$1 $2");
    equation = regex_replace(equation, rgx_digit_operator, "$1 $2 ");
    return *this;
}

// Adds spaces around parentheses to support tokenization by white space.
regex Infix_calculator::rgx_parentheses("([()])");

Infix_calculator &Infix_calculator::format_pad_parentheses() {
    equation = regex_replace(equation, rgx_parentheses, " $1 ");
    return *this;
}

// Multiple spaces are condensed to one. Improves readability for humans.
regex Infix_calculator::rgx_spaces("(\\s{2,})");

Infix_calculator &Infix_calculator::format_condense_spaces() {
    equation = regex_replace(equation, rgx_spaces, " ");
    return *this;
}

// Validate
regex Infix_calculator::rgx_allowed_chars("([^()\\s\\d+\\-*\\/.^])", std::regex_constants::icase);

bool Infix_calculator::validate_characters() {
    bool result{false};
    if (regex_search(equation, rgx_allowed_chars)) {
        cerr << endl << "Detected invalid characters in: " << equation << endl;
        result = true;
    }
    return result;
}

/*
 * Finds sequences of 3 or more operators, and doubled * or /. Also finds more than one operator
 * between sets of parentheses. The algorithm gets this wrong so its excluded. The Mac OS Numbers
 * application and MS Excel get these right so these's room for improvement.
 */
regex Infix_calculator::rgx_excess_operators
        ("([+\\-*\\/]\\s{0,}){3,}|([*\\/^]\\s*){2,}|\\)\\s*([+\\-*\\/^]\\s*[+\\-*\\/^])\\s*\\(");

bool Infix_calculator::validate_operator_sequence() {
    bool result{false};
    if (regex_search(equation, rgx_excess_operators)) {
        cerr << endl << "Detected invalid sequence of operators in: " << equation << endl;
        result = true;
    }
    return result;
}

regex Infix_calculator::rgx_left_parenthesis("(\\()");
regex Infix_calculator::rgx_right_parenthesis("(\\))");

bool Infix_calculator::validate_balanced_parentheses() {
    auto it_l = sregex_iterator(equation.begin(), equation.end(), rgx_left_parenthesis);
    long left = distance(it_l, sregex_iterator());

    auto it_r = sregex_iterator(equation.begin(), equation.end(), rgx_right_parenthesis);
    long right = distance(it_r, sregex_iterator());

    bool result{false};
    if (left not_eq right) {
        cerr << endl << "Detected unbalanced parentheses in: " << equation << std::endl;
        result = true;
    }
    return result;
}

// Calculate
double Infix_calculator::plus(const double &x, const double &y) {
    return x + y;
};

double Infix_calculator::minus(const double &x, const double &y) {
    return x - y;
};

double Infix_calculator::multiply(const double &x, const double &y) {
    return x * y;
};

double Infix_calculator::divide(const double &x, const double &y) {
    return x / y;
};

double Infix_calculator::exponent(const double &x, const double &y) {
    return std::pow(x, y);
};

map<char, function<double(const double &, const double &)>> Infix_calculator::operators = {
        {'+', &Infix_calculator::plus},
        {'-', &Infix_calculator::minus},
        {'*', &Infix_calculator::multiply},
        {'/', &Infix_calculator::divide},
        {'^', &Infix_calculator::exponent},
};

/*
 * Implements the PEMDAS rule for order of operations.
 * 1. Parentheses, Exponents, Roots
 * 2. Multiplication, Division
 * 3. Addition, Subtraction
 */
map<const char, const int> Infix_calculator::operator_precedence = {
        {'(', 1},
        {')', 1},
        {'^', 1},
        {'*', 2},
        {'/', 2},
        {'+', 3},
        {'-', 3},
};

void Infix_calculator::calculate() {
    char operator_;      // operator is a reserved word.
    double operand_l;    // Left operand/value.
    double operand;      // Right operand/value.
    double result{0.0};
    // All algorithm credit to David Matuszek. U Penn CIT 594, 2002.
    // 1 Pop the operator from the operator stack.
    operator_ = operator_stack.front();
    operator_stack.pop_front();
    // 2 Pop the value stack twice, getting two operands.
    if (int(value_stack.size()) > 1) {
        operand = value_stack.front();
        value_stack.pop_front();
        operand_l = value_stack.front();
        value_stack.pop_front();
    } else {
        cerr << "Equation lacks sufficient values." << std::endl;
        auto err = std::make_error_condition(std::errc::operation_canceled);
        throw std::runtime_error(err.message());
    }
    // 3 Apply the operator to the operands, in the correct order.
    debug ? cout << "operator " << operator_ << ", operand_l " << operand_l << ", operand "
                 << operand << std::endl : cout;
    result = Infix_calculator::operators[operator_](operand_l, operand);
    // 4 Push the result onto the value stack.
    value_stack.emplace_front(result);
    debug ? cout << "Pushing " << result << " onto value stack. Size = "<< int(value_stack.size())
                 << std::endl : cout;
};

bool Infix_calculator::is_double(const string &token) {
    try {
        boost::lexical_cast<double>(token);
    }
    catch (boost::bad_lexical_cast &) {
        return false;
    }
    return true;
};

enum token_type {
    number              = 121,
    variable            = 122,
    left_parenthesis    = 123,
    right_parenthesis   = 124,
    arithmetic_operator = 125,
};

void Infix_calculator::parse() {
    parsed_equation.clear();
    string token;                      // Equation will be parsed into grammatical tokens.
    equation = format();
    if (not validate()) {
        regex token_regex(R"(\S+)");
        auto tokens_begin = sregex_iterator(equation.begin(), equation.end(), token_regex);
        auto tokens_end = sregex_iterator();
        for (sregex_iterator it = tokens_begin; it != tokens_end; ++it) {
            std::smatch match = *it;
            token = match.str();
            if (is_double(token)) {
                parsed_equation.push_front(make_tuple(token_type::number, token));
            } else if (token == "(") {
                parsed_equation.push_front(make_tuple(token_type::left_parenthesis, token));
            } else if (token == ")") {
                parsed_equation.push_front(make_tuple(token_type::right_parenthesis, token));
            } else {
                auto search = operators.find(*token.c_str());
                if (search != operators.end()) {
                    parsed_equation.push_front(make_tuple(token_type::arithmetic_operator, token));
                }
            }
        }
        parsed_equation.reverse();
    }
};

// Public Interface.
Infix_calculator::Infix_calculator(string equation, bool debug) : debug(debug) {
    this->equation = move(equation);
};

string Infix_calculator::format() {
    this->format_fix_minus()
            .format_fix_divide()
            .format_fix_multiply()
            .format_pad_operators()
            .format_pad_parentheses()
            .format_condense_spaces();
    return equation;
};

bool Infix_calculator::validate() {
    return this->validate_characters() or
           this->validate_operator_sequence() or
           this->validate_balanced_parentheses();
};

double Infix_calculator::solve() {
    string token;                      // Equation will be parsed into tokens.
    char operator_;                    // operator is a reserved word.
    const char *lp{"("};               // Left-Parenthesis.
    const char *rp{")"};               // Right-Parenthesis.
    double result{0.0};                // The solution to the equation.
    string input{equation};
    equation = format();
    if (not validate()) {
        debug ? cout << endl << equation << " Looks better than " << input << endl << endl : cout;
        regex token_regex(R"(\S+)");
        auto tokens_begin = sregex_iterator(equation.begin(), equation.end(), token_regex);
        auto tokens_end = sregex_iterator();
        // All algorithm credit to David Matuszek. U Penn CIT 594, 2002.
        // 1. While there are still tokens to be read in,
        for (sregex_iterator it = tokens_begin; it != tokens_end; ++it) {
            std::smatch match = *it;
            // 1.1 Get the next token.
            token = match.str();
            debug ? cout << "token: " << token << endl : cout;
            // 1.2 If the token is:
            // 1.2.1 A number: push it onto the value stack.
            if (is_double(token)) {
                value_stack.emplace_front(std::atof(token.c_str()));
                debug ? cout << "Pushing " << token << " onto value stack. Size = "
                             << int(value_stack.size()) << endl : cout;

                // 1.2.2 A variable: get its value, and push onto the value stack. (Not implemented.)
                // 1.2.3 A left parenthesis: push it onto the operator stack.
            } else if (*token.c_str() == *lp) {
                operator_stack.emplace_front(*token.c_str());
                debug ? cout << "Pushing " << token << " onto operator stack. Size = "
                             << int(operator_stack.size()) << endl : cout;
                // 1.2.4 A right parenthesis:
            } else if (*token.c_str() == *rp) {
                // 1 While the thing on top of the operator stack is not a
                //   left parenthesis,
                while (operator_stack.front() != *lp) {
                    calculate();
                }
                // 2 Pop the left parenthesis from the operator stack, and discard it.
                operator_stack.pop_front();
                // 1.2.5 An operator (call it operator_):
            } else {
                auto search = operators.find(*token.c_str());
                if (search != operators.end()) {
                    operator_ = *token.c_str();
                    // 1 While the operator stack is not empty, and the top thing on the
                    //   operator stack has the same or greater precedence as operator_,
                    //   and that thing is not a left parenthesis:
                    while (int(operator_stack.size()) > 0 and
                           int(value_stack.size()) >= 2 and
                           operator_precedence[operator_stack.front()]
                           <= operator_precedence[operator_] and
                           operator_stack.front() != *lp) {
                        calculate();
                    }
                    // 2 Push operator_ onto the operator stack.
                    operator_stack.emplace_front(operator_);
                    debug ? cout << "Pushed " << operator_ << " onto the operator stack. Size = "
                                 << int(operator_stack.size()) << endl : cout;
                }
            }
        }
        // 2. While the operator stack is not empty, calculate
        while (int(operator_stack.size()) > 0) {
            calculate();
        }
        // 3. At this point the operator stack should be empty, and the value
        //    stack should have only one value in it, which is the final result.
        result = value_stack.front();
    } else {
        auto err = std::make_error_condition(std::errc::invalid_argument);
        throw std::invalid_argument(err.message());
    }
    return result;
};

double Infix_calculator::compute() {
    parse();
    char operator_;    // operator is a reserved word.
    // All algorithm credit to David Matuszek. U Penn CIT 594, 2002.
    // 1. While there are still tokens to be read in,
    cout << endl << equation << endl;
    for (auto token : parsed_equation) {
        // 1.1 Get the next token.
        int type = static_cast<int>(std::get<0>(token));
        string value = boost::lexical_cast<string>(std::get<1>(token));
        // 1.2 If the token is:
        switch (type) {
            // 1.2.1 A number: push it onto the value stack.
            case token_type::number : {
                value_stack.emplace_front(std::atof(value.c_str()));
                debug ? cout << "Pushing " << value_stack.front() << " onto value stack. Size = "
                             << int(value_stack.size()) << endl : cout;
                break;
            } // 1.2.2 A variable: get its value, and push onto the value stack. (Not implemented.)
            case token_type::variable : {
                // Do nothing. Not implemented.
                break;
            } // 1.2.3 A left parenthesis: push it onto the operator stack.
            case token_type::left_parenthesis : {
                operator_stack.emplace_front(*value.c_str());
                debug ? cout << "Pushed " << operator_stack.front() << " onto operator stack. Size"
                        " = " << int(operator_stack.size()) << endl : cout;
                break;
            } // 1.2.4 A right parenthesis:
            case token_type::right_parenthesis : {
                // 1 While the thing on top of the operator stack is not a left parenthesis,
                while (operator_stack.front() != '(') {
                    calculate();
                }
                // 2 Pop the left parenthesis from the operator stack, and discard it.
                operator_stack.pop_front();
                break;
            } // 1.2.5 An operator (call it operator_):
            case token_type::arithmetic_operator : {
                auto search = operators.find(*value.c_str());
                if (search != operators.end()) {
                    operator_ = *value.c_str();
                    // 1 While the operator stack is not empty, and the top thing on the
                    //   operator stack has the same or greater precedence as operator_,
                    //   and that thing is not a left parenthesis:
                    while (int(operator_stack.size()) > 0 and
                           int(value_stack.size()) >= 2 and
                           operator_precedence[operator_stack.front()] <=
                           operator_precedence[operator_] and
                           operator_stack.front() != '(') {
                        calculate();
                    }
                    // 2 Push operator_ onto the operator stack.
                    operator_stack.emplace_front(operator_);
                    debug ? cout << "Pushed " << operator_ << " onto the operator stack. Size = "
                                 << int(operator_stack.size()) << endl : cout;
                }
                break;
            }
        }
    }
    // 2. While the operator stack is not empty, calculate
    while (int(operator_stack.size()) > 0) {
        calculate();
    }
    // 3. At this point the operator stack should be empty, and the value stack should have
    // only one value in it, which is the final result.
    return value_stack.front();
}