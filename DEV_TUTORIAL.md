
## 环境

**git和vscode**

安装git和vscode，安装时记得勾选以下选项或类似选项（如果有）

- 将"git bash here"加入右键菜单（context menu）
- 将git加入PATH环境变量。
- 将"用Code打开"加入右键菜单（context menu） 

其他选项默认即可。


**CMake**

安装时**务必勾选以下内容**

![](images/2021-04-24-11-59-00.png)

 
**Visual Studio**

有C++模块即可

**VSCode插件：CMake Tools**

![](images/2021-04-24-12-01-37.png)
 
## 打开项目

安装完后选择一个文件夹，右键git bash here。

输入

```
git clone https://gitee.com/Binary-Song/MetaParser.git
```

会出现一个文件夹MetaParser，右键该文件夹，选择“通过VSCode打开”

![](images/2021-04-24-12-12-02.png)

 
## 配置和编译

先配置项目才可以编译。

ctrl+shift+P 输入quick start，选择编译器
![](images/2021-04-24-12-17-01.png)

选择 amd64

点击![](images/2021-04-24-12-18-21.png)按钮配置


点击模块右边的按钮编译。

![](images/2021-04-24-12-20-18.png)

右键,run in terminal，运行（不debug）。

![](images/2021-04-24-12-22-41.png)

运行结果在下方

![](images/2021-04-24-12-23-59.png)

## 项目结构

每个模块都有自己的文件夹，文件夹名即是模块名，开发者只需编辑自己的文件夹即可。

如果需要增加头文件（.hpp），直接在模块文件夹中增加。如果需要增加源文件（.cpp），修改在模块文件夹中的CMakeList.txt的第一行:

```
add_library(模块名 源文件1 源文件2 ...)
```

如果需要增加依赖的模块，修改CMakeList.txt的第三行

```
target_link_libraries(模块名 PUBLIC 引用的模块1 引用的模块2 ... )
```

之后就可以在自己的模块中写：

```
#include <引用的模块中的头文件>
```

来引用其他模块了。

测试文件写在 模块名-test.cpp中。不要将main函数写在除此之外的其它地方。

## 运行测试的方法

方法1：

右键测试对象，Debug

![](images/2021-04-24-11-39-26.png)

缺点：无法输入

方法2：

1. 切换到 VC++ CMake debug

![](images/2021-04-24-11-42-07.png)


2. 设置为Launch Target

![](images/2021-04-24-11-48-12.png)

3. 按F5即可弹出黑窗口