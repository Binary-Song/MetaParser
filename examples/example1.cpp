/// @file 示例1 解析规则文件，显示符号id和符号名，并进行词法分析。
/// MetaParser 第一步需要加载规则文件（rules1.txt），将文本转换成 Rule 对象(参见shared/rules.hpp)。Rule包括词法规则和语法规则。词法规则可以作为词法分析器 Lexer 对象构造函数的输入，用于初始化Lexer对象。之后可以用该对象对指定字符串(code1.txt)进行词法分析。本示例演示这些步骤
#include <iostream>
#include <fstream>
#include <sstream>

#include <input-resolver.hpp>
#include <meta-lexer.hpp>
std::string read_file_into_string(const char *file_name);

int main()
{
    // 创建解析器对象
    InputFileResolver resolver;
    // 通过解析文件，得到语言的所有规则。包括词法规则和语法规则
    Rules rules;
    // 错误代码，0为成功，调用本函数后，rules对象被写入
    int errcode = resolver.resolve_input_file(__FILE__ "/../rules1.txt", rules);
    if (errcode)
    {
        // 若有错误，就输出错误消息，退出程序
        std::cout << resolver.full_diag_msg << std::endl;
        return 1;
    }

    // 输出所有符号
    std::cout << "listing all symbols:\n";

    for (auto &&name_id_pair : resolver.symbol_id_to_name)
    {
        std::string name = name_id_pair.second;
        symbol_id id = name_id_pair.first;

        std::cout <<"id=" <<id<<"\t" <<name << "\t";
        std::cout << "type=";
        if (is_termin(id))
            std::cout << "terminal" << std::endl;
        else if (is_non_termin(id))
            std::cout << "nonterminal" << std::endl;
        else
            std::cout << "empty" << std::endl;
    }

    // 用词法规则创建词法分析器
    Lexer lexer(rules.lexer_rules);
    // 读取文件到字符串，它是我们要编译的“代码”
    const char * file_path = __FILE__ "/../code1.txt";
    std::string test_code = read_file_into_string(file_path);

    std::vector<Token> tokens;
    // 词法分析
    errcode = lexer.analyze(test_code, tokens);
    if (errcode)
    { // 如果有词法错误（不认识的单词），就输出错误消息，退出程序
        std::cout << lexer.diag_msg;
        return 1;
    }

    // 遍历每个单词
    std::cout << "listing all tokens:\n";

    for (Token &token : tokens)
    {
        // token中只有单词的类型（对应的终结符id）和单词在“代码”中的首尾下标。
        // 首下标（begin）是单词第一个字符的下标
        // 尾下标（end）是单词最后一个字符的下一个字符的下标
        // 下面这行通过begin和end将单词的内容保存到token_content字符串中
        std::string token_content(
            test_code.begin() + token.begin, // token.begin 是单词的开始在test_code中的下标（从0开始）
            test_code.begin() + token.end);  // token.end 是单词的结尾后一个字符test_code中的下标（从0开始）

        // 输出单词和对应类型
        std::cout << "`"<<token_content << "`\t" << resolver.symbol_name(token.id) << std::endl;
    }

    // 遍历每个符号

    return 0;
}

std::string read_file_into_string(const char *file_name)
{
    std::ifstream t(file_name);
    std::stringstream buffer;
    buffer << t.rdbuf();
    return buffer.str();
}
