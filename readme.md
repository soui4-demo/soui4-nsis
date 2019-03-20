编译方法：
1、在环境变量中增加SOUI4LIB，指向soui4的代码路径。
2、编译setup\setupdll下的vs工程
3、将编译生成的setupdll.dll复制到setup\setupmain\plugins路径下，替换原有版本。
4、用nsis编译setup\setupmain\demo.nsi，正常情况将生成setup.exe，运行即可看到效果。

启程软件 2023-9-15
