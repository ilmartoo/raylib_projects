#pragma once
#ifndef _HELPERS_H_
#define _HELPERS_H_

// Retrieves the maximum value
#define max(a,b) (((a) > (b)) ? (a) : (b))
// Retrieves the minimum value
#define min(a,b) (((a) < (b)) ? (a) : (b))

// Retrieves the size of an array
#define countof(array) (size)(sizeof(array) / sizeof(*(array)))
// Retrieves the size of a string
#define lengthof(str) (countof(str) - 1)

// Allocates a new element in the heap
#define new(data_type) (data_type*)malloc(sizeof(data_type))
// Allocates a new array of elements in the heap
#define new_array(data_type, number_of_elements) (data_type*)malloc(sizeof(data_type) * number_of_elements)

#ifdef bool

    // Optional type
    typedef struct Optional {
        bool ok;
        void *data;
    } Optional;


#else // !bool

    // Optional type
    typedef struct Optional {
        int ok;
        void *data;
    } Optional;

#endif // bool


#endif // _HELPERS_H_
