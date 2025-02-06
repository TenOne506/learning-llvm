
进入`build`文件夹,执行如下命令
1. build
```
cmake -G Ninja -DLLVM_ENABLE_PROJECTS=clang ../llvm

ninja
```
2. execute
```
./bin/opt -load-pass-plugin=lib/MyPass.so -passes="my-pass" -disable-output ../llvm/lib/Transforms/MyPass/test.ll
```