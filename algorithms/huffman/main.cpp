#include "huffman.hpp"

#include <algorithm>
#include <string>
#include <optional>
#include <fstream>
#include <ostream>
#include <iostream>

namespace {
    void verbose(const huffman_tree &tree) {
        auto table = tree.build_table();
        std::vector<std::pair<int16_t, std::vector<bool>>> pairs;

        for (uint64_t offset = 0; offset < table.size(); ++offset) {
            if (!table[offset].empty())
                pairs.emplace_back(offset, table[offset]);
        }
        std::sort(pairs.begin(), pairs.end(), [](auto lhs, auto rhs) {
            return lhs.second < rhs.second;
        });

        for (const auto &content: pairs) {
            std::cout << to_string(content.second) << " " << content.first << std::endl;
        }
    }

    void make_compress(const std::string &input_file, const std::string &output_file, bool is_verbose) {
        std::ifstream input_stream(input_file, std::ios::binary);
        input_stream >> std::noskipws;
        std::ofstream output_stream(output_file, std::ios_base::binary);
        huffman_encoder encoder(input_stream);
        input_stream.clear();
        input_stream.seekg(0, std::ios::beg);

        encoder.encode(input_stream, output_stream);

        std::cout << encoder.stats << std::endl;

        if (is_verbose)
            verbose(encoder.tree);
    }

    void make_decompress(const std::string &input_file, const std::string &output_file, bool is_verbose) {
        std::ifstream input_stream(input_file, std::ios::binary);
        input_stream >> std::noskipws;

        std::ofstream output_stream(output_file, std::ios::binary);

        huffman_decoder decoder(input_stream);

        decoder.decode(output_stream);

        std::cout << decoder.stats << std::endl;

        if (is_verbose)
            verbose(decoder.tree);
    }

    void print_usage(const std::string &name, std::ostream &os) {
        os << "Usage:" << std::endl;
        os << "\t" << name << " [-v] -d source destination" << std::endl;
        os << "\t" << name << " [-v] -c source destination" << std::endl;
    }
}

int main(int argc, char **argv) {
    program_arguments arguments(argc, argv, print_usage);
    optional_cli_argument verbose('v');
    char_cli_argument compress('c', 2);
    char_cli_argument decompress('d', 2);

    auto compress_option = arguments.option_for(compress);
    auto decompress_option = arguments.option_for(decompress);
    auto verbose_option = arguments.option_for(verbose);

    if (compress_option) {
        auto input_file = compress_option->arguments[0];
        auto output_file = compress_option->arguments[1];
        make_compress(input_file, output_file, verbose_option.has_value());
    } else if (decompress_option) {
        auto input_file = decompress_option->arguments[0];
        auto output_file = decompress_option->arguments[1];
        make_decompress(input_file, output_file, verbose_option.has_value());
    } else {
        arguments.print_usage(std::cerr);
    }
}
