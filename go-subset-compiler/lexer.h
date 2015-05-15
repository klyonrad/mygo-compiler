#ifndef LEXER_H
#define LEXER_H
#pragma once

#include <string>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <vector>
#include <fstream>
using namespace std;


enum Token{
    Plus, //0
    Minus,//1
    Times, //2
    Divide,//3
    Semikolon, //4
    Deklaration,//5
    Identifier,//6
    Number,//7
    String,//8
    ERROR//9
};

class Lexer
{
public:
    Lexer();
    void setProgram(string);
    string getStringValue();
    int getIntValue();
    Token advance();
    char getActualChar();
    double getFloatValue();
    Token getLastToken();

    string stringValue;
    double doubleValue;
    int intValue;
    string identifyer;
    Token token;
    string program;
    unsigned int actualChar;
private:
};

#endif // LEXER_H
