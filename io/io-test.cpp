#include "io.hpp"
#include <iostream>
int main()
{
    // 创建解析器对象
    InputFileResolver resolver;
    // 通过解析文件，得到语言的所有规则。包括词法规则和语法规则
    Rules rules;
    // 错误代码，0为成功
    int errcode = resolver.read_input(__FILE__ "/../input.txt", rules);
    if (errcode)
    {
        // 若有错误，就输出错误消息，退出程序
        std::cout << resolver.full_diag_msg << std::endl;
        return 1;
    }

    // 下面全部都是输出

    std::cout << "Input file successfully loaded:\n";

    // 输出所有词法规则
    std::cout << "Lexer rules:\n";

    for (auto &&r : rules.lexer_rules)
    {
        std::cout << "id= " << r.token_id << ", terminal symbol name= " << resolver.symbol_name(r.token_id) << "\n";
    }

    // 输出所有语法规则
    std::cout << "Parser rules:\n";
    for (auto &&r : rules.parser_rules)
    {
        std::cout << "id=" << r.left << ", non-terminal symbol name=" << resolver.symbol_name(r.left) << "\n";
    }
    for (auto &&r : rules.parser_rules)
    {
        std::cout << resolver.symbol_name(r.left) << " -> ";
        for (auto &&rr : r.right)
        {
            std::cout << " " << resolver.symbol_name(rr);
        }
        std::cout << "\n";
    }
}
