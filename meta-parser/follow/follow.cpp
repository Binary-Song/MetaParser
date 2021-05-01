#include <meta-parser.hpp>
#include <algorithm>
#include <iostream>

void print_production(const Rules &rules, const ParserRule &production)
{
    std::cout << rules.to_name(production.left) << "->";
    for (auto p : production.right)
    {
        std::cout << rules.to_name(p) << " ";
    }
    std::cout << "\n";
}

void Parser::compute_follow_set()
{
    // InputResolver 规定至少有1个产生式，且第一个定义的产生式的左部为开始符号
    // 初始化
    for (int i = -rules.terminal_count + 1; i <= rules.non_terminal_count; i++)
    {
        follow_set.insert({i, std::set<symbol_id>()});
    }

    size_t diff_size = 1;
    while (diff_size)
    {
        diff_size = 0;
        // FOLLOW(S) += {ε}
        follow_set[1].insert(eof_id);

        // 若A->αBβ是一个产生式，则将FIRST(β)-{ε}加入FOLLOW(B)中。
        // 如果β能推出ε，则把FOLLOW(A)也加入FOLLOW(B)中。
        for (auto &&production : rules.parser_rules)
        {
            // print_production(rules, production);
            for (auto &&i = production.right.cbegin(); i != production.right.cend(); i++)
            {
                if (is_non_termin(*i))
                {
                    // 求出FIRST(β) - {ε}
                    auto first_set_of_beta = get_sequence_first_set(i + 1, production.right.cend() );
                    first_set_of_beta.erase(nil_id);
                    // 保存insert之前的size
                    size_t size0 = follow_set[*i].size();
                    // FOLLOW(B) += FIRST(β) - {ε}
                    follow_set[*i].insert(first_set_of_beta.begin(), first_set_of_beta.end());
                    // 计算insert前后size的差
                    diff_size += follow_set[*i].size() - size0;
                    // 若β能推出ε
                    if (is_empty_deriving_sequence(i + 1, production.right.cend() ))
                    {
                        // 保存insert之前的size
                        size_t size0 = follow_set[*i].size();
                        // FOLLOW(B) += FOLLOW(A)
                        follow_set[*i].insert(follow_set[production.left].begin(), follow_set[production.left].end());
                        // 计算insert前后size的差
                        diff_size += follow_set[*i].size() - size0;
                    }
                }
            }
        }
    }
}

bool Parser::is_empty_deriving_sequence(std::vector<symbol_id>::const_iterator begin,
                                        std::vector<symbol_id>::const_iterator end)
{
    for (auto i = begin; i != end; i++)
    {
        if (this->empty_derivings[*i] == 0)
        {
            return false;
        }
    }
    return true;
}

std::set<symbol_id> Parser::get_sequence_first_set(std::vector<symbol_id>::const_iterator begin,
                                                   std::vector<symbol_id>::const_iterator end)
{
    // 分成几种互斥的情况，下面用标号表示
    // 0. 当符号串长度为0时，FIRST={ε}
    if (begin == end)
    {
        return std::set<symbol_id>{nil_id};
    }
    // 1. 第一个符号不能推出空的情况，result = FIRST(第一个符号)
    if (this->empty_derivings[*begin] == 0)
    {
        return this->first_set[*begin];
    }
    // 2. 前几个符号FIRST都含空，第i个符号FIRST不含空
    bool early_exit = false;
    std::set<symbol_id> result;
    for (auto i = begin; i != end; i++)
    {
        // result = result ∪ FIRST(符号i)
        result.insert(this->first_set[*i].begin(), this->first_set[*i].end());
        // FIRST不含空，就提前退出循环
        if (this->first_set[*i].count(nil_id) == 0)
        {
            early_exit = true;
            break;
        }
    }
    if (early_exit)
    {
        // p.s. 此处属于情况2
        // 所以要去掉ε
        // result = result - {ε}
        result.erase(nil_id);
        return result;
    }
    // 3. 所有符号FIRST含空，不用去掉ε
    return result;
}