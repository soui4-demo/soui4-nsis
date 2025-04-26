# 编译方法：
## 编译setup\setupdll下的vs工程，由于集成的soui5是使用vs2008编译的mt库，setupdll也只能使用vs2008编译。使用其它版本VS需要自己编译soui5
## 将编译生成的setupdll.dll复制到setup\setupmain\plugins路径下，替换原有版本。
## 用nsis编译setup\setupmain\demo.nsi，正常情况将生成setup.exe，运行即可看到效果。

# 启程软件 2025-4-26
