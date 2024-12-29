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

// Preprocessor definitions
#define POOL_SIZE 64

// Forward declaration
/** !
 *  Print each element in a tuple 
 * 
 * @param p_tuple the tuple
 * 
 * @return 1 on success, 0 on error
*/
int print_all_elements ( tuple *p_tuple );

// Function declarations
/** !
 * Bump allocator 
 * 
 * @param p_ptr TODO
 * @param size  TODO
 * 
 * @return pointer to usable memory on success, null pointer on error 
 */
void *bump_alloc ( void *p_ptr, size_t size );

// Pool data
char   _pool[POOL_SIZE] = { 0 };
signed  used = 0;

// Entry point
int main ( int argc, const char* argv[] )
{

    // Suppress warnings
    (void) argc;
    (void) argv;

    // Initialized data
    tuple *p_tuple = (void *) 0,
          *p_slice = (void *) 0;

    // Options
    tuple_allocator(bump_alloc);

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

void *bump_alloc ( void *p_ptr, size_t size ) 
{

    // Argument check
    if ( size == 0 && p_ptr == 0 ) goto free_all;
    if ( size == 0 ) goto free_mem;
    if ( size > (POOL_SIZE - used) ) goto gc; 
    
    // Static data
    static void *p_last_ptr = (void *) -1;

    gc_done:

    // Resize
    if ( p_last_ptr == p_ptr ) goto resize;
    
    // Initialized data
    void *ret = (void *) 0;

    // Store the return value 
    ret = &_pool[used];

    // Update the quantity of memory that has been allocated
    used += size;

    log_info("\033[01;03mAllocate\033[0m ( %d )          -> \033[03m%d bytes allocated @\033[03;04m0x%04x\033[24m; [ %d+%d / %d ] bytes\n", size, size, (unsigned short) ret, size, (used - size), POOL_SIZE);

    done:

    // Update the last pointer for the next call
    p_last_ptr = ret;

    // Done
    return ret;

    gc:
    {
        log_error("\033[01;03mGC\033[0m       ( )            -> \033[03m%d bytes in use before free\033[0m \n", used, POOL_SIZE);
        
        used = 0;
        
        goto gc_done;
    }

    free_mem:
    {

        // Initialized data
        size_t old_used = used;
        
        // Mark as free
        used = 0;

        if ( size > (POOL_SIZE - used) )
        {   
            log_error("\033[01;03mUnable to allocate %d bytes\033[00m\n", size);
            return 0;
        }

        // Continue 
        if ( size ) goto gc_done;

        log_error("\033[01;03mFree\033[0m     ( \033[03;04m0x%04x\033[0m )     -> ✔\n", (unsigned short) p_ptr);

        goto done;
    }
    
    free_all:
    {
        if ( used != -1 )
            log_error("\033[01;03mReset\033[0m    ( )            -> \033[03m%d bytes used at time of reset\033[0m\n", used);

        used = 0;

        return 0;
    }

    resize:
    {
        // Initialized data
        size_t start     = (size_t) p_ptr,
               end       = _pool + used;
        size_t last_size = end - start;
        signed diff      = size - last_size;
        
        // Update the quantity of memory that has been allocated
        used += diff;
        ret = p_last_ptr;

        if ( diff > 0 )
            log_warning("\033[01;03mResize\033[00m   ( \033[03;04m0x%04x\033[0m, %2d ) -> \033[03m%d byte(s) added to %d byte allocation;\033[0m (Total: %d bytes)\n", (unsigned short) p_ptr, last_size + diff,  diff, last_size, last_size + diff);
        else
            log_warning("\033[01;03mResize\033[00m   ( \033[03;04m0x%04x\033[0m, %2d ) -> \033[03m%d byte(s) removed from %d byte allocation;\033[0\033[03mm (Total: %d bytes)\n", (unsigned short) p_ptr, last_size + diff, -diff, last_size, last_size + diff);

        // Done
        goto done;
    }
}
