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
#include "jsontree/jsontree_tools.hpp"


std::string json_player_settings_data{R"(
{
    "name": "Eryndor Nightblade",
    "age": 34,
    "properties": [1, 2, 3, 4, 5]
})"};


struct PlayerSettings {
    std::string name{};
    int age{};
    std::array<int, 5> properties{};
};

template <size_t N>
bool populate_int_array_from_json_tree(const JsonNode* array_node, std::array<int, N>& dst_array) {
    size_t index = 0;
    for (auto const item : array_node->get_children()) {
        if (!item->is_value()) { return false; }
        if (!item->is_int()) { return false; }
        dst_array[index++] = item->get_value_int();
    }
    return true;
}

bool populate_string_from_json_tree(const JsonNode* key_node, std::string& dst_value) {
    if (!key_node->is_key()) { return false; }
    auto const key_value_node = key_node->get_key_value_node();
    if (!key_value_node->is_value()) { return false; }
    if (!key_value_node->is_string()) { return false; }
    dst_value = key_value_node->get_value_string();
    return true;
}

bool populate_int_from_json_tree(const JsonNode* key_node, int& dst_value) {
    if (!key_node->is_key()) { return false; }
    auto const key_value_node = key_node->get_key_value_node();
    if (!key_value_node->is_value()) { return false; }
    if (!key_value_node->is_int()) { return false; }
    dst_value = key_value_node->get_value_int();
    return true;
}

bool player_settings_from_json_tree(const JsonTree& tree, PlayerSettings& settings) {
    // check is we have json root object
    if (!tree.valid()) { return false; }
    if (!tree.get_root()->is_object()) { return false; }
    // iterate over root keys
    bool success = true;
    for (auto const node : tree.get_root()->get_children()) {
        if (node->is_key()) {
            auto const key{node->get_key_name()};
            if (key == "name") {
                success &= populate_string_from_json_tree(node, settings.name);
                continue;
            }
            if (key == "age") {
                success &= populate_int_from_json_tree(node, settings.age);
                continue;
            }
            if (key == "properties") {
                success &= populate_int_array_from_json_tree(node->get_key_value_node(), settings.properties);
            }
            // ignore unknown key
        }
    }
    return success;
}


int main() {
    std::cout << "Construct settings from json" << std::endl;
    std::cout << "JsonData: " << json_player_settings_data << std::endl;

    JsonTree json_player_settings(json_player_settings_data);
    json_player_settings.parse();
    if (!json_player_settings.valid()) {
        std::cout << "Failed to parse json data!" << std::endl;
        std::cout << "Error: " << get_json_parse_error_message(json_player_settings.get_error_code()) << std::endl;
        return 1;
    }
    std::cout << "Parsed successfully!" << std::endl;
    std::cout << json_player_settings << std::endl;

    PlayerSettings settings;

    if (!player_settings_from_json_tree(json_player_settings, settings)) {
        std::cout << "Failed to load settings!" << std::endl;
        return 1;
    }
    std::cout << "Loaded settings:" << std::endl;
    std::cout << "Name: " << settings.name << std::endl;
    std::cout << "Age: " << settings.age << std::endl;
    std::cout << "Properties: ";
    for (size_t i = 0; i < settings.properties.size(); ++i) {
        std::cout << settings.properties[i];
        if (i < settings.properties.size() - 1) { std::cout << ", "; }
    }

    return 0;
}
