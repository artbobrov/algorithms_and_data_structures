//
// Created by Artem Bobrov on 08.01.2018.
//

#ifndef COLLECTIONS_STD_CONTAINERS_OUTPUT_HPP
#define COLLECTIONS_STD_CONTAINERS_OUTPUT_HPP

#include <vector>
#include <list>
#include <fstream>

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vct);

std::ostream &operator<<(std::ostream &os, const std::list<int> &lst);

#endif //COLLECTIONS_STD_CONTAINERS_OUTPUT_HPP
