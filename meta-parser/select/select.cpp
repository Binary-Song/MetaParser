#include <meta-parser.hpp>

void Parser::compute_select_set()
{
    for (auto &&p : rules.parser_rules)
    {
        if (!is_empty_deriving_sequence(p.right.begin(), p.right.end()))
        {
            // 如果 α 不能推出 ε，则：SELECT(A → α) = FIRST(α)
            select_set.push_back(get_sequence_first_set(p.right.begin(), p.right.end()));
        }
        else
        {
            // 如果 α 推出 ε，则：SELECT(A → α) = ( FIRST(α) – {ε} ) ∪ FOLLOW(A)
            auto ss = get_sequence_first_set(p.right.begin(), p.right.end());
            ss.erase(nil_id);
            auto follow_A = follow_set[p.left];
            ss.insert(follow_A.begin(), follow_A.end());
            select_set.push_back(ss);
        }
    }
}
