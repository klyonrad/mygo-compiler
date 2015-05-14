
#include <iostream>
#include "lexer.h"
#include "interpreter.h"

using namespace std;

int main(){
    string program;
    cout << "tinyGo Parser v0.1:\n" << endl;
    Lexer* myLexer = new Lexer();
    Interpreter i(myLexer);
    while(true){
        try{
            cout << "tinyGo> ";
            getline(cin, program);
            myLexer->setProgram(program);
            i.start();
        }catch(const char* c){
            cout.flush();
            cout << c << endl;
        }
    }
    return 0;
}
