#ifndef _RULES_H_
#define _RULES_H_

#include <string>
#include <vector>
#include <regex>
#include <map>

#include <stdint.h>
#include <symbol.hpp>

/// 词法规则。包含正则表达式和对应的终结符
struct LexerRule
{
    /// 终结符id
    symbol_id token_id;
    /// 正则表达式
    std::regex reg;
    /// 是否在词法分析时只匹配，不添加token。一般用作空格和注释
    bool skipped;

    /// 指定id，构造词法规则
    LexerRule(symbol_id id = 0) : token_id(id) {}

    /// 指定id和正则表达式，构造词法规则。涉及将regex字符串编译为自动机，耗时慎用。
    LexerRule(symbol_id id, std::string const &pattern, bool skipped = false)
        : token_id(id), reg(pattern), skipped(skipped) {}
};

/// 语法规则。包含产生式的左部和右部。
struct ParserRule
{
    /// 产生式的左部的符号id，只支持1个左部符号
    symbol_id left;
    /// 产生式的右部的符号id，可以是多个终结符或非终结符。
    std::vector<symbol_id> right;
};

/// 语言的规则
struct Rules
{
    /// 词法规则集
    std::vector<LexerRule> lexer_rules;
    /// 语法规则集
    std::vector<ParserRule> parser_rules;

    /// 符号的id到符号名称的映射，便于通过id查找符号名。id为ID的符号，其名称为 @ref symbol_id_to_name [ID]。
    std::map<symbol_id, std::string> symbol_id_to_name;

    /// 符号的名称到符号id的映射，便于通过符号名查找id，名称为name的符号，其id为 @ref symbol_name_to_id [name]。
    std::map<std::string, symbol_id> symbol_name_to_id;

    /// 终结符数量
    int terminal_count = 0;

    /// 非终结符数量
    int non_terminal_count = 0;

    std::string to_name(symbol_id id) const
    {
        return symbol_id_to_name.at(id);
    }

    symbol_id to_id(std::string const &name) const
    {
        return symbol_name_to_id.at(name);
    }
};
#endif // _RULES_H_