#include <io.hpp>
#include <fstream>
#include <regex>
#include <iostream>
#include <algorithm>

#define LERR_ILL_DECL 101
#define LERR_CANT_COMPILE_REGEX 102
#define LERR_DUP_DECL 103
#define PERR_ILL_DECL 201

std::set<std::string> left_names;

InputFileResolver::InputFileResolver()
    : lex_rule_decl_pattern{"([a-zA-Z0-9_]+)\\s+(:=)\\s+(.+)", std::regex_constants::optimize},
      lex_rule_parse_rule_separator_pattern{"===\\s*", std::regex_constants::optimize},
      empty_line_pattern("\\s*", std::regex_constants::optimize),
      parse_rule_decl_pattern("([a-zA-Z0-9_]+)\\s+->(\\s+[a-zA-Z0-9_]+)+") {}

int InputFileResolver::read_input(const char *file_name, Rules &result)
{
    // 确保本函数只能被调用1次
    if (used == false)
        used = true;
    else
        throw "This function can only be used once for each object!";

    std::ifstream file{file_name};
    int errs = 0;
    std::string line;
    bool handling_lexer_rule = true;
    // 读取每一行到line中，行号为line_no
    for (unsigned long line_no = 1; std::getline(file, line); line_no++)
    {
        // 跳过空行
        if (std::regex_match(line, empty_line_pattern))
        {
            continue;
        }
        // 分隔符“===”能切换词法区和语法区
        if (std::regex_match(line, lex_rule_parse_rule_separator_pattern))
        {
            handling_lexer_rule = !handling_lexer_rule;
        }

        int error_code;
        if (handling_lexer_rule)
            error_code = _handle_lexer_rule(line, result);
        else
            error_code = _handle_parser_rule(line, result);
        // 生成诊断信息
        diag_msg = "error " + std::to_string(error_code) + ": " + diag_msg + " @  " + std::string(file_name) + ":" + std::to_string(line_no);

        if (error_code)
            errs++;
    }
    return errs;
}

int InputFileResolver::_handle_lexer_rule(std::string const &line, Rules &result)
{
    // 检查规则定义是否满足格式
    std::smatch matches;
    if (std::regex_match(line, matches, lex_rule_decl_pattern) == false)
    {
        diag_msg = "illegal declaration";
        return LERR_ILL_DECL;
    }

    // left,  right 对应词法规则的左部和右部：
    // number  :=    [0-9]+
    // left          right
    std::string left{matches[1].first, matches[1].second};
    std::string right{matches[3].first, matches[3].second};

    // 检查左部无重复
    if (terminal_name_set.count(left))
    {
        diag_msg = "duplicate declaration of token `" + left + "`";
        return LERR_DUP_DECL;
    }

    // 自动增加的id
    symbol_id id = -((symbol_id)result.lexer_rules.size() + 1);

    // 构造新的词法规则
    LexerRule new_rule;
    new_rule.token_id = id;
    try
    {
        new_rule.reg = std::regex{right}; // 尝试编译regex
    }
    catch (...)
    {
        diag_msg = "regular expression `" + right + "` cannot be compiled";
        return LERR_CANT_COMPILE_REGEX;
    }

    // 登记新的规则
    result.lexer_rules.push_back(std::move(new_rule));
    terminal_name_set.insert(left);
    terminal_names.push_back(left);
    return 0;
}

int InputFileResolver::_handle_parser_rule(std::string const &line, Rules &result)
{
    // 检查规则定义是否满足格式
    std::smatch matches;
    if (std::regex_match(line, matches, parse_rule_decl_pattern) == false)
    {
        diag_msg = "illegal declaration";
        return LERR_ILL_DECL;
    }
    for (auto &&m : matches)
    {
        std::string s(m.first, m.second);
        std::cout << s << std::endl;
    }
    return 0;
}

int main()
{
    InputFileResolver res;
    Rules rules;
    int errcode;
    if (errcode = res.read_input("D:/Projects/CompilerExperiments/parser/input.txt", rules))
    {
        std::cout << res.diag_msg << std::endl;
    }
}