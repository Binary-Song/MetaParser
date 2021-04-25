#include <meta-lexer.hpp>
struct char_pos
{
    int line;
    int column;
};
char_pos get_line_no(std::string const &str, std::string::const_iterator target)
{
    int line = 1;
    int column = 0;
    auto i = str.begin();
    while (i != target)
    {
        if (*i == '\n')
        {
            column = 0;
            line++;
        }
        else // new line
        {
            column++;
        }
        i++;
    }
    char_pos pos;
    pos.column = column;
    pos.line = line;
    return pos;
}
int Lexer::analyze(std::string const &str, std::vector<Token> &tokens)
{
    auto target_start = str.begin();
    while (target_start != str.end())
    {
        bool matched = false;
        for (auto &&rule : rules)
        {
            std::smatch matches;
            if (std::regex_search(target_start, str.end(), matches, rule.reg, std::regex_constants::match_continuous))
            {
                auto begin = target_start - str.begin();
                auto end = matches[0].second - str.begin();
                tokens.push_back(Token(rule.token_id, begin, end));
                target_start = matches[0].second;
                matched = true;
                break;
            }
        }
        if (!matched) // error
        {
            char_pos pos = get_line_no(str, target_start);
            diag_msg += "lexcial error: unknown token at line " + std::to_string(pos.line) + ", col " + std::to_string(pos.column) + "\n";
            return 1;
        }
    }
    return 0;
}