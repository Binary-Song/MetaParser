#include <meta-parser.hpp> 
Parser::Parser(Rules const &rules, bool do_ll1_computations)
    : rules(rules), prdtbl(rules.non_terminal_count, rules.terminal_count)
{
    if (do_ll1_computations)
    {
        compute_empty_deriving_symbols();
        compute_first_set();
        compute_follow_set();
        compute_select_set();
        if (generate_predict_table())
        {
            throw Exception("Not a LL(1) grammar.");
        }
    }
}
