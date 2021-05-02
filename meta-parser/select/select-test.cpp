#include <iostream>
#include <fstream>
#include <sstream>

#include <input-resolver.hpp>
#include <meta-lexer.hpp>

#include <shared.hpp>
#include <meta-parser.hpp>

InputFileResolver resolver;
Rules rules;

symbol_id get_id(std::string s)
{
    return rules.symbol_name_to_id[s];
}

int main()
{
    const char rules_file_path[] = ROOT_DIR "/data/rules1.txt"; 

    // 解析文件，写入rules对象
    int errcode = resolver.resolve_input_file(rules_file_path, rules);
    if (errcode)
    {
        std::cout << resolver.diag_msg << std::endl;
        return 1;
    }

    // 初始化语法分析器。
    // rules是语言的全部规则
    // false表示不需要在构造时求empty_derivings、first_set等集合。
    Parser parser(rules, false);

    // 手动赋值empty_derivings
    parser.empty_derivings[get_id("S")] = 1;
    parser.empty_derivings[get_id("A")] = 1;
    parser.empty_derivings[get_id("B")] = 1;
    parser.empty_derivings[get_id("C")] = 0;
    parser.empty_derivings[get_id("D")] = 0;
    parser.empty_derivings[get_id("$eof")] = 0;
    parser.empty_derivings[get_id("/")] = 1;
    parser.empty_derivings[get_id("a")] = 0;
    parser.empty_derivings[get_id("b")] = 0;
    parser.empty_derivings[get_id("c")] = 0;
    parser.empty_derivings[get_id("d")] = 0;

    // 手动赋值first_set
    parser.first_set[get_id("S")] = {get_id("b"), get_id("a"), get_id("/")};
    parser.first_set[get_id("A")] = {get_id("b"), get_id("/")};
    parser.first_set[get_id("B")] = {get_id("a"), get_id("/")};
    parser.first_set[get_id("C")] = {get_id("a"), get_id("b"), get_id("c")};
    parser.first_set[get_id("D")] = {get_id("a"), get_id("c")};
    parser.first_set[get_id("$eof")] = {get_id("$eof")};
    parser.first_set[get_id("/")] = {get_id("/")};
    parser.first_set[get_id("a")] = {get_id("a")};
    parser.first_set[get_id("b")] = {get_id("b")};
    parser.first_set[get_id("c")] = {get_id("c")};
    parser.first_set[get_id("d")] = {get_id("d")};

    parser.compute_follow_set();
    parser.compute_select_set();

    for (int i = 0; i < parser.select_set.size(); i++)
    {
        std::cout << "select(";

        // 输出产生式

        // resolver.symbol_name函数可以根据id取名字
        std::cout << resolver.symbol_name(rules.parser_rules[i].left) << " -> ";
        for (auto &&rr : rules.parser_rules[i].right)
        {
            std::cout << " " << resolver.symbol_name(rr);
        }

        std::cout << ")= ";

        for (auto &&elem : parser.select_set[i])
        {
            std::cout << " " << rules.to_name(elem) << " ";
        }

        std::cout << "\n";
    }
    return 0;
}