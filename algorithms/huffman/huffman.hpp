#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include <array>
#include <functional>
#include <ostream>

std::string to_string(std::vector<bool> const &bitvector);

struct statistic {
    uint64_t input_file_size = 0;
    uint64_t output_content_size = 0;
    const uint64_t additional_content_size = sizeof(uint32_t) * 256;

    friend std::ostream &operator<<(std::ostream &os, const statistic &statistic);
};

struct cli_option {
    explicit cli_option(char character, std::vector<std::string> arguments = std::vector<std::string>());
    friend std::ostream &operator<<(std::ostream &os, const cli_option &option);
    char character;
    std::vector<std::string> arguments;
};

class program_arguments;

class cli_argument {
public:
    static constexpr char argument_prefix = '-';

    [[nodiscard]] virtual std::optional<cli_option> get_option(const program_arguments &arguments) const = 0;
};

class char_cli_argument: public cli_argument {
public:
    char_cli_argument(char character, uint32_t arguments_number, bool is_optional = false);

    [[nodiscard]] std::optional<cli_option> get_option(const program_arguments &arguments) const override;

    char character;
    uint32_t arguments_number;
    bool is_optional;
};

class optional_cli_argument: public cli_argument {
public:
    explicit optional_cli_argument(char character);

    [[nodiscard]] std::optional<cli_option> get_option(const program_arguments &arguments) const override;
private:
    char character;
};

class program_arguments {
public:
    using usage_t = std::function<void(const std::string &, std::ostream &)>;

    program_arguments(int argv, char **argc, usage_t usage);
    friend std::ostream &operator<<(std::ostream &os, const program_arguments &argument);

    [[nodiscard]] std::optional<cli_option> option_for(const cli_argument &argument) const;

    [[nodiscard]] std::vector<std::string> tokens() const noexcept;

    void print_usage(std::ostream &os);

private:
    usage_t _usage;
    std::string _program_name;
    std::vector<std::string> _tokens;
};

struct huffman_node final {
    huffman_node(char data, uint32_t count);
    huffman_node(huffman_node *left_child, huffman_node *right_child, uint32_t count);

    virtual ~huffman_node();

    [[nodiscard]] bool is_leaf() const noexcept;
    [[nodiscard]] uint32_t weight() const noexcept;

    friend std::ostream &operator<<(std::ostream &os, const huffman_node &node);

    uint8_t data;
    uint32_t count;

    huffman_node *left = nullptr;
    huffman_node *right = nullptr;
};

struct huffman_tree final {
    static constexpr uint32_t CHARACTERS_COUNT = std::numeric_limits<uint8_t>::max() + 1;

    using char_counter = std::array<uint32_t, huffman_tree::CHARACTERS_COUNT>;
    using table = std::array<std::vector<bool>, CHARACTERS_COUNT>;

    explicit huffman_tree(const std::vector<std::pair<char, uint32_t>> &char_counters);
    huffman_tree() = default;

    ~huffman_tree();

    [[nodiscard]] table build_table() const;

    friend std::ostream &operator<<(std::ostream &os, const huffman_tree &tree);
    huffman_node *root = nullptr;
};

class bit_reader {
public:
    static constexpr uint8_t MAX_BIT_OFFSET = 7;

    explicit bit_reader(std::istream &stream, statistic &stat);

    huffman_tree::char_counter read_counter();

    std::optional<char> read_huffman_char(const huffman_tree &tree);

    huffman_node *current_node = nullptr;
private:
    std::istream &stream;
    statistic &stat;

    int8_t _bit_offset = -1;
    uint8_t buffer = 0;
};

class bit_writer {
public:
    static constexpr std::uint8_t MAX_BUFFER_SIZE = sizeof(std::uint8_t) * 8;

    explicit bit_writer(std::ostream &stream, statistic &stat);

    void write(uint32_t value);
    void write(const std::vector<bool> &bitset);

    void finish();
private:
    std::ostream &stream;
    statistic &stat;

    std::uint8_t _buffer_size = 0;
    std::uint8_t _buffer = 0;
};

class huffman_decoder final {
public:

    explicit huffman_decoder(std::istream &stream);

    void decode(std::ostream &output_stream);

    statistic stats;
    bit_reader reader;
    huffman_tree tree;
};

class huffman_encoder final {
public:

    explicit huffman_encoder(std::istream &stream);

    void encode(std::istream &input_stream, std::ostream &output_stream);

    statistic stats;
    huffman_tree::char_counter counter;
    huffman_tree tree;
};

std::vector<std::pair<char, uint32_t>> prepare_counter(const huffman_tree::char_counter &counter);