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

#ifndef __jsontree__jsontree_tools_hpp
#define __jsontree__jsontree_tools_hpp

#include <iostream>
#include <string>
#include "jsontree.hpp"


inline std::string get_json_parse_error_message(const JsonTreeParseError& error_code) {
    switch (error_code) {
    case JsonTreeParseError::no_error:
        return "no error";
    case JsonTreeParseError::empty_json_data:
        return "empty json data";
    case JsonTreeParseError::unknown_token:
        return "unknown token";
    case JsonTreeParseError::first_node_must_be_object_or_array:
        return "first node must be object or array";
    case JsonTreeParseError::no_parent:
        return "no parent";
    case JsonTreeParseError::missing_comma:
        return "missing coma";
    case JsonTreeParseError::missing_colon:
        return "missing colon";
    case JsonTreeParseError::key_must_be_string:
        return "key must be string";
    case JsonTreeParseError::unexpected_node:
        return "unexpected node";
    case JsonTreeParseError::end_of_object_without_begin:
        return "end of object without begin";
    case JsonTreeParseError::end_of_object_mismatch:
        return "end of object mismatch";
    case JsonTreeParseError::end_of_array_without_begin:
        return "end of array without begin";
    case JsonTreeParseError::end_of_array_mismatch:
        return "end of array mismatch";
    case JsonTreeParseError::colon_without_object:
        return "colon without object";
    case JsonTreeParseError::comma_without_array_or_object:
        return "comma without array or object";
    case JsonTreeParseError::comma_without_children:
        return "comma without children";
    case JsonTreeParseError::unexpected_literal:
        return "unexpected literal";
    case JsonTreeParseError::invalid_number_literal:
        return "invalid number literal";
    case JsonTreeParseError::trailing_comma:
        return "trailing comma";
    case JsonTreeParseError::unexpected_end_of_data:
        return "unexpected end of data";
    default:
        return "unknown error";
    }
}

inline void print_json_value(std::ostream& out, const JsonNode* node) {
    if (node == nullptr) {
        return;
    }
    switch (node->get_value_type()) {
    case JsonValueType::v_string:
        out << "STRING|" << node->get_value_string();
        break;
    case JsonValueType::v_int:
        out << "INT|" << node->get_value_int();
        break;
    case JsonValueType::v_double:
        out << "DOUBLE|" << node->get_value_double();
        break;
    case JsonValueType::v_boolean:
        out << "BOOL|" << std::boolalpha << node->get_value_boolean();
        break;
    case JsonValueType::v_null:
        out << "NULL";
        break;
    }
}

inline void print_json_node(std::ostream& out, const JsonNode* node, const int indent_level = 0) {
    if (node == nullptr) {
        out << "null";
        return;
    }
    const std::string indent(indent_level * 2, ' ');
    switch (node->get_type()) {
    case JsonNodeType::object:
        out << indent << "Object";
        break;
    case JsonNodeType::array:
        out << indent << "Array";
        break;
    case JsonNodeType::key:
        out << indent << "Key|Value|" << node->get_value_string();
        break;
    case JsonNodeType::value:
        out << indent << "Value|";
        print_json_value(out, node);
        break;
    default:
        out << indent << "error";
        break;
    }
}

inline std::ostream& operator<<(std::ostream& out, const JsonNode* node) {
    print_json_node(out, node);
    return out;
}

inline void print_json_subtree(std::ostream& out, const JsonNode* root, const int indent_level) {
    print_json_node(out, root, indent_level);
    out << std::endl;
    for (auto const node : root->get_children()) {
        print_json_subtree(out, node, indent_level + 1);
    }
}

inline void print_json_tree(std::ostream& out, const JsonTree& tree) {
    if (!tree.parsed()) {
        out << "JSON Tree is not parsed!" << std::endl;
    } else {
        print_json_subtree(out, tree.get_root(), 0);
    }
}

inline std::ostream& operator<<(std::ostream& out, const JsonTree& tree) {
    print_json_tree(out, tree);
    return out;
}

inline void print_json_tree_info(const JsonTree& tree) {
    std::cout << "JSON Tree" << std::endl;
    std::cout << tree.get_json_data() << std::endl;
    std::cout << "is_parsed: " << std::boolalpha << tree.parsed() << std::endl;
    std::cout << "is_valid: " << std::boolalpha << tree.valid() << std::endl;
    std::cout << "error_message: " << get_json_parse_error_message(tree.get_error_code()) << std::endl;
    std::cout << "index: " << tree.get_index() << std::endl;
}

#endif //__jsontree__jsontree_tools_hpp
