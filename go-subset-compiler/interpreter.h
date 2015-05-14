#ifndef INTERPRETER_H
#define INTERPRETER_H
#pragma once

#include <map>
#include "lexer.h"
#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;


typedef double (*BiOperator)(double, double);

class Interpreter
{
public:
    Interpreter(Lexer* lex);
    void programm();
    void line();
    void start();
    double expression();
private:
    Lexer* lex;
    map<string, double> vars;
    map<Token, BiOperator> ops;
};

#endif // INTERPRETER_H
