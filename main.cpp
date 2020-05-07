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

    void erase(const value_type &key);  // FIXME

    void clear();

    bool contains(const value_type &key) const;

    [[nodiscard]] size_type size() const;

    [[nodiscard]] bool isEmpty() const;

    void display() const;


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

    Node *find_node(Node *root, const value_type &key) const;

    Node *rotate_left(Node *node);

    Node *rotate_right(Node *node);

    void erase_node(Node *node); //FIXME

    void erase_subtree(Node *root);

    void display_subtree(Node *node) const;

    void display_node(Node *node) const;
};

template<typename T>
void RBTree<T>::insert(const value_type &key) {
    Node *node = new Node(key);
    node->color = Color::Red;
    insert_node(_root, node);
    ++_size;
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
    Node *uncle = (grandad->right == node->parent ? grandad->left : grandad->right);

    if (uncle && uncle->color == Color::Red) {
        uncle->color = Color::Black;
        parent->color = Color::Black;
        grandad->color = Color::Red;
        balance_from_node(grandad);
    } else {
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
typename RBTree<T>::size_type RBTree<T>::size() const {
    return _size;
}

template<typename T>
bool RBTree<T>::isEmpty() const {
    return _size == 0;
}

template<typename T>
typename RBTree<T>::Node *RBTree<T>::rotate_left(RBTree::Node *node) {
    Node *right_child = node->right;
    Node *node_parent = node->parent;

    node->right = right_child->left;
    if (right_child->left) {
        right_child->left->parent = node;
    }

    right_child->left = node;
    node->parent = right_child;

    right_child->parent = node_parent;
    if (node_parent) {
        node_parent->left = right_child;
    }

    while (_root->parent) {
        _root = _root->parent;
    }

    return right_child;
}

template<typename T>
typename RBTree<T>::Node *RBTree<T>::rotate_right(RBTree::Node *node) {
    Node *left_child = node->left;
    Node *node_parent = node->parent;

    node->left = left_child->right;
    if (left_child->right) {
        left_child->right->parent = node;
    }

    left_child->right = node;
    node->parent = left_child;

    left_child->parent = node_parent;
    if (node_parent) {
        node_parent->right = left_child;
    }

    while (_root->parent) {
        _root = _root->parent;
    }
    return left_child;
}

template<typename T>
bool RBTree<T>::contains(const value_type &key) const {
    return find_node(_root, key) != nullptr;
}

template<typename T>
void RBTree<T>::clear() {
    erase_subtree(_root);
}

template<typename T>
void RBTree<T>::erase_subtree(RBTree::Node *root) {
    if (root) {
        erase_node(root->left);
        erase_node(root->right);
        erase_node(root);
    }
}

template<typename T>
void RBTree<T>::erase_node(RBTree::Node *node) {
    //FIXME
}

template<typename T>
void RBTree<T>::erase(const value_type &key) {
    erase_node(find_node(key));
}

template<typename T>
void RBTree<T>::display() const {
    display_subtree(_root);
}

template<typename T>
typename RBTree<T>::Node *RBTree<T>::find_node(RBTree::Node *root, const value_type &key) const {
    if (root) {
        if (root->key < key) {
            return find_node(root->right, key);
        }
        if (root->key > key) {
            return find_node(root->left, key);
        }
    }
    return root;
}

template<typename T>
void RBTree<T>::display_subtree(RBTree::Node *node) const {
    if (!node) {
        return;
    }
    display_subtree(node->right);
    display_subtree(node->left);
    display_node(node);
}

template<typename T>
void RBTree<T>::display_node(Node *node) const {
    std::cout << node->key << " "
              << (node->left ? std::to_string(node->left->key) : "null") << " "
              << (node->right ? std::to_string(node->right->key) : "null") << " "
              << (node->color == Color::Black ? "B" : "R") << "\n";
}


int main() {
    RBTree<int> tree;
    int q;
    std::cin >> q;
    int command, value;
    for (size_t i = 0; i < q; ++i) {
        std::cin >> command;
        std::cin >> value;
        if (command == 0) {
            if (!tree.contains(value)) {
                tree.insert(value);
            }
        } else {
            std::cout << (tree.contains(value) ? "Yes\n" : "No\n");
        }
    }
    std::cout << tree.size() << "\n";
    tree.display();

    return 0;
}