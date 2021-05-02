#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <utility>
#include <algorithm>
#include <cassert>
/// 一个二维矩阵，创建之后就不可以改变大小。
template <typename cell_t>
class Matrix
{
public:
    size_t rn;
    size_t cn;
    cell_t *buffer;

    Matrix(size_t rn, size_t cn)
        : buffer(new cell_t[rn * cn]),
          rn(rn), cn(cn) {}

    Matrix(size_t rn, size_t cn, cell_t init_values)
        : buffer(new cell_t[rn * cn]),
          rn(rn), cn(cn)
    {
        std::fill(buffer, buffer + rn * cn, init_values);
    }

    ~Matrix()
    {
        delete[] buffer;
    }

    cell_t &at(std::pair<size_t, size_t> pos)
    {
        assert(chk_pos(pos.first, pos.second));
        return buffer[flatten(pos.first, pos.second)];
    }

    cell_t at(std::pair<size_t, size_t> pos) const
    {
        assert(chk_pos(pos.first, pos.second));
        return buffer[flatten(pos.first, pos.second)];
    }

    cell_t *begin()
    {
        return buffer;
    }

    cell_t *end()
    {
        return buffer + rn * cn;
    }

    cell_t const *begin() const
    {
        return buffer;
    }

    cell_t const *end() const
    {
        return buffer + rn * cn;
    }

    cell_t const *cbegin() const
    {
        return buffer;
    }

    cell_t const *cend() const
    {
        return buffer + rn * cn;
    }

private:
    bool chk_pos(size_t r, size_t c) const
    {
        return r < rn && c < cn;
    }
    size_t flatten(size_t r, size_t c) const
    {
        return r * cn + c;
    }
};
#endif // _MATRIX_H_