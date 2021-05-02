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

#define RULE_FILE_PATH ROOT_DIR "/data/rules1.txt"

int main()
{

    const char rules_file_path[] = RULE_FILE_PATH; 
 
    // 解析文件，写入rules对象
    int errcode = resolver.resolve_input_file(rules_file_path, rules);
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
        std::cout << id << ":" << name << std::endl;
    }

    // 初始化语法分析器。
    // rules是语言的全部规则
    // false表示不需要在构造时求empty_derivings、first_set等集合。
    Parser parser(rules);
      
    return 0;
}