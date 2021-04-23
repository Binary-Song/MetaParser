#include <string>
#include <vector>
#include <map>
#include <regex>
#include <set>
#include <stdint.h>

using symbol_id = std::int32_t;

struct LexerRule
{
    symbol_id token_id;
    std::regex reg;

    LexerRule(symbol_id id = 0) : token_id(id) {}
    LexerRule(symbol_id id, std::string const &pattern) : token_id(id), reg(pattern) {}
};

struct ParserRule
{
    symbol_id left;
    std::vector<symbol_id> right;
};

struct Rules
{
    std::vector<LexerRule> lexer_rules;
    std::vector<ParserRule> parser_rules;
};

class InputFileResolver
{
    bool used = false;

    std::vector<std::string> terminal_names;
    std::vector<std::string> nonterminal_names;
    std::set<std::string> terminal_name_set;
    std::set<std::string> nonterminal_name_set;

    std::regex lex_rule_decl_pattern;
    std::regex lex_rule_parse_rule_separator_pattern;
    std::regex empty_line_pattern;
    std::regex parse_rule_decl_pattern;

public:
    /// 诊断信息，由read_input负责写入。
    std::string diag_msg;

    /// 构造涉及正则表达式的编译，较耗时，应避免
    /// 拷贝本类对象。
    InputFileResolver();

    /// 读取输入文件，输出各种规则。如果发生错误，
    /// 返回错误数量（0为成功），诊断信息存放在diag_msg中。
    /// 每个对象只能调用一次。
    int read_input(const char *file_name, Rules &result);

    /// 返回符号的名称。
    std::string &symbol_name(symbol_id sid)
    {
        if (is_terminal(sid))
            return terminal_names[sid];
        return nonterminal_names[-sid];
    }

    /// 返回符号是否为终结符，true：终结符，false：非终结符
    bool is_terminal(symbol_id sid)
    {
        return sid > 0;
    }

private:
    int _handle_lexer_rule(std::string const &line, Rules &result);
    int _handle_parser_rule(std::string const &line, Rules &result);
};
