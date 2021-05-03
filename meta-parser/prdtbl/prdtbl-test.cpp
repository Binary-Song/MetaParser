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

#define RULE_FILE_PATH PROJECT_ROOT "/data/rules3.txt"
#define CODE_FILE_PATH PROJECT_ROOT "/data/code3.txt"

class ParserTest
{
public:
    int test()
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
        for (auto &&pair : rules.symbol_id_to_name)
        {
            auto id = pair.first;
            auto name = pair.second;
            std::cout << id << ":" << name << std::endl;
        }

        // 初始化语法分析器。
        // rules是语言的全部规则
        // false表示不需要在构造时求empty_derivings、first_set等集合。

        Parser parser(rules, false);
        int rc = parser.generate_parser();
        print_select_set(parser);
        if (rc)
        {
            std::cout << "not a ll1" << std::endl;
        }
        print_predict_table(parser);

        // 开始编译
        Lexer lexer(rules.lexer_rules);
        std::vector<Token> tokens;
        std::string code = read_file_into_string(CODE_FILE_PATH);
        rc = lexer.analyze(code, tokens);
        assert(rc == 0);
        SyntaxTree ast;
        Parser::AnalyzeResult r;
        rc = parser.analyze(CODE_FILE_PATH, code, tokens, ast, r);
        if (rc)
        {
            std::cout << r.msg;
            return rc;
        }
        print_syntax_tree(parser,code,tokens, ast);
        return 0;
    }
};

int main()
{

    return ParserTest().test();
}