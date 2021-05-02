#ifndef _PRDTBL_H_
#define _PRDTBL_H_

#include <matrix.hpp>

class PredictTable
{
private:
    Matrix<size_t> table;

public:
    using rule_index = size_t;

    PredictTable(size_t nt_count, size_t t_count) : table(nt_count, t_count) {}

    rule_index &at(symbol_id nt, symbol_id t)
    {
        return table.at( symid_to_index(nt, t));
    }

private:
    std::pair<size_t, size_t> symid_to_index(symbol_id nt, symbol_id t)
    {
        return {nt - 1, -t};
    }
};
#endif // _PRDTBL_H_