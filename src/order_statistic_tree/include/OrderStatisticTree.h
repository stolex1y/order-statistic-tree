#ifndef ORDER_STATISTIC_TREE_H
#define ORDER_STATISTIC_TREE_H

#include <stdexcept>

class OrderStatisticTree {
protected:
    struct Node {
        enum class Color {
            RED = 0,
            BLACK = 1
        };

        Color color = Color::RED;
        int key = 0;
        std::size_t count = 1;
        Node *left = nullptr;
        Node *right = nullptr;
        Node *parent = nullptr;

        explicit Node(
                int key,
                Color color = Color::RED,
                Node *parent = nullptr,
                Node *left = nullptr,
                Node *right = nullptr
        ) : key(key), color(color), parent(parent),
            left(left), right(right) {}

        Node(const Node &other) : key(other.key), color(other.color), count(other.count) {
            if (other.left) {
                left = new Node(*other.left);
            }
            if (other.right) {
                right = new Node(*other.right);
            }
            update_children_parent();
        }

        Node(Node &&other) noexcept: Node() {
            swap(*this, other);
        }

        Node &operator=(Node other) {
            swap(*this, other);
            return *this;
        }

        ~Node() {
            delete right;
            delete left;
        }

        void left_rotate(Node *&root_node) {
            if (!right)
                throw std::logic_error("Right child must be not null!");
            auto y = right;
            right = y->left;
            update_right_child_parent();

            y->left = this;
            this->set_parent_child_to(y);
            y->parent = parent;
            parent = y;

            if (this == root_node)
                root_node = y;

            update_count();
            y->update_count();
        }

        void right_rotate(Node *&root_node) {
            if (!left)
                throw std::logic_error("Left child must be not null!");
            auto y = left;
            left = y->right;
            update_left_child_parent();

            y->right = this;
            this->set_parent_child_to(y);
            y->parent = parent;
            parent = y;

            if (this == root_node)
                root_node = y;

            update_count();
            y->update_count();
        }

        [[nodiscard]] std::size_t left_count() const {
            return left ? left->count : 0;
        }

        [[nodiscard]] std::size_t right_count() const {
            return right ? right->count : 0;
        }

        [[nodiscard]] friend bool equals(const Node *first, const Node *second) {
            if (first == nullptr ^ second == nullptr)
                return false;
            else if (first == nullptr && second == nullptr)
                return true;
            return first->color == second->color &&
                   first->key == second->key &&
                   equals(first->left, second->left) &&
                   equals(first->right, second->right);
        }

        friend void swap(Node &first, Node &second) noexcept {
            using std::swap;
            swap(first.count, second.count);
            swap(first.key, second.key);
            swap(first.color, second.color);
            swap_children(first, second);
            swap_parents(first, second);
        }

    private:
        Node() = default;

        void update_children_parent() {
            update_left_child_parent();
            update_right_child_parent();
        }

        void update_left_child_parent() {
            if (left)
                left->parent = this;
        }

        void update_count() {
            std::size_t new_count = 1;
            if (left)
                new_count += left->count;
            if (right)
                new_count += right->count;
            this->count = new_count;
        }

        void update_right_child_parent() {
            if (right)
                right->parent = this;
        }

        void set_parent_child_to(Node *new_child) {
            if (!parent)
                return;
            if (parent->left == this)
                parent->left = new_child;
            else
                parent->right = new_child;
        }

        friend void swap_children(Node &first, Node &second) {
            std::swap(first.left, second.left);
            std::swap(first.right, second.right);
            first.update_children_parent();
            second.update_children_parent();
        }

        friend void swap_parents(Node &first, Node &second) {
            first.set_parent_child_to(&second);
            second.set_parent_child_to(&first);
            std::swap(first.parent, second.parent);
        }
    };

    Node *root = nullptr;
    std::size_t count = 0;

public:
    OrderStatisticTree() = default;

    OrderStatisticTree(const OrderStatisticTree &other) :
            root(new Node(*other.root)), count(other.count) {}

    OrderStatisticTree(OrderStatisticTree &&other) noexcept: OrderStatisticTree() {
        swap(*this, other);
    }

    OrderStatisticTree &operator=(OrderStatisticTree tree) {
        swap(*this, tree);
        return *this;
    }

    ~OrderStatisticTree() {
        delete root;
    }

    bool insert(int key) {
        if (contains(key))
            return false;

        Node *new_node = new Node(key);
        const auto parent = find_position_to_add(key);
        new_node->parent = parent;
        if (!parent)
            root = new_node;
        else if (new_node->key < parent->key)
            parent->left = new_node;
        else
            parent->right = new_node;
        insert_fixup(new_node);
        increment_from_bottom_to_top(new_node);
        count++;
        return true;
    }

    [[nodiscard]] bool contains(int key) const {
        Node *cur = root;
        while (cur) {
            if (cur->key == key)
                return true;
            else if (key < cur->key)
                cur = cur->left;
            else
                cur = cur->right;
        }
        return false;
    }

    [[nodiscard]] bool empty() const {
        return root == nullptr;
    }

    [[nodiscard]] bool not_empty() const {
        return !empty();
    }

    bool operator==(const OrderStatisticTree &other) const {
        return count == other.count && equals(root, other.root);
    }

    bool operator!=(const OrderStatisticTree &other) const {
        return !operator==(other);
    }

    [[nodiscard]] std::size_t size() const {
        return count;
    }

    [[nodiscard]] int find_order_statistic(std::size_t k) const {
        if (k > size())
            throw std::logic_error("k mustn't be more than tree size!");
        return find_order_statistic(root, k);
    }

    [[nodiscard]] std::size_t less_count(int key) const {
        auto cur = find_position_to_add(key);
        if (!cur)
            return 0;

        std::size_t lower_count = 0;
        if (key >= cur->key) {
            lower_count += cur->left_count();
            if (key > cur->key)
                lower_count++;
        }

        auto parent = cur->parent;
        while (parent) {
            if (cur == parent->right) {
                lower_count += parent->left_count();
                if (key > parent->key)
                    lower_count++;
            }
            cur = parent;
            parent = cur->parent;
        }
        return lower_count;
    }

    friend void swap(OrderStatisticTree &first, OrderStatisticTree &second) {
        std::swap(first.root, second.root);
        std::swap(first.count, second.count);
    }

private:
    void insert_fixup(Node *added) {
        using Color = Node::Color;

        auto cur = added;
        auto parent = cur->parent;
        while (parent && parent->color == Color::RED) {
            const auto grandpa = parent->parent;
            if (parent == grandpa->left) {
                const auto uncle = grandpa->right;
                if (uncle && uncle->color == Color::RED) {
                    uncle->color = Color::BLACK;
                    parent->color = Color::BLACK;
                    grandpa->color = Color::RED;
                    cur = grandpa;
                } else {
                    if (cur == parent->right) {
                        std::swap(cur, parent);
                        cur->left_rotate(root);
                    }
                    parent->color = Color::BLACK;
                    grandpa->color = Color::RED;
                    grandpa->right_rotate(root);
                }
            } else {
                const auto uncle = grandpa->left;
                if (uncle && uncle->color == Color::RED) {
                    uncle->color = Color::BLACK;
                    parent->color = Color::BLACK;
                    grandpa->color = Color::RED;
                    cur = grandpa;
                } else {
                    if (cur == parent->left) {
                        std::swap(cur, parent);
                        cur->right_rotate(root);
                    }
                    parent->color = Color::BLACK;
                    grandpa->color = Color::RED;
                    grandpa->left_rotate(root);
                }
            }
            parent = cur->parent;
        }
        root->color = Color::BLACK;
    }

    [[nodiscard]] int find_order_statistic(const Node *node, std::size_t k) const {
        const std::size_t left_size = node->left_count();
        if (k == left_size + 1)
            return node->key;
        else if (k <= left_size)
            return find_order_statistic(node->left, k);
        else
            return find_order_statistic(node->right, k - left_size - 1);
    }

    static void increment_from_bottom_to_top(const Node *added) {
        auto parent = added->parent;
        while (parent) {
            parent->count++;
            parent = parent->parent;
        }
    }

    [[nodiscard]] Node *find_position_to_add(int key) const {
        Node *parent = nullptr;
        Node *cur = root;
        while (cur) {
            parent = cur;
            if (key < cur->key)
                cur = cur->left;
            else
                cur = cur->right;
        }
        return parent;
    }
};

#endif //ORDER_STATISTIC_TREE_H
