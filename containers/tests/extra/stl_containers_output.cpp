//
// Created by Artem Bobrov on 08.01.2018.
//

#include "stl_containers_output.hpp"

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vct) {
	using std::to_string;
	std::string output = '[' + to_string(vct.size()) + ',' + to_string(vct.capacity()) + "] ";
	for (auto &element: vct)
		output += to_string(element) + " ";
	output.pop_back();
	os << output;
	return os;
}

std::ostream &operator<<(std::ostream &os, const std::list<int> &lst) {
	using std::to_string;
	std::string output = '[' + to_string(lst.size()) + "] ";
	for (auto &element: lst)
		output += to_string(element) + " ";
	output.pop_back();
	os << output;
	return os;
}