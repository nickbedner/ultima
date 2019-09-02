# Ultima

Ultima is a WIP language designed for system/game programming with an emphasis on performance, control flow, simplicity, and fast compile times.

## Getting Started

Build with CMake<br/>

### Infleunces

C - Base of language, many C pragma optimizations core part of Ultima<br>
C++ - Strings, cross platform threading<br>
D - Array slices<br>
Jai - Data-oriented design<br>
Java - String concatenation, text output formatting<br>
Rust - Types, match keyword<br>
HolyC - Types, switch without range check<br>
Python - Style, snake case<br>
Zig - Optimality<br>

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
    for (int32 x = 0; x < 4; x++)
        for (int32 y = 0; y < 4; y++)
            for (int32 z = 0; z < 4; z++)
                print("x: %int32 y: %int32 z: %int32\n", x, y, z)
                if (x == 2 && y == 2 && z == 2)
                    escape 3 // Go up three loop levels

    return 0
```