/*
* jsontree - JSON Parsing and library in C++20
 * Copyright 2025 Marcin Markiewicz, marcin.kivrin@gmail.com
 *
 * This file is a part of jsontree project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>
 *
 * Project: jsontree
 *
 */

#include <iostream>
#include <string>
#include "jsontree/jsontree.hpp"


int main() {
    std::string json_data{R"([10, 20, 30])"};
    JsonTree json_tree(json_data);
    json_tree.parse(); // no exception handling required
    if (!json_tree.valid()) {
        std::cout << "Failed to parse json data!" << std::endl;
        return 1;
    }
    const auto array_node = json_tree.get_root();
    std::cout << "[" << std::endl;
    for (auto const item : array_node->get_children()) {
        std::cout << "    " << item->get_value_int() << "," << std::endl;
    }
    std::cout << "]" << std::endl;
    return 0;
}
