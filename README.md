# jsontree

JSON Parsing and library in C++20

Copyright 2025 Marcin Markiewicz, marcin.kivrin@gmail.com

Project home page https://github.com/mmarksnippety/jsontree

## Details

JsonTree is a header only library written in C++20. Its only task is to parse JSON data into a tree.
The code has a set of tests. Check the `/tests` directory. This code only parse string to tree, 
with json structure checking. This code was designed to use in embedded environment without 
handling of exceptions.

## Example of use

Example from `/examples/simple_array/main.cpp`

```c++
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
```

Output:

```text
[
    10,
    20,
    30,
]
```

See other examples in `/examples/`

There is same set of "debug" tools in `/includes/jsontree_tools.hpp`


