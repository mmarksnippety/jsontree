#include "jsontree.hpp"


void test_parse_embedded_object() {
    std::cout << "Test with simple value...";
    JsonTree tree(R"({"k1": "example", "k2": {"k2.2": "example2"}})");
    assert(tree.parse());
    assert(tree.get_is_valid());
    assert(tree.get_nodes().size() == 7);
    // check tree structure
    const auto root = tree.get_root();
    assert(root->is_object());
    assert(root->get_children().size() == 2);
    auto const k1_node = root->get_children().front();
    assert(k1_node->is_key());
    assert(k1_node->get_children().size() == 1);
    assert(k1_node->get_children().front()->is_value());
    assert(k1_node->get_children().front()->is_string());
    assert(k1_node->get_children().front()->get_value_string() == "example");
    auto const k2_node = root->get_children().back();
    assert(k2_node->is_key());
    assert(k2_node->get_children().size() == 1);
    assert(k2_node->get_children().front()->is_object());
    assert(k2_node->get_children().front()->get_children().size() == 1);
    auto const k2_2_node = k2_node->get_children().front()->get_children().front();
    assert(k2_2_node->is_key());
    assert(k2_2_node->get_children().size() == 1);
    assert(k2_2_node->get_children().front()->is_value());
    assert(k2_2_node->get_children().front()->is_string());
    assert(k2_2_node->get_children().front()->get_value_string() == "example2");
    std::cout << "PASSED" << std::endl;
}

void test_parse_array_of_objects() {
    std::cout << "Test with array of objects...";
    JsonTree tree
    (
        R"([
        {"k1": "example1"},
        {"k2": "example2"}
    ])"
    );
    assert(tree.parse());
    assert(tree.get_is_valid());
    assert(tree.get_nodes().size() == 7);
    // check tree structure
    const auto root = tree.get_root();
    assert(root->is_array());
    assert(root->get_children().size() == 2);
    const auto first_item = root->get_children().front();
    assert(first_item->is_object());
    assert(first_item->get_children().size() == 1);
    const auto first_item_key = first_item->get_children().front();
    assert(first_item_key->is_key());
    assert(first_item_key->get_children().size() == 1);
    assert(first_item_key->get_children().front()->is_value());
    assert(first_item_key->get_children().front()->is_string());
    assert(first_item_key->get_children().front()->get_value_string() == "example1");
    const auto second_item = root->get_children().back();
    assert(second_item->is_object());
    assert(second_item->get_children().size() == 1);
    const auto second_item_key = second_item->get_children().front();
    assert(second_item_key->is_key());
    assert(second_item_key->get_children().size() == 1);
    assert(second_item_key->get_children().front()->is_value());
    assert(second_item_key->get_children().front()->is_string());
    assert(second_item_key->get_children().front()->get_value_string() == "example2");
    std::cout << "PASSED" << std::endl;
}

void test_parse_array_of_mixed_items() {
    std::cout << "Test with array of mixed items...";
    JsonTree tree
    (
        R"([
        {"k1": "example1"},
        123
    ])"
    );
    assert(tree.parse());
    assert(tree.get_is_valid());
    assert(tree.get_nodes().size() == 5);
    assert(tree.get_root()->is_array());
    assert(tree.get_root()->get_children().size() == 2);
    assert(tree.get_root()->get_children().front()->is_object());
    assert(tree.get_root()->get_children().back()->is_int());
    assert(tree.get_root()->get_children().back()->get_value_int() == 123);
    std::cout << "PASSED" << std::endl;
}
