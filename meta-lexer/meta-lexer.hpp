#ifndef _META_LEXER_H_
#define _META_LEXER_H_

#include <rules.hpp>

/// 单词。所有单词都属于一个终结符。每个终结符表示一类的单词。
/// 如：number := [0-9]+ 为终结符
/// 123 为单词。
struct Token
{
public:
    /// 单词对应的终结符id
    symbol_id id;
    /// 单词的开始位置
    size_t begin;
    /// 单词的结束位置（属于单词的最后一个字符的后1个位置）
    size_t end;

    Token(symbol_id id = 0, size_t begin = 0, size_t end = 0)
        : id(id), begin(begin), end(end) {}
};      
      
/// 词法分析器
class Lexer
{

    std::vector<LexerRule> const &rules;

public:
    /// 是否添加文件终结符
    bool add_eof = false;
    /// 诊断信息，由analyze写入
    std::string diag_msg;

    /// 给定语言的词法规则，构造词法分析器
    Lexer(std::vector<LexerRule> const &rules)
        : rules(rules) {}

    /// 进行词法分析，输出单词序列（tokens），返回错误代码。0表示成功，其他值表示失败
    int analyze(std::string const &str, std::vector<Token> &tokens);
};
#endif // _META-LEXER_H_