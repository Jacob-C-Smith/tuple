// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <tuple/tuple.h>

// Forward declaration
int print_all_elements(tuple* a);

// Entry point
int main ( int argc, const char* argv[] )
{

    // Initialized data
    char     *elements[]       = { "Dogs", "Cats", "Birds", "Fish", (void*)0 };
    tuple    *p_tuple          = (void *) 0;
    char     *slice_of_tuple[] = { 0, 0, (void *)0 };

    // Make an tuple with 4 elements
    tuple_from_elements(&p_tuple, elements);

    // Print the tuples' keys
    print_all_elements(p_tuple);

    // Get elements [1:2]
    tuple_slice(p_tuple, &slice_of_tuple, 1, 2);

    // Print the slice
    printf("%s\n",slice_of_tuple[0]);
    printf("%s\n",slice_of_tuple[1]);
    
    // Destroy the tuple
    tuple_destroy(&p_tuple);
    
    // Success
    return EXIT_SUCCESS;
}

// Print the tuples' elements
int print_all_elements(tuple* p_tuple)
{

    if ( p_tuple == 0 )
        return 0;

    // Get the tuples' elements
    void   **pp_elements   = 0;
    size_t   count         = 0;

    tuple_get(p_tuple, 0, &count);
    pp_elements = calloc(count, sizeof(void *));
    tuple_get(p_tuple, pp_elements, 0);

    // Iterate over each element
    for (size_t i = 0; i < count; i++)

        // Print each key
        printf("[%lld] %s\n", i, (char *)pp_elements[i]);
    
    // Formatting
    putchar('\n');
    
    // Success
    return 1;
}