/** !
 * @file tuple/tuple.h 
 * 
 * @author Jacob Smith
 * 
 * Include header for tuple library
 */

// Include guard
#pragma once

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

// Debug mode
#undef NDEBUG

// Platform dependent macros
#ifdef _WIN64
#define DLLEXPORT extern __declspec(dllexport)
#else
#define DLLEXPORT
#endif

// Memory management macro
#ifndef TUPLE_REALLOC
#define TUPLE_REALLOC(p, sz) realloc(p,sz)
#endif

// Forward declarations
struct tuple_s;

// Type definitions
/** !
 *  @brief The type definition of a tuple struct
 */
typedef struct tuple_s tuple;

// Allocaters
/** !
 *  Allocate memory for a tuple
 *
 * @param pp_tuple return
 *
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int tuple_create ( const tuple **const pp_tuple );

// Constructors
/** !
 *  Construct a tuple with a specific size
 *
 * @param pp_tuple return
 * @param size number of elements in a tuple
 *
 * @sa tuple_create
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int tuple_construct ( tuple **const pp_tuple, size_t size );

/** !
 *  Construct a tuple from a list of elements
 *
 * @param pp_tuple return
 * @param elements pointer to null terminated tuple of element pointers
 * @param size number of elements. 
 *
 * @sa tuple_construct
 * @sa tuple_from_arguments
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int tuple_from_elements ( const tuple **const pp_tuple, void *const *const elements, size_t size );

/** !
 *  Construct a tuple from parameters
 *
 * @param pp_tuple      return
 * @param element_count the quantity of variadic arguments 
 * @param ...           variadic elements
 *
 * @sa tuple_construct
 * @sa tuple_from_elements
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int tuple_from_arguments ( const tuple **const pp_tuple, int element_count, ... );

// Accessors
/** !
 * Index a tuple with a signed number. If index is negative, index = size - |index|, such that
 * [A,B,C,D,E] index(-2) -> D
 * 
 * @param p_tuple tuple
 * @param index signed index. 
 * @param pp_value return
 * 
 * @sa tuple_get
 * @sa tuple_slice
 * 
 * @return 1 on success, 0 on error 
 */
DLLEXPORT int tuple_index ( const tuple *const p_tuple, signed index, void **const pp_value );

/** !
 *  Get a tuple of elements
 *
 * @param p_tuple tuple
 * @param pp_elements return
 *
 * @sa tuple_index
 * @sa tuple_slice
 *
 * @return  1 on success, 0 on error 
 */
DLLEXPORT int tuple_get ( const tuple * const p_tuple, const void ** const pp_elements, size_t *const p_count );

/** !
 * Get a slice of the tuple specified by a lower bound and an upper bound
 * 
 * @param p_tuple tuple
 * @param pp_elements return
 * @param lower_bound the lower bound of the tuple
 * @param upper_bound the upper bound of the tuple
 * 
 * @sa tuple_index
 * @sa tuple_get
 * 
 * @return 1 on success, 0 on error 
*/
DLLEXPORT int tuple_slice ( const tuple *const p_tuple, const void ** const pp_elements, signed lower_bound, signed upper_bound );

/** !
 *  Is a tuple empty?
 * 
 * @param p_tuple a tuple
 * 
 * @return true if tuple has no contents else false
 */
DLLEXPORT bool tuple_is_empty ( const tuple *const p_tuple );

/** !
 *  Get the size of a tuple
 * 
 * @param p_tuple a tuple
 * 
 * @return size of tuple
 */
DLLEXPORT size_t tuple_size ( const tuple *const p_tuple );

// Iterators
/** !
 * Call function on every element in p_tuple
 *
 * @param p_tuple tuple
 * @param function pointer to function of type void (*)(void *value, size_t index)
 * 
 * @return 1 on success, 0 on error
 */
DLLEXPORT int tuple_foreach_i ( const tuple *const p_tuple, void (*const function)(void *const value, size_t index) );

// Destructors
/** !
 *  Destroy and deallocate a tuple
 *
 * @param pp_tuple tuple
 *
 * @sa tuple_create
 *
 * @return 1 on success, 0 on error
 */
DLLEXPORT int tuple_destroy ( tuple **const pp_tuple );