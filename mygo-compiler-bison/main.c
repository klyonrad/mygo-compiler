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

float evaluate(SExpression *e)
{
    switch (e->type) {
        case eVALUE:
            return e->value;
        case eFLOAT:
            return e->fvalue;
        case eMULTIPLY:
            return evaluate(e->left) * evaluate(e->right);
        case ePLUS:
            return evaluate(e->left) + evaluate(e->right);
        case eDIVIDE:
            return evaluate(e->left) / evaluate(e->right);
        case eMINUS:
            return evaluate(e->left) - evaluate(e->right);
        case eIDENTIFIER:
            return fvars[e->svalue];
        case eDEKLERATION:
            fvars[e->svalue] = evaluate(e->right);
            return fvars[e->svalue];
        default:
            assert(false);
            // shouldn't be here
            return 0;
    }
}

typedef struct{
  string term;
  float expected;
  float result;
}Test;

// tests ob zwei Zahlen gleich sind, hat dabei aber eine gewisse Toleranz
bool test(float expected, float actual){
  printf("expected: %f actual: %f\n", actual, expected);
  float diff = expected < actual ? actual-expected : expected-actual;
  return diff < 0.001;
}

int main(void)
{
    SExpression *e = NULL;
    vector<Test> tests;
    // simple int Tests
    tests.push_back({"4/4", 1.0, 0});
    tests.push_back({"4-4", 0.0, 0});
    tests.push_back({"4*4", 16, 0});
    tests.push_back({"4+4", 8.0, 0});
    // simple float Tests
    tests.push_back({"3.0/4.0", 0.75, 0});
    tests.push_back({"4.5-3.0", 1.5, 0});
    tests.push_back({"2.2*1.5", 3.3, 0});
    tests.push_back({"2.2+1.5", 3.7, 0});
    tests.push_back({"12", 12.0, 0});
    tests.push_back({"a := 15; a;", 15.0, 0});

    //komplexe Tests
    tests.push_back({"4 + 2 * 10 - 3 / ( 5 + 1 )", 23.5});

    printf("\033[1;33mTesting Build\033[0m (%d Tests)..\n", tests.size());
    int testsFailed = 0;
    for(int i = 0; i<tests.size();i++){
      e = getAST(tests[i].term.c_str());
      tests[i].result = evaluate(e);
      if(!test(tests[i].expected, tests[i].result)){
        testsFailed++;
        printf("Test %d \033[1;31mfailed\033[0m '%s' is %f but was expected to be %f.\n", i+1, tests[i].term.c_str(), tests[i].result, tests[i].expected);
      }else
        printf("Test %d \033[1;32mpassed\033[0m\n", i);
      deleteExpression(e);
    }
    printf("%f%% passed. %lu passed, %d failed.\n", ((float)tests.size()-testsFailed)*100/tests.size(), (tests.size()-testsFailed), testsFailed);
    return 0;
}
