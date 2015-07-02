/*
 * Expression.h
 * Definition of the structure used to build the syntax tree.
 */
#ifndef __EXPRESSION_H__
#define __EXPRESSION_H__

#include <string>
#include <iostream>

#include "llvm/Analysis/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "globals.h"
using namespace std;
using namespace llvm;


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
    virtual Value* codeGen() = 0;
    Expression* left;
    Expression* right;
    Expression(Expression* _left, Expression* _right, string _type):left(_left), right(_right), type(_type){}
    virtual string getType(){return type;}
};

class FloatExpression : public Expression{
    public:
        float value;
        FloatExpression(float _value):Expression(NULL, NULL,"Float"), value(_value){}
        virtual Value* codeGen(){ return ConstantFP::get(getGlobalContext(), APFloat(value)); }
};


class IdentifierExpression : public Expression{
    public:
        string name;
        IdentifierExpression(string _name):Expression(NULL, NULL,"Identifier"), name(_name){}
        virtual Value* codeGen(){
            Value* v = namedValues[name];
            if(!v){
                fprintf(stderr, "Error: Variable %s not declared", name.c_str());
                return NULL;
            }
            return v;
        }
};


class DeklarationExpression : public Expression{
    public:
        virtual ~DeklarationExpression(){delete(left, right);}
        DeklarationExpression(Expression* _left, Expression* _right):Expression(_left, _right, "Deklaration"){}
        virtual Value* codeGen(){
            IdentifierExpression* id = dynamic_cast<IdentifierExpression*>(left);
            if(id){
                Value* value = right->codeGen();
                namedValues[id->name] = value;
                return value;
            }
            else
                assert(false);
        }
};

class LineExpression : public Expression{
    public:
        virtual ~LineExpression(){delete(left, right);}
        LineExpression(Expression* _actualLine, Expression* _otherLines):Expression(_actualLine, _otherLines, "Line"){}
        virtual Value* codeGen(){
            Value* l = left->codeGen();
            Value* r = right->codeGen();
            return (r==NULL)?l:r;
        }
};


class IntegerExpression : public Expression{
    public:
        float value;
        IntegerExpression(int _value):Expression(NULL, NULL, "Integer"), value((float)_value){}
        virtual Value* codeGen(){ return ConstantFP::get(getGlobalContext(), APFloat(value)); }
};


class BiOperationExpression : public Expression{
    public:
        EOperationType opType;
        ~BiOperationExpression(){delete(type, left, right);}
        BiOperationExpression(EOperationType _opType, Expression* _left, Expression* _right):Expression(_left, _right, "BiOperation"),  opType(_opType){}
        virtual Value* codeGen(){
            Value* lValue = left->codeGen();
            Value* rValue = right->codeGen();

            if(lValue == 0 && rValue == 0)
                return ConstantFP::get(getGlobalContext(), APFloat(0.0f));
            switch(opType){
                    case ePLUS: return llvmBuilder.CreateFAdd(lValue, rValue, "addtmp");
                    case eMINUS: return llvmBuilder.CreateFSub(lValue, rValue, "subtmp");
                    case eDIVIDE: return llvmBuilder.CreateFDiv(lValue, rValue, "divtmp");
                    case eMULTIPLY: return llvmBuilder.CreateFMul(lValue, rValue, "multmp");
                    default: return 0;
                }
        }
};

class EOFExpression : public Expression {
public:
    EOFExpression():Expression(NULL, NULL, "EOF"){}
    virtual Value* codeGen(){return NULL;}
};

class PrintExpression : public Expression{
    public:
        ~PrintExpression(){delete(left);delete(right);}
        PrintExpression(Expression* _param):Expression(_param, NULL, "Print"){}
        virtual Value* codeGen(){}
};


class FunctionExpression : public Expression{
    public:
        FunctionExpression(Expression* left, Expression* right):Expression(left, right, "Function"){}
        virtual llvm::Function* codeGen(){
            std::vector<Type*> args(0, Type::getFloatTy(getGlobalContext()));
            FunctionType* functionType = FunctionType::get(Type::getFloatTy(getGlobalContext()), args, false);
            IdentifierExpression* id = dynamic_cast<IdentifierExpression*>(left);
            if(id){
                Function* function = Function::Create(functionType, Function::ExternalLinkage, id->name, llvmModule);
                if (function->getName() != id->name) {
                  function->eraseFromParent();
                  function = llvmModule->getFunction(id->name);
                  if(!function->empty()){
                      return 0;
                  }
                }
                   BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", function);
                   llvmBuilder.SetInsertPoint(block);
                   if(Value* retVal = right->codeGen()){
                       llvmBuilder.CreateRet(retVal);
                       verifyFunction(*function);
                       function->dump();
                       return function;
                   }
				   //error reading body, remove function
                   function->eraseFromParent();
                   return 0;
            }
        }
};

class mainFuncExpression : public Expression{
	public:
		mainFuncExpression(Expression* right):Expression(NULL, right, "mainFunc"){}
		virtual llvm::Function* codeGen(){
            std::vector<Type*> args(0, Type::getFloatTy(getGlobalContext()));
            FunctionType* functionType = FunctionType::get(Type::getFloatTy(getGlobalContext()), args, false);
			
			Function* mainFunction = llvm::Function::Create(functionType, llvm::Function::ExternalLinkage, "main", llvmModule);
            BasicBlock* block = BasicBlock::Create(getGlobalContext(), "entry", mainFunction);
            llvmBuilder.SetInsertPoint(block);
			
            if(Value* retVal = right->codeGen()){
                llvmBuilder.CreateRet(retVal);
                verifyFunction(*mainFunction);
                mainFunction->dump();
                return mainFunction;
            }
			//error reading body, remove function
            mainFunction->eraseFromParent();
            return 0;
		}
};

class FunctionCallExpression : public Expression{ //stub
	public:
		FunctionCallExpression(Expression* left, Expression* right):Expression(left, right, "FunctionCall"){}
};
#endif // __EXPRESSION_H__
