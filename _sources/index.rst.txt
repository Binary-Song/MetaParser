欢迎阅读 MetaParser 项目API文档
============================

.. toctree::
   :maxdepth: 4
   :caption: 目录:


目录
^^^^^^^^^^^^^^^^^
   
.. toctree::
   :maxdepth: 2

   self
   api/index

2021.5.1 更新日志
^^^^^^^^^^^^^^^

目前求FOLLOW集的函数我已经写完，然而gitee上还没有看到大家的贡献记录。凭我一人之力无法完成整个项目，也无法代大家完成答辩，所以还请大家加快进度。

为了帮助大家完成各自的任务，我再次丰富完善了本文档和代码注释，并更新了语法分析器类的API，使之包含更多必要的信息来求各种集合。

更新对eof的处理
#############

eof是一个自动定义的 **终结符** 。输入解析器在解析规则文件时，会自动定义符号eof的id为-1，其名称为 ``$eof`` （为了避免与用户定义的符号名冲突，特地在前面加了一个 ``$``  ）。另一个默认定义的符号是ε，id为0，名称为 ``/``。

词法分析器在将代码输入转化成单词序列时，也会在末尾添加一个 eof 。

之后提到终结符时，都包括eof。这个符号一般不需要特殊的处理，把它当作正常的终结符即可。 

举个例子，假设用户给出的规则文件包含3个终结符t1,t2,t3的定义和3个非终结符N1,N2,N3的定义，那么最终的符号id-符号名称的对应关系为：

== == 
id 名称
-- --
-4 ``t3``
-3 ``t2``
-2 ``t1``
-1 ``$eof`` 
0  ``/`` 
1  ``N1``
2  ``N2``
3  ``N3``
== ==

此时，InputResolver输出的Rules对象的terminal_count为4，non_terminal_count为3（见下文）。

Parser类的rules成员的类型改为Rules，并在Rules中添加成员
################################################

Parser类的rules成员原本的类型为std::vector<ParserRule>，即一个产生式的列表。然而Parser只有此信息不方便遍历终结符和非终结符。因此，我将rules的类型改为Rules，并在Rules类中添加了几个成员。现在，Rules类型包含以下几个数据成员：

.. code-block:: cpp

   /// 词法规则集
   std::vector<LexerRule> lexer_rules;
   /// 语法规则集
   std::vector<ParserRule> parser_rules;

   /// 符号的id到符号名称的映射，便于通过id查找符号名。id为ID的符号，其名称为 @ref symbol_id_to_name [ID]。
   std::map<symbol_id, std::string> symbol_id_to_name;

   /// 符号的名称到符号id的映射，便于通过符号名查找id，名称为name的符号，其id为 @ref symbol_name_to_id [name]。
   std::map<std::string, symbol_id> symbol_name_to_id;

   /// 终结符数量（包括eof）
   int terminal_count;

   /// 非终结符数量
   int non_terminal_count;

此外，还加入了两个方便的成员函数，便于id和name的互转。

.. code-block:: cpp
 
   std::string to_name(symbol_id id); // id 转 name 

   symbol_id to_id(std::string const &name; // name 转 id

这样，若按id从小到大的顺序遍历所有符号，只需：

.. code-block:: cpp

   for(auto&& pair : rules.symbol_id_to_name) // 关于 c++11 的 `range-based for 循环` 和 `auto自动推断类型`，请参看百度。
   {
      symbol_id id = pair.first; // 符号的id
      std::string name = pair.second; // 符号的名称
      ...
   }

即可。
 
若要遍历所有终结符（包括eof），可以用以下代码：

.. code-block:: cpp

   for(int i = - rules.terminal_count ; i <= -1; i++) // i ∈ [-terminal_count, -1]
   {
      symbol_id id = i; // 符号的id
      std::string name = rules.to_name(id); // 符号的名称
   }

若要遍历所有非终结符，可以用以下代码：

.. code-block:: cpp

   for(int i = 1 ; i <= rules.non_terminal_count ; i++) // i ∈ [1, non_terminal_count]
   {
      symbol_id id = i; // 符号的id
      std::string name = rules.to_name(id); // 符号的名称
   }


更新对empty_derivings和first_set的定义
###################################

为了加速后续处理，对empty_derivings和first_set进行了合理的拓展定义。现在定义 **每一个符号，包括终结符、非终结符和ε，都要定义empty_derivings和first_set** ，具体的定义可以参看“语法分析器”栏目，这里简述如下：

针对empty_derivings：

   - 规定：终结符 **不能** 推出空（若 i<0 则 empty_derivings[i]=0）； ε **能** 推出空（empty_derivings[0]=1）。
   - 非终结符的定义不变。

针对first_set：

   - 规定： ε 的FIRST集合是拥有一个元素的集合 {ε}（first_set[0]={0}）。
   - 终结符 t 的FIRST集合是 {t} （若 i<0 则 first_set[i]={i}）。
   - 非终结符 t 的FIRST集合定义不变。

