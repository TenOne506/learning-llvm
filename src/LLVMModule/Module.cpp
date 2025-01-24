/// hellomodule.cpp

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <llvm/Support/raw_ostream.h>

int main() {
  llvm::LLVMContext context;
  llvm::Module *module = new llvm::Module("helloModule", context);
  module->print(llvm::outs(), nullptr);
  return 0;
}