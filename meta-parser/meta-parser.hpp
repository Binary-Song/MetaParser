#ifndef _META_PARSER_H_
#define _META_PARSER_H_

#include <rules.hpp>
#include <meta-lexer.hpp>
#include <set>
class Parser
{
public:
    std::vector<ParserRule> const &rules;

    /// 给定语言的语法规则，初始化语法分析器。初始化包括4步：
    /// 1. 计算能推导出空的非终结符
    /// 2. 计算first集
    /// 3. 计算follow集
    /// 4. 计算select集
    Parser(std::vector<ParserRule> const &rules)
        : rules(rules)
    {
        _compute_empty_deriving_symbols();
        _compute_first_set();
        _compute_follow_set();
        _compute_select_set();
    }

private:
    /// 计算能推导出空的非终结符，计算结果存放在_empty_derivings中。
    /// TODO: 实现本函数
    void _compute_empty_deriving_symbols();

    /// _compute_empty_deriving_symbols 的结果。假设非终结符N的id为i，则 _empty_derivings[i] 表示该N是否能经过一步或多步推导出空。0表示不能，1表示能。对于整数x，若x不是某个非终结符的id，则_empty_derivings[x]的值无意义。
    std::vector<std::int8_t> _empty_derivings;

    /// 计算FIRST集合，计算结果存放在first_set中。
    /// TODO: 实现本函数
    void _compute_first_set();

    /// _compute_first_set 的结果。假设符号X的id为i，则 first_set[abs(i)] 是X的FIRST集（abs为取绝对值）。
    /// X既可以是终结符也可以是非终结符。终结符的首符号集就是自己。
    std::vector<std::set<symbol_id>> first_set;

    /// 计算follow集合，计算结果存放在follow_set中。
    /// TODO: 实现本函数
    void _compute_follow_set();

    /// _compute_select_set 的结果。假设非终结符N的id为i，则 follow_set[i] 是N的FOLLOW集。
    std::vector<std::set<symbol_id>> follow_set;

    /// 计算select集合，计算结果存放在select_set中。
    /// TODO: 实现本函数
    void _compute_select_set();

    /// _compute_follow_set 的结果。对于产生式rules[i]，则 select_set[i] 是rules[i]的SELECT集。
    std::vector<std::set<symbol_id>> select_set;
};

#endif // _META-PARSER_H_