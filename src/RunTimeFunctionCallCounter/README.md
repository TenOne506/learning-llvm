
进入`build`文件夹,执行如下命令
1. build
```
cmake -G Ninja -DLLVM_ENABLE_PROJECTS=clang ../llvm

ninja
```
2. execute
```
./bin/opt -load-pass-plugin=lib/RunTimeFunctionCallCounter.so -passes="runtime-function-call-counter" ../llvm/lib/Transforms/RunTimeFunctionCallCounter/Test.ll -o Test.bin
```