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
    tuple *p_tuple          = (void *) 0;
    const char *_p_slice_of_tuple[] = { 0, 0, (void *)0 };

    // Output
    log_info("Constructing tuple (\"Dogs\", \"Cats\", \"Birds\", \"Fish\")\n");

    // Make a 4 element tuple with variadic arguments
    tuple_from_arguments(&p_tuple, 4, "Dogs", "Cats", "Birds", "Fish");

    // Print the tuples' keys
    print_all_elements(p_tuple);

    // Output
    log_info("Tuple slice [1:2]\n");

    // Get elements [1:2]
    tuple_slice(p_tuple, (const void **const) &_p_slice_of_tuple, 1, 2);

    // Print the slice
    printf("%s\n",_p_slice_of_tuple[0]);
    printf("%s\n",_p_slice_of_tuple[1]);
    
    // Destroy the tuple
    tuple_destroy(&p_tuple);
    
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
    pp_elements = calloc(size, sizeof(void *));

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
    free(pp_elements);
    
    // Success
    return 1;
}