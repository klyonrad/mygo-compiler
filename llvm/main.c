/*
 * main.c file
 */

#include "llvm/Analysis/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include "Expression.h"
#include "Parser.h"
#include "Lexer.h"

#include "globals.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <assert.h>
#include <stdlib.h>
#include <map>
#include <iostream>
#include <iomanip>

using namespace std;
using namespace llvm;

int yyparse(Expression **expression, yyscan_t scanner);

extern "C"
double putchard(double X) {
  putchar((char)X);
  return 0;
}

Expression *getAST(const char *expr)
{
    Expression *expression;
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

float calculate(Expression *e) {
   IntegerExpression* integer = dynamic_cast<IntegerExpression*>(e);
   if(integer)
       return integer->value;

   FloatExpression* decimal = dynamic_cast<FloatExpression*>(e);
   if(decimal)
       return decimal->value;

   BiOperationExpression* op = dynamic_cast<BiOperationExpression*>(e);
   if(op){
        switch(op->opType){
            case eMULTIPLY:
                return calculate(e->left) * calculate(e->right);
            case ePLUS:
                return calculate(e->left) + calculate(e->right);
            case eDIVIDE:
                return calculate(e->left) / calculate(e->right);
            case eMINUS:
                return calculate(e->left) - calculate(e->right);
            default:
                assert(false);
        }
   }

   IdentifierExpression* id = dynamic_cast<IdentifierExpression*>(e);
   if(id)
        return fvars[id->name];

   assert(false);
   return 0;
}

float evaluate(Expression *e)
{



    LineExpression* line = dynamic_cast<LineExpression*>(e);
    if(line){
        float v1 = evaluate(e->left);
        float v2 = evaluate(e->right);
        return (v2==0)?v1:v2;
    }

    IntegerExpression* integer = dynamic_cast<IntegerExpression*>(e);
    if(integer)
        return integer->value;

    FloatExpression* decimal = dynamic_cast<FloatExpression*>(e);
    if(decimal)
        return decimal->value;

    BiOperationExpression* op = dynamic_cast<BiOperationExpression*>(e);
    if(op){
         switch(op->opType){
             case eMULTIPLY:
                 return calculate(e->left) * calculate(e->right);
             case ePLUS:
                 return calculate(e->left) + calculate(e->right);
             case eDIVIDE:
                 return calculate(e->left) / calculate(e->right);
             case eMINUS:
                 return calculate(e->left) - calculate(e->right);
             default:
                 assert(false);
         }
    }

    IdentifierExpression* id = dynamic_cast<IdentifierExpression*>(e);
    if(id)
         return fvars[id->name];

    DeklarationExpression* dek = dynamic_cast<DeklarationExpression*>(e);
    if(dek){

        IdentifierExpression* id = dynamic_cast<IdentifierExpression*>(dek->left);
        if(id){
            string varName = id->name;
            fvars[varName] = evaluate(dek->right);
            return fvars[varName];
        } else
            assert(false);
        }

    PrintExpression* print = dynamic_cast<PrintExpression*>(e);
    if(print)
        cout << calculate(e->left) << endl;

    EOFExpression* eof = dynamic_cast<EOFExpression*>(e);
    if(eof)
        return 0;


    assert(false);
    return 0;
}

void printTree (Expression *p, int indent=0) //copied from http://stackoverflow.com/a/13484997/1796645
{
    if(p != NULL) {
        if(p->left) printTree(p->left, indent+4);
        if(p->right) printTree(p->right, indent+4);
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }

        IdentifierExpression* id = dynamic_cast<IdentifierExpression*>(p);
        if(id){
            cout << id->name << endl;
            return;
        }
        IntegerExpression* integer = dynamic_cast<IntegerExpression*>(p);
        if(integer){
            cout << integer->value << endl;
            return;
        }
        FloatExpression* decimal = dynamic_cast<FloatExpression*>(p);
        if(decimal){
            cout << decimal->value << endl;
            return;
        }
        cout << p->getType() << endl;
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

    //LLVMContext& context = getGlobalContext();
	//bison debug option:
    yydebug = 0;
    LLVMContext &context = getGlobalContext();
    llvmModule = new Module("Moule of awsomeness", context);
	
    Expression *e = NULL;
    vector<Test> tests;
	
    fvars["b"] = 10.0;

	tests = {
        // simple int Tests

        /*
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
        {"a := 15;", 15.0, 0},
        {"a := ( 7 + 8 ); a;", 15.0, 0},
        {"a := 15; c:= (a + 5); c;", 20.0, 0},
		//komplexe Tests
        {"vier := 4.5; fuenf := 4.5; vier + fuenf;", 9.0, 0},
        {"4 + ( 2 * 10 ) - ( 3 / ( 5 + 1 ) );", 23.5, 0},*/
        {"test(){4*5};"}
        //{"vier := 4; fuenf := 5; vier + ( 2 * 10 ) - ( 3 / ( fuenf + 1 ) );", 23.5, 0},
        //{"println(5);", 0, 0}
	};

    printf("\033[1;33mTesting Build\033[0m (%d Tests)..\n", tests.size());
    int testsFailed = 0;
    for(int i = 0; i<tests.size();i++){
      e = getAST(tests[i].term.c_str());
      e->codeGen();
      if ( e != NULL ) {


      tests[i].result = evaluate(e);
      printTree(e); cout << endl;
      if(!test(tests[i].expected, tests[i].result, tests[i].term) ){
        testsFailed++;
        printf("Test %d \033[1;31mfailed\033[0m '%s'" /*is %f but was expected to be %f.\n"*/, i+1, tests[i].term.c_str()/*, tests[i].result, tests[i].expected*/ );
        cout << " is " << tests[i].result << " but was expected to be " << tests[i].expected << " ." << endl;
      }else
        printf("Test %d \033[1;32mpassed\033[0m\n", i);
        //deleteExpression(e);
      }
      else {
          printf("Test %d \033[1;31m syntax error\033[0m", i+1);
          cout << endl;
      }
      llvmModule->dump();
    }
    printf("%f%% passed. %lu passed, %d failed.\n", ((float)tests.size()-testsFailed)*100/tests.size(), (tests.size()-testsFailed), testsFailed);
    return 0;
}
