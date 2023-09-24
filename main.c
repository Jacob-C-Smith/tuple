// Standard library
#include <stdio.h>
#include <stdlib.h>

// Include
#include <tuple/tuple.h>

// Forward declaration
/** !
 *  Print the elements of the tuple as strings
 * 
 * @param p_tuple the tuple
 * 
 * @return 1 on success, 0 on error
*/
int print_all_elements ( tuple *p_tuple );

// Entry point
int main ( int argc, const char* argv[] )
{

    // Initialized data
    tuple *p_tuple          = (void *) 0;
    char  *slice_of_tuple[] = { 0, 0, (void *)0 };

    // Log
    printf("Constructing tuple (\"Dogs\", \"Cats\", \"Birds\", \"Fish\")\n");

    // Make a tuple with 4 elements
    tuple_from_arguments(&p_tuple, 4, "Dogs", "Cats", "Birds", "Fish");

    // Print the tuples' keys
    print_all_elements(p_tuple);

    // Log
    printf("Tuple slice [1:2]\n");

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
int print_all_elements ( tuple *p_tuple )
{

    // Argument check
    if ( p_tuple == 0 ) return 0;

    // Initialized data
    void   **pp_elements   = 0;
    size_t   count         = 0;

    // Get quantity of elements in the tuple
    tuple_get(p_tuple, 0, &count);

    // Allocate memory for tuple contents
    pp_elements = calloc(count, sizeof(void *));

    // Error check
    if ( pp_elements == (void *) 0 ) return 0;

    // Get the contents of the tuple
    tuple_get(p_tuple, pp_elements, 0);

    // Iterate over each element
    for (size_t i = 0; i < count; i++)

        // Print each key
        printf("[%lld] %s\n", i, (char *)pp_elements[i]);
    
    // Formatting
    putchar('\n');

    // Clean up
    free(pp_elements);
    
    // Success
    return 1;
}