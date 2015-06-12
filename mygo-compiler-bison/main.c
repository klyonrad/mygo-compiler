/*
 * main.c file
 */

#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include <map>
#include <iostream>

using namespace std;

static map <string, float> fvars;

int yyparse(SExpression **expression, yyscan_t scanner);

SExpression *getAST(const char *expr)
{
    SExpression *expression;
    YY_BUFFER_STATE state;
    yyscan_t scanner;

    if (yylex_init(&scanner)) {
        // couldn't initialize
        return NULL;
    }

    state = yy_scan_string(expr, scanner);

    if (yyparse(&expression, scanner)) {
        // error parsing
        return NULL;
    }

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return expression;
}

string evaluateId(SExpression *e){
    if(e->type == eIDENTIFIER)
        return string(e->svalue);
    else {
        cout << "Fehlerhafter Identifier" << endl;
        return "";
    }
}

float calculate(SExpression *e) {
   switch (e->type) {
    case eVALUE:
        return e->value;
    case eFLOAT:
        return e->fvalue;
    case eMULTIPLY:
        return calculate(e->left) * calculate(e->right);
    case ePLUS:
        return calculate(e->left) + calculate(e->right);
    case eDIVIDE:
        return calculate(e->left) / calculate(e->right);
    case eMINUS:
        return calculate(e->left) - calculate(e->right);
    case eIDENTIFIER:
        return fvars[e->svalue];
    default:
        assert(false);
        return 0;
   }
}

float evaluate(SExpression *e)
{
    switch (e->type) {
        case eLINE: {
            float v1 = evaluate(e->left);
            float v2 = evaluate(e->right);
            //return (v2==NULL) ? v1 : v2;
            return (v2==0)?v1:v2;
    }
        case eVALUE:
            return e->value;
        case eFLOAT:
            return e->fvalue;
        case eMULTIPLY:
            return calculate(e->left) * calculate(e->right);
        case ePLUS:
            return calculate(e->left) + calculate(e->right);
        case eDIVIDE:
            return calculate(e->left) / calculate(e->right);
        case eMINUS:
            return calculate(e->left) - calculate(e->right);
        case eIDENTIFIER:
            return fvars[e->svalue];
        case eDEKLERATION:
            fvars[evaluateId(e->left)] = evaluate(e->right);
            return fvars[e->svalue];
    case eEOF:
        return 0;
        default:
            assert(false);
            // shouldn't be here
            return 0;
    }
}

void printGraph (SExpression *e) {
  switch (e->type) {
    case eLINE:
        printGraph(e->left);
        std::cout << endl;
        printGraph(e->right);
        break;

    case eVALUE:
        cout << e->value;
        break;
    case eFLOAT:
        cout << e->fvalue;
        break;
    case eMULTIPLY:
        cout << "(";
        printGraph(e->left);
        std::cout << ") * (" ;
        printGraph(e->right);
        cout << ")";
        break;
    case ePLUS:
        cout << "(";
        printGraph(e->left);
        std::cout << ") + (" ;
        printGraph(e->right);
        cout << ")";
        break;
    case eDIVIDE:
        cout << "(";
        printGraph(e->left);
        std::cout << ") / (" ;
        printGraph(e->right);
        cout << ")";
        break;
    case eMINUS:
        cout << "(";
        printGraph(e->left);
        std::cout << ") - (" ;
        printGraph(e->right);
        cout << ")";
        break;
    case eIDENTIFIER:
        cout << e->svalue;
        break;
    case eDEKLERATION:
        printGraph(e->left);
        std::cout << " := (" ;
        printGraph(e->right);
        cout << ")";
        break;
    case eEOF:
      // TODO
      break;
    default:
        assert(false);
        // shouldn't be here
        cout << "error!";
    }
}

typedef struct{
  string term;
  float expected;
  float result;
}Test;

// tests ob zwei Zahlen gleich sind, hat dabei aber eine gewisse Toleranz
bool test(float expected, float actual, std::string term){
	std::cout << term << " |||  ";
  //printf("expected: %f actual: %f\n", actual, expected);
    std::cout << "expected: " << expected << " actual: " << actual << endl;
  float diff = expected < actual ? actual-expected : expected-actual;
  return diff < 0.001;
}

int main(void)
{
	//bison debug option:
    yydebug = 0;
	
    SExpression *e = NULL;
    vector<Test> tests;
	
    fvars["b"] = 10.0;

	tests = {
        // simple int Tests
        {"4/4;", 1.0, 0},
        {"4-4;", 0.0, 0},
        {"4*4;", 16, 0},
        {"4+4;", 8.0, 0},
        // simple float Tests
        {"3.0/4.0;", 0.75, 0},
        {"4.5-3.0;", 1.5, 0},
        {"2.2*1.5;", 3.3, 0},
        {"2.2+1.5;", 3.7, 0},
        {"12;", 12.0, 0},
        {"5 + 5; 2+2;", 4.0, 0},
        {"a := 15; 12;", 12.0, 0},
        {"b;", 10, 0},
        {"a := 15;", 0.0, 0},
        {"a := ( 7 + 8 ); a;", 15.0, 0},
        {"a := 15; c:= a + 5; c;", 20.0, 0},
		//komplexe Tests
        {"vier := 4.5; fuenf := 4.5; vier + fuenf;", 9.0, 0},
        {"4 + ( 2 * 10 ) - ( 3 / ( 5 + 1 ) );", 23.5, 0},
        {"vier := 4; fuenf := 5; vier + ( 2 * 10 ) - ( 3 / ( fuenf + 1 ) );", 23.5, 0}
	};

    printf("\033[1;33mTesting Build\033[0m (%d Tests)..\n", tests.size());
    int testsFailed = 0;
    for(int i = 0; i<tests.size();i++){
      e = getAST(tests[i].term.c_str());
      tests[i].result = evaluate(e);
      //printGraph(e); cout << endl;
      if(!test(tests[i].expected, tests[i].result, tests[i].term) ){
        testsFailed++;
        printf("Test %d \033[1;31mfailed\033[0m '%s'" /*is %f but was expected to be %f.\n"*/, i+1, tests[i].term.c_str()/*, tests[i].result, tests[i].expected*/ );
        cout << " is " << tests[i].result << " but was expected to be " << tests[i].expected << " ." << endl;
      }else
        printf("Test %d \033[1;32mpassed\033[0m\n", i);
        //deleteExpression(e);
    }
    printf("%f%% passed. %lu passed, %d failed.\n", ((float)tests.size()-testsFailed)*100/tests.size(), (tests.size()-testsFailed), testsFailed);
    return 0;
}
