
进入`build`文件夹,执行如下命令
1. build
```
cmake -G Ninja -DLLVM_ENABLE_PROJECTS=clang ../llvm

ninja
```
2. compiler
```
clang -S -emit-llvm ../llvm/lib/Transforms/SimpleModulePass/Test.c -o ../llvm/lib/Transforms/SimpleModulePass/Test.ll
```
3. execute
```
./bin/opt -load-pass-plugin=lib/CompileTimeFunctionCallCounter.so -passes="compile-time-function-call-counter" -disable-output ../llvm/lib/Transforms/CompileTimeFunctionCallCounter/Test.ll

```