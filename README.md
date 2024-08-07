# tuple
[![CMake](https://github.com/Jacob-C-Smith/tuple/actions/workflows/cmake.yml/badge.svg?branch=main)](https://github.com/Jacob-C-Smith/tuple/actions/workflows/cmake.yml)

**Dependencies:**\
[![sync](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/sync/actions/workflows/cmake.yml)
[![log](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml/badge.svg)](https://github.com/Jacob-C-Smith/log/actions/workflows/cmake.yml)

 A minimal tuple implementation written in C. 
 
 > 0 [Try it](#try-it)
 > 
 > 1 [Download](#download)
 >
 > 2 [Build](#build)
 >
 > 3 [Example](#example)
 >
 >> 3.1 [Example output](#example-output)
 >
 > 4 [Tester](#tester)
 >
 > 5 [Definitions](#definitions)
 >
 >> 5.1 [Type definitions](#type-definitions)
 >>
 >> 5.2 [Function definitions](#function-definitions)

## Try it
[![Open in GitHub Codespaces](https://github.com/codespaces/badge.svg)](https://codespaces.new/Jacob-C-Smith/tuple?quickstart=1)

Wait for a few moments, then click the play button on the bottom of the window. This will run the example program.

 ## Download
 To download tuple, execute the following command
 ```bash
 $ git clone https://github.com/Jacob-C-Smith/tuple
 ```
 ## Build
 To build on UNIX like machines, execute the following commands in the same directory
 ```bash
 $ cd tuple
 $ cmake .
 $ make
 ```
  This will build the example program, the tester program, and dynamic / shared libraries

  To build tuple for Windows machines, open the base directory in Visual Studio, and build your desired target(s)
 ## Example
 To run the example program, execute this command
 ```
 $ ./tuple_example
 ```
 ### Example output
 ```
Constructing tuple ("Dogs", "Cats", "Birds", "Fish")
[0] Dogs
[1] Cats
[2] Birds
[3] Fish

Tuple slice [1:2]
Cats
Birds
 ```
 [Source](main.c)
## Tester
 To run the tester program, execute this command after building
 ```
 $ ./tuple_test
 ```
 [Source](tuple_test.c)
 
 [Tester output](test_output.txt)
 ## Definitions
 ### Type definitions
 ```c
 typedef struct tuple_s tuple;
 ```
 ### Function definitions
 ```c 
// Allocaters
int tuple_create ( const tuple **const pp_tuple );

// Constructors
int tuple_construct      ( tuple       **const pp_tuple, size_t               size );
int tuple_from_elements  ( const tuple **const pp_tuple, void   *const *const elements     , size_t size );
int tuple_from_arguments ( const tuple **const pp_tuple, int                  element_count, ... );

// Accessors
int    tuple_index    ( const tuple *const p_tuple, signed             index      , void   **const pp_value );
int    tuple_get      ( const tuple *const p_tuple, const void **const pp_elements, size_t  *const p_count );
int    tuple_slice    ( const tuple *const p_tuple, const void **const pp_elements, signed         lower_bound, signed upper_bound );
bool   tuple_is_empty ( const tuple *const p_tuple );
size_t tuple_size     ( const tuple *const p_tuple );

// Iterators
int tuple_foreach_i ( const tuple *const p_tuple, void (*const function)(void *const value, size_t index) );

// Destructors
int tuple_destroy ( tuple **const pp_tuple );
```