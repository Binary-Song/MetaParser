#include <meta-parser.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

void Parser::compute_first_set()
{
    // InputResolver 规定至少有1个产生式，且第一个定义的产生式的左部为开始符号
    // 初始化
    for (int i = -rules.terminal_count + 1; i <= rules.non_terminal_count; i++)
    {
        first_set.insert({i, std::set<symbol_id>()});
    }

    for (int i = -rules.terminal_count; i <= 0; i++)
    {
        first_set[i] = {i}; //终结符和空的first集都是它本身
    }

    size_t diff_size = 1;
    while (diff_size)
    {
        diff_size = 0;
        for (auto &&production : rules.parser_rules)
        {
            if (is_termin(*production.right.cbegin()) || *production.right.cbegin() == 0) //若X—>a a∈VT 或X—> 空 first[x]=a或空
            {
                size_t size0 = first_set[production.left].size();
                first_set[production.left].insert({*production.right.cbegin()});
                diff_size += first_set[production.left].size() - size0;
            }
            else
            {
                int judge1 = 1;
                int judge2 = 1;
                
                for (auto &&i = production.right.cbegin(); i != production.right.cend(); i++)
                {
                    if (is_non_termin(*i))
                    {
                    judge1=1;
                    }
                    else
                    {
                        judge1=0;
                        break;
                    }
                }
                if (judge1) //若X->Y1 Y2 Y3...YN ∈VN
                {
                    for (auto &&i = production.right.cbegin(); i < production.right.cend(); i++)//当Y1,Y2....YI-1->空
                    {
                             if(empty_derivings[*i]==0)
                             {
                                 break;
                             }
                    }
                    for (auto &&i = production.right.cbegin(); i != production.right.cend(); i++)
                    {

                            // 求出FIRST(Y) - {ε}
                            auto first_set_of_beta = get_sequence_first_set(i, production.right.cend());
                            first_set_of_beta.erase(nil_id);
                            // 保存insert之前的size
                            size_t size0 = first_set[production.left].size();
                            // First(X) += FIRST(Y) - {}
                            first_set[production.left].insert(first_set_of_beta.begin(), first_set_of_beta.end());
                            // 计算insert前后size的差
                            diff_size += first_set[production.left].size() - size0;
                            if (!empty_derivings[production.left])//当Y无法推出空时
                            {
                                judge2 = 0;
                            }

                    }
                    if (judge2)//若在满足X->Y1 Y2 Y3...YN ∈VN 条件下所有Y->空
                    {
                          for (auto &&i = production.right.cbegin(); i != production.right.cend(); i++)
                          {
                              // 保存insert之前的sizeirst
                              size_t size0 = first_set[production.left].size();
                               // First(A) += First(b)+{空}
                            first_set[production.left].insert(first_set[production.left].begin(), first_set[production.left].end());
                            first_set[production.left].insert({0});
                             // 计算insert前后size的差
                                diff_size += first_set[production.left].size() - size0;
                          }
                    }
                }
            }
        }
    }
}
