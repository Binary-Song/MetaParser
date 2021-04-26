/// @file 

#include "input-resolver.hpp"
#include <iostream>
#include <stdlib.h>
int main()
{
    // 创建解析器对象
    InputFileResolver resolver;
    // 通过解析文件，得到语言的所有规则。包括词法规则和语法规则
    Rules rules;
    // 错误代码，0为成功
    int errcode = resolver.resolve_input_file(__FILE__ "/../input.txt", rules);
    if (errcode)
    {
        // 若有错误，就输出错误消息，退出程序
        std::cout << resolver.full_diag_msg << std::endl;
        return 1;
    }

    // 下面全部都是输出

    std::cout << "Input file successfully loaded:\n";
    std::cout << "id=0, symbol name= " << resolver.symbol_name(0) << "\n";
    // 输出所有词法规则
    std::cout << "Lexer rules:\n";

    for (auto &&r : rules.lexer_rules)
    {
        std::cout << "id= " << r.token_id << ", terminal symbol name= " << resolver.symbol_name(r.token_id) << "\n";
    }

    // 输出所有语法规则
    std::cout << "Parser rules:\n";
    // 先输出非终结符定义
    for (auto &&r : rules.parser_rules)
    {
        std::cout << "id=" << r.left << ", non-terminal symbol name=" << resolver.symbol_name(r.left) << "\n";
    }
    // 再输出产生式
    for (auto &&r : rules.parser_rules)
    {
        // resolver.symbol_name函数可以根据id取名字
        std::cout << resolver.symbol_name(r.left) << " -> ";
        for (auto &&rr : r.right)
        {
            std::cout << " " << resolver.symbol_name(rr);
        }
        std::cout << "\n";
    }
    getchar();
}
