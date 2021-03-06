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
    bool isOperator(Token);// todo
    double operatorFunction (Token, double, double); // todo
    void deklaration(string identifier);
    Lexer* lex;
    map<string, double> vars; // number variables
    map<Token, BiOperator> ops; // map between operator and corresponding function
};

#endif // INTERPRETER_H
