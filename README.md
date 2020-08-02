# Ultima

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Ultima is a WIP language designed for system/game programming with an emphasis on performance, control flow, simplicity, and fast compile times.

## Getting Started

Build with CMake<br/>

### Influences

C - Base of language, many C pragma optimizations core part of Ultima<br>
C++ - Strings, cross platform threading<br>
D - Array slices, variant<br>
GLSL - Vectors, matrices, operators<br>
Jai - Data-oriented design, reduced friction<br>
Java - String concatenation, text output formatting<br>
Rust - Types, match, tagged union<br>
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

  // Fallthrough keyword useful for things like Duff's device
  fallthrough switch (test_value)
    case 0: print("Miss\n")
    case 1: print("Miss\n")
    case 2: print("Hit\n")
    case 3: print("Hit\n")
    case 4: print("Hit\n")

  // Use jump similar to switch but does not perform a range check for performance
  jump (test_value)
    case 0: print("Poopoo\n")
    case 1: print("One cheek sneak\n")
    case 2: print("Yadda yadda yadda\n")

  // Safetly get out of nested loops without goto or return with escape
  for (int32 x = 0; x < 4; x++)
    for (int32 y = 0; y < 4; y++)
      for (int32 z = 0; z < 4; z++)
        print("x: %int32 y: %int32 z: %int32\n", x, y, z)
        if (x == 2 && y == 2 && z == 2)
          escape 3 // Go up three loop levels

  // Tagged unions are supported through variant
  struct Menu { void* menu_elements }
  struct Game { void* game_elements }
  variant State { struct Menu, struct Game }

  variant State cur_state = (struct Game){ null }
  switch (cur_state)
    case struct Menu: print("Current state is menu\n")
    case struct Game: print("Current state is game\n")

  return 0
```

### Thoughts

No int/float must have bits<br>
64 bit/LLVM only<br>
C on fire<br>
