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

static map <string, Value*> namedValues;
static Module* llvmModule;
static IRBuilder<> llvmBuilder(getGlobalContext());


#endif // GLOBALS_H
