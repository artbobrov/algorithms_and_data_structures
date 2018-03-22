//
// Created by Artem Bobrov on 20.03.2018.
//

#ifndef CONTAINERS_GENERATOR_HPP
#define CONTAINERS_GENERATOR_HPP
#include <random>
class generator {
	std::default_random_engine gen;
public:
	explicit generator(unsigned int seed): gen(seed) {}
	size_t get_size(size_t max_size = 10) {
		std::uniform_int_distribution<size_t> uid(0, max_size);
		return static_cast<size_t>(uid(gen));
	}
	int get_value() {
		std::uniform_int_distribution<int> uid(-100, 100);
		return static_cast<int>(uid(gen));
	}
};

#endif //CONTAINERS_GENERATOR_HPP
