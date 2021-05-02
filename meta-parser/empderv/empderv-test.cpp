#include <meta-parser.hpp>
#include <input-resolver.hpp>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <queue>
#include <algorithm>
#include <string>
#include <fstream>
#include <sstream>
#include <meta-lexer.hpp>
#include <shared.hpp>

using namespace std;

InputFileResolver resolver;
Rules rules;

int main()
{
    // 解析文件，写入rules对象
    int errcode = resolver.resolve_input_file(__FILE__ "/../input1.txt", rules);
    if (errcode)
    {
        std::cout << resolver.diag_msg << std::endl;
        return 1;
    }
    // 输出每个符号的id和名称
    for (auto &&pair : resolver.symbol_id_to_name)
    {
        auto id = pair.first;
        auto name = pair.second;
        std::cout << id << name << std::endl;
    }

    cout << endl;
    // 初始化语法分析器。
    // rules是语言的全部规则
    // false表示不需要在构造时求empty_derivings、first_set等集合。
    Parser parser(rules, false);

    parser.compute_empty_deriving_symbols();

    for (auto &&pair : parser.empty_derivings)
    {
        std::cout << rules.to_name(pair.first) << " " << (int)pair.second << '\n';
    }

    return 0;
}