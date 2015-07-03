#ifndef GLOBALS_H
#define GLOBALS_H

#include "llvm/Analysis/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

#include <map>
#include <string>

using namespace std;
using namespace llvm;

//not static!!! extern as a quick hack to fix llvm::Module visibility
extern map <string, Value*> namedValues;
extern map <string, float> fvars;
extern Module* llvmModule;
extern IRBuilder<> llvmBuilder;


#endif // GLOBALS_H
