#ifndef _SYMBOL_H_
#define _SYMBOL_H_
#include <stdint.h>

/// 符号（分为终结符或非终结符）的id。
using symbol_id = std::int32_t;

/// 返回符号是否为终结符
inline bool is_termin(symbol_id sid)
{
    return sid < 0;
}

/// 返回符号是否为非终结符
inline bool is_non_termin(symbol_id sid)
{
    return sid > 0;
}

/// ε的id
static const symbol_id nil_id = 0;

/// 文件结尾eof的id
static const symbol_id eof_id = -1;

#endif // _SYMBOL_H_