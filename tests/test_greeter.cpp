#include <cassert>
#include <string>

#include "project_align/greeter.h"

int main() {
    const std::string result = project_align::greet("world");
    assert(result == "Hello, world!");
    return 0;
}
