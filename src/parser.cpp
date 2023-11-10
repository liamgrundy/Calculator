#include "parser.h"

std::map<std::string, double> _variables;

double parser::unaryOperation(double x, Operator op) {
    switch (op) {
        case Operator::ADD:
            return x;
        case Operator::SUBTRACT:
            return -x;
        default:
            return x;
    }
}

double parser::binaryOperation(double x, double y, Operator op) {
    switch (op) {
        case Operator::ADD:
            return x + y;
        case Operator::SUBTRACT:
            return x - y;
        case Operator::MULTIPLY:
            return x * y;
        case Operator::DIVIDE:
            return x / y;
        default:
            return x;
    }
}

void parser::skipSpace(const char *&pos) {
    while (*pos == ' ') {
        ++pos;
    }
}

const char *parser::nextCharacter(const char *&pos) {
    skipSpace(++pos);
    return pos;
}

const char *parser::peekNextCharacter(const char *pos) {
    skipSpace(++pos);
    return pos;
}

bool parser::parseEnd(const char *start, const char *end) {
    return (*start == '\0') || (start == end);
}

double parser::parseNumber(const char *&pos, const char *end) {
    double num = 0.0;
    double divisor = 1.0;
    bool foundDecimalPoint = false;
    while (!parseEnd(pos, end)) {
        if (*pos == '.' && !foundDecimalPoint) {
            foundDecimalPoint = true;
            ++pos;
        }
        else if (isdigit(*pos)) {
            divisor *= foundDecimalPoint ? 10.0 : 1.0;
            num = 10 * num + (int)(*pos - '0');
            ++pos;
        }
        else {
            break;
        }
    }
    return num / divisor;
}

std::string parser::parseIdentifier(const char *&pos, const char *end) {
    const char *start = pos;
    while (!parseEnd(pos, end) && (isalnum(*pos) || *pos == '_')) {
        ++pos;
    }
    return { start, size_t(pos - start) };
}

double parser::assignVariable(const std::string &name, double value) {
    return _variables[name] = value;;
}

double parser::assignAddVariable(const std::string &name, double value) {
    return _variables[name] += value;;
}

double parser::assignSubtractVariable(const std::string &name, double value) {
    return _variables[name] -= value;;
}

double parser::assignMultiplyVariable(const std::string &name, double value) {
    return _variables[name] *= value;;
}

double parser::assignDivideVariable(const std::string &name, double value) {
    return _variables[name] /= value;
}

bool parser::isDefinedVariable(std::string &name) {
    return _variables.find(name) != _variables.end();
}

double parser::getVariable(std::string &name) {
    if (!isDefinedVariable(name)) {
        throw std::runtime_error("Variable '" + name + "' is undefined");
    }
    return _variables[name];
}

double parser::parseValue(const char *&pos, const char *end) {
    if (isdigit(*pos)) {
        return parseNumber(pos, end);
    }
    else if (isalpha(*pos) || *pos == '_') {
        std::string variableName = std::move(parseIdentifier(pos, end));
        return getVariable(variableName);
    }

    throw std::runtime_error("parseValue(): Invalid syntax, unexpected symbol " + std::string(1, *pos));
}

double parser::evaluateSubexpression(const char *&pos, const char *end) {
    if (*pos != '(') {
        return parseValue(pos, end);
    }

    nextCharacter(pos);

    const char *rightBracketPos = pos;
    while (!parseEnd(rightBracketPos, end) && *rightBracketPos != ')') {
        ++rightBracketPos;
    }

    if (!isalnum(*pos) && *pos != '+' && *pos != '-' && *pos != '_') {
        throw std::runtime_error("evaluateSubexpression(): Invalid syntax, unexpected symbol " + std::string(1, *pos));
    }
    double left = evaluateTerm(pos, rightBracketPos);

    nextCharacter(pos);

    return left;
}

double parser::evaluateUnarySubtract(const char *&pos, const char *end) {
    if (*pos != '+' && *pos != '-') {
        return evaluateSubexpression(pos, end);
    }

    Operator op;
    if (*pos == '+') {
        op = Operator::ADD;
    }
    else {
        op = Operator::SUBTRACT;
    }

    nextCharacter(pos);

    if (!isalnum(*pos) && *pos != '(' && *pos != '_') {
        throw std::runtime_error("evaluateUnarySubtract(): Invalid syntax, unexpected symbol " + std::string(1, *pos));
    }
    double value = evaluateSubexpression(pos, end);

    return unaryOperation(value, op);
}

double parser::evaluateFactor(const char *&pos, const char *end) {
    if (!isalnum(*pos) && *pos != '+' && *pos != '-' && *pos != '(' && *pos != '_') {
        throw std::runtime_error("evaluateFactor(): Invalid syntax, unexpected symbol " + std::string(1, *pos));
    }
    double left = evaluateUnarySubtract(pos, end);

    while (!parseEnd(pos, end)) {
        skipSpace(pos);

        Operator op;
        switch (*pos) {
            case '*':
                op = Operator::MULTIPLY;
                break;
            case '/':
                op = Operator::DIVIDE;
                break;
            default:
                return left;
        }

        nextCharacter(pos);

        if (!isalnum(*pos) && *pos != '+' && *pos != '-' && *pos != '(' && *pos != '_') {
            throw std::runtime_error("evaluateFactor(): Invalid syntax, unexpected symbol " + std::string(1, *pos));
        }
        double right = evaluateUnarySubtract(pos, end);

        left = binaryOperation(left, right, op);
    }

    return left;
}

double parser::evaluateTerm(const char *&pos, const char *end) {
    if (!isalnum(*pos) && *pos != '+' && *pos != '-' && *pos != '(' && *pos != '_') {
        throw std::runtime_error("evaluateTerm(): Invalid syntax, unexpected symbol " + std::string(1, *pos));
    }
    double left = evaluateFactor(pos, end);

    while (!parseEnd(pos, end)) {
        skipSpace(pos);

        Operator op;
        switch (*pos) {
            case '+':
                op = Operator::ADD;
                break;
            case '-':
                op = Operator::SUBTRACT;
                break;
            default:
                return left;
        }

        nextCharacter(pos);

        if (!isalnum(*pos) && *pos != '+' && *pos != '-' && *pos != '(' && *pos != '_') {
            throw std::runtime_error("evaluateTerm(): Invalid syntax, unexpected symbol " + std::string(1, *pos));
        }
        double right = evaluateFactor(pos, end);

        left = binaryOperation(left, right, op);
    }

    return left;
}

double parser::evaluateAssignment(const char *&pos, const char *end) {
    const char *start = pos;

    if (!isalpha(*pos) && *pos != '_') {
        return evaluateTerm(pos, end);
    }

    std::string variableName = std::move(parser::parseIdentifier(pos, end));

    skipSpace(pos);

    if (parseEnd(pos, end)) {
        return getVariable(variableName);
    }
    else if (*pos == '+' && *peekNextCharacter(pos) == '=') {
        nextCharacter(pos);
        nextCharacter(pos);

        double value = evaluateAssignment(pos, end);
        return assignAddVariable(variableName, value);
    }
    else if (*pos == '-' && *peekNextCharacter(pos) == '=') {
        nextCharacter(pos);
        nextCharacter(pos);

        double value = evaluateAssignment(pos, end);
        return assignSubtractVariable(variableName, value);
    }
    else if (*pos == '*' && *peekNextCharacter(pos) == '=') {
        nextCharacter(pos);
        nextCharacter(pos);

        double value = evaluateAssignment(pos, end);
        return assignMultiplyVariable(variableName, value);
    }
    else if (*pos == '/' && *peekNextCharacter(pos) == '=') {
        nextCharacter(pos);
        nextCharacter(pos);

        double value = evaluateAssignment(pos, end);
        return assignDivideVariable(variableName, value);
    }
    else if (*pos != '=') {
        pos = start;
        return evaluateTerm(pos, end);
    }
    else {
        nextCharacter(pos);
        double value = evaluateAssignment(pos, end);
        return assignVariable(variableName, value);
    }
}

double parser::evaluateExpression(const char *&pos, const char *end) {
    skipSpace(pos);
    double result = evaluateAssignment(pos, end);
    skipSpace(pos);
    if (pos != end)
        throw std::runtime_error("evaluateExpression(): Invalid syntax, unexpected symbol " + std::string(1, *pos));
    return result;
}

double parser::calculate(const std::string &str) {
    const char *start = str.c_str();
    const char *end = start + str.size();
    return evaluateExpression(start, end);
}
