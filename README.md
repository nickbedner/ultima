# Ultima

Ultima is a WIP language designed for system/game programming with an emphasis on performance, control flow, simplicity, and fast compile times.

## Getting Started

Build with CMake<br/>

### Infleunces

C - Base of language, many C pragma optimizations core part of Ultima
C++ - Strings, cross platform threading
D - Parallelism
Jai - Data-oriented design
Java - String concatenation, text output formatting
Rust - Types, match keyword
HolyC - Types, switch without range check
Python - Style, snake case
Zig - Optimality

### Examples

```

int64 main()
    print("Hello World!\n")

    // Regular for loop
    for (int32 loop_num = 0; loop_num < 10; loop_num++)
        print("Loop num: " + loop_num + "\n")

    // Same loop but unrolled
    unroll for (int32 loop_num = 0; loop_num < 10; loop_num++)
        print("Loop num: " + loop_num + "\n")

    // Now in parallel
    parallel for (int32 loop_num = 0; loop_num < 10; loop_num++)
        print("Loop num: " + loop_num + "\n")

    // Switches break by default and have many new useful features
    int8 test_value = 2

    // Assignment, case ranges, case or
    int8 new_value = switch (test_value)
        case 0..4: 10
        case 5 || 7: 20

    // Brackets remove range check for performance
    switch [test_value]
        case 0: print("Poopoo\n")
        case 1: print("One cheek sneak\n")
        case 2: print("Yadda yadda yadda\n")

    // Switch will fallthrough for things like Duff's device
    fallthrough switch (test_value)
        case 0: print("Miss\n")
        case 1: print("Miss\n")
        case 2: print("Hit\n")
        case 3: print("Hit\n")
        case 4: print("Hit\n")

    // Safetly get out of nested loops without goto or return
    for (int32 x = 0; x < 10; x++)
        for (int32 y = 0; y < 10; y++)
            for (int32 z = 0; z < 10; z++)
                if (x == 5 && y == 5 && z == 5)
                    escape 3 // Go up three loop levels

    return 0
```