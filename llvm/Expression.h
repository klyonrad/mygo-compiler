/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>
#include <iostream>
using namespace std;


typedef enum tagEOperationType
{
    eMULTIPLY,
    ePLUS,
    eMINUS,
    eDIVIDE,
} EOperationType;

class Expression{
public:
    string type;
    virtual ~Expression(){}
    //virtual Value* codeGen() = 0;
    Expression* left;
    Expression* right;
    Expression(Expression* _left, Expression* _right, string _type):left(_left), right(_right), type(_type){}
    virtual string getType(){return type;}
};

class IntegerExpression : public Expression{
    public:
        int value;
        IntegerExpression(int _value):Expression(NULL, NULL, "Integer"), value(_value){}
};

class FloatExpression : public Expression{
    public:
        float value;
        FloatExpression(float _value):Expression(NULL, NULL,"Float"), value(_value){}
};

class DeklarationExpression : public Expression{
    public:
        virtual ~DeklarationExpression(){delete(left, right);}
        DeklarationExpression(Expression* _left, Expression* _right):Expression(_left, _right, "Deklaration"){}
};

class LineExpression : public Expression{
    public:
        virtual ~LineExpression(){delete(left, right);}
        LineExpression(Expression* _actualLine, Expression* _otherLines):Expression(_actualLine, _otherLines, "Line"){}
};

class IdentifierExpression : public Expression{
    public:
        string name;
        IdentifierExpression(string _name):Expression(NULL, NULL,"Identifier"), name(_name){}
};

class BiOperationExpression : public Expression{
    public:
        EOperationType opType;
        ~BiOperationExpression(){delete(type, left, right);}
        BiOperationExpression(EOperationType _opType, Expression* _left, Expression* _right):Expression(_left, _right, "BiOperation"),  opType(_opType){}
};

class EOFExpression : public Expression {
public:
    EOFExpression():Expression(NULL, NULL, "EOF"){}
};

class PrintExpression : public Expression{
    public:
        ~PrintExpression(){delete(left);delete(right);}
        PrintExpression(Expression* _param):Expression(_param, NULL, "Print"){}
};
#endif // __EXPRESSION_H__
