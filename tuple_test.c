/** !
 * @file tuple_test.c
 * @author Jacob C Smith
 */
// TODO: Improve documentation

// Include
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sync/sync.h>

#include <tuple/tuple.h>

// Possible elements
char *A_element   = "A",
     *B_element   = "B",
     *C_element   = "C",
     *D_element   = "D",
     *X_element   = "X";

// Expected results
char  *_elements     [] = { 0x0 };
char  *A_elements    [] = { "A", 0x0 };
char  *B_elements    [] = { "B", 0x0 };
char  *C_elements    [] = { "C", 0x0 };
char  *AB_elements   [] = { "A", "B", 0x0 };
char  *BC_elements   [] = { "B", "C", 0x0 };
char  *AC_elements   [] = { "A", "C", 0x0 };
char  *ABC_elements  [] = { "A", "B", "C", 0x0 };

// Test results
enum result_e {
    zero,
    one,
    match
};

typedef enum result_e result_t;

int total_tests      = 0,
    total_passes     = 0,
    total_fails      = 0,
    ephemeral_tests  = 0,
    ephemeral_passes = 0,
    ephemeral_fails  = 0;


// Forward declarations
int print_time_pretty        ( double seconds );
int run_tests                ();
int print_final_summary      ();
int print_test               ( const char  *scenario_name, const char    *test_name, bool passed );

bool test_get                ( int (*tuple_constructor)(tuple **), void **expected_values, result_t expected );

int test_empty_tuple         ( int (*tuple_constructor)(tuple **), char  *name );
int test_one_element_tuple   ( int (*tuple_constructor)(tuple **), char  *name, void **values );
int test_two_element_tuple   ( int (*tuple_constructor)(tuple **), char  *name, void **values );
int test_three_element_tuple ( int (*tuple_constructor)(tuple **), char  *name, void **values );

int construct_empty                     ( tuple **pp_tuple );
int construct_empty_fromelementsABC_ABC ( tuple **pp_tuple );
int construct_empty_fromelementsAB_AB   ( tuple **pp_tuple );
int construct_empty_fromelementsA_A     ( tuple **pp_tuple );

// Entry point
int main ( int argc, const char* argv[] )
{
    
    // Initialized data
    timestamp t0 = 0,
              t1 = 0;

    // Initialize the timer library
    timer_init();

    // Formatting
    printf("|==============|\n| TUPLE TESTER |\n|==============|\n\n");

    // Start
    t0 = timer_high_precision();

    // Run tests
    run_tests();

    // Stop
    t1 = timer_high_precision();

    // Report the time it took to run the tests
    printf("tuple took ");
    print_time_pretty ( (double)(t1-t0)/(double)timer_seconds_divisor() );
    printf(" to test\n");

    // Exit
    return ( total_passes == total_tests ) ? EXIT_SUCCESS : EXIT_FAILURE;
}

int print_time_pretty ( double seconds )
{

    // Initialized data
    double _seconds     = seconds;
    size_t days         = 0,
           hours        = 0,
           minutes      = 0,
           __seconds    = 0,
           milliseconds = 0,
           microseconds = 0;

    // Days
    while ( _seconds > 86400.0 ) { days++;_seconds-=286400.0; };

    // Hours
    while ( _seconds > 3600.0 ) { hours++;_seconds-=3600.0; };

    // Minutes
    while ( _seconds > 60.0 ) { minutes++;_seconds-=60.0; };

    // Seconds
    while ( _seconds > 1.0 ) { __seconds++;_seconds-=1.0; };

    // milliseconds
    while ( _seconds > 0.001 ) { milliseconds++;_seconds-=0.001; };

    // Microseconds        
    while ( _seconds > 0.000001 ) { microseconds++;_seconds-=0.000001; };

    // Print days
    if ( days ) 
        printf("%d D, ", days);
    
    // Print hours
    if ( hours )
        printf("%d h, ", hours);

    // Print minutes
    if ( minutes )
        printf("%d m, ", minutes);

    // Print seconds
    if ( __seconds )
        printf("%d s, ", __seconds);
    
    // Print milliseconds
    if ( milliseconds )
        printf("%d ms, ", milliseconds);
    
    // Print microseconds
    if ( microseconds )
        printf("%d us", microseconds);
    
    // Success
    return 1;
}

int run_tests ( )
{
    // ... -> []
    test_empty_tuple(construct_empty, "empty");

    // ... -> [ A ]
    test_one_element_tuple(construct_empty_fromelementsA_A, "empty_fromelementsA_A", A_elements);

    // ... -> [ A, B ]
    test_two_element_tuple(construct_empty_fromelementsAB_AB, "empty_fromelementsAB_AB", AB_elements);

    // ... -> [ A, B, C ]
    test_three_element_tuple(construct_empty_fromelementsABC_ABC, "empty_fromelementsABC_ABC", ABC_elements);

    // Success
    return 1;
}

int print_final_summary ( )
{

    // Accumulate
    total_tests  += ephemeral_tests,
    total_passes += ephemeral_passes,
    total_fails  += ephemeral_fails;

    // Print
    printf("\nTests: %d, Passed: %d, Failed: %d (%%%.3f)\n",  ephemeral_tests, ephemeral_passes, ephemeral_fails, ((float)ephemeral_passes/(float)ephemeral_tests*100.f));
    printf("Total: %d, Passed: %d, Failed: %d (%%%.3f)\n\n",  total_tests, total_passes, total_fails, ((float)total_passes/(float)total_tests*100.f));
 
    ephemeral_tests  = 0;
    ephemeral_passes = 0;
    ephemeral_fails  = 0;

    // Success
    return 1;
}

int print_test ( const char *scenario_name, const char *test_name, bool passed )
{

    // Initialized data
    printf("%s_test_%-17s %s\n",scenario_name, test_name, (passed) ? "PASS" : "FAIL");

    // Increment the counters
    {
        if (passed)
        {
            ephemeral_passes++;
        }
        else
        {
            ephemeral_fails++;
        }

        ephemeral_tests++;
    }

    // Success
    return 1;
}

bool test_get ( int(*tuple_constructor)(tuple **pp_tuple), void **expected_values, result_t expected )
{

    // Initialized data
    result_t  result       = 0;
    tuple    *p_tuple       = 0;
    void     *result_value = 0;

    // Build the tuple
    tuple_constructor(&p_tuple);

    // Get a key 
    //result_value = tuple_get(p_tuple, key);

    if (result_value == expected_values)
        result = match;
    else
        result = zero;

    // Free the tuple
    tuple_destroy(&p_tuple);

    // Return result
    return (result == expected);
}

bool test_get_count ( int(*tuple_constructor)(tuple **pp_tuple), size_t expected_size, result_t expected )
{
    // Initialized data
    result_t  result       = 0;
    tuple    *p_tuple      = 0;
    size_t    result_value = 0;

    // Build the tuple
    tuple_constructor(&p_tuple);

    // Get the size of the tuple
    tuple_get(p_tuple, 0, &result_value);

    if (result_value == expected_size)
        result = match;
    else
        result = zero;

    // Free the tuple
    tuple_destroy(&p_tuple);

    // Return result
    return (result == expected);
}

bool test_size ( int(*tuple_constructor)(tuple **pp_tuple), size_t expected_size, result_t expected )
{
    // Initialized data
    result_t  result       = 0;
    tuple    *p_tuple      = 0;

    // Build the tuple
    tuple_constructor(&p_tuple);

    // Get the size of the tuple

    if ( tuple_size(p_tuple) == expected_size )
        result = match;
    else
        result = zero;

    // Free the tuple
    tuple_destroy(&p_tuple);

    // Return result
    return (result == expected);
}

bool test_index ( int(*tuple_constructor)(tuple **pp_tuple), signed idx, void *expected_value, result_t expected )
{
    // Initialized data
    result_t  result       = 0;
    tuple    *p_tuple      = 0;
    void     *result_value = 0;

    // Build the tuple
    tuple_constructor(&p_tuple);

    // Index the tuple
    result = tuple_index(p_tuple, idx, &result_value);
    
    if ( result == 1 )
        if ( result_value == expected_value )
            result = match;

    // Free the tuple
    tuple_destroy(&p_tuple);

    // Return result
    return (result == expected);
}

bool test_slice ( int(*tuple_constructor)(tuple **pp_tuple), signed lower, signed upper, void **expected_value, result_t expected )
{
    // Initialized data
    result_t  result       = 0;
    tuple    *p_tuple      = 0;
    void     *result_values[] = { 0, 0, 0, 0, (void *) 0 };

    // Build the tuple
    tuple_constructor(&p_tuple);

    // Index the tuple
    result = tuple_slice(p_tuple, &result_values, lower, upper);
    
    // Error check
    if ( result == zero )
        goto done;

    result = match;

        for (size_t i = 0; i < upper-lower; i++)
        {
            if ( result_values[i] != expected_value[i] )
                result = zero;
        }
        

    done:
    // Free the tuple
    tuple_destroy(&p_tuple);

    // Return result
    return (result == expected);
}

int construct_empty ( tuple **pp_tuple )
{
    // Construct a tuple
    tuple_construct(pp_tuple, 0);

    // tuple = []
    return 1;
}

int construct_empty_fromelementsABC_ABC ( tuple **pp_tuple )
{

    tuple_from_elements(pp_tuple, (void **)ABC_elements, 3);

    // Success
    return 1;
}

int construct_empty_fromelementsAB_AB ( tuple **pp_tuple )
{

    tuple_from_elements(pp_tuple, (void **)AB_elements, 2);

    // Success
    return 1;
}

int construct_empty_fromelementsA_A ( tuple **pp_tuple )
{

    tuple_from_elements(pp_tuple, (void **)A_elements, 1);

    // Success
    return 1;
}

int test_empty_tuple ( int (*tuple_constructor)(tuple **pp_tuple), char *name)
{

    printf("Scenario: %s\n", name);

    print_test(name, "tuple_index0"   , test_index(tuple_constructor, 0, (void *)0, zero) );
    print_test(name, "tuple_index1"   , test_index(tuple_constructor, 1, (void *)0, zero) );
    print_test(name, "tuple_get"      , test_get(tuple_constructor, (void **)_elements, zero) );    
    print_test(name, "tuple_get_count", test_get_count(tuple_constructor, 0, match) );

    print_final_summary();

    return 1;
}

int test_one_element_tuple ( int (*tuple_constructor)(tuple **pp_tuple), char *name, void **values )
{

    printf("SCENARIO: %s\n", name);

    print_test(name, "tuple_get"      , test_get(tuple_constructor, values, zero) );
    print_test(name, "tuple_size"     , test_size(tuple_constructor, 1, match));
    print_test(name, "tuple_index0"   , test_index(tuple_constructor, 0, values[0], match) );
    print_test(name, "tuple_index1"   , test_index(tuple_constructor, 1, (void *)0, zero) );
    print_test(name, "tuple_get"      , test_get(tuple_constructor, values, zero) );    
    print_test(name, "tuple_get_count", test_get_count(tuple_constructor, 1, match) );
    
    print_final_summary();
    
    // Success
    return 1;
}

int test_two_element_tuple ( int (*tuple_constructor)(tuple **pp_tuple), char *name, void **values )
{
    printf("SCENARIO: %s\n", name);

    print_test(name, "tuple_get"      , test_get(tuple_constructor, values, zero) );
    print_test(name, "tuple_size"     , test_size(tuple_constructor, 2, match));
    print_test(name, "tuple_index0"   , test_index(tuple_constructor, 0, values[0], match) );
    print_test(name, "tuple_index1"   , test_index(tuple_constructor, 1, values[1], match) );  
    print_test(name, "tuple_index2"   , test_index(tuple_constructor, 2, (void *)0, zero) );  
    print_test(name, "tuple_get_count", test_get_count(tuple_constructor, 2, match) );
    
    print_final_summary();
    
    // Success
    return 1;
}

int test_three_element_tuple ( int (*tuple_constructor)(tuple **pp_tuple), char *name, void **values )
{
    printf("SCENARIO: %s\n", name);

    print_test(name, "tuple_get"      , test_get(tuple_constructor, values, zero) );
    print_test(name, "tuple_size"     , test_size(tuple_constructor, 3, match));
    print_test(name, "tuple_index0"   , test_index(tuple_constructor, 0, values[0], match) );
    print_test(name, "tuple_index1"   , test_index(tuple_constructor, 1, values[1], match) );  
    print_test(name, "tuple_index2"   , test_index(tuple_constructor, 2, values[2], match) );  
    print_test(name, "tuple_index3"   , test_index(tuple_constructor, 3, (void *)0, zero) );  
    print_test(name, "tuple_get_count", test_get_count(tuple_constructor, 3, match) );
    
    print_final_summary();
    
    // Success
    return 1;
}
