/// @file 

#include <input-resolver.hpp>
#include <fstream>
#include <regex>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <assert.h>

#define LERR_ILL_DECL 101
#define LERR_CANT_COMPILE_REGEX 102
#define LERR_DUP_DECL 103
#define PERR_ILL_DECL 201
#define PERR_DUP_DECL 203
#define PERR_UNDEFINED 204

std::set<std::string> left_names;

InputFileResolver::InputFileResolver()
    : _lex_rule_decl_pattern{R"(([a-zA-Z0-9_]+)\s+:=\s+([^\s]+)\s*)", std::regex_constants::optimize},
      _separator_pattern{"===\\s*", std::regex_constants::optimize},
      _empty_line_pattern("\\s*", std::regex_constants::optimize),
      _parse_rule_decl_pattern(R"(([a-zA-Z0-9_]+)\s+->((\s+[a-zA-Z0-9_/]+)+)\s*)", std::regex_constants::optimize) {}

int InputFileResolver::resolve_input_file(const char *file_name, Rules &result)
{
    // 确保本函数只能被调用1次
    if (used == false)
        used = true;
    else
        throw "This function can only be used once for each object!";

    // 提前定义好“空“为正斜杠
    symbol_name_to_id["/"] = nil_id;
    symbol_id_to_name[nil_id] = "/";

    // 再定义文件结尾EOF
    symbol_name_to_id["EOF"] = eof_id;
    symbol_id_to_name[eof_id] = "EOF";

    // 准备读文件
    std::ifstream file{file_name};
    int errs = 0;
    std::string line;

    // 读取每一行到line中，行号为line_no
    for (int pass = 1; pass <= 2; pass++)
    {
        bool handling_lexer_rule = true;
        file.clear();
        file.seekg(0);
        for (unsigned long line_no = 1; std::getline(file, line); line_no++)
        {
            // 跳过空行
            if (std::regex_match(line, _empty_line_pattern))
            {
                continue;
            }
            // 分隔符“===”能切换词法区和语法区
            if (std::regex_match(line, _separator_pattern))
            {
                handling_lexer_rule = !handling_lexer_rule;
                continue;
            }

            int error_code = 0;
            if (handling_lexer_rule)
            {
                if (pass == 1)
                    error_code = _handle_lexer_rule(line, result);
            }
            else
            {
                if (pass == 1)
                    error_code = _handle_parser_rule_first_pass(line, result);
                else
                    error_code = _handle_parser_rule_second_pass(line, result);
            }
            // 生成诊断信息
            if (error_code)
            {
                full_diag_msg += "error " + std::to_string(error_code) + ": " + _diag_msg_reason + " @  " + std::string(file_name) + ":" + std::to_string(line_no) + "\n";
                errs++;
            }
        }
    }
    return errs;
}

int InputFileResolver::_handle_lexer_rule(std::string const &line, Rules &result)
{
    // 检查规则定义是否满足格式
    std::smatch matches;
    if (std::regex_match(line, matches, _lex_rule_decl_pattern) == false)
    {
        _diag_msg_reason = "illegal declaration";
        return LERR_ILL_DECL;
    }

    // left,  right 对应词法规则的左部和右部：
    // number  :=    [0-9]+
    // left          right
    std::string left{matches[1].first, matches[1].second};
    std::string right{matches[2].first, matches[2].second};

    // 检查左部无重复
    if (symbol_name_to_id.count(left))
    {
        _diag_msg_reason = "duplicate declaration of symbol `" + left + "`";
        return LERR_DUP_DECL;
    }

    // 自动增加的id
    symbol_id id = -((symbol_id)result.lexer_rules.size() + 2);

    // 构造新的词法规则
    LexerRule new_rule;
    new_rule.token_id = id;
    try
    {
        new_rule.reg = std::regex{right}; // 尝试编译regex
    }
    catch (...)
    {
        _diag_msg_reason = "regular expression `" + right + "` cannot be compiled";
        return LERR_CANT_COMPILE_REGEX;
    }

    // 登记新的规则
    result.lexer_rules.push_back(std::move(new_rule));
    symbol_name_to_id[left] = id;
    symbol_id_to_name[id] = left;

    return 0;
}

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

int InputFileResolver::_handle_parser_rule_first_pass(std::string const &line, Rules &result)
{
    // 检查规则定义是否满足格式
    std::smatch matches;
    if (std::regex_match(line, matches, _parse_rule_decl_pattern) == false)
    {
        _diag_msg_reason = "illegal declaration";
        return LERR_ILL_DECL;
    }

    std::string left(matches[1].first, matches[1].second);

    if (!symbol_name_to_id.count(left))
    {
        symbol_id id = (symbol_id)symbol_name_to_id.size() + 1;
        symbol_name_to_id[left] = id;
        symbol_id_to_name[id] = left;
    }

    return 0;
}

int InputFileResolver::_handle_parser_rule_second_pass(std::string const &line, Rules &result)
{
    // 检查规则定义是否满足格式
    std::smatch matches;
    if (std::regex_match(line, matches, _parse_rule_decl_pattern) == false)
    {
        _diag_msg_reason = "illegal declaration";
        return LERR_ILL_DECL;
    }

    std::string left_str(matches[1].first, matches[1].second);
    std::string right_str(matches[2].first, matches[2].second);

    auto right_words = split_into_words(right_str);

    ParserRule new_rule;
    new_rule.left = symbol_name_to_id[left_str];
    for (auto &&right_word : right_words)
    {
        if (symbol_name_to_id.count(right_word) == 0)
        {
            _diag_msg_reason = "undefined symbol `" + right_word + "`";
            return PERR_UNDEFINED;
        }
        new_rule.right.push_back(symbol_name_to_id[right_word]);
    }

    result.parser_rules.push_back(new_rule);

    // log

    return 0;
}
