#include "huffman.hpp"

#include <utility>
#include <vector>
#include <queue>
#include <ostream>
#include <istream>
#include <algorithm>
#include <cstring>

bit_reader::bit_reader(std::istream &stream, statistic &stat)
    : stream(stream), stat(stat) {
}

huffman_tree::char_counter bit_reader::read_counter() {
    huffman_tree::char_counter counter;
    const auto size = sizeof(uint32_t);
    for (auto &element : counter) {

        char bytes[size];
        stream.read(bytes, size);

        memcpy(&element, bytes, size);
    }
    return counter;
}

std::optional<char> bit_reader::read_huffman_char(const huffman_tree &tree) {
    if (_bit_offset < 0) {
        char c = 0;
        if (!stream.read(&c, 1))
            return {};
        ++stat.input_file_size;

        buffer = c;

        _bit_offset = MAX_BIT_OFFSET;
    }

    for (; current_node && _bit_offset >= 0;) {
        if (current_node->is_leaf()) {
            if (current_node->count == 0) {
                return {};
            }
            auto character = current_node->data;
            --current_node->count;
            current_node = tree.root;

            if (tree.root == current_node && tree.root->is_leaf())
                --_bit_offset;

            return character;
        }

        auto bit = buffer >> _bit_offset & 1u; // NOLINT(hicpp-signed-bitwise)
        --_bit_offset;

        if (bit == 1)
            current_node = current_node->right;
        else
            current_node = current_node->left;

    }

    if (current_node && current_node->is_leaf()) {
        if (current_node->count == 0)
            return {};
        auto character = current_node->data;
        --current_node->count;
        current_node = tree.root;

        return character;
    }

    return read_huffman_char(tree);
}

bit_writer::bit_writer(std::ostream &stream, statistic &stat): stream(stream), stat(stat) {
}

void bit_writer::write(const uint32_t value) {
    const auto size = sizeof(value);
    char data[size];
    memcpy(data, &value, size);
    for (const auto &offset : data) {
        stream << offset;
    }
}

void bit_writer::write(const std::vector<bool> &bitset) {
    for (auto it = bitset.rbegin(); it != bitset.rend(); ++it) {
        std::uint8_t sign = *it;
        _buffer |= uint8_t(sign << (7u - _buffer_size));
        ++_buffer_size;

        if (_buffer_size == bit_writer::MAX_BUFFER_SIZE) {
            stream << _buffer;
            ++stat.output_content_size;

            _buffer_size = 0;
            _buffer = 0;
        }
    }
}

void bit_writer::finish() {
    if (_buffer_size != 0) {
        stream << _buffer;
        ++stat.output_content_size;
    }
}

namespace {
    huffman_tree make_tree(bit_reader &reader) {
        auto counter = reader.read_counter();
        return huffman_tree(prepare_counter(counter));
    }
}

huffman_decoder::huffman_decoder(std::istream &stream): reader(bit_reader(stream, stats)), tree(make_tree(reader)) {
}

void huffman_decoder::decode(std::ostream &output_stream) {
    reader.current_node = tree.root;
    while (auto character = reader.read_huffman_char(tree)) {
        output_stream << character.value();
        ++stats.output_content_size;
    }
}

namespace {
    huffman_tree::char_counter count_characters(std::istream &stream, statistic &stats) {
        huffman_tree::char_counter counter{};
        uint8_t character = '\0';
        while (stream >> character) {
            ++stats.input_file_size;
            ++counter[character];
        }
        return counter;
    }
}

huffman_encoder::huffman_encoder(std::istream &stream)
    : counter(count_characters(stream, stats)), tree(prepare_counter(counter)) {
}

std::string to_string(std::vector<bool> const &bitvector) {
    std::string ret;
    ret.reserve(bitvector.size());
    for (const auto &bit: bitvector) {
        ret.push_back(bit ? '1' : '0');
    }
    return ret;
}
void huffman_encoder::encode(std::istream &input_stream, std::ostream &output_stream) {
    bit_writer writer(output_stream, stats);

    for (const auto value: counter) {
        writer.write(value);
    }

    const auto table = tree.build_table();

    uint8_t character = '\0';
    while (input_stream >> character) {
        auto bitset = table[character];
        writer.write(bitset);
    }

    writer.finish();

}

huffman_node::huffman_node(char data, uint32_t count): data(data), count(count) {}

huffman_node::huffman_node(huffman_node *left_child, huffman_node *right_child, uint32_t count)
    : data('\0'), count(count), left(left_child), right(right_child) {
}

bool huffman_node::is_leaf() const noexcept {
    return left == nullptr && right == nullptr;
}

uint32_t huffman_node::weight() const noexcept {
    return count;
}

huffman_node::~huffman_node() {
    delete left;
    delete right;
}
std::ostream &operator<<(std::ostream &os, const huffman_node &node) {
    os << "[" << node.data << "](" << node.count << ")";
    return os;
}

namespace {
    typedef bool (*comparator_t)(huffman_node *, huffman_node *);

    bool comparator(huffman_node *lhs, huffman_node *rhs) {
        auto lhs_weight = lhs->weight();
        auto rhs_weight = rhs->weight();

        return lhs_weight > rhs_weight || (lhs_weight == rhs_weight && lhs->data > rhs->data);
    }

    huffman_node *merge(huffman_node *lhs, huffman_node *rhs) {
        auto node = new huffman_node('\0', lhs->count + rhs->count);

        node->left = lhs;
        node->right = rhs;

        return node;
    }
}

huffman_tree::huffman_tree(const std::vector<std::pair<char, uint32_t>> &char_counters) {
    std::priority_queue<huffman_node *, std::vector<huffman_node *>, comparator_t> queue(comparator);

    for (const auto &element: char_counters) {
        auto node = new huffman_node(element.first, element.second);
        queue.push(node);
    }
    if (queue.empty())
        return;

    while (queue.size() > 1) {
        auto first = queue.top();
        queue.pop();
        auto second = queue.top();
        queue.pop();

        auto node = merge(first, second);

        queue.push(node);
    }

    root = queue.top();
}

huffman_tree::~huffman_tree() {
    delete root;
}

namespace {
    constexpr uint8_t COUNT = 10;
    void __print_helper(std::ostream &os, huffman_node *node, int space) {
        if (node == nullptr)
            return;

        space += COUNT;

        __print_helper(os, node->right, space);

        os << std::endl;
        for (int i = COUNT; i < space; i++)
            os << " ";
        os << *node << "\n";

        __print_helper(os, node->left, space);
    }

    void print(std::ostream &os, huffman_node *root) {
        __print_helper(os, root, 0);
    }

    void make_huffman_table(std::vector<bool> &&bitset, huffman_tree::table &table, huffman_node *node = nullptr) {
        if (!node)
            return;

        if (node->is_leaf()) {
            std::reverse(bitset.begin(), bitset.end());
            table[node->data] = bitset;
            return;
        }

        auto left_bitset = bitset;
        left_bitset.push_back(false);
        auto right_bitset = bitset;
        right_bitset.push_back(true);

        make_huffman_table(std::move(left_bitset), table, node->left);
        make_huffman_table(std::move(right_bitset), table, node->right);
    }
}

std::ostream &operator<<(std::ostream &os, const huffman_tree &tree) {
    print(os, tree.root);
    return os;
}

huffman_tree::table huffman_tree::build_table() const {
    table huffman_table;

    if (root == nullptr)
        return huffman_table;
    if (root->is_leaf()) {
        huffman_table[root->data] = {false};
    } else
        make_huffman_table(std::vector<bool>(), huffman_table, root);

    return huffman_table;
}

std::vector<std::pair<char, uint32_t>> prepare_counter(const huffman_tree::char_counter &counter) {
    std::vector<std::pair<char, uint32_t>> result;
    for (uint64_t character = 0; character < counter.size(); ++character) {
        if (counter[character] == 0)
            continue;
        result.emplace_back(character, counter[character]);
    }

    std::stable_sort(result.begin(), result.end(), [](auto lhs, auto rhs) {
        return lhs.second < rhs.second || (lhs.second == rhs.second && lhs.first > rhs.first);
    });
    return result;
}

char_cli_argument::char_cli_argument(char character, uint32_t arguments_number, bool is_optional)
    : character(character), arguments_number(arguments_number), is_optional(is_optional) {
}

std::optional<cli_option> char_cli_argument::get_option(const program_arguments &arguments) const {
    auto tokens = arguments.tokens();
    std::string argument{cli_argument::argument_prefix, character};
    auto argument_it = std::find(tokens.begin(), tokens.end(), argument) + 1;
    auto argument_end_iterator = argument_it + arguments_number;

    if (std::distance(argument_end_iterator, tokens.end()) >= 0) {
        return std::make_optional<cli_option>(character, std::vector(argument_it, argument_end_iterator));
    }

    return {};
}

cli_option::cli_option(char character, std::vector<std::string> arguments):
    character(character), arguments(std::move(arguments)) {
}
std::ostream &operator<<(std::ostream &os, const cli_option &option) {
    os << "character: " << option.character << " arguments: ";
    for (const auto &element: option.arguments) {
        os << element << " | ";
    }
    return os;
}

optional_cli_argument::optional_cli_argument(const char character): character(character) {
}

std::optional<cli_option> optional_cli_argument::get_option(const program_arguments &arguments) const {
    auto tokens = arguments.tokens();
    std::string argument{cli_argument::argument_prefix, character};

    if (std::find(tokens.begin(), tokens.end(), argument) != tokens.end()) {
        return std::make_optional<cli_option>(character);
    }
    return {};
}

program_arguments::program_arguments(int argv, char **argc, usage_t usage): _usage(std::move(usage)) {
    _program_name = argc[0];
    for (int offset = 1; offset < argv; ++offset) {
        _tokens.emplace_back(argc[offset]);
    }
}

std::ostream &operator<<(std::ostream &os, const program_arguments &argument) {
    os << "_tokens: ";
    for (const auto &_token : argument._tokens) {
        os << _token << " | ";
    }
    return os;
}

std::optional<cli_option> program_arguments::option_for(const cli_argument &argument) const {
    return argument.get_option(*this);
}

std::vector<std::string> program_arguments::tokens() const noexcept {
    return _tokens;
}

void program_arguments::print_usage(std::ostream &os) {
    _usage(_program_name, os);
}

std::ostream &operator<<(std::ostream &os, const statistic &statistic) {
    os << statistic.input_file_size << std::endl;
    os << statistic.output_content_size << std::endl;
    os << statistic.additional_content_size;
    return os;
}
