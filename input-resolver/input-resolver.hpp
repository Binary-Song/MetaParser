#ifndef _IO_H_
#define _IO_H_

#include <string>
#include <vector>
#include <map>
#include <regex>
#include <set>
#include <stdint.h>
#include "shared.hpp"

class InputFileResolver
{
public:
    /// 是否被使用过了
    bool used = false;

    /// 诊断信息，由resolve_input_file负责写入。
    std::string full_diag_msg;

    /// 构造涉及正则表达式的编译，较耗时，应避免拷贝本类对象。
    InputFileResolver();

    /// 读取输入文件，输出各种规则(放在result中)。如果发生错误，返回错误数量（0为成功），诊断信息存放在full_diag_msg中。每个对象只能调用一次。
    int resolve_input_file(const char *file_name, Rules &result);

    /// 返回符号的名称。
    std::string &symbol_name(symbol_id sid)
    {
        return _symbol_id_to_name[sid];
    }

private:
    std::string _diag_msg_reason;

    std::map<symbol_id, std::string> _symbol_id_to_name;
    std::map<std::string, symbol_id> _symbol_name_to_id;
    std::regex _lex_rule_decl_pattern;
    std::regex _separator_pattern;
    std::regex _empty_line_pattern;
    std::regex _parse_rule_decl_pattern;

    int _handle_lexer_rule(std::string const &line, Rules &result);
    int _handle_parser_rule_first_pass(std::string const &line, Rules &result);
    int _handle_parser_rule_second_pass(std::string const &line, Rules &result);
};

#endif // _IO_H_