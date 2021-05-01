/// @file

#include <input-resolver.hpp>
#include <fstream>
#include <regex>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <assert.h>

// 错误代码定义

#define LERR_ILL_DECL 101
#define LERR_CANT_COMPILE_REGEX 102
#define LERR_DUP_DECL 103
#define PERR_ILL_DECL 201
#define PERR_DUP_DECL 203
#define PERR_UNDEFINED 204
#define ERR_ILL_DECL 501
#define ERR_MISSING_START 502
std::set<std::string> left_names;

InputFileResolver::InputFileResolver()
    : _lex_rule_decl_pattern{R"(([a-zA-Z0-9_]+)\s+:=\s+([^\s]+)\s*)", std::regex_constants::optimize},
      _separator_pattern{"===\\s*", std::regex_constants::optimize},
      _empty_line_pattern("\\s*", std::regex_constants::optimize),
      _parse_rule_decl_pattern(R"(([a-zA-Z0-9_]+)\s+->((\s+[a-zA-Z0-9_/]+)+)\s*)", std::regex_constants::optimize) {}

int InputFileResolver::resolve_input_file(const char *file_name, Rules &result)
{
    this->out_rules = &result;

    // 确保本函数只能被调用1次
    if (used == false)
        used = true;
    else
        throw "This function can only be called once for each object!";

    // 提前定义好“空“为正斜杠
    link_id_with_name(0, "/");

    // 再定义文件结尾EOF
    add_terminal("$eof");

    // 准备读文件
    std::ifstream file{file_name};

    std::string line;

    // 读取每一行到line中，行号为line_no
    for (int pass = 1; pass <= 2; pass++)
    {
        // 本行的模式
        // 0: 词法定义
        // 1: 语法定义
        // 2: 错误
        int mode = 0;
        file.clear();
        file.seekg(0);
        for (unsigned long line_no = 1; std::getline(file, line); line_no++)
        {
            // 跳过空行
            if (std::regex_match(line, _empty_line_pattern))
                continue;

            std::smatch matches;
            if (std::regex_match(line, matches, this->_lex_rule_decl_pattern))
                mode = 0;
            else if (std::regex_match(line, matches, this->_parse_rule_decl_pattern))
                mode = 1;
            else
                mode = 2;

            int error_code = 0;
            if (mode == 0)
            {
                if (pass == 1)
                    error_code = handle_lexer_rule(line, matches);
            }
            else if (mode == 1)
            {
                if (pass == 1)
                    error_code = handle_parser_rule_first_pass(line, matches);
                else
                    error_code = handle_parser_rule_second_pass(line, matches);
            }
            else
            {
                error_code = ERR_ILL_DECL;
                _diag_msg_reason = "illegal declaration";
            }
            // 生成诊断信息
            if (error_code)
            {
                append_local_error(error_code, file_name, line_no, _diag_msg_reason);
            }
        }
    }

    if (non_terminal_count == 0)
    {
        append_global_error(ERR_MISSING_START, file_name, "starting symbol undefined");
    }

    if (errs)
    {
        result = Rules();
    }
    else
    {
        result.non_terminal_count = non_terminal_count;
        result.terminal_count = terminal_count;
        result.symbol_id_to_name = symbol_id_to_name;
        result.symbol_name_to_id = symbol_name_to_id;
    }
    return errs;
}

int InputFileResolver::handle_lexer_rule(std::string const &line, std::smatch &matches)
{

    // “left”, “right” 对应词法规则的左部和右部：
    // number  :=    [0-9]+
    //   👆“left”       👆“right”
    std::string left(matches[1].first, matches[1].second);
    std::string right(matches[2].first, matches[2].second);

    // 检查左部无重复
    if (symbol_name_to_id.count(left))
    {
        _diag_msg_reason = "duplicate declaration of symbol `" + left + "`";
        return LERR_DUP_DECL;
    }
    // 新的词法规则
    symbol_id id = add_terminal(left);
    if (add_lexer_rule(id, right))
    {
        _diag_msg_reason = "regular expression `" + right + "` cannot be compiled";
        return LERR_CANT_COMPILE_REGEX;
    }
    return 0;
}

/// 将字符串按空格分割成单词序列
inline std::vector<std::string> split_into_words(std::string const &str)
{
    std::vector<std::string> v;
    std::stringstream stream(str);
    std::string word;
    while (stream >> word)
    {
        v.push_back(word);
    }
    return v;
}

/// 第一趟扫描词法规则：声明非终结符
int InputFileResolver::handle_parser_rule_first_pass(std::string const &line, std::smatch &matches)
{
    std::string left(matches[1].first, matches[1].second);
    if (!symbol_name_to_id.count(left))
        add_non_terminal(left);
    return 0;
}

/// 第一趟扫描词法规则：添加规则
int InputFileResolver::handle_parser_rule_second_pass(std::string const &line, std::smatch &matches)
{
    std::string left_str(matches[1].first, matches[1].second);
    std::string right_str(matches[2].first, matches[2].second);
    return add_parser_rule(left_str, split_into_words(right_str));
}

/// 将错误信息写到 @ref diag_msg 末尾。并给 @ref errs 加1
void InputFileResolver::append_local_error(int errcode, std::string const &file_name, int line_no, std::string const &reason)
{
    diag_msg += "error " + std::to_string(errcode) + ": " + reason + " @  " + std::string(file_name) + ":" + std::to_string(line_no) + "\n";
    errs++;
}

void InputFileResolver::append_global_error(int code, std::string const &file_name, std::string const &reason)
{
    diag_msg += "error " + std::to_string(code) + ": " + reason + " @  " + std::string(file_name) + "\n";
    errs++;
}

/// 在map中添加name和id的双向引用
void InputFileResolver::link_id_with_name(symbol_id id, std::string const &name)
{
    assert(symbol_id_to_name.count(id) == 0 && symbol_name_to_id.count(name) == 0);
    symbol_id_to_name[id] = name;
    symbol_name_to_id[name] = id;
}

/// 在map中添加name和id的双向引用。id是自动设置的（-1,-2,-3,...）
symbol_id InputFileResolver::add_terminal(std::string const &name)
{
    terminal_count++;
    link_id_with_name(-terminal_count, name);
    return -terminal_count;
}

/// 在map中添加name和id的双向引用。id是自动设置的（1,2,3,...）
symbol_id InputFileResolver::add_non_terminal(std::string const &name)
{
    non_terminal_count++;
    link_id_with_name(non_terminal_count, name);
    return non_terminal_count;
}

/// 根据id和regex构造词法规则，将其添加到out_rules中。返回是否出错(0表示成功)。
int InputFileResolver::add_lexer_rule(symbol_id id, std::string &regex)
{
    try
    {
        out_rules->lexer_rules.push_back(LexerRule(id, regex));
    }
    catch (...)
    {
        return 1;
    }
    return 0;
}

/// 根据产生式的左部和右部，构造语法规则，添加到out_rules中。如果有错，将错写入_diag_msg_reason，返回错误代码。
int InputFileResolver::add_parser_rule(std::string const &left, std::vector<std::string> const &right_words)
{
    ParserRule new_rule;
    new_rule.left = symbol_name_to_id[left];
    for (auto &&right_word : right_words)
    {
        if (symbol_name_to_id.count(right_word) == 0)
        {
            _diag_msg_reason = "undefined symbol `" + right_word + "`";
            return PERR_UNDEFINED;
        }
        new_rule.right.push_back(symbol_name_to_id[right_word]);
    }

    out_rules->parser_rules.push_back(new_rule);
    return 0;
}