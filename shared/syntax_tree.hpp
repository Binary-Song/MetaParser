#ifndef _SYNTAX_TREE_H_
#define _SYNTAX_TREE_H_

#include <vector>
#include <memory>
#include <symbol.hpp>

/// 抽象语法树
class SyntaxTree
{
public:
    class Node;

    SyntaxTree()
    {
        root = new Node();
    }

    ~SyntaxTree()
    {
        delete root;
    }

    /// 根节点
    Node *root = nullptr;

    /// 树的结点
    class Node
    {
    private:
        std::vector<Node *> children;

    public:
        symbol_id id;
        size_t token_index = -1;

        Node(symbol_id id = 0)
            : id(id)
        {
        }

        ~Node()
        {
            for (auto &&child : children)
            {
                delete child;
            }
        }

        std::vector<Node *> const &get_children() const
        {
            return children;
        }

        Node *add_child(symbol_id id)
        {
            auto &&c = get_children();
            children.push_back(new Node(id));
            return children.back();
        }
    };
};

#endif // _SYNTAX_TREE_H_