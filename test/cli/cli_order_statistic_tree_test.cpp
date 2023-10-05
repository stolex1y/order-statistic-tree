#include <gtest/gtest.h>
#include <sstream>

extern int run();

static void add_insert_query(std::stringstream &stream, int key) {
    stream << "k " << key << "\n";
}

static void add_invalid_find_order_statistic_query(std::stringstream &stream) {
    stream << "m " << "asdf" << "\n";
}

static void add_find_order_statistic_query(std::stringstream &stream, std::size_t k) {
    stream << "m " << k << "\n";
}

static void add_lower_count_query(std::stringstream &stream, int key) {
    stream << "n " << key << "\n";
}

static void add_insert_queries(std::stringstream &stream, const std::vector<int> &keys) {
    for (const auto key: keys) {
        add_insert_query(stream, key);
    }
}

static void add_lower_count_queries(std::stringstream &stream, const std::vector<int> &keys) {
    for (const auto key: keys) {
        add_lower_count_query(stream, key);
    }
}


static void add_find_order_statistic_queries(std::stringstream &stream, const std::vector<int> &indexes) {
    for (const auto k: indexes) {
        add_find_order_statistic_query(stream, k);
    }
}

static void run_with_stream(const std::stringstream &input, std::stringstream &output) {
    const auto old_input = std::cin.rdbuf();
    const auto old_output = std::cout.rdbuf();
    std::cin.rdbuf(input.rdbuf());
    std::cout.rdbuf(output.rdbuf());
    EXPECT_EQ(0, run());
    std::cin.rdbuf(old_input);
    std::cout.rdbuf(old_output);
}

static std::vector<int> generate_serial_keys(int n) {
    std::vector<int> keys(n);
    std::generate_n(keys.begin(), n, [i = 1]() mutable {
        return i++;
    });
    return keys;
}

static void skip_n_lines(std::stringstream &stream, std::size_t n) {
    std::string line;
    for (std::size_t i = 0; i < n; i++) {
        std::getline(stream, line);
    }
}

template<typename ValueType, typename Container>
static void expect_values(std::stringstream &stream, const Container &values) {
    for (const int expected: values) {
        ValueType actual;
        stream >> actual;
        EXPECT_TRUE(stream.good());
        EXPECT_EQ(expected, actual);
    }
}

static void expect_msg(std::stringstream &stream, const std::string &msg) {
    std::string line;
    std::getline(stream, line);
    EXPECT_TRUE(stream.good());
    EXPECT_EQ(msg, line);
}

TEST(CliTest, FindOrderStatistics) {
    std::stringstream input;
    std::stringstream output;

    auto keys = generate_serial_keys(static_cast<int>(10e4));
    add_insert_queries(input, keys);
    add_find_order_statistic_queries(input, keys);

    run_with_stream(input, output);
    skip_n_lines(output, keys.size());
    expect_values<int>(output, keys);
}

TEST(CliTest, UnknownQueryError) {
    std::stringstream input;
    std::stringstream output;

    input << "adf " << 10 << "\n";

    run_with_stream(input, output);
    expect_msg(output, "Unknown query.");
}

TEST(CliTest, FindOrderStatisticWithInvalidIndex) {
    std::stringstream input;
    std::stringstream output;

    add_find_order_statistic_query(input, 1);

    run_with_stream(input, output);
    expect_msg(output, "The key number must be greater than zero, "
                       "but not greater than the storage size.");
}

TEST(CliTest, GetLowerCount) {
    std::stringstream input;
    std::stringstream output;

    auto keys = generate_serial_keys(static_cast<int>(10e4));
    add_insert_queries(input, keys);
    add_lower_count_queries(input, keys);

    run_with_stream(input, output);
    skip_n_lines(output, keys.size());
    std::vector<std::size_t> expected(keys.size());
    std::transform(keys.begin(), keys.end(), expected.begin(), [](auto i) { return i - 1; });
    expect_values<std::size_t>(output, expected);
}

TEST(CliTest, GetLowerCountWithInvalidValue) {
    std::stringstream input;
    std::stringstream output;

    add_insert_queries(input, {1});
    add_invalid_find_order_statistic_query(input);

    run_with_stream(input, output);
    skip_n_lines(output, 1);
    expect_msg(output, "Expected integer argument.");
}

TEST(CliTest, InsertUniqueValue) {
    std::stringstream input;
    std::stringstream output;

    auto keys = generate_serial_keys(static_cast<int>(10e5));
    add_insert_queries(input, keys);

    run_with_stream(input, output);

    for (std::size_t i = 0; i < keys.size(); i++)
        expect_msg(output, "Successfully added.");
}

TEST(CliTest, InsertExistedValue) {
    std::stringstream input;
    std::stringstream output;

    add_insert_queries(input, {1, 1});

    run_with_stream(input, output);
    expect_msg(output, "Successfully added.");
    expect_msg(output, "The key already exists. Try something different.");
}
