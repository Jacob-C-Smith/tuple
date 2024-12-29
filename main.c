/** !
 * Tuple example program
 * 
 * @file main.c
 * 
 * @author Jacob Smith
 */

// Standard library
#include <stdio.h>
#include <stdlib.h>

// log module
#include <log/log.h>

// tuple
#include <tuple/tuple.h>

// Forward declaration
/** !
 *  Print each element in a tuple 
 * 
 * @param p_tuple the tuple
 * 
 * @return 1 on success, 0 on error
*/
int print_all_elements ( tuple *p_tuple );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // Initialized data
    tuple *p_tuple = (void *) 0,
          *p_slice = (void *) 0;

    // Output
    printf("Constructing tuple (\"Dogs\", \"Cats\", \"Birds\", \"Fish\")\n");

    // Make a 4 element tuple with variadic arguments
    tuple_from_arguments(&p_tuple, 4, "Dogs", "Cats", "Birds", "Fish");

    // Print the tuples' keys
    print_all_elements(p_tuple);

    // Output
    printf("Tuple slice [1:2]\n");

    // Get elements [1:2]
    tuple_from_slice(&p_slice, p_tuple, 1, 2);

    // Print the slice
    print_all_elements(p_slice);

    // Destroy the tuples
    tuple_destroy(&p_tuple);
    tuple_destroy(&p_slice);
    
    // Success
    return EXIT_SUCCESS;
}

// Print the tuples' elements
int print_all_elements ( tuple *p_tuple )
{

    // Argument check
    if ( p_tuple == 0 ) return 0;

    // Initialized data
    const void **pp_elements = 0;
    size_t       size        = 0;

    // Get quantity of elements in the tuple
    size = tuple_size(p_tuple);

    // Allocate memory for tuple contents
    pp_elements = realloc(0, ((1 + size) * sizeof(void *)));

    // Error check
    if ( pp_elements == (void *) 0 ) return 0;

    // Get the contents of the tuple
    tuple_slice(p_tuple, pp_elements, 0, (signed long long) size);

    // Iterate over each element
    for (size_t i = 0; i < size; i++)

        // Print each key
        printf("[%lu] %s\n", i, (char *)pp_elements[i]);
    
    // Formatting
    putchar('\n');

    // Clean up
    pp_elements = realloc(pp_elements, 0);
    
    // Success
    return 1;
}
