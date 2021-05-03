#ifndef _PRDTBL_H_
#define _PRDTBL_H_

#include <matrix.hpp>

class PredictTable
{
private:
    Matrix<int64_t> table;

public:
    using rule_index = int64_t;

    PredictTable(size_t nt_count, size_t t_count) : table(nt_count, t_count)
    {
        int64_t i = -1;
        for (auto &&elem : table)
        {
            elem = i;
            i--;
        }
    }

    rule_index &at(symbol_id nt, symbol_id t)
    {
        return table.at(symid_to_index(nt, t));
    }

    rule_index at(symbol_id nt, symbol_id t) const
    {
        return table.at(symid_to_index(nt, t));
    }

private:
    std::pair<size_t, size_t> symid_to_index(symbol_id nt, symbol_id t) const
    {
        return {nt - 1, -t - 1};
    }
};
#endif // _PRDTBL_H_