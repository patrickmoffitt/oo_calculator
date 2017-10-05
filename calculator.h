//
// Created by Patrick Moffitt on 10/4/17.
//

#ifndef INC_9_CALCULATOR_CALCULATOR_H
#define INC_9_CALCULATOR_CALCULATOR_H

#include <string>
#include <map>
#include <list>
#include <regex>

using std::string;
using std::map;
using std::regex;
using std::function;
using std::list;

class Infix_calculator {

public:
    explicit Infix_calculator(string equation);
    string format();
    bool validate();
    double solve();

private:
    // Format
    static regex rgx_looks_like_minus;
    Infix_calculator &format_fix_minus();

    static regex rgx_div_char;
    Infix_calculator &format_fix_divide();

    static regex rgx_times_char;
    Infix_calculator &format_fix_multiply();

    static regex rgx_operator_sign;
    static regex rgx_digit_operator;
    Infix_calculator &format_pad_operators();

    static regex rgx_parentheses;
    Infix_calculator &format_pad_parentheses();

    static regex rgx_spaces;
    Infix_calculator &format_condense_spaces();

    // Validate
    static regex rgx_allowed_chars;
    bool validate_characters();

    static regex rgx_excess_operators;
    bool validate_operator_sequence();

    static regex rgx_left_parenthesis;
    static regex rgx_right_parenthesis;
    bool validate_balanced_parentheses();

    // Calculate
    string equation;
    list<char> operator_stack;
    list<double> value_stack;
    static double plus(const double &x, const double &y);
    static double minus(const double &x, const double &y);
    static double multiply(const double &x, const double &y);
    static double divide(const double &x, const double &y);
    static double exponent(const double &x, const double &y);
    static map<char, function<double(const double &, const double &)>> operators;
    static map<const char, const int> operator_precedence;
    void calculate();
    bool is_double(const string &token);
};
#endif //INC_9_CALCULATOR_CALCULATOR_H
