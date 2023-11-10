#ifndef CALCULATOR_PARSER_H
#define CALCULATOR_PARSER_H

#include <map>
#include <stdexcept>
#include <string>
#include "log.h"

namespace parser {
    enum class Operator {
        NONE, ADD, SUBTRACT, MULTIPLY, DIVIDE
    };

    inline double unaryOperation(double x, Operator op);

    inline double binaryOperation(double x, double y, Operator op);

    inline void skipSpace(const char *&pos);

    inline const char *nextCharacter(const char *&pos);

    inline const char *peekNextCharacter(const char *pos);

    inline bool parseEnd(const char *start, const char *end);

    double parseNumber(const char *&pos, const char *end);

    std::string parseIdentifier(const char *&pos, const char *end);

    inline bool isDefinedVariable(std::string &name);

    inline double assignVariable(const std::string &name, double value);

    inline double assignAddVariable(const std::string &name, double value);

    inline double assignSubtractVariable(const std::string &name, double value);

    inline double assignMultiplyVariable(const std::string &name, double value);

    inline double assignDivideVariable(const std::string &name, double value);

    inline double getVariable(std::string &name);

    double parseValue(const char *&pos, const char *end);

    double evaluateSubexpression(const char *&pos, const char *end);

    double evaluateUnarySubtract(const char *&pos, const char *end);

    double evaluateFactor(const char *&pos, const char *end);

    double evaluateTerm(const char *&pos, const char *end);

    double evaluateExpression(const char *&pos, const char *end);

    double evaluateAssignment(const char *&pos, const char *end);

    double calculate(const std::string &str);
}

#endif //CALCULATOR_PARSER_H
