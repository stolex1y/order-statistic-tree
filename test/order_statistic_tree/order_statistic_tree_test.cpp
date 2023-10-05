#include <gtest/gtest.h>
#include <queue>
#include <random>
#include <unordered_set>

#include "OrderStatisticTree.h"

class OrderStatisticTreeTestSuite : public testing::Test, public OrderStatisticTree {
public:
    static int black_height(const Node *node) {
        if (!node)
            return 0;
        const auto left_height = black_height(node->left);
        if (node->left)
            return ((node->left->color == Node::Color::BLACK) ? 1 : 0) + left_height;
        else
            return left_height;
    }

    template<class Container>
    void insert(const Container &keys) {
        for (const auto key: keys) {
            OrderStatisticTree::insert(key);
        }
    }

    std::vector<Node *> bfs() {
        std::vector<Node *> nodes;
        std::queue<Node *> queue;
        queue.push(root);
        while (!queue.empty()) {
            const auto cur = queue.front();
            queue.pop();
            nodes.push_back(cur);
            if (cur->left)
                queue.push(cur->left);
            if (cur->right)
                queue.push(cur->right);
        }
        return nodes;
    }
};

static std::unordered_set<int> generate_keys(int n) {
    std::random_device dev;
    std::mt19937 engine(dev());
    std::uniform_int_distribution<int> uniform_dist(INT_MIN, INT_MAX);

    std::unordered_set<int> keys;
    while (keys.size() != n) {
        keys.insert(uniform_dist(engine));
    }
    return keys;
}

static std::vector<int> generate_serial_keys(int n) {
    std::vector<int> keys(n);
    std::generate_n(keys.begin(), n, [i = 0]() mutable {
        return i++;
    });
    return keys;
}

TEST_F(OrderStatisticTreeTestSuite, CopyTree) {
    insert(generate_keys(100));
    OrderStatisticTree other;
    OrderStatisticTree &this_tree = *static_cast<OrderStatisticTree *>(this);
    EXPECT_TRUE(other != this_tree);
    other = this_tree;
    EXPECT_TRUE(other == this_tree);
}

TEST_F(OrderStatisticTreeTestSuite, BlackHeight) {
    auto keys = generate_serial_keys(10000);
    insert(keys);
    const auto nodes = bfs();
    for (const auto node: nodes) {
        const int left_height = black_height(node->left) +
                                ((node->left && node->left->color == Node::Color::BLACK) ? 1 : 0);
        const int right_height = black_height(node->right) +
                                 ((node->right && node->right->color == Node::Color::BLACK) ? 1 : 0);
        EXPECT_EQ(left_height, right_height);
    }
}

TEST_F(OrderStatisticTreeTestSuite, InsertionUnique) {
    const auto keys = generate_serial_keys(static_cast<int>(10e5));
    insert(keys);
    for (const auto key: keys) {
        EXPECT_TRUE(contains(key));
    }
}

TEST_F(OrderStatisticTreeTestSuite, InsertionNotUnique) {
    const auto keys = generate_keys(100);
    insert(keys);
    for (const auto key: keys) {
        EXPECT_TRUE(contains(key));
        EXPECT_FALSE(OrderStatisticTree::insert(key));
    }
}

TEST_F(OrderStatisticTreeTestSuite, RootIsBlack) {
    const auto keys = generate_keys(100);
    insert(keys);
    EXPECT_EQ(Node::Color::BLACK, root->color);
}

TEST_F(OrderStatisticTreeTestSuite, RedParentIsNotRed) {
    const auto keys = generate_keys(100);
    insert(keys);
    const auto nodes = bfs();
    for (const auto node: nodes) {
        if (node->color == Node::Color::RED && node->parent) {
            EXPECT_EQ(Node::Color::BLACK, node->parent->color);
        }
    }
}

TEST_F(OrderStatisticTreeTestSuite, LessCounts) {
    auto keys = generate_serial_keys(static_cast<int>(10e5));
    insert(keys);
    for (std::size_t i = 0; i < keys.size(); i++) {
        EXPECT_EQ(i, less_count(keys[i]));
    }
    EXPECT_EQ(keys.size(), less_count(keys.back() + 1));
}

TEST_F(OrderStatisticTreeTestSuite, OrderStatistics) {
    auto keys = generate_serial_keys(static_cast<int>(10e5));
    insert(keys);
    for (std::size_t i = 0; i < keys.size(); i++) {
        EXPECT_EQ(keys[i], find_order_statistic(i + 1));
    }
}

TEST_F(OrderStatisticTreeTestSuite, TreeSize) {
    const int size = 100;
    auto keys = generate_keys(size);
    EXPECT_EQ(size, keys.size());
    insert(keys);
    EXPECT_EQ(size, this->size());
}

TEST_F(OrderStatisticTreeTestSuite, MoveTree) {
    auto keys = generate_keys(100);
    insert(keys);

    OrderStatisticTree &this_tree = *static_cast<OrderStatisticTree *>(this);
    OrderStatisticTree temp(this_tree);
    EXPECT_TRUE(this_tree == temp);

    OrderStatisticTree new_tree(std::move(temp));
    EXPECT_TRUE(this_tree == new_tree);
    EXPECT_EQ(0, temp.size());
}
