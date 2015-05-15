#include "interpreter.h"

Interpreter::Interpreter(Lexer* lex)
{
    this->lex = lex;
    ops.insert(pair<Token, BiOperator>(Plus, [](double a, double b){return a+b;}));
    ops.insert(pair<Token, BiOperator>(Minus, [](double a, double b){return a-b;}));
    ops.insert(pair<Token, BiOperator>(Times, [](double a, double b){return a*b;}));
    ops.insert(pair<Token, BiOperator>(Divide, [](double a, double b){return a/b;}));
}

void Interpreter::start(){
    lex->advance();
    programm();
}

void Interpreter::programm(){
    if(lex->getLastToken() == EOF || lex->getLastToken() == ERROR)
        return;
    else{
        line();
        programm();
    }
}

void Interpreter::line(){
    Token t = lex->getLastToken();
    if(t == Identifier){
        t = lex->advance();
        if(t==Deklaration){
            string id = lex->identifyer;
            lex->advance();
            vars.insert(pair<string, double>(id, expression()));
        }else if(t==Semikolon){
            cout << vars[lex->identifyer] << endl;
        }else if(ops.find(t) != ops.end()){
            string id = lex->identifyer;
            lex->advance();
            cout << ops.at(t)(vars[id], expression()) << endl;
        }else
            throw "Unexpexted Token: " + t;
    }else
        if(t==Number){
            cout << expression() << endl;
        }else
            throw "Unexpexted Token: " + t;
    lex->advance();
}


double Interpreter::expression(){
    double value;
    Token t = lex->getLastToken();
    if(t==Number) {
        value = lex->doubleValue;
    }
    else if(t==Identifier) {
        value = vars[lex->identifyer];
    }
    else if (t == ParenthesisLeft) {
        lex->advance();
        value = expression(); // here recursion
        
        if(lex->getLastToken() == ParenthesisRight){
            return value;
        }
        else{
            throw "Missing )";
        }
    }
    t = lex->advance();
    
    if(ops.find(t) != ops.end()) {
        lex->advance(); // consume operator
        return ops[t](value, expression() ); //calls the corresponding function
    } else {
        return value;
    }
    
    
    if(t == ERROR){
        throw "Unexpected end of line (you might missed a ;)";
    }
    lex->advance();
    
    return ops[t](value, expression()); //consume operator
}

