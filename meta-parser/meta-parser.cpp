#include <meta-parser.hpp>
#include <stack>
#include <cassert>
#include <sstream>
Parser::Parser(Rules const &rules, bool do_analyze)
    : rules(rules), prdtbl(rules.non_terminal_count, rules.terminal_count)
{
    if (do_analyze)
    {
        int error = generate_parser();
        if (error)
        {
            throw Exception(error, "Error occurred when calling Parser::generate_parser! See comments on Parser::generate_parser for explaination of error codes.");
        }
    }
}

int Parser::generate_parser()
{
    compute_empty_deriving_symbols();
    compute_first_set();
    compute_follow_set();
    compute_select_set();
    if (generate_predict_table())
    {
        return 1;
    }
    return 0;
}

struct BuildTreeContext
{
    std::vector<ParserRule> const &productions;
    PredictTable const &predict_table;
    std::vector<Token> const &token_stream;

    BuildTreeContext(std::vector<ParserRule> const &productions,
                     PredictTable const &predict_table,
                     std::vector<Token> const &token_stream)
        : productions(productions), predict_table(predict_table), token_stream(token_stream)
    {
    }
};

/// 递归下降构建抽象语法树。返回错误代码：0成功，其余为错误码。
int build_tree_recur(BuildTreeContext const &context,
                     std::vector<Token>::const_iterator &token_stream_ptr,
                     SyntaxTree::Node *node)
{
    if (is_termin(node->id))
    {
        assert(token_stream_ptr != context.token_stream.cend() &&
               token_stream_ptr->id == node->id);
        node->token_index = token_stream_ptr - context.token_stream.begin();
        ++token_stream_ptr;
        return 0;
    }
    // non termainal: 将正确的右部所有符号加入孩子中
    auto production_index = context.predict_table.at(node->id, token_stream_ptr->id);
    if (production_index < 0)
    {
        return -production_index;
    }
    for (auto &&right_symbol : context.productions[production_index].right)
    {
        if (right_symbol != 0)
        {
            auto child = node->add_child(right_symbol);
            auto err = build_tree_recur(context, token_stream_ptr, child);
            if (err)
                return err;
        }
    }
    return 0;
}

int Parser::analyze(std::string const &file_name, std::string const &code,
                    std::vector<Token> const &tokenstream,
                    SyntaxTree &syntax_tree,
                    AnalyzeResult &res)
{
    syntax_tree.root->id = 1;
    BuildTreeContext context(rules.parser_rules, prdtbl, tokenstream);
    auto head = tokenstream.cbegin();

    std::stringstream ss;
    int rc = build_tree_recur(context, head, syntax_tree.root);
    if (rc)
    {
        auto e = head->begin;
        int line_no = 1;
        int col_no = 1;
        for (size_t i = 0; i != e; i++)
        {
            if (code[i] == '\n')
            {
                line_no++;
                col_no = 1;
            }
            else
            {
                col_no++;
            }
        }
        std::string token(code.begin() + head->begin, code.begin() + head->end);

        ss << "error " << rc << ": syntactical error, cannot parse token `"
           << token
           << "` @ " << file_name << "(" << line_no << "," << col_no << ")";

        res.msg = ss.str();
    }
    res.error_token = head;
    return rc;
}
