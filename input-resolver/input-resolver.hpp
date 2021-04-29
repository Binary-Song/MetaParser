#ifndef _IO_H_
#define _IO_H_

#include <string>
#include <vector>
#include <map>
#include <regex>
#include <set>
#include <stdint.h>
#include <assert.h>

#include <rules.hpp>

/// 输入文件解析器。 @ref InputFileResolver 能将以规定格式写成的文件解析成 @ref Rules 对象。调用 @ref resolve_input_file 来解析文件，每个对象只能调用一次这个函数。
class InputFileResolver
{
public:
    /// 指示这个对象是否调用过 @ref resolve_input_file 了。若调用过则不能再次调用。
    bool used = false;

    /// 诊断信息，由 @ref resolve_input_file 负责写入。
    std::string full_diag_msg;

    /// 构造涉及正则表达式的编译，较耗时，应谨慎拷贝本类对象。
    InputFileResolver();

    /// 读取输入文件，输出各种规则。如果发生错误，返回错误数量（0为成功），诊断信息存放在full_diag_msg中。每个对象只能调用一次。
    /// @param file_name 输入文件的路径
    /// @param[out] rules 输出的规则集
    /// @return 错误数量（0为成功）
    int resolve_input_file(const char *file_name, Rules &rules);

    /// 返回符号的名称。
    /// @param sid 符号的id
    /// @return 符号的名称
    std::string &symbol_name(symbol_id sid)
    {
        return symbol_id_to_name[sid];
    }

    /// 终结符数量
    int terminal_count = 0;

    /// 非终结符数量
    int non_terminal_count = 0;

    /// 符号的id到符号名称的映射。id为k的符号，名称为 @ref symbol_id_to_name [k]。
    std::map<symbol_id, std::string> symbol_id_to_name;

    /// 符号的名称到符号id的映射。名称为k的符号，id为 @ref symbol_name_to_id [k]。
    std::map<std::string, symbol_id> symbol_name_to_id;

private:
    Rules *out_rules;

    std::string _diag_msg_reason;
    std::regex _lex_rule_decl_pattern;
    std::regex _separator_pattern;
    std::regex _empty_line_pattern;
    std::regex _parse_rule_decl_pattern;

    void link_id_with_name(symbol_id id, std::string const &name);

    symbol_id add_terminal(std::string const &name);

    symbol_id add_non_terminal(std::string const &name);

    int add_lexer_rule(symbol_id id, std::string &regex);

    int add_parser_rule(std::string const& left, std::vector<std::string> const& right_words);

    // void add_parser_rule(std::string const &name);

    int handle_lexer_rule(std::string const &line, std::smatch &matches);
    int handle_parser_rule_first_pass(std::string const &line, std::smatch &matches);
    int handle_parser_rule_second_pass(std::string const &line, std::smatch &matches);
};

#endif // _IO_H_