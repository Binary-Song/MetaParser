#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <meta-parser.hpp>
#include <TextTable.h>
#include <sstream>
inline void print_select_set(Parser const &parser)
{
    for (int i = 0; i < parser.select_set.size(); i++)
    {
        std::cout << "select(";

        // 输出产生式

        // resolver.symbol_name函数可以根据id取名字
        std::cout << parser.rules.to_name(parser.rules.parser_rules[i].left) << " -> ";
        for (auto &&rr : parser.rules.parser_rules[i].right)
        {
            std::cout << " " << parser.rules.to_name(rr);
        }

        std::cout << ")= ";

        for (auto &&elem : parser.select_set[i])
        {
            std::cout << " " << parser.rules.to_name(elem) << " ";
        }

        std::cout << "\n";
    }
}

inline std::string production_to_string(Parser const &parser, int production_index)
{
    if (production_index < 0)
        return "e#" + std::to_string(-production_index);

    auto &&production = parser.rules.parser_rules[production_index];

    std::stringstream ss;
    ss << parser.rules.to_name(production.left);
    ss << "->";
    for (auto right_sym : production.right)
    {
        ss << parser.rules.to_name(right_sym);
        ss << " ";
    }

    return ss.str();
}

inline void print_predict_table(Parser const &parser)
{
    TextTable tbl('-', '|', '+');
    // 第一行
    tbl.add("");
    for (symbol_id i = -parser.rules.terminal_count; i <= -1; i++)
    {
        tbl.add(parser.rules.to_name(i));
    }
    tbl.endOfRow();
    // 第一行结束
    for (symbol_id nt = 1; nt <= parser.rules.non_terminal_count; nt++)
    {
        // 之后的每一行
        tbl.add(parser.rules.to_name(nt));
        for (symbol_id t = -parser.rules.terminal_count; t < 0; t++)
        {
            std::string prod_str = production_to_string(parser, parser.prdtbl.at(nt, t));
            tbl.add(prod_str);
        }
        tbl.endOfRow();
    }
    tbl.setAlignment(2, TextTable::Alignment::RIGHT);
    std::cout << tbl;
}

inline void _print_tree_node_recur(Parser const &parser,
                                   std::string const &code,
                                   std::vector<Token> const &tokens,
                                   SyntaxTree::Node const &node,
                                   std::string indent, bool last)
{
    std::cout << indent;
    if (last)
    {
        std::cout << "\\--";
        indent += "  ";
    }
    else
    {
        std::cout << "|--";
        indent += "| ";
    }
    // 结点内容
    std::cout << parser.rules.to_name(node.id);
    if (is_termin(node.id))
    {
        std::string token_str(
            code.begin() + tokens[node.token_index].begin,
            code.begin() + tokens[node.token_index].end);
        std::cout << " (`" << token_str << "`) ";
    }
    std::cout << std::endl;

    auto &&children = node.get_children();
    for (int i = 0; i < children.size(); i++)
    {
        _print_tree_node_recur(parser, code, tokens, *children[i], indent, i == children.size() - 1);
    }
}

inline void print_syntax_tree(Parser const &parser,
                              std::string const &code,
                              std::vector<Token> const &tokens,
                              SyntaxTree const &ast)
{
    _print_tree_node_recur(parser, code, tokens, *(ast.root), "", true);
}

#endif // _DISPLAY_H_