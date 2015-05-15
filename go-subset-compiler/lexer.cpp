#include "lexer.h"

void Lexer::setProgram(string program){
    this->program = program;
    actualChar = 0;
    stringValue = "";
    intValue = 0;
    doubleValue = 0;
    identifyer ="";
    token = Plus;
}

string Lexer::getStringValue(){
    return stringValue;
}

int Lexer::getIntValue(){
    return intValue;
}

double Lexer::getFloatValue(){
    return doubleValue;
}

Token Lexer::advance(){
    while(isspace(getActualChar()))
        actualChar++;


    if(isalpha(getActualChar())){ //this will be an identifier
        identifyer = getActualChar();
        actualChar++;
        while(isalnum(getActualChar())){
            identifyer += getActualChar();
            actualChar++;
        }
        token = Identifier;
    }

    else if(getActualChar() == '"'){
        stringValue = "";
        do{
            actualChar++;
            if(getActualChar() != '"')
             stringValue+=getActualChar();
        }while(getActualChar()!='"');
        actualChar++;
        token = String;
    }

    else if(isdigit(getActualChar()) || getActualChar() == '.'){
        string NumStr = "";
        do{
            NumStr += getActualChar();
            actualChar++;
        } while(isdigit(getActualChar()) || getActualChar() == '.');
        doubleValue = strtod(NumStr.c_str(), 0);
        token = Number;
    }

    else if(getActualChar() == '#'){
        do actualChar++;
        while(getActualChar() != EOF && getActualChar() != '\n' && getActualChar() != '\r');
    }
    else if(getActualChar() == ':'){
        actualChar++;
        if(getActualChar() == '='){
            token = Deklaration;
            actualChar++;
        }
        else
            token = ERROR;
    }
    else if(getActualChar() == '+'){
        token = Plus;
        actualChar++;
    } else if(getActualChar() == '-'){
        token = Minus;
        actualChar++;
    } else if(getActualChar() == '*'){
        token = Times;
        actualChar++;
    } else if(getActualChar() == '/'){
        token = Divide;
        actualChar++;
    } else if(getActualChar() == ';'){
        token = Semikolon;
        actualChar++;
    } else if(getActualChar() == '('){
        token = ParenthesisLeft;
        actualChar++;
    } else if(getActualChar() == ')'){
        token = ParenthesisRight;
        actualChar++;
    }

    else if(getActualChar() == EOF)
        token = ERROR;

    return token;
}

char Lexer::getActualChar(){
    if(actualChar >= program.size())
        return EOF;
    else
        return program.c_str()[actualChar];
}

Token Lexer::getLastToken(){
    return token;
}

Lexer::Lexer()
{
actualChar = 0;
}
