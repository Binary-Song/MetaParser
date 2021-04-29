#ifndef _RULES_H_
#define _RULES_H_

#include <string>
#include <vector>
#include <regex>
#include <map>

#include <stdint.h>

/// 符号（分为终结符或非终结符）的id是一个32位整数,
/// 正的id表示非终结符(non-terminal)
/// 负的id表示终结符(token或terminal)
/// id为0表示空。
using symbol_id = std::int32_t;

/// 返回符号是否为终结符
inline bool is_termin(symbol_id sid)
{
    return sid < 0;
}

/// 返回符号是否为非终结符
inline bool is_non_termin(symbol_id sid)
{
    return sid > 0;
}

/// 空的id
static const symbol_id nil_id = 0;

/// 文件结尾的id
static const symbol_id eof_id = -1;

/// 词法规则。包含正则表达式和对应的终结符
struct LexerRule
{
    /// 终结符id
    symbol_id token_id;
    /// 正则
    std::regex reg;

    /// 指定id，构造词法规则
    LexerRule(symbol_id id = 0) : token_id(id) {}

    /// 指定id和正则表达式，构造词法规则。涉及将regex字符串编译为自动机，耗时慎用。
    LexerRule(symbol_id id, std::string const &pattern) : token_id(id), reg(pattern) {}
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
    
    /// 符号的id到符号名称的映射，便于通过id查找符号名。id为k的符号，名称为 @ref symbol_id_to_name [k]。
    std::map<symbol_id, std::string> symbol_id_to_name;

    /// 符号的名称到符号id的映射，便于通过符号名查找id，id为 @ref symbol_name_to_id [k]。
    std::map<std::string, symbol_id> symbol_name_to_id;

    /// 终结符数量
    int terminal_count = -1;

    /// 非终结符数量
    int non_terminal_count = -1;
};
#endif // _RULES_H_