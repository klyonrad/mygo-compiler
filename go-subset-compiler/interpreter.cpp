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

void Interpreter::deklaration(string identifier){
    vars.insert(pair<string, double>(identifier, expression()));
}

void Interpreter::line(){
    Token t = lex->getLastToken();
    
    if(t != Identifier){
        cout << expression() << endl;
    }else{ // is an identifier
        string id = lex->identifyer;
        t = lex->advance();
        if(ops.find(t) != ops.end() ) {
            lex->advance();
            cout << ops[t](vars[id], expression() ) << endl;
        } else if(t == Deklaration){
            lex->advance();
            deklaration(id);
            cout << "variable declared" << endl;
            
        }
        
        if (lex->getLastToken() != Semikolon) {
            throw "Unexpected Token in line(). Forgot semicolon?";
        }
    }
    
    lex->advance(); // necessary ??? if multiple lines probably
    
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
            lex->advance();
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

