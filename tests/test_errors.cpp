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
#include <cassert>
#include "jsontree.hpp"


void test_bad_float_extra_dot() {
    std::cout << "Test with bad float with extra dot...";
    JsonTree tree("[89.78.77]");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::invalid_number_literal);
    std::cout << "PASSED" << std::endl;
}

void test_bad_float_extra_minus() {
    std::cout << "Test with bad float with extra minus...";
    JsonTree tree("[89-77.45]");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::invalid_number_literal);
    std::cout << "PASSED" << std::endl;
}

void test_bad_float_plus() {
    std::cout << "Test with bad float with extra minus...";
    JsonTree tree("[34+e15]");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::invalid_number_literal);
    std::cout << "PASSED" << std::endl;
}

void test_bad_int() {
    std::cout << "Test with bad int...";
    JsonTree tree("[123, 345, 34-45]");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::invalid_number_literal);
    std::cout << "PASSED" << std::endl;
}

void test_parse_empty_string() {
    std::cout << "Test with empty string...";
    JsonTree tree("");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::empty_json_data);
    assert(tree.get_index() == 0);
    std::cout << "PASSED" << std::endl;
}

void test_parse_only_whitespaces() {
    std::cout << "Test with string contains only whitespaces...";
    JsonTree tree("     \n\t");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::empty_json_data);
    assert(tree.get_index() == tree.get_json_data().size());
    std::cout << "PASSED" << std::endl;
}

void test_first_item_is_int() {
    std::cout << "Test int first node...";
    JsonTree tree("123");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::first_node_must_be_object_or_array);
    assert(tree.get_index() == 3);
    std::cout << "PASSED" << std::endl;
}

void test_first_item_is_string() {
    std::cout << "Test string first node...";
    JsonTree tree("\"example\"");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::first_node_must_be_object_or_array);
    assert(tree.get_index() == 9);
    std::cout << "PASSED" << std::endl;
}

void test_string_not_closed() {
    std::cout << "Test string not closed...";
    JsonTree tree(R"({"k1: "example"})");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::unexpected_literal);
    std::cout << "PASSED" << std::endl;
}

void test_literal_after_main_container() {
    std::cout << "Test literal after main container...";
    JsonTree tree(R"({"k1": "example"}"k2":"v2")");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::no_parent);
    std::cout << "PASSED" << std::endl;
}

void test_missing_comma_in_object_children() {
    std::cout << "Test missing comma in object children...";
    JsonTree tree(R"({"k1": "example" "k2": "v2"})");

    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::missing_comma);
    std::cout << "PASSED" << std::endl;
}

void test_missing_comma_in_array_children() {
    std::cout << "Test missing coma in array children...";
    JsonTree tree(R"([1 2 3])");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::missing_comma);
    std::cout << "PASSED" << std::endl;
}

void test_missing_comma_in_array_of_objects() {
    std::cout << "Test missing comma in array of objects...";
    JsonTree tree
    (
        R"([
        {"k1":"v1"},
        {"k2":"v2"}
        {"k3":"v3"}
    ])"
    );
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::missing_comma);
    std::cout << "PASSED" << std::endl;
}

void test_missing_colon_in_objects_key_value() {
    std::cout << "Test missing colon in objects key value...";
    JsonTree tree(R"({"k1" "v1","k2": "v2"})");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::missing_colon);
    std::cout << "PASSED" << std::endl;
}

void test_end_of_object_without_begin() {
    std::cout << "Test end of object without begin...";
    JsonTree tree("}");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::end_of_object_without_begin);
    std::cout << "PASSED" << std::endl;
}

void test_end_of_object_mismatch() {
    std::cout << "Test end of object mismatch...";
    JsonTree tree(R"(["k1"})");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::end_of_object_mismatch);
    std::cout << "PASSED" << std::endl;
}

void test_end_of_array_without_begin() {
    std::cout << "Test end of array without begin...";
    JsonTree tree("]");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::end_of_array_without_begin);
    std::cout << "PASSED" << std::endl;
}

void test_end_of_array_mismatch() {
    std::cout << "Test end of object mismatch...";
    JsonTree tree(R"({"k1"])");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::end_of_array_mismatch);
    std::cout << "PASSED" << std::endl;
}

void test_colon_without_object() {
    std::cout << "Test colon without object...";
    JsonTree tree("[123, 456:, 789]");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::colon_without_object);
    std::cout << "PASSED" << std::endl;
}

void test_comma_without_array_or_object() {
    std::cout << "Test comma without array or object...";
    JsonTree tree(",");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::comma_without_array_or_object);
    std::cout << "PASSED" << std::endl;
}

void test_comma_without_children() {
    std::cout << "Test comma without children...";
    JsonTree tree("[,123,234,456]");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::comma_without_children);
    std::cout << "PASSED" << std::endl;
}

void test_unexpected_literal() {
    std::cout << "Test unexpected literal...";
    JsonTree tree(R"({"k1":"value","k2":nullable})");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::unexpected_literal);
    std::cout << "PASSED" << std::endl;
}

void test_trailing_comma_in_object() {
    std::cout << "Test trailing comma in object...";
    JsonTree tree(R"({"k1":"v1","k2":"v2",})");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::trailing_comma);
    std::cout << "PASSED" << std::endl;
}

void test_trailing_comma_in_array() {
    std::cout << "Test trailing comma in array...";
    JsonTree tree(R"([1,2,3,])");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::trailing_comma);
    std::cout << "PASSED" << std::endl;
}

void test_not_closed_object() {
    std::cout << "Test end of object missing...";
    JsonTree tree(R"({"k1":"v1","k2":{"k2.1":"v2"})");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::unexpected_end_of_data);
    std::cout << "PASSED" << std::endl;
}

void test_not_closed_array() {
    std::cout << "Test end of array missing...";
    JsonTree tree(R"(["v1","v2")");
    assert(!tree.parse());
    assert(!tree.valid());
    assert(tree.get_error_code() == JsonTreeParseError::unexpected_end_of_data);
    std::cout << "PASSED" << std::endl;
}
