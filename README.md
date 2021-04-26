#  MetaParser

元编译器

## 特性

- 输入正则表达式，生成词法分析器

- 输入产生式，生成LL(1)语法分析器

- 显示预测表

## 模块说明

### shared

共享模块，别的模块都可以依赖它。

### input-resolver

输入文件，输出词法规则和语法规则

### meta-lexer

输入词法规则，生成词法分析器

### meta-parser

输入语法规则，生成语法分析器

### driver

使用生成的词法和语法分析器，编译文件，返回其是否该语言的句子。

**开发者请参见本目录中的DEV_TUTORIAL.md和INTERFACING.md**