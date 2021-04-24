#include "io.hpp"
#include <iostream>
int main()
{
    InputFileResolver resolver;
    Rules rules;
    int errcode = resolver.read_input(__FILE__ "/../input.txt", rules);
    if (errcode)
    {
        std::cout << resolver.full_diag_msg << std::endl;
        return 1;
    }

    std::cout << "Input file successfully loaded:\n";
    std::cout << "Lexer rules:\n";

    for (auto &&r : rules.lexer_rules)
    {
        std::cout << "[" << r.token_id << "]: " << resolver.symbol_name(r.token_id) << "\n";
    }
    std::cout << "Parser rules:\n";

    for (auto &&r : rules.parser_rules)
    {
        std::cout << "[" << r.left << "]: " << resolver.symbol_name(r.left) << "\n";
    }

    for (auto &&r : rules.parser_rules)
    {
        std::cout << r.left << " -> ";
        for (auto &&r : r.right)
        {
            std::cout << " " << r;
        }
        std::cout << "\n";
    }
}
