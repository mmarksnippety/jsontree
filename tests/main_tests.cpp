#include <iostream>
#include "jsontree.hpp"
#include "test_simple.cpp"
#include "test_embedded.cpp"
#include "test_errors.cpp"


int main() {
    std::cout << "Running tests..." << std::endl;
    std::cout << "================" << std::endl;
    test_parse_empty_string();
    test_parse_only_whitespaces();
    test_first_item_is_int();
    test_first_item_is_string();
    test_string_not_closed();
    test_literal_after_main_container();
    test_missing_comma_in_object_children();
    test_missing_comma_in_array_children();
    test_missing_comma_in_array_of_objects();
    test_missing_colon_in_objects_key_value();
    test_end_of_object_without_begin();
    test_end_of_object_mismatch();
    test_end_of_array_without_begin();
    test_end_of_array_mismatch();
    test_colon_without_object();
    test_comma_without_array_or_object();
    test_comma_without_children();
    test_unexpected_literal();
    test_trailing_comma_in_object();
    test_trailing_comma_in_array();
    test_not_closed_object();
    test_not_closed_array();
    test_bad_float_extra_dot();
    test_bad_float_extra_minus();
    test_bad_float_plus();
    test_bad_int();

    test_parse_error_empty_object();
    test_parse_object_with_simple_value();
    test_parse_object_with_many_simple_values();
    test_parse_empty_array();
    test_parse_array_with_simple_values();
    test_parse_embedded_object();
    test_parse_array_of_objects();
    test_parse_array_of_mixed_items();


    std::cout << "================" << std::endl;
    std::cout << "All tests passed!" << std::endl;

    return 0;
}
