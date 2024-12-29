/** !
 * tuple library interface
 * 
 * @file tuple/tuple.h 
 * 
 * @author Jacob Smith
 */

// include guard
#pragma once

// standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

// log module
#include <log/log.h>

// forward declarations
struct tuple_s;

// type definitions
/** ! @brief A tuple is an immutable, iterable, integer indexed collection */
typedef struct tuple_s tuple;

/** !
 *  @brief The type definition for a tuple iterator function
 * 
 *  @param p_value the element
 *  @param i       the index of the element
 * 
 *  @return void
 */
typedef void  (fn_tuple_foreach)  ( const void *const p_value, size_t i ); 
typedef void *(fn_tuple_allocate) ( void *p_pointer, size_t size ); 

// Initializers
/** !
 * This gets called once before main
 * 
 * @param void
 * 
 * @return void
*/
void tuple_init ( void )  __attribute__((constructor));

/** !
 *  Update the tuple allocator
 *
 * @param pfn_tuple_allocate the new allocator
 *
 * @sa tuple_init
 *
 * @return 1 on success, 0 on error
 */
int tuple_allocator ( fn_tuple_allocate *pfn_tuple_allocate );

// Constructors
/** !
 *  Construct a tuple from a list of elements
 *
 * @param pp_tuple result
 * @param elements pointer to null terminated tuple of element pointers
 * @param size     number of elements. 
 *
 * @sa tuple_construct
 * @sa tuple_from_arguments
 * @sa tuple_from_slice
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
int tuple_from_elements ( tuple **const pp_tuple, void *const *const elements, size_t size );

/** !
 *  Construct a tuple from parameters
 *
 * @param pp_tuple result
 * @param count    the quantity of variadic arguments 
 * @param ...      variadic elements
 
 * @sa tuple_construct
 * @sa tuple_from_elements
 * @sa tuple_from_slice
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */
int tuple_from_arguments ( tuple **const pp_tuple, size_t count, ... );

/** !
 *  Construct a tuple from a slice of another tuple
 *
 * @param pp_tuple    result
 * @param p_tuple     a tuple, to be slice()'d
 * @param lower_bound the lower bound of the tuple
 * @param upper_bound the upper bound of the tuple
 * 
 * @sa tuple_construct
 * @sa tuple_from_elements
 * @sa tuple_from_arguments
 * @sa tuple_destroy
 *
 * @return 1 on success, 0 on error
 */

int tuple_from_slice ( tuple **pp_tuple, const tuple *const p_tuple, signed long long lower_bound, signed long long upper_bound );

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
int tuple_index ( const tuple *const p_tuple, signed long long index, void **const pp_value );

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
int tuple_slice ( const tuple *const p_tuple, const void **const pp_elements, signed long long lower_bound, signed long long upper_bound );

/** !
 *  Is a tuple empty?
 * 
 * @param p_tuple a tuple
 * 
 * @return true if tuple has no contents else false
 */
bool tuple_is_empty ( const tuple *const p_tuple );

/** !
 *  Get the size of a tuple
 * 
 * @param p_tuple a tuple
 * 
 * @return size of tuple
 */
size_t tuple_size ( const tuple *const p_tuple );

// Iterators
/** !
 * Call function on every element in a tuple
 *
 * @param p_tuple          the tuple
 * @param fn_tuple_foreach pointer to iterator function
 * 
 * @return 1 on success, 0 on error
 */
int tuple_foreach ( const tuple *const p_tuple, fn_tuple_foreach *pfn_tuple_foreach );

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
int tuple_destroy ( tuple **const pp_tuple );

// Cleanup
/** !
 * This gets called once after main
 * 
 * @param void
 * 
 * @return void
*/
void tuple_exit ( void ) __attribute__((destructor));
