#include <iostream>
#include "lexer.h"
#include "interpreter.h"

using namespace std;

int main(){
    string program;
    cout << "tinyGo Parser v0.1:\n" << endl;
    Lexer* l = new Lexer();
    Interpreter i(l);
    while(true){
        try{
            cout << "tinyGo> ";
            getline(cin, program);
            l->setProgram(program);
            i.start();
        }catch(const char* c){
            cout.flush();
            cout << c << endl;
        }
    }
    return 0;
}
