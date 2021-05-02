#include <meta-parser.hpp>
#include <algorithm>
#include <iostream>

using namespace std;

void Parser::compute_empty_deriving_symbols() 
{
    
    // InputResolver 规定至少有1个产生式，且第一个定义的产生式的左部为开始符号
    // 初始化
    for (int i = -rules.terminal_count + 1; i <= rules.non_terminal_count; i++)
    {
        empty_derivings[i]=0;
    }

    for (auto &&production : rules.parser_rules)//自动遍历production语句
    {
      if(*production.right.cbegin()==0)//如果式子右部为空，则将左部的非终结符标记置1
      {
        empty_derivings[production.left]=1;
      }
    }

  int temp=1,temp1=1,count=0,before=0;  
  while(temp)
  {
    for (auto &&production : rules.parser_rules)//自动遍历production语句
    { 
      if(empty_derivings[production.left]==0)//如果非终结符不能推出空
      {
        temp1=1;
        for (auto &&i = production.right.cbegin(); i < production.right.cend(); i++)//从右部移到最后一位
        {
           empty_derivings[production.left]=1;//终结符标记置一
           if(is_termin(*i)==1)//如果是终结符
            {
             empty_derivings[production.left]=0;//终结符标记置0
             temp1=0;
            }
           if(is_non_termin(*i)==1)//如果是终结符
           {
             if(empty_derivings[*i]==0)//如果终结符不能推出空
             {
              empty_derivings[production.left]=0;//终结符标记置0
              temp1=0;   
             }
           }
           if(temp1==0)//如果右边有一项不能推出空，则退出
           break;
        }
      }
    }

    count=0;
    for (auto &&production : rules.parser_rules)//自动遍历production语句
      {
         if(empty_derivings[production.left]==1)//如果非终结符标记为1
         {
            count++;
         }
      } 

    if(count!=before)//如果两次非终结符可推出空总数不一致
    {
      temp=1;
      before=count;
    }

    else//如果两次非终结符可推出空总数一致
    temp=0;
 
  }
}