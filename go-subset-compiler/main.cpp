
#include <iostream>
#include "lexer.h"
#include "interpreter.h"

using namespace std;

int main(int argc, char* argv[]){
    string program;
    cout << "tinyGo Parser v0.1.2:" << endl << endl;
    Lexer* myLexer = new Lexer();
    Interpreter myInterpreter(myLexer);
    
    if (argc == 1) {
        while(true){
            try{
                cout << "tinyGo> ";
                getline(cin, program);
                myLexer->setProgram(program);
                myInterpreter.start();
            }catch(const char* c){
                cout.flush();
                cout << c << endl;
            }
        }
    }
    else if (argc == 2) {
        std::cout << "tinyGo reading from file" << endl;
        char * filePath = argv[1]; //argv[0] is the path of the binary itself
        try{
            std::ifstream infile (filePath);
            while (!infile.eof()) {
                getline (infile, program);
                std::cout << program << "    ||||  "; //print every source code line and next to that the output of the interpreter
                myLexer->setProgram(program);
                myInterpreter.start();
            }
            
        }catch(const char* c){
            
            cout.flush();
            cout << c << endl;
        }
    }
    return 0;
}
