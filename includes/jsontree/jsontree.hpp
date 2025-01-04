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

#ifndef __jsontree__jsontree_hpp
#define __jsontree__jsontree_hpp


#include <string>
#include <string_view>
#include <stack>
#include <cctype>
#include <functional>
#include <iomanip>
#include <list>


enum class JsonTreeParseError {
    no_error,
    empty_json_data,
    unknown_token,
    first_node_must_be_object_or_array,
    no_parent,
    missing_comma,
    missing_colon,
    key_must_be_string,
    unexpected_node,
    end_of_object_without_begin,
    end_of_object_mismatch,
    end_of_array_without_begin,
    end_of_array_mismatch,
    colon_without_object,
    comma_without_array_or_object,
    comma_without_children,
    unexpected_literal,
    invalid_number_literal,
    trailing_comma,
    unexpected_end_of_data,
};

enum class JsonNodeType {
    object,
    array,
    key,
    value,
};

enum class JsonValueType {
    v_string,
    v_int,
    v_double,
    v_boolean,
    v_null,
};

union JsonValue {
    int v_int{};
    double v_double;
    bool v_boolean;
    std::string_view v_string;
};

class JsonNode {
    JsonNodeType type;
    JsonValueType value_type{JsonValueType::v_null};
    JsonValue value{};
    std::list<JsonNode*> children{};

    void set_key_type() { type = JsonNodeType::key; }

public:
    friend class JsonTree;

    explicit JsonNode(const JsonNodeType type_): type(type_) {}

    explicit JsonNode
    (const int value): type(JsonNodeType::value), value_type(JsonValueType::v_int), value{.v_int = value} {}

    explicit JsonNode
    (const double value): type(JsonNodeType::value), value_type(JsonValueType::v_double), value{.v_double = value} {}

    explicit JsonNode
    (const bool value): type(JsonNodeType::value), value_type(JsonValueType::v_boolean), value{.v_boolean = value} {}

    explicit JsonNode
    (const std::string_view value)
        : type(JsonNodeType::value), value_type(JsonValueType::v_string), value{.v_string = value} {}

    explicit JsonNode(): type(JsonNodeType::value) {}

    [[nodiscard]] auto is_array() const { return type == JsonNodeType::array; }
    [[nodiscard]] auto is_object() const { return type == JsonNodeType::object; }
    [[nodiscard]] auto is_container() const { return type == JsonNodeType::object || type == JsonNodeType::array; }
    [[nodiscard]] auto is_key() const { return type == JsonNodeType::key; }
    [[nodiscard]] auto is_value() const { return type == JsonNodeType::value; }
    [[nodiscard]] auto is_string() const { return value_type == JsonValueType::v_string; }
    [[nodiscard]] auto is_int() const { return value_type == JsonValueType::v_int; }
    [[nodiscard]] auto is_double() const { return value_type == JsonValueType::v_double; }
    [[nodiscard]] auto is_boolean() const { return value_type == JsonValueType::v_boolean; }
    [[nodiscard]] auto is_null() const { return value_type == JsonValueType::v_null; }
    [[nodiscard]] auto get_type() const { return type; }
    [[nodiscard]] auto get_value_type() const { return value_type; }
    [[nodiscard]] auto get_value() const { return value; }
    [[nodiscard]] auto get_value_string() const { return value.v_string; }
    [[nodiscard]] auto get_value_int() const { return value.v_int; }
    [[nodiscard]] auto get_value_double() const { return value.v_double; }
    [[nodiscard]] auto get_value_boolean() const { return value.v_boolean; }
    void add_child(JsonNode* node) { children.push_back(node); }
    [[nodiscard]] auto& get_children() const { return children; }
    [[nodiscard]] auto get_key_name() const { return get_value_string(); }
    [[nodiscard]] auto get_key_value_node() const { return children.front(); }

};


class JsonTree {
    const std::string_view json_data;
    std::stack<JsonNode*, std::list<JsonNode*>> parents{};
    std::list<JsonNode*> nodes{};
    JsonTreeParseError error_code{JsonTreeParseError::no_error};
    bool is_valid{false};
    bool is_parsed{false};
    // parse context
    size_t index{0};
    char current_char{};
    std::string_view last_token{};

    void add_node(JsonNode* node);
    void parse_skip_initial_whitespaces();
    bool parse_rule_skip_whitespaces();
    bool parse_rule_object_start();
    bool parse_rule_object_end();
    bool parse_rule_array_start();
    bool parse_rule_array_end();
    bool parse_rule_colon();
    bool parse_rule_comma();
    bool parse_rule_string();
    bool parse_rule_number();
    bool parse_rule_literal();

    using parse_rule_t = bool(JsonTree::*)();
    const std::array<parse_rule_t, 10> parse_rules{&JsonTree::parse_rule_skip_whitespaces,
                                                   &JsonTree::parse_rule_object_start, &JsonTree::parse_rule_object_end,
                                                   &JsonTree::parse_rule_array_start, &JsonTree::parse_rule_array_end,
                                                   &JsonTree::parse_rule_colon, &JsonTree::parse_rule_comma,
                                                   &JsonTree::parse_rule_string, &JsonTree::parse_rule_number,
                                                   &JsonTree::parse_rule_literal};

public:
    explicit JsonTree(const std::string_view& json_data) : json_data(json_data) {}
    JsonTree(const JsonTree& other) = delete;
    JsonTree(JsonTree&& other) noexcept = delete;

    ~JsonTree() {
        for (const auto node_ptr : nodes) {
            delete node_ptr;
        }
    }

    [[nodiscard]] auto get_json_data() const { return json_data; }
    [[nodiscard]] auto get_error_code() const { return error_code; }
    [[nodiscard]] auto get_is_valid() const { return is_valid; }
    [[nodiscard]] auto get_is_parsed() const { return is_parsed; }
    [[nodiscard]] auto get_index() const { return index; }
    [[nodiscard]] auto get_root() const { return nodes.front(); }
    [[nodiscard]] auto empty() const { return nodes.empty(); }
    [[nodiscard]] auto& get_nodes() const { return nodes; }

    bool parse() {
        if (is_parsed) { return is_valid; }
        is_parsed = true;
        parse_skip_initial_whitespaces();
        if (index == json_data.size()) {
            error_code = JsonTreeParseError::empty_json_data;
            is_valid = false;
            return is_valid;
        }
        // std::cout << std::endl << "Data: " << json_data.substr(index) << std::endl;
        while (index < json_data.size() && error_code == JsonTreeParseError::no_error) {
            // std::cout << "Parsing: " << std::to_string(index) << "|" << last_token << "|" << std::endl;
            current_char = json_data[index];
            if (std::any_of
                (
                    parse_rules.begin(),
                    parse_rules.end(),
                    [this](const auto& rule) {
                        return std::invoke(rule, this);
                    }
                    )) { continue; }
            error_code = JsonTreeParseError::unknown_token;
        }
        // std::cout << "Parsing: " << std::to_string(index) << "|" << last_token << "|" << std::endl;
        // check parents
        if (error_code == JsonTreeParseError::no_error && !parents.empty()) {
            error_code = JsonTreeParseError::unexpected_end_of_data;
        }
        //
        is_valid = error_code == JsonTreeParseError::no_error;
        return is_valid;
    }
};

inline void JsonTree::add_node(JsonNode* node) {
    const auto is_nodes_empty = nodes.empty();
    nodes.push_back(node);
    // special case: if nodes are empty, then we want to add only container
    if (is_nodes_empty) {
        if (node->is_container()) {
            parents.push(node);
        } else {
            error_code = JsonTreeParseError::first_node_must_be_object_or_array;
        }
        return;
    }
    // add element to object
    if (parents.empty()) {
        error_code = JsonTreeParseError::no_parent;
        return;
    }
    if (parents.top()->is_object()) {
        if (last_token != "{" && last_token != ",") {
            error_code = JsonTreeParseError::missing_comma;
            return;
        }
        if (node->is_string()) {
            node->set_key_type();
            parents.top()->add_child(node);
            parents.push(node); // move parent to key
            return;
        }
        error_code = JsonTreeParseError::key_must_be_string;
        return;
    }
    // add element to array
    if (parents.top()->is_array()) {
        if (last_token != "[" && last_token != ",") {
            error_code = JsonTreeParseError::missing_comma;
            return;
        }
        parents.top()->add_child(node);
        if (node->is_container()) {
            parents.push(node);
        }
        return;
    }
    // add element to key
    if (parents.top()->is_key()) {
        if (last_token != ":") {
            error_code = JsonTreeParseError::missing_colon;
            return;
        }
        parents.top()->add_child(node);
        if (node->is_container()) {
            parents.push(node);
        } else {
            parents.pop();
        }
        return;
    }
    // impossible to be here
    error_code = JsonTreeParseError::unexpected_node;
}

inline void JsonTree::parse_skip_initial_whitespaces() {
    while (index < json_data.size() && std::isspace(json_data[index])) {
        index++;
    }
}

/**
 * Rule must return true if applied
 */

inline bool JsonTree::parse_rule_skip_whitespaces() {
    if (std::isspace(current_char)) {
        index++;
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_object_start() {
    if (current_char == '{') {
        index++;
        add_node(new JsonNode(JsonNodeType::object));
        last_token = json_data.substr(index - 1, 1);
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_object_end() {
    if (current_char == '}') {
        if (last_token == ",") {
            error_code = JsonTreeParseError::trailing_comma;
            return true;
        }
        if (parents.empty()) {
            error_code = JsonTreeParseError::end_of_object_without_begin;
            return true;
        }
        if (!parents.empty() && !parents.top()->is_object()) {
            error_code = JsonTreeParseError::end_of_object_mismatch;
            return true;
        }
        parents.pop();
        if (!parents.empty() && parents.top()->is_key()) {
            parents.pop(); // object was value of key, so pop key
        }
        index++;
        last_token = json_data.substr(index - 1, 1);
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_array_start() {
    if (current_char == '[') {
        index++;
        add_node(new JsonNode(JsonNodeType::array));
        last_token = json_data.substr(index - 1, 1);
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_array_end() {
    if (current_char == ']') {
        if (last_token == ",") {
            error_code = JsonTreeParseError::trailing_comma;
            return true;
        }
        if (parents.empty()) {
            error_code = JsonTreeParseError::end_of_array_without_begin;
            return true;
        }
        if (!parents.top()->is_array()) {
            error_code = JsonTreeParseError::end_of_array_mismatch;
            return true;
        }
        parents.pop();
        if (!parents.empty() && parents.top()->is_key()) {
            parents.pop(); // object was value of key, so pop key
        }
        index++;
        last_token = json_data.substr(index - 1, 1);
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_colon() {
    if (current_char == ':') {
        if (parents.empty() or !parents.top()->is_key()) {
            error_code = JsonTreeParseError::colon_without_object;
            return true;
        }
        index++;
        last_token = json_data.substr(index - 1, 1);
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_comma() {
    if (current_char == ',') {
        // TODO: Check if this case is possible
        if (parents.empty() || !parents.top()->is_container()) {
            error_code = JsonTreeParseError::comma_without_array_or_object;
            return true;
        }
        if (parents.top()->get_children().empty()) {
            error_code = JsonTreeParseError::comma_without_children;
            return true;
        }
        index++;
        last_token = json_data.substr(index - 1, 1);
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_string() {
    if (current_char == '"') {
        const size_t start = ++index; // Skip the opening quote
        while (index < json_data.size() && json_data[index] != '"') {
            if (json_data[index] == '\\' && index + 1 < json_data.size()) {
                // Handle escape sequences
                index++;
            }
            index++;
        }
        const auto value = json_data.substr(start, index - start);
        add_node(new JsonNode(value));
        index++; // Skip the closing quote
        last_token = json_data.substr(start - 1, index - start + 1); // last token with quotes
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_number() {
    if (std::isdigit(current_char) || current_char == '-') {
        bool contains_dot = false;
        bool contains_e = false;
        const size_t start = index;
        while (index < json_data.size() && (std::isdigit
            (json_data[index]) || json_data[index] == '.' || json_data[index] == 'e' || json_data[index] == 'E' ||
            json_data[index] == '+' || json_data[index] == '-')) {
            if (json_data[index] == '.') {
                if (contains_dot) {
                    error_code = JsonTreeParseError::invalid_number_literal;
                    return true;
                }
                contains_dot = true;
            }
            if (json_data[index] == 'e' || json_data[index] == 'E') {
                if (contains_e) {
                    error_code = JsonTreeParseError::invalid_number_literal;
                    return true;
                }
                contains_e = true;
            }
            if (json_data[index] == '-' && start != index && json_data[index - 1] != 'e' && json_data[index - 1] !=
                'E') {
                error_code = JsonTreeParseError::invalid_number_literal;
                return true;
            }
            if (json_data[index] == '+' && json_data[index - 1] != 'e' && json_data[index - 1] != 'E') {
                error_code = JsonTreeParseError::invalid_number_literal;
                return true;
            }
            index++;
        }
        const auto value = json_data.substr(start, index - start);
        if (contains_dot || contains_e) {
            add_node(new JsonNode(std::stod(std::string(value))));
        } else {
            add_node(new JsonNode(std::stoi(std::string(value))));
        }
        last_token = value;
        return true;
    }
    return false;
}

inline bool JsonTree::parse_rule_literal() {
    if (std::isalpha(current_char)) {
        const size_t start = index;
        while (index < json_data.size() && std::isalpha(json_data[index])) {
            index++;
        }
        const auto literal = json_data.substr(start, index - start);
        if (literal == "true" || literal == "false") {
            add_node(new JsonNode(literal == "true"));
            last_token = literal;
            return true;
        }
        if (literal == "null") {
            add_node(new JsonNode());
            last_token = literal;
            return true;
        }
        error_code = JsonTreeParseError::unexpected_literal;
        return true;
    }
    return false;
}

#endif //__jsontree__jsontree_hpp
