#include <meta-parser.hpp>
#include <cassert>
#include <cstdlib>
#include <matrix.hpp>
#include <prdtbl.hpp>

int Parser::generate_predict_table()
{
    for (size_t pi = 0; pi < rules.parser_rules.size(); pi++)
    {
        auto &&production = rules.parser_rules[pi];
        auto &&select = select_set[pi];
        for (auto &&t : select)
        { 
            auto &&cell = prdtbl.at(production.left, t);
            if (cell < 0)
                cell = pi;
            else
                return 1; 
        }
    }
    return 0;
}
