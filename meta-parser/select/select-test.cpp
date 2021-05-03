#include <iostream>
#include <fstream>
#include <sstream>
 

#include <input-resolver.hpp>
#include <meta-lexer.hpp>

#include <shared.hpp>
#include <meta-parser.hpp>

#include <debug/display.hpp>

InputFileResolver resolver;
Rules rules;

int main()
{
    const char rules_file_path[] = PROJECT_ROOT "/data/rules1.txt";

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
    parser.empty_derivings[rules.to_id("S")] = 1;
    parser.empty_derivings[rules.to_id("A")] = 1;
    parser.empty_derivings[rules.to_id("B")] = 1;
    parser.empty_derivings[rules.to_id("C")] = 0;
    parser.empty_derivings[rules.to_id("D")] = 0;
    parser.empty_derivings[rules.to_id("$eof")] = 0;
    parser.empty_derivings[rules.to_id("/")] = 1;
    parser.empty_derivings[rules.to_id("a")] = 0;
    parser.empty_derivings[rules.to_id("b")] = 0;
    parser.empty_derivings[rules.to_id("c")] = 0;
    parser.empty_derivings[rules.to_id("d")] = 0;

    // 手动赋值first_set
    parser.first_set[rules.to_id("S")] = {rules.to_id("b"), rules.to_id("a"), rules.to_id("/")};
    parser.first_set[rules.to_id("A")] = {rules.to_id("b"), rules.to_id("/")};
    parser.first_set[rules.to_id("B")] = {rules.to_id("a"), rules.to_id("/")};
    parser.first_set[rules.to_id("C")] = {rules.to_id("a"), rules.to_id("b"), rules.to_id("c")};
    parser.first_set[rules.to_id("D")] = {rules.to_id("a"), rules.to_id("c")};
    parser.first_set[rules.to_id("$eof")] = {rules.to_id("$eof")};
    parser.first_set[rules.to_id("/")] = {rules.to_id("/")};
    parser.first_set[rules.to_id("a")] = {rules.to_id("a")};
    parser.first_set[rules.to_id("b")] = {rules.to_id("b")};
    parser.first_set[rules.to_id("c")] = {rules.to_id("c")};
    parser.first_set[rules.to_id("d")] = {rules.to_id("d")};

    parser.compute_follow_set();
    parser.compute_select_set();

    print_select_set(parser);
    return 0;
}