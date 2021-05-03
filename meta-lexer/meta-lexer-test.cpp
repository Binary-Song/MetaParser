/// @file

#include <iostream>
#include <meta-lexer.hpp>
#include <input-resolver.hpp>
#include <fstream>
#include <sstream>
#include <shared.hpp>
 

int main()
{
    const char rules_file_path[] = PROJECT_ROOT "/data/rules3.txt";
    const char code_file_path[] = PROJECT_ROOT "/data/code3.txt";


    // 创建解析器对象
    InputFileResolver resolver;
    // 通过解析文件，得到语言的所有规则。包括词法规则和语法规则
    Rules rules;
    // 错误代码，0为成功
    int errcode = resolver.resolve_input_file(rules_file_path, rules);
    if (errcode)
    {
        // 若有错误，就输出错误消息，退出程序
        std::cout << resolver.diag_msg << std::endl;
        return 1;
    }
    // 创建词法分析器
    Lexer lexer(rules.lexer_rules);
    // 读取文件到字符串
    std::string test_code = read_file_into_string(code_file_path);
    // 词法分析
    std::vector<Token> tokens;
    errcode = lexer.analyze(test_code, tokens);
    if (errcode)
    {
        std::cout << lexer.diag_msg;
    }
    // 下面都是输出
    for (auto &&token : tokens)
    {
        std::string token_content(
            test_code.begin() + token.begin, // token.begin 是单词的开始在test_code中的下标（从0开始）
            test_code.begin() + token.end);  // token.end 是单词的结尾后一个字符test_code中的下标（从0开始）
        std::cout << token_content << ": " << resolver.symbol_name(token.id) << std::endl;
    }
    return 0;
}
