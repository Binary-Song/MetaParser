.. _api_input:

输入文件解析器
==============

输入按一定格式编写的规则文件，输出规则对象。

输入文件包括多行，每一行都可以是词法定义或语法定义。词法定义形式如下：

*终结符名* := *正则表达式*

例如：

    number := [0-9]+

注意：

- 终结符的名称只能由字母、数字和下划线构成
- 正则表达式中不能含有空格，如要匹配空格，请使用\\s

语法定义形式如下：

*非终结符名* => *产生式右部*

产生式右部是由空格或tab分隔的终结符和非终结符名组成的序列。

例如：

    add := [+]

    mult := [*]

    E -> T add T

    T -> F mult F

    F -> number 

可以在产生式右侧用/表示空（ε）：

    A -> /

第一条产生式的左部为开始符号。

注意：

- 非终结符的名称只能由字母、数字和下划线构成
- 所有非终结符都必须至少出现在产生式左侧1次，否则将产生“符号未定义”的错误


.. doxygenfile:: input-resolver.hpp