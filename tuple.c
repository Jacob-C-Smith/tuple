/** !
 * tuple library
 * 
 * @file tuple.c 
 * 
 * @author Jacob Smith
 */

// Headers
#include <tuple/tuple.h>

// Structure definitions
struct tuple_s
{
    size_t   element_count; // Integer quantity of elements
    void   **elements;      // Tuple contents
};

// Forward declarations
int tuple_add ( tuple *const p_tuple, void *const p_element );

// Function declarations
int tuple_create ( const tuple **const pp_tuple )
{

    // Argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;

    // Allocate memory for a tuple
    tuple *p_tuple = TUPLE_REALLOC(0, sizeof(tuple));

    // Error checking
    if ( p_tuple == (void *) 0 ) goto no_mem;

    // Zero set
    memset(p_tuple, 0, sizeof(tuple));

    // Return the allocated memory
    *pp_tuple = p_tuple;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for parameter \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif
                
                // Error
                return 0;
        }
    }
}

int tuple_construct ( tuple **const pp_tuple, size_t size )
{

    // Argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;

    // Initialized data
    tuple *p_tuple = 0;

    // Allocate a tuple
    if ( tuple_create(pp_tuple) == 0 ) goto failed_to_create_tuple;
    
    // Get a pointer to the allocated tuple
    p_tuple = *pp_tuple;

    // Set the count and iterator max
    p_tuple->element_count = size;

    // Allocate "size" number of properties
    p_tuple->elements = TUPLE_REALLOC(0, size * sizeof(void *));

    // Error checking
    if ( p_tuple->elements == (void *) 0 ) goto no_mem;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for parameter \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            zero_size:
                #ifndef NDEBUG
                    printf("[tuple] Zero provided for parameter \"size\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
                
        }

        // Tuple errors
        {
            failed_to_create_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Failed to create tuple in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard Library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

int tuple_from_elements ( const tuple **const pp_tuple, void *const *const elements, size_t size )
{

    // Argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;
    if ( elements == (void *) 0 ) goto no_elements;

    // Initialized data
    tuple *p_tuple = 0;

    // Allocate a tuple
    if ( tuple_construct(&p_tuple, size) == 0 ) goto failed_to_allocate_tuple;        

    // Iterate over each key
    for (size_t i = 0; elements[i]; i++)

        // Add the key to the tuple
        p_tuple->elements[i] = elements[i];

    // Set the quantity of elements
    p_tuple->element_count = size;

    // Return
    *pp_tuple = p_tuple;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            no_elements:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Tuple errors
        {
            failed_to_allocate_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Call to \"tuple_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int tuple_from_arguments ( const tuple **const pp_tuple, int element_count, ... )
{

    // Argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;

    // Uninitialized data
    va_list list;

    // Initialized data
    tuple *p_tuple = 0;

    // Initialize the variadic list
    va_start(list, element_count);

    // Allocate a tuple
    if ( tuple_construct(&p_tuple, element_count) == 0 ) goto failed_to_allocate_tuple;        

    // Iterate over each key
    for (size_t i = 0; i < element_count; i++)

        // Add the key to the tuple
        p_tuple->elements[i] = va_arg(list, void *);
    
    // End the variadic list
    va_end(list);

    // Return
    *pp_tuple = p_tuple;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            no_elements:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"keys\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }

        // Tuple errors
        {
            failed_to_allocate_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Call to \"tuple_construct\" returned an erroneous value in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int tuple_index ( const tuple *const p_tuple, signed index, void **const pp_value )
{

    // Argument check
    if ( p_tuple                == (void *) 0 ) goto no_tuple;
    if ( p_tuple->element_count ==          0 ) goto no_elements;
    if ( pp_value               == (void *) 0 ) goto no_value;

    // Error check
    if ( p_tuple->element_count == abs(index) ) goto bounds_error;

    // Positive index
    if ( index >= 0 )
        *pp_value = p_tuple->elements[index];

    // Negative numbers
    else 
        *pp_value = p_tuple->elements[p_tuple->element_count - ( index * -1 )];

    // Success
    return 1;

    // Error handling
    {
        no_tuple:
            #ifndef NDEBUG
                printf("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        out_of_bounds:
            #ifndef NDEBUG
                printf("[tuple] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error 
            return 0;

        no_value:
            #ifndef NDEBUG
                printf("[tuple] Null pointer provided for parameter \"pp_vale\" in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;

        no_elements:
            #ifndef NDEBUG
                printf("[tuple] Can not index an empty tuple in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error 
            return 0;
        
        bounds_error:
            #ifndef NDEBUG
                printf("[tuple] Index out of bounds in call to function \"%s\"\n", __FUNCTION__);
            #endif

            // Error
            return 0;
    }
}

int tuple_get ( const tuple *const p_tuple, const void **const pp_elements, size_t *const p_count )
{

    // Argument check
    if ( p_tuple == (void *) 0 ) goto no_tuple;

    // Return the elements
    if ( pp_elements )
        memcpy(pp_elements, p_tuple->elements, sizeof(void *)*p_tuple->element_count);
    
    // Return the count
    if ( p_count )
        *p_count = p_tuple->element_count;

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

int tuple_slice ( const tuple *const p_tuple, const void **const pp_elements, signed lower_bound, signed upper_bound )
{

    // Argument check
    if ( p_tuple == (void *) 0 ) goto no_tuple;
    if ( lower_bound < 0 ) goto erroneous_lower_bound;
    if ( p_tuple->element_count < upper_bound ) goto erroneous_upper_bound;

    // Return the elements
    if ( pp_elements )
        memcpy(pp_elements, &p_tuple->elements[lower_bound], sizeof(void *) * ( upper_bound - lower_bound + 1 ) );

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;

            erroneous_lower_bound:
                #ifndef NDEBUG
                    printf("[tuple] Parameter \"lower_bound\" must be greater than zero in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
                
            erroneous_upper_bound:
                #ifndef NDEBUG
                    printf("[tuple] Parameter \"upper_bound\" must be less than or equal to tuple size in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error 
                return 0;
        }
    }
}

bool tuple_is_empty ( const tuple *const p_tuple )
{

    // Argument check
    if ( p_tuple == (void *) 0 ) goto no_tuple;

    // Success
    return ( p_tuple->element_count == 0 );

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

size_t tuple_size ( const tuple *const p_tuple )
{

    // Argument check
    if ( p_tuple == (void *) 0 ) goto no_tuple;

    // Success
    return p_tuple->element_count;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for parameter \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int tuple_add ( tuple *const p_tuple, void *const p_element )
{

    // Argument check
    if ( p_tuple == (void *) 0 ) goto no_tuple;

    // Increment the entry counter
    p_tuple->element_count++;

    // Resize iterable max?
    if ( p_tuple->element_count >= p_tuple->element_count)
    {
    
        // Double the size
        p_tuple->element_count *= 2;
    
        // Reallocate iterable tuples
        p_tuple->elements = TUPLE_REALLOC(p_tuple->elements, p_tuple->element_count * sizeof(void *));
    
        // Error checking
        if ( p_tuple->elements == (void *) 0 ) goto no_mem;
    }

    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }

        // Standard library errors
        {
            no_mem:
                #ifndef NDEBUG
                    printf("[Standard library] Failed to allocate memory in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int tuple_foreach ( const tuple *const p_tuple, void (*const function)(void *const value, size_t index) )
{

    // Argument check
    if ( p_tuple  == (void *) 0 ) goto no_tuple;
    if ( function == (void *) 0 ) goto no_free_func;

    // Iterate over each element in the tuple
    for (size_t i = 0; i < p_tuple->element_count; i++)
        
        // Call the free function
        function(p_tuple->elements[i], i);

    // Success
    return 1;

    // Error handling
    {
        
        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"p_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
            
            no_free_func:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"function\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}

int tuple_destroy ( tuple **const pp_tuple )
{

    // Argument check
    if ( pp_tuple == (void *) 0 ) goto no_tuple;

    // Initialized data
    tuple *p_tuple = *pp_tuple;

    // No more pointer for end user
    *pp_tuple = (tuple *) 0;

    // Free the tuple contents
    if ( p_tuple->element_count )
        (void)TUPLE_REALLOC(p_tuple->elements, 0);

    // Free the tuple
    (void)TUPLE_REALLOC(p_tuple, 0);
    
    // Success
    return 1;

    // Error handling
    {

        // Argument errors
        {
            no_tuple:
                #ifndef NDEBUG
                    printf("[tuple] Null pointer provided for \"pp_tuple\" in call to function \"%s\"\n", __FUNCTION__);
                #endif

                // Error
                return 0;
        }
    }
}
