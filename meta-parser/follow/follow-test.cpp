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
    // 解析文件，写入rules对象
    int errcode = resolver.resolve_input_file(__FILE__ "/../rules1.txt", rules);
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

    for (auto &&pair : parser.follow_set)
    {
        auto &&id = pair.first;
        auto &&set = pair.second;

        std::cout << "follow set of " << rules.symbol_id_to_name[id] << ":\n";
        for (auto &&elem : set)
        {
            std::cout << "    " << rules.symbol_id_to_name[elem] << "\n";
        }
    }
    return 0;
}