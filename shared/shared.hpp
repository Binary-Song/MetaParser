#include <string>
#include <vector> 
#include <regex> 
#include <stdint.h>

using symbol_id = std::int32_t;

struct LexerRule
{
    symbol_id token_id;
    std::regex reg;

    LexerRule(symbol_id id = 0) : token_id(id) {}
    LexerRule(symbol_id id, std::string const &pattern) : token_id(id), reg(pattern) {}
};

struct ParserRule
{
    symbol_id left;
    std::vector<symbol_id> right;
};

struct Rules
{
    std::vector<LexerRule> lexer_rules;
    std::vector<ParserRule> parser_rules;
};