#ifndef _META_PARSER_H_
#define _META_PARSER_H_

#include <rules.hpp>
#include <meta-lexer.hpp>
#include <set>
#include <map>

/// 语法分析器
class Parser
{
public:
    /// 定义语言的各种规则
    Rules const &rules;

    /// 给定语言的语法规则，初始化语法分析器。
    /// @param rules
    /// 定义语言的各种规则。
    /// @param do_ll1_computations
    /// 若为true，将进行LL(1)文法的判别和预测分析表的生成。若为false，将不会进行任何计算。
    /// 如果需要适合手动输入first,follow和select等集合。请将它设置为false。
    /// @note @ref InputFileResolver 可以读取文件并输出规则，以提供给@ref Parser 的构造函数。
    Parser(Rules const &rules, bool do_ll1_computations = true)
        : rules(rules)
    {
        if (do_ll1_computations)
        {
            compute_empty_deriving_symbols();
            compute_first_set();
            compute_follow_set();
            compute_select_set();
        }
    }

    /// 计算能推导出空的非终结符，计算结果存放在 @ref empty_derivings 中。
    /// TODO: 实现本函数
    void compute_empty_deriving_symbols();

    /// @ref compute_empty_deriving_symbols 的结果。假设非终结符N的id为i，则 @ref empty_derivings [i] 表示N是否能经过一步或多步推导出空。0表示不能，1表示能。
    /// @warning 规定终结符 **不能** 推出空； @ref nil_id "ε" **能** 推出空。
    std::map<symbol_id, std::int8_t> empty_derivings;

    /// 计算FIRST集合，计算结果存放在 @ref first_set 中。
    /// TODO: 实现本函数
    void compute_first_set();

    /// @ref compute_first_set 的结果。假设符号X的id为i，则 @ref first_set [i] 是X的FIRST集。
    /// X既可以是终结符也可以是非终结符。终结符的FIRST集合就是自己。
    /// @warning 规定 @ref nil_id "ε" 的FIRST集合是 {ε}。
    std::map<symbol_id, std::set<symbol_id>> first_set;

    /// 计算follow集合，计算结果存放在 @ref follow_set 中。
    void compute_follow_set();

    /// @ref compute_select_set 的结果。假设非终结符N的id为i，则 @ref follow_set [i] 是N的FOLLOW集。
    std::map<symbol_id, std::set<symbol_id>> follow_set;

    /// 计算select集合，计算结果存放在 @ref select_set 中。
    void compute_select_set();

    ///  @ref compute_follow_set 的结果。对于产生式  @ref rules [i]， @ref  select_set [i] 是  @ref  rules [i]的SELECT集。
    std::vector<std::set<symbol_id>> select_set;

    /// 返回一个符号串α的FIRST集合。此函数调用的前提是 @ref first_set 已经被算出。本函数已经在follow.cpp实现。
    /// @param begin 指向符号串α的第一个符号的迭代器
    /// @param end 指向符号串α的最后一个符号再下一个符号的迭代器
    std::set<symbol_id> get_sequence_first_set(
        std::vector<symbol_id>::const_iterator begin,
        std::vector<symbol_id>::const_iterator end);

    ///求符号串α是否能=>*ε。此函数调用的前提是 @ref empty_derivings 已经被算出。本函数已经在follow.cpp实现。
    /// @param begin 指向符号串α的第一个符号的迭代器
    /// @param end 指向符号串α的最后一个符号再下一个符号的迭代器
    bool is_empty_deriving_sequence(
        std::vector<symbol_id>::const_iterator begin,
        std::vector<symbol_id>::const_iterator end);
};

#endif // _META-PARSER_H_