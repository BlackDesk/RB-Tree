#include <iostream>
#include <vector>
#include <string>
#include <cstddef>

template<typename T>
class RBTree {
private:
    struct Node;
public:
    using value_type = T;

    using size_type = std::size_t;

    RBTree() = default;

    ~RBTree() = default;

    void insert(const value_type &key);

    void erase(const value_type &key);

    bool contains(const value_type &key);

    void size();

    bool isEmpty();

    void clear();


private:
    // Meta
    enum Color {
        Black,
        Red,
    };

    class Node {
    public:
        value_type key;
        int color{Color::Red};
        Node *left{nullptr};
        Node *right{nullptr};
        Node *parent{nullptr};

        explicit Node(const value_type &key,
                      const Color &color = Color::Red) : key(key), color(color) {};
    };

    // Fields
    Node *_root{nullptr};
    size_type _size{0};

    // Methods
    void insert_node(Node *root, Node *node);

    void insert_straight(Node *subtree_root, Node *node);

    void balance_from_node(Node *node);

    Node *rotate_left(Node *root);

    Node *rotate_right(Node *root);

};

template<typename T>
void RBTree<T>::insert(const value_type &key) {
    Node *node = new Node(key);
    node->color = Color::Red;
    insert_node(_root, node);
}

template<typename T>
void RBTree<T>::insert_node(RBTree::Node *root, RBTree::Node *node) {
    insert_straight(root, node);
    balance_from_node(node);
}

template<typename T>
void RBTree<T>::insert_straight(RBTree::Node *subtree_root, RBTree::Node *node) {
    if (_root == nullptr) {
        _root = node;
        return;
    }
    if (subtree_root) {
        node->parent = subtree_root;
        if (subtree_root->key < node->key) {
            insert_straight(subtree_root->right, node);
        } else {
            insert_straight(subtree_root->left, node);
        }
    } else {
        if (node->parent->key < node->key) {
            node->parent->right = node;
        } else {
            node->parent->left = node;
        }
    }
}

template<typename T>
void RBTree<T>::balance_from_node(RBTree::Node *node) {
    if (node->color == Color::Black) {
        return;
    }

    if (node == _root) {
        node->color = Color::Black;
        return;
    }

    if (node->parent->color == Color::Black) {
        return;
    }

    Node *parent = node->parent;
    Node *grandad = node->parent->parent;
    Node *uncle = (grandad->right == node->parent ? grandad->right : grandad->left);

    if (uncle && uncle->color == Color::Red) {               // Uncle is also red
        uncle->color = Color::Black;                // Change uncle to black
        parent->color = Color::Black;         // Change dad to black
        grandad->color = Color::Red;   // Change grandad to red
        balance_from_node(grandad);    // Balance from grandad
    } else {                                        // Uncle is black
        if (grandad->left == parent && parent->right == node) {
            rotate_left(parent);
            rotate_right(grandad);
            node->color = Color::Black;
            grandad->color = Color::Red;
        } else if (grandad->right == parent && parent->left == node) {
            rotate_right(parent);
            rotate_left(grandad);
            node->color = Color::Black;
            grandad->color = Color::Red;
        } else if (grandad->right == parent && parent->right == node) {
            rotate_left(grandad);
            grandad->color = Color::Red;
            parent->color = Color::Black;
        } else if (grandad->left == parent && parent->left == node) {
            rotate_right(grandad);
            grandad->color = Color::Red;
            parent->color = Color::Black;
        } else {
            throw std::exception();
        }
    }
}


template<typename T>
void RBTree<T>::size() {
    return _size;
}

template<typename T>
bool RBTree<T>::isEmpty() {
    return _size == 0;
}

template<typename T>
typename RBTree<T>::Node *RBTree<T>::rotate_left(RBTree::Node *root) {
    Node *right_child = root->right;
    root->right = right_child->left;
    if (root->right) {
        root->right->parent = root;
    }
    right_child->left = root;
    root->parent = right_child;
    return right_child;
}

template<typename T>
typename RBTree<T>::Node *RBTree<T>::rotate_right(RBTree::Node *root) {
    Node *left_child = root->left;
    root->left = left_child->right;
    if (root->left) {
        root->left->parent = root;
    }
    left_child->right = root;
    root->parent = left_child;
    return left_child;
}


int main() {
    RBTree<int> tree;
    tree.insert(0);
    tree.insert(1);
    tree.insert(-1);
    tree.insert(3);
    tree.insert(-3);
    tree.insert(2);
    tree.insert(-2);
    return 0;
}