# Calculator
## Introduction
A basic calculator which uses a recursive descent parser to evaluate mathematical expressions which are entered via the command line.
The mathematical operations follow the rules of operator precedence, and brackets can be used so signal that a subexpression should be evaluated.
Allows the definition of basic variable types, which are double-precision floating point numbers.

## Syntax
- Arithmetic operators:
  - addition: +
  - subtraction: -
  - multiplication: *
  - division: /

- Assignment
  - Assignment using =, +=, -=, *=, /= similar to C/C++ syntax
- Variables
  - Variables are declared by writing their name and assigning a value or the result of an expression using the assignment operator =
  - Variables can be reassigned by assigning them again
  - Variables which have been defined can be used to evaluate expressions
  - A runtime error occurs if an undeclared variable is used in an expression
 
## Examples
Evaluate a simple mathematical expression:
```python
5.7 * (3.1 - 1.6) / 0.65
```

Declare and assign a variable:
```python
voltage = 5.735 * 8.453
```

Use variable in expression:
```python
3.6 * voltage + 1.5
```
