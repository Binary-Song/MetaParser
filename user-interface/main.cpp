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

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        std::cout << "USAGE: parse <rule_file> <code_file>" << std::endl;
        return 1;
    }

    const char *rules_file_path = argv[1];
    const char *code_file_path = argv[2];

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
    int rc = parser.generate_parser();
    print_select_set(parser);
    if (rc)
    {
        std::cout << "error: not a ll1 grammar." << std::endl;
        return 1;
    }
    print_predict_table(parser);

    // 开始编译

    if (!std::ifstream(code_file_path))
    {
        std::cout << "error: cannot open code file @ " << code_file_path << std::endl;
        return 1;
    }

    Lexer lexer(rules.lexer_rules);
    std::vector<Token> tokens;

    std::string code = read_file_into_string(code_file_path);
    rc = lexer.analyze(code, tokens);
    assert(rc == 0);
    SyntaxTree ast;
    Parser::AnalyzeResult r;
    rc = parser.analyze(code_file_path, code, tokens, ast, r);
    if (rc)
    {
        std::cout << r.msg;
        return rc;
    }
    print_syntax_tree(parser, code, tokens, ast);
    return 0;
}
