#include <iostream>
#include <vector>
#include <string>
#include <cstddef>

template<typename T>
class RBTree {
public:
    using value_type = T;
    using size_type = std::size_t;

    RBTree();

    ~RBTree();

    void insert(const value_type &key);

    void erase(const value_type &key);

    bool contains(const value_type &key);

    void size();

    bool isEmpty();

    void clear();


private:
    struct Node {
        int key;
        int color;
        Node *left;
        Node *right;
        Node *parent;
    };
    Node *root;
    size_type _size;

};
