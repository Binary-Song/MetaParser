#include <iostream>
#include <fstream>
#include <sstream>

#include <input-resolver.hpp>
#include <meta-lexer.hpp>

#include <shared.hpp>

int main()
{ 
    InputFileResolver resolver; 
    Rules rules; 
    int errcode = resolver.resolve_input_file(__FILE__ "/../rules1.txt", rules);
    if (errcode)
    { 
        std::cout << resolver.full_diag_msg << std::endl;
        return 1;
    }

    Lexer lexer(rules.lexer_rules);  
    std::vector<Token> tokens; 
    errcode = lexer.analyze(read_file_into_string(__FILE__ "/../code1.txt"), tokens);
    if (errcode)
    {
        std::cout << lexer.diag_msg;
        return 1;
    }
    return 0;
}