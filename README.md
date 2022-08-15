# Ultima

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Ultima is a WIP language designed for system/game programming with an emphasis on performance, control flow, simplicity, and fast compile times

## Getting Started

Build with CMake<br/>

### Influences

C - Base of language, many C pragma optimizations core part of Ultima<br>
C++ - Strings, cross platform threading, attirbutes<br>
D - Array slices, variant, attirbutes, alias<br>
GLSL - Vectors, matrices, operators<br>
Go - Defer<br>
Jai - Data-oriented design, reduced friction<br>
Java - String concatenation, text output formatting, annotations, array style<br>
Rust - Types, match, tagged union<br>
HolyC - Types, switch without range check<br>
Python - Style, snake case<br>
Zig - Optimality<br>

### Examples

```
i64 main()
  print("Hello World!\n")

  // Regular for loop
  for (i32 loop_num = 0; loop_num < 10; loop_num++)
    print("Loop num: " + loop_num + "\n")

  // Same loop but unrolled
  unroll for (i32 loop_num = 0; loop_num < 10; loop_num++)
    print("Loop num: " + loop_num + "\n")

  // Now in parallel
  parallel for (i32 loop_num = 0; loop_num < 10; loop_num++)
    print("Loop num: " + loop_num + "\n")

  // Switches break by default and have many new useful features
  i8 test_value = 2
  // Assignment, case ranges, case or
  i8 new_value = switch (test_value)
    case 0..4: 10
    case 5 || 7: 20

  // Fallthrough keyword
  fallthrough switch (test_value)
    case 0: print("Miss\n")
    case 1: print("Miss\n")
    case 2: print("Hit\n")
    case 3: print("Hit\n")
    case 4: print("Hit\n")

  // Use jump similar to switch but does not perform a range check for performance
  jump (test_value)
    case 0: print("First jump\n")
    case 1: print("Second jump\\n")
    case 2: print("Yadda yadda yadda\n")

  // Safetly get out of nested loops without goto or return with escape
  for (i32 x = 0; x < 4; x++)
    for (i32 y = 0; y < 4; y++)
      for (i32 z = 0; z < 4; z++)
        print("x: %i32 y: %i32 z: %i32\n", x, y, z)
        if (x == 2 && y == 2 && z == 2)
          escape 3 // Go up three loop levels

  // Tagged unions are supported through variant
  struct Menu { void* menu_elements }
  struct Game { void* game_elements }
  variant State { struct Menu, struct Game }

  variant State cur_state = (struct Game){ NULL }
  switch (cur_state)
    case struct Menu: print("Current state is menu\n")
    case struct Game: print("Current state is game\n")

  // Junk array
  i32[1024] array_stuff

  return 0

// Expression attribute to automatically return value of final function line
// Nodiscard attribute to provide compiler warning if returned value isn't used
f32 square(f32 x) @expression @nodiscard
  x * x

// Kinda Java/D style hmm, perhaps make this unique to ultima?
struct vec3 alias vec3 {
  f32 x, y, z

  const ZERO = { 0.0f, 0.0f, 0.0f}
  const ONE =  { 1.0f, 1.0f, 1.0f}
  // Note: Think about this, allow reference OR value?
  // Define exactly what will be added, can mix types?
  + (vec3* left, vec3* right) @expression
    { left.x + right.x, left.y + right.y, left.z, + right.z }
  + (vec3 left, vec3 right) @expression
    { left.x + right.x, left.y + right.y, left.z, + right.z }
  + (mat3x3 left, vec3 right) @expression
    { left.x + right.x, left.y + right.y, left.z, + right.z }

  f32 total() @expression @nodiscard
    x + y + z
}

void another_func()
  vec3 one = vec3.ONE
  vec3 two = vec3.ONE

  f32 total = vec.total()

  vec3 three = one + two
  f32 first_value = three[0]

```

### Thoughts

Compile time code execution with optional on release only in specific areas<br>
No int/float must have bits<br>
X64/ARM64 LLVM only<br>
Operator overloading<br>
Struct functions<br>
nullptr?<br>
defer<br>
C++/D/Java style function attributes/annotations<br>
calloc with NULL is malloc but named malloc to handle both and alignment<br>
Brackets optional indents preferred? Maybe restrict to arrays/struct/unions/variants<br>
Access struct members like arrays<br>
No references<br>
Not for golfing remove {}?<br>

* pointer<br>
& address<br>
* dereference? Think up something different?<br>
Contracts?<br>
