#include <iostream>
#include <cassert>
#include "jsontree.hpp"
#include "jsontree_tools.hpp"


void test_parse_error_empty_object() {
    std::cout << "Test with empty object...";
    JsonTree tree("{}");
    assert(tree.parse());
    assert(tree.get_is_valid());
    assert(tree.get_is_parsed());
    assert(tree.get_index() == 2);
    assert(tree.get_root()->is_object());
    assert(tree.get_root()->get_children().empty());
    std::cout << "PASSED" << std::endl;
}

void test_parse_object_with_simple_value() {
    std::cout << "Test with simple value...";
    JsonTree tree(R"({"k1": "example"})");
    assert(tree.parse());
    assert(tree.get_is_valid());
    assert(tree.get_nodes().size() == 3);
    // check nodes
    auto it = tree.get_nodes().begin();
    auto it_end = tree.get_nodes().end();
    auto node = *it;
    // 1
    assert(node->is_object());
    assert(node->get_children().size() == 1);
    // 2
    ++it;
    assert(it != it_end);
    node = *it;
    assert(node->is_key());
    assert(node->get_children().size() == 1);
    // 3
    ++it;
    assert(it != it_end);
    node = *it;
    assert(node->is_value());
    assert(node->get_children().empty());
    assert(node->is_string());
    assert(node->get_value_string() == "example");
    //
    ++it;
    assert(it == tree.get_nodes().end());
    //
    std::cout << "PASSED" << std::endl;
}

void test_parse_object_with_many_simple_values() {
    std::cout << "Test with many simple values...";
    const std::string json_data = R"({
        "k1": "example",
        "k2.0": 123,
        "k2.1": -321,
        "k3.0": 1.23,
        "k3.1": -1.23,
        "k4.0": 2.3e5,
        "k4.1": 2.3E-5,
        "k4.2": -4.6e-5,
        "k4.3": -7.68E+5,
        "k5.1": null,
        "k5.2": true,
        "k5.3": false
    })";
    JsonTree tree(json_data);
    assert(tree.parse());
    assert(tree.get_is_valid());
    assert(tree.get_is_parsed());
    assert(tree.get_index() == json_data.size());
    assert(tree.get_root()->is_object());
    // test node types
    constexpr std::array types = {
        JsonValueType::v_string, JsonValueType::v_int, JsonValueType::v_int, JsonValueType::v_double,
        JsonValueType::v_double, JsonValueType::v_double, JsonValueType::v_double, JsonValueType::v_double,
        JsonValueType::v_double, JsonValueType::v_null, JsonValueType::v_boolean, JsonValueType::v_boolean,
    };
    auto key_it = tree.get_root()->get_children().begin();
    const auto key_it_end = tree.get_root()->get_children().end();
    auto value_type_it = types.begin();
    const auto value_type_it_end = types.end();
    assert(tree.get_root()->get_children().size() == types.size());
    while (key_it != key_it_end && value_type_it != value_type_it_end) {
        const JsonNode* key_node = *key_it;
        assert(key_node->is_key());
        assert(key_node->get_children().size() == 1);
        assert(key_node->get_children().front()->is_value());
        assert(key_node->get_children().front()->get_value_type() == (*value_type_it));
        ++key_it;
        ++value_type_it;
    }
    std::cout << "PASSED" << std::endl;
}

void test_parse_empty_array() {
    std::cout << "Test with empty array...";
    JsonTree tree("[]");
    assert(tree.parse());
    assert(tree.get_is_valid());
    assert(tree.get_is_parsed());
    assert(tree.get_index() == 2);
    assert(tree.get_root()->is_array());
    assert(tree.get_root()->get_children().empty());
    std::cout << "PASSED" << std::endl;
}

void test_parse_array_with_simple_values() {
    std::cout << "Test with array with simple values...";
    const std::string json_data = R"([
        "string",
        123,
        -321,
        null
    ])";
    JsonTree tree(json_data);
    // base checking
    assert(tree.parse());
    assert(tree.get_is_valid());
    assert(tree.get_is_parsed());
    assert(tree.get_index() == json_data.size());
    assert(tree.get_root()->is_array());
    assert(tree.get_root()->get_children().size() == 4);
    assert(tree.get_nodes().size() == 5);
    // check node types
    constexpr std::array types = {
        JsonValueType::v_string, JsonValueType::v_int, JsonValueType::v_int, JsonValueType::v_null
    };
    auto item_it = tree.get_root()->get_children().begin();
    const auto item_it_end = tree.get_root()->get_children().end();
    auto value_type_it = types.begin();
    const auto value_type_it_end = types.end();
    assert(tree.get_root()->get_children().size() == types.size());
    while (item_it != item_it_end && value_type_it != value_type_it_end) {
        assert((*item_it)->is_value());
        assert((*item_it)->get_value_type() == (*value_type_it));
        ++item_it;
        ++value_type_it;
    }
    std::cout << "PASSED" << std::endl;
}
